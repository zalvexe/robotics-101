#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include <vector>
#include <string>
#include <cmath> 
#include <boost/asio.hpp> 

using boost::asio::ip::tcp;

class WS20PCDDriver : public rclcpp::Node
{
public:
    WS20PCDDriver() : Node("ws20_pcd_driver"), io_service_(), socket_(io_service_)
    {
        scan_pub_ = this->create_publisher<sensor_msgs::msg::LaserScan>("scan", 10);
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100), std::bind(&WS20PCDDriver::publishScan, this));
        
        try
        {
            tcp::resolver resolver(io_service_);
            tcp::resolver::query query("192.168.1.10", "10940"); // IP SAMA PORT DAPET DARI MANA ;_;
            tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
            boost::asio::connect(socket_, endpoint_iterator);
        }
        catch (boost::system::system_error& e)
        {
            RCLCPP_ERROR(this->get_logger(), "Failed to connect to LiDAR: %s", e.what());
            rclcpp::shutdown();
        }
    }

private:
    void publishScan()
    {
        if (!socket_.is_open())
        {
            RCLCPP_ERROR(this->get_logger(), "Socket not open");
            return;
        }
        std::vector<char> buffer(1024);
        boost::system::error_code error;
        size_t len = socket_.read_some(boost::asio::buffer(buffer), error);

        if (error == boost::asio::error::eof)
        {
            RCLCPP_ERROR(this->get_logger(), "Connection closed by LiDAR");
            return;
        }
        else if (error)
        {
            RCLCPP_ERROR(this->get_logger(), "Error reading from LiDAR: %s", error.message().c_str());
            return;
        }

        // Parse the data according to the LiDAR's communication protocol
        std::vector<float> ranges;
        std::vector<float> intensities;
        float angle_min = -M_PI;
        float angle_max = M_PI;
        float angle_increment = 0.0174533; 
        float range_max = 10.0; 

        std::istringstream iss(std::string(buffer.data(), len));
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
        scan_msg->scan_time = 0.1;
        scan_msg->range_min = 0.0;
        scan_msg->range_max = range_max;
        scan_msg->ranges = ranges;
        scan_msg->intensities = intensities;

        scan_pub_->publish(*scan_msg);
        RCLCPP_INFO(this->get_logger(), "Published LaserScan message with %zu ranges", ranges.size());
    }

    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr scan_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
    boost::asio::io_service io_service_;
    tcp::socket socket_;
};

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<WS20PCDDriver>());
    rclcpp::shutdown();
    return 0;
}