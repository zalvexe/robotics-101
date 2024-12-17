#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <nav_msgs/msg/occupancy_grid.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <geometry_msgs/msg/pose_with_covariance_stamped.hpp>

// headers in STL
#include <memory>
#include <cmath>
#include <type_traits>
#include <stdio.h>
#include <float.h>
#include <vector>
#include <queue>
#include <deque>
#include <algorithm>
#include <unordered_map>
#include <bits/stdc++.h>
#include <mutex>
#include <thread>

// PCL Libraries
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/PCLPointCloud2.h>
#include <pcl/conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/conditional_removal.h>
#include <pcl/features/fpfh_omp.h>
#include <pcl/features/normal_3d_omp.h>
#include <pcl/features/don.h>
#include <pcl/registration/ndt.h>
#include <pcl/registration/icp.h>
#include <pcl/kdtree/kdtree.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
#include <pcl/segmentation/extract_clusters.h>
#include <pcl/segmentation/conditional_euclidean_clustering.h>
#include <pcl/common/common.h>
#include <pcl/common/pca.h>
#include <pcl/search/organized.h>
#include <pcl/search/kdtree.h>
#include <pclomp/ndt_omp.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/passthrough.h>

class ScanLocalizer : public rclcpp::Node
{
public:
    ScanLocalizer();
    ~ScanLocalizer();
    void LaserScanCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg);
    void OccupancyGrid2DMapCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg);
    void InitPoseCallback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg);
    void ICPMatching(pcl::PointCloud<pcl::PointXYZI>::Ptr &InputData, pcl::PointCloud<pcl::PointXYZI>::Ptr &TargetData);
    void NDTMatching(pcl::PointCloud<pcl::PointXYZI>::Ptr &InputData, pcl::PointCloud<pcl::PointXYZI>::Ptr &TargetData);

private:
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr sub_scan_;
    rclcpp::Subscription<nav_msgs::msg::OccupancyGrid>::SharedPtr sub_occupancy_grid_;
    rclcpp::Subscription<geometry_msgs::msg::PoseWithCovarianceStamped>::SharedPtr sub_init_pose_;

    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_point_cloud_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_static_map_point_cloud_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_transformed_cloud_;
    rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr pub_odometry_;

    nav_msgs::msg::Odometry m_odom_scan_;
    pcl::PointCloud<pcl::PointXYZI>::Ptr m_static_map_ptr_;
    bool b_rviz_init_;
    Eigen::Matrix4f prev_guess_, init_guess_;
};

ScanLocalizer::ScanLocalizer() : Node("scan_matching_localizer_node"), b_rviz_init_(false)
{
    sub_scan_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
        "/scan", 1, std::bind(&ScanLocalizer::LaserScanCallback, this, std::placeholders::_1));
    sub_occupancy_grid_ = this->create_subscription<nav_msgs::msg::OccupancyGrid>(
        "/map", 1, std::bind(&ScanLocalizer::OccupancyGrid2DMapCallback, this, std::placeholders::_1));
    sub_init_pose_ = this->create_subscription<geometry_msgs::msg::PoseWithCovarianceStamped>(
        "/initialpose", 1, std::bind(&ScanLocalizer::InitPoseCallback, this, std::placeholders::_1));

    pub_point_cloud_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/scan_to_pointcloud2", 1);
    pub_static_map_point_cloud_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/map_to_pointcloud2", 1);
    pub_transformed_cloud_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("/registered_points", 1);
    pub_odometry_ = this->create_publisher<nav_msgs::msg::Odometry>("/odom", 1);
}

ScanLocalizer::~ScanLocalizer()
{
    RCLCPP_INFO(this->get_logger(), "ScanLocalizer destructor.");
}

void ScanLocalizer::InitPoseCallback(const geometry_msgs::msg::PoseWithCovarianceStamped::SharedPtr msg)
{
    Eigen::Matrix3f mat3 = Eigen::Quaternionf(msg->pose.pose.orientation.w,
                                              msg->pose.pose.orientation.x,
                                              msg->pose.pose.orientation.y,
                                              msg->pose.pose.orientation.z)
                               .toRotationMatrix();
    prev_guess_.block(0, 0, 3, 3) = mat3;
    prev_guess_(0, 3) = msg->pose.pose.position.x;
    prev_guess_(1, 3) = msg->pose.pose.position.y;
    b_rviz_init_ = true;
}

void ScanLocalizer::LaserScanCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
{
    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud_in_ptr(new pcl::PointCloud<pcl::PointXYZI>);

    if (msg->ranges.empty())
    {
        RCLCPP_ERROR(this->get_logger(), "Empty scan data");
        return;
    }

    // Convert the laser scan data to the pointcloud type (pcl::PointCloud<pcl::PointXYZI>)
    float angle_min = msg->angle_min;
    float angle_max = msg->angle_max;
    float angle_increment = msg->angle_increment;
    std::vector<float> ranges = msg->ranges;
    std::vector<float> intensities = msg->intensities;

    cloud_in_ptr->is_dense = false;
    cloud_in_ptr->width = ranges.size();
    cloud_in_ptr->height = 1;
    cloud_in_ptr->points.resize(ranges.size());

    float angle_temp;

    for (size_t i = 0; i < ranges.size(); ++i)
    {
        pcl::PointXYZI point_buf;
        angle_temp = angle_min + i * angle_increment;
        point_buf.x = ranges[i] * cos(angle_temp);
        point_buf.y = ranges[i] * sin(angle_temp);
        point_buf.z = 0;
        point_buf.intensity = 0; // msg->intensities[i]; // to be changed
        cloud_in_ptr->points[i] = point_buf;
    }

    sensor_msgs::msg::PointCloud2 laser_to_pointcloud2_msg;
    pcl::toROSMsg(*cloud_in_ptr, laser_to_pointcloud2_msg);
    laser_to_pointcloud2_msg.header = msg->header;
    pub_point_cloud_->publish(laser_to_pointcloud2_msg);

    // Scan matching algorithm
    if (cloud_in_ptr->points.empty() || m_static_map_ptr_->points.empty())
        return;

    // You can switch the registration algorithm between ICP and NDT_OMP
    // 1. ICP (No multi-thread)
    ICPMatching(cloud_in_ptr, m_static_map_ptr_); // Too slow

    // 2. NDT_OMP (multi-thread)
    // NDTMatching(cloud_in_ptr, m_static_map_ptr_);
}

void ScanLocalizer::OccupancyGrid2DMapCallback(const nav_msgs::msg::OccupancyGrid::SharedPtr msg)
{
    // Convert the occupancy grid to a point cloud
    m_static_map_ptr_.reset(new pcl::PointCloud<pcl::PointXYZI>());
    for (unsigned int width = 0; width < msg->info.width; ++width)
    {
        for (unsigned int height = 0; height < msg->info.height; ++height)
        {
            // Convert occupied grid to the x-y coordinates to put the target (pcl::PointCloud<pcl::PointXYZI>)
            if (msg->data[height * msg->info.width + width] > 80)
            {
                pcl::PointXYZI obstacle;
                obstacle.x = width * msg->info.resolution + msg->info.resolution / 2 + msg->info.origin.position.x;
                obstacle.y = height * msg->info.resolution + msg->info.resolution / 2 + msg->info.origin.position.y;
                obstacle.z = 0;
                obstacle.intensity = msg->data[height * msg->info.width + width];
                m_static_map_ptr_->push_back(obstacle);
            }
        }
    }

    sensor_msgs::msg::PointCloud2 static_map_to_pointcloud2_msg;
    pcl::toROSMsg(*m_static_map_ptr_, static_map_to_pointcloud2_msg);
    static_map_to_pointcloud2_msg.header.frame_id = "map";
    static_map_to_pointcloud2_msg.header.stamp = msg->header.stamp;
    pub_static_map_point_cloud_->publish(static_map_to_pointcloud2_msg);

    RCLCPP_INFO(this->get_logger(), "MAP IS LOADED");
}

void ScanLocalizer::ICPMatching(pcl::PointCloud<pcl::PointXYZI>::Ptr &InputData, pcl::PointCloud<pcl::PointXYZI>::Ptr &TargetData)
{
    // You should initialize the init pose using RVIZ 2D pose estimate tool.
    if (!b_rviz_init_)
    {
        init_guess_ = Eigen::Matrix4f::Identity();
    }
    else
    {
        init_guess_ = prev_guess_;
    }
    pcl::IterativeClosestPoint<pcl::PointXYZI, pcl::PointXYZI> icp;
    icp.setInputSource(InputData);
    icp.setInputTarget(TargetData);
    icp.setMaximumIterations(50);

    // Run registration algorithm, and put the transformation matrix of previous step.
    pcl::PointCloud<pcl::PointXYZI>::Ptr result(new pcl::PointCloud<pcl::PointXYZI>());
    icp.align(*result, init_guess_);

    if (icp.hasConverged())
    {
        std::cout << "converged." << std::endl
                  << "The score is " << icp.getFitnessScore() << std::endl;
        std::cout << "Transformation matrix:" << std::endl;
        std::cout << icp.getFinalTransformation() << std::endl;
    }
    else
    {
        return;
    }

    init_guess_.block<3, 3>(0, 0) = icp.getFinalTransformation().block<3, 3>(0, 0);
    init_guess_.block<3, 1>(0, 3) = icp.getFinalTransformation().block<3, 1>(0, 3);

    // Publish registered cloud
    pcl::PointCloud<pcl::PointXYZI>::Ptr final_cloud(new pcl::PointCloud<pcl::PointXYZI>());
    pcl::transformPointCloud(*InputData, *final_cloud, init_guess_);

    sensor_msgs::msg::PointCloud2 final_cloud_to_pointcloud2_msg;
    pcl::toROSMsg(*final_cloud, final_cloud_to_pointcloud2_msg);
    final_cloud_to_pointcloud2_msg.header.frame_id = "map";
    pub_transformed_cloud_->publish(final_cloud_to_pointcloud2_msg);

    // Publish Odometry
    nav_msgs::msg::Odometry odom_msg;
    odom_msg.header.frame_id = "map";
    odom_msg.child_frame_id = "base_link";
    odom_msg.header.stamp = this->now();
    // Fill in the odometry message with the transformation data
    // (This part needs to be implemented based on your specific requirements)
    pub_odometry_->publish(odom_msg);

    prev_guess_ = init_guess_;
}

void ScanLocalizer::NDTMatching(pcl::PointCloud<pcl::PointXYZI>::Ptr &InputData, pcl::PointCloud<pcl::PointXYZI>::Ptr &TargetData)
{
    if (!b_rviz_init_)
    {
        init_guess_ = Eigen::Matrix4f::Identity();
    }
    else
    {
        init_guess_ = prev_guess_;
    }

    boost::shared_ptr<pclomp::NormalDistributionsTransform<pcl::PointXYZI, pcl::PointXYZI>> ndt(new pclomp::NormalDistributionsTransform<pcl::PointXYZI, pcl::PointXYZI>());
    ndt->setInputSource(InputData);
    ndt->setInputTarget(TargetData);
    ndt->setTransformationEpsilon(0.01);
    ndt->setMaximumIterations(32);
    ndt->setResolution(1.0);
    ndt->setNeighborhoodSearchMethod(pclomp::DIRECT7);
    ndt->setNumThreads(5);

    pcl::PointCloud<pcl::PointXYZI>::Ptr output_cloud(new pcl::PointCloud<pcl::PointXYZI>);
    ndt->align(*output_cloud, init_guess_);

    if (ndt->hasConverged())
    {
        std::cout << "converged." << std::endl
                  << "The score is " << ndt->getFitnessScore() << std::endl;
        std::cout << "Transformation matrix:" << std::endl;
        std::cout << ndt->getFinalTransformation() << std::endl;
    }
    else
    {
        return;
    }

    init_guess_.block<3, 3>(0, 0) = ndt->getFinalTransformation().block<3, 3>(0, 0);
    init_guess_.block<3, 1>(0, 3) = ndt->getFinalTransformation().block<3, 1>(0, 3);

    // Publish registered cloud
    pcl::PointCloud<pcl::PointXYZI>::Ptr final_cloud(new pcl::PointCloud<pcl::PointXYZI>());
    pcl::transformPointCloud(*InputData, *final_cloud, init_guess_);

    sensor_msgs::msg::PointCloud2 final_cloud_to_pointcloud2_msg;
    pcl::toROSMsg(*final_cloud, final_cloud_to_pointcloud2_msg);
    final_cloud_to_pointcloud2_msg.header.frame_id = "map";
    pub_transformed_cloud_->publish(final_cloud_to_pointcloud2_msg);

    // Publish Odometry
    nav_msgs::msg::Odometry odom_msg;
    odom_msg.header.frame_id = "map";
    odom_msg.child_frame_id = "base_link";
    odom_msg.header.stamp = this->now();
    // Fill in the odometry message with the transformation data
    // (This part needs to be implemented based on your specific requirements)
    pub_odometry_->publish(odom_msg);

    prev_guess_ = init_guess_;
}

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<ScanLocalizer>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}