#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/registration/icp.h>
#include <Eigen/Dense>
#include <iostream>

class ICPNode : public rclcpp::Node {
public:
    ICPNode() : Node("icp_node") {
        pcl_subs = this->create_subscription<sensor_msgs::msg::PointCloud2>(
            "/laser_pointcloud", 10, std::bind(&ICPNode::pointcloud_callback, this, std::placeholders::_1));
    }

private:
    void pointcloud_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg) {
        pcl::fromROSMsg(*msg, *current_cloud_);
        if (!previous_cloud_->empty() && !current_cloud_->empty()) {

            pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp;
            icp.setInputSource(current_cloud_);
            icp.setInputTarget(previous_cloud_);
            pcl::PointCloud<pcl::PointXYZ> Final;
            icp.align(Final);

            if (icp.hasConverged()) {
                RCLCPP_INFO(this->get_logger(), "ICP has converged, score is %f", icp.getFitnessScore());
                std::stringstream ss;
                Eigen::Matrix4f transformation = icp.getFinalTransformation();
                ss<<transformation.format(Eigen::IOFormat(Eigen::StreamPrecision, Eigen::DontAlignCols, ", ", ", ", "", "", "[", "]"));
                RCLCPP_INFO(this->get_logger(), "Transformation matrix:\n%s", ss.str().c_str());
            } else {
                RCLCPP_WARN(this->get_logger(), "ICP did not converge.");
            }

            *previous_cloud_ = *current_cloud_;  
        }
    }

    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr pcl_subs;
    pcl::PointCloud<pcl::PointXYZ>::Ptr previous_cloud_{new pcl::PointCloud<pcl::PointXYZ>};
    pcl::PointCloud<pcl::PointXYZ>::Ptr current_cloud_{new pcl::PointCloud<pcl::PointXYZ>};
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ICPNode>());
    rclcpp::shutdown();
    return 0;
}