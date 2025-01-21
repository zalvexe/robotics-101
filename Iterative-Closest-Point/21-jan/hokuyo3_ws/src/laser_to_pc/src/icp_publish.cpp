#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "laser_geometry/laser_geometry.hpp"
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/registration/icp.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <limits>
#include <fstream>

class PublishICP : public rclcpp::Node
{
public:
    PublishICP(): Node("final_icp_node")
    {
        laser_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "scan", 10, std::bind(&PublishICP::laserCallback, this, std::placeholders::_1));
        fixed_map_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "laser_scan_broadcaster", 10, std::bind(&PublishICP::fixedMapCallback, this, std::placeholders::_1));
        pc_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("laser_pointcloud", 10);
        icp_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("icp_pointcloud", 10);

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

        if(icp.hasConverged()) RCLCPP_INFO(this->get_logger(), "ICP converged");
        else RCLCPP_ERROR(this->get_logger(), "ICP did not converge");

        sensor_msgs::msg::PointCloud2 final_cloud_msg;
        pcl::toROSMsg(final_cloud, final_cloud_msg);
        final_cloud_msg.header.frame_id = "laser_frame";
        icp_pub_->publish(final_cloud_msg);
    }
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub_;
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr fixed_map_sub_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pc_pub_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr icp_pub_;
    laser_geometry::LaserProjection projector_;
    pcl::PointCloud<pcl::PointXYZ>::Ptr fixed_map_{new pcl::PointCloud<pcl::PointXYZ>};
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<PublishICP>());
    rclcpp::shutdown();
    return 0;
}
//plis bisa omagad besok dicoba askdjlaskdjlawdoaijdlaksjd

