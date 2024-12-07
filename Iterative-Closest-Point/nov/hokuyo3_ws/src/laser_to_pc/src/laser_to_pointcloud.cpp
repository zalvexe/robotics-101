#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "laser_geometry/laser_geometry.hpp"

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
        sensor_msgs::msg::PointCloud2 cloud_out;
        projector_.projectLaser(*msg, cloud_out);
        pc_pub_->publish(cloud_out);
    }

    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub_;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pc_pub_;
    laser_geometry::LaserProjection projector_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LaserScanToPointCloud>());
    rclcpp::shutdown();
    return 0;
}

//pointcloud dapet (x,y,z) dari laser scan lidar
//pake library https://github.com/AtsushiSakai/PythonRobotics/blob/master/SLAM/iterative_closest_point/iterative_closest_point.py buat jadiin R sama T
//dapetin position dalam X Y Z dan orientation dalam X Y Z W
//pake complementary filter buat fusion odom sama lidar

