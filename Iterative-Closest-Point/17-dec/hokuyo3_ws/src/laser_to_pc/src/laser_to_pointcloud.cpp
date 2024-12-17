#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "laser_geometry/laser_geometry.hpp"
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <limits>
#include <fstream>

class LaserScanToPointCloud : public rclcpp::Node
{
public:
    LaserScanToPointCloud(): Node("laser_scan_to_pointcloud")
    {
        laser_sub_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "scan", 10, std::bind(&LaserScanToPointCloud::laserCallback, this, std::placeholders::_1));
        pc_pub_ = this->create_publisher<sensor_msgs::msg::PointCloud2>("laser_pointcloud", 10);
    }

private:
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

        std::ofstream ofs("laser_pointcloud.txt");

        if(ofs.is_open())
        {
            for(size_t i = 0; i < pcl_cloud.points.size(); ++i)
            {
                float angle = msg->angle_min + i * msg->angle_increment;
                float angle_deg = angle*(180.0/M_PI);
                ofs << pcl_cloud.points[i].x << " " << pcl_cloud.points[i].y << " " << pcl_cloud.points[i].z << " " << angle_deg << std::endl;
            }
            ofs.close();
            RCLCPP_INFO(this->get_logger(), "Saved point cloud to laser_pointcloud.txt");
        }
        else{
            RCLCPP_ERROR(this->get_logger(), "Failed to open laser_pointcloud.txt");
        }
    }
    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pc_pub_;
    laser_geometry::LaserProjection projector_;
};

int main(int argc, char *argv[])
{
    RCLCPP_INFO(rclcpp::get_logger("laser_to_pointcloud"), "Starting laser_to_pointcloud node...");
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LaserScanToPointCloud>());
    rclcpp::shutdown();
    return 0;
}

//pointcloud dapet (x,y,z) dari laser scan lidar
//pake library https://github.com/AtsushiSakai/PythonRobotics/blob/master/SLAM/iterative_closest_point/iterative_closest_point.py buat jadiin R sama T
//dapetin position dalam X Y Z dan orientation dalam X Y Z W
//pake complementary filter buat fusion odom sama lidar

