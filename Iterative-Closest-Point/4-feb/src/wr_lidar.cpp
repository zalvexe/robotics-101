#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include <vector>
#include <string>
#include <cmath>
#include <serial/serial.h> 

class WS20PCDDriver : public rclcpp::Node
{
public:
    WS20PCDDriver() : Node("ws20_pcd_driver")
    {
        scan_pub_ = this->create_publisher<sensor_msgs::msg::LaserScan>("scan", 10);
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100), std::bind(&WS20PCDDriver::publishScan, this));
        
        // Initialize the serial connection
        try
        {
            serial_.setPort("/dev/ttyUSB0");
            serial_.setBaudrate(115200);
            serial::Timeout to = serial::Timeout::simpleTimeout(1000);
            serial_.setTimeout(to);
            serial_.open();
        }
        catch (serial::IOException& e)
        {
            RCLCPP_ERROR(this->get_logger(), "Unable to open port");
            rclcpp::shutdown();
        }

        if (serial_.isOpen())
        {
            RCLCPP_INFO(this->get_logger(), "Serial port initialized");
        }
        else
        {
            RCLCPP_ERROR(this->get_logger(), "Serial port not opened");
            rclcpp::shutdown();
        }
    }

private:
    void publishScan()
    {
        if (!serial_.isOpen())
        {
            RCLCPP_ERROR(this->get_logger(), "Serial port not open");
            return;
        }

        // Read data from the LiDAR
        std::string data;
        serial_.readline(data);

        // Parse the data according to the LiDAR's communication protocol
        std::vector<float> ranges;
        std::vector<float> intensities;
        float angle_min = -M_PI;
        float angle_max = M_PI;
        float angle_increment = 0.0174533; // Example value, adjust as needed
        float range_max = 10.0; // Set your desired max range limit here

        // Example parsing logic (adjust according to the actual protocol)
        std::istringstream iss(data);
        float range, intensity;
        while (iss >> range >> intensity)
        {
            ranges.push_back(range);
            intensities.push_back(intensity);
        }

        if (ranges.empty())
        {
            RCLCPP_WARN(this->get_logger(), "No data found from WS 20 PCD LiDAR");
            return;
        }

        angle_increment = (angle_max - angle_min) / (ranges.size() - 1);

        auto scan_msg = std::make_shared<sensor_msgs::msg::LaserScan>();
        scan_msg->header.stamp = this->now();
        scan_msg->header.frame_id = "laser_frame";
        scan_msg->angle_min = angle_min;
        scan_msg->angle_max = angle_max;
        scan_msg->angle_increment = angle_increment;
        scan_msg->time_increment = 0.0;
        scan_msg->scan_time = 0.1; // Assuming a scan time of 0.1 seconds
        scan_msg->range_min = 0.0;
        scan_msg->range_max = range_max;
        scan_msg->ranges = ranges;
        scan_msg->intensities = intensities;

        scan_pub_->publish(*scan_msg);
        RCLCPP_INFO(this->get_logger(), "Published LaserScan message with %zu ranges", ranges.size());
    }

    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr scan_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    serial::Serial serial_; // Serial connection
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<WS20PCDDriver>());
    rclcpp::shutdown();
    return 0;
}