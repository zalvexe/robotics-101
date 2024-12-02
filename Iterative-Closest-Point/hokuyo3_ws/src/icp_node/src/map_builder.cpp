#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/filters/voxel_grid.h>

class MapBuilderNode : public rclcpp::Node {
public:
    MapBuilderNode() : Node("map_builder_node"), aggregated_cloud_(new pcl::PointCloud<pcl::PointXYZ>) {
        pcl_subs_ = this->create_subscription<sensor_msgs::msg::PointCloud2>(
            "/laser_pointcloud", 10, std::bind(&MapBuilderNode::pointcloud_callback, this, std::placeholders::_1));
        timer_ = this->create_wall_timer(
            std::chrono::seconds(10), std::bind(&MapBuilderNode::save_map, this));
    }

private:
    void pointcloud_callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg) {
        pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
        pcl::fromROSMsg(*msg, *cloud);

        // Apply a voxel grid filter to downsample the point cloud
        pcl::VoxelGrid<pcl::PointXYZ> voxel_grid;
        voxel_grid.setInputCloud(cloud);
        voxel_grid.setLeafSize(0.1f, 0.1f, 0.1f);
        pcl::PointCloud<pcl::PointXYZ>::Ptr filtered_cloud(new pcl::PointCloud<pcl::PointXYZ>);
        voxel_grid.filter(*filtered_cloud);

        // Aggregate the filtered point cloud
        *aggregated_cloud_ += *filtered_cloud;
    }

    void save_map() {
        if (aggregated_cloud_->empty()) {
            RCLCPP_WARN(this->get_logger(), "Aggregated cloud is empty, not saving.");
            return;
        }
    
        std::string filename = "fixed_map.pcd";
        pcl::io::savePCDFileASCII(filename, *aggregated_cloud_);
        RCLCPP_INFO(this->get_logger(), "Saved aggregated cloud to %s", filename.c_str());
    }

    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr pcl_subs_;
    rclcpp::TimerBase::SharedPtr timer_;
    pcl::PointCloud<pcl::PointXYZ>::Ptr aggregated_cloud_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MapBuilderNode>());
    rclcpp::shutdown();
    return 0;
}