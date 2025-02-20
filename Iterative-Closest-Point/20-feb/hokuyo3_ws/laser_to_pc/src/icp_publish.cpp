#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "laser_geometry/laser_geometry.hpp"
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/registration/icp.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "romusa2_messages/msg/euler_pose.hpp"
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <limits>
#include <fstream>

/*
tasks:
1. icp pointcloud converged [done]
2. icp quarternion (x y w z) [done]
3. euler coordinate for pose (x y yaw) [done]
4. lokalisasi icp [done]
5. complementary filter

adds on:
1. filter (high pass, etc)
*/

class PublishICP : public rclcpp::Node
{
public:
    PublishICP(): Node("final_icp_node")
    {
        laser_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "scan", 10, std::bind(&PublishICP::laserCallback, this, std::placeholders::_1));
        fixed_map_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "scan_fixed", 10, std::bind(&PublishICP::fixedMapCallback, this, std::placeholders::_1));
        pc_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("laser_pointcloud", 10);
        icp_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("icp_pointcloud", 10);
        pose_pub_ = this->create_publisher<geometry_msgs::msg::PoseStamped>("icp_pose", 10);
        euler_pose_pub = this->create_publisher<romusa2_messages::msg::EulerPose>("euler_pose", 10);
    }

private:
    void fixedMapCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
    {
        sensor_msgs::msg::PointCloud2 cloud_out;
        projector_.projectLaser(*msg, cloud_out);
        pcl::fromROSMsg(cloud_out, *fixed_map_);
    }

    void laserCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg)
    {
        //filter
        auto filtered_ranges = msg->ranges;
        float max_range_limit = 10.0;
        for (auto& range : filtered_ranges)
        {
            if (std::isinf(range) || range > max_range_limit)
            {
                range = max_range_limit;
            }
        }
        //new data
        auto filtered_msg = std::make_shared<sensor_msgs::msg::LaserScan>(*msg);
        filtered_msg->ranges = filtered_ranges;

        // RCLCPP_INFO(this->get_logger(), "angle_min: %f, angle_max: %f, angle_increment: %f", msg->angle_min, msg->angle_max, msg->angle_increment);

        sensor_msgs::msg::PointCloud2 cloud_out;
        projector_.projectLaser(*filtered_msg, cloud_out);
        pc_pub_->publish(cloud_out);

        pcl::PointCloud<pcl::PointXYZ> pcl_cloud;
        pcl::fromROSMsg(cloud_out, pcl_cloud);

        //icp calc
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_in(new pcl::PointCloud<pcl::PointXYZ>(pcl_cloud));
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_out_icp(new pcl::PointCloud<pcl::PointXYZ>);

        pcl::IterativeClosestPoint<pcl::PointXYZ, pcl::PointXYZ> icp;
        icp.setInputSource(cloud_in);
        icp.setInputTarget(fixed_map_);
        pcl::PointCloud<pcl::PointXYZ> final_cloud;
        icp.align(final_cloud);

        if(icp.hasConverged()) {
            RCLCPP_INFO(this->get_logger(), "ICP converged"); //converged = has found the transformation (rotation n translation) that aligns the source & target pointcloud
            
            Eigen::Matrix4f transformation = icp.getFinalTransformation();
            Eigen::Matrix3f rotation = transformation.block<3,3>(0,0);
            Eigen::Vector3f translation = transformation.block<3,1>(0,3);

            //swap
            float x = translation.y();
            float y = -translation.x();
            float z = translation.z();
            //quarternion
            Eigen::Quaternionf quat(rotation);
            tf2::Quaternion quat_tf(quat.x(), quat.y(), quat.z(), quat.w());

            //euler yaw pitch roll
            double yaw, pitch, roll;
            tf2::Matrix3x3(quat_tf).getRPY(roll, pitch, yaw);

            yaw = -yaw;
            // if(yaw < 0) yaw += 2*M_PI;
            //pose
            geometry_msgs::msg::PoseStamped pose_msg;
            pose_msg.header.stamp = this->get_clock()->now();
            pose_msg.header.frame_id = "laser";
            pose_msg.pose.position.x = x;
            pose_msg.pose.position.y = y;
            pose_msg.pose.position.z = z;
            pose_msg.pose.orientation.x = quat_tf.x();
            pose_msg.pose.orientation.y = quat_tf.y();
            pose_msg.pose.orientation.z = quat_tf.z();
            pose_msg.pose.orientation.w = quat_tf.w();

            pose_pub_->publish(pose_msg);

            romusa2_messages::msg::EulerPose euler_pose_msg;
            euler_pose_msg.x = x;
            euler_pose_msg.y = y;
            euler_pose_msg.yaw = yaw;
            euler_pose_pub->publish(euler_pose_msg);
            
        }
            else RCLCPP_ERROR(this->get_logger(), "ICP did not converge");

        sensor_msgs::msg::PointCloud2 final_cloud_msg;
        pcl::toROSMsg(final_cloud, final_cloud_msg);
        final_cloud_msg.header.frame_id = "laser";
        icp_pub_->publish(final_cloud_msg);
    }
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub_;
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr fixed_map_sub_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pc_pub_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr icp_pub_;
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr pose_pub_;
    rclcpp::Publisher<romusa2_messages::msg::EulerPose>::SharedPtr euler_pose_pub;
    laser_geometry::LaserProjection projector_;
    pcl::PointCloud<pcl::PointXYZ>::Ptr fixed_map_{new pcl::PointCloud<pcl::PointXYZ>};
};

int main(int argc, char *argv[])
{
    RCLCPP_INFO(rclcpp::get_logger("final_icp_node"), "Starting final_icp_node...");
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PublishICP>());
    rclcpp::shutdown();
    return 0;
}

