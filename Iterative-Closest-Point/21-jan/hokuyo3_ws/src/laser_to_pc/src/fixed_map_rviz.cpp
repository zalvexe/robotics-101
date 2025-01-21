#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath> 

class LaserScanBroadcaster : public rclcpp::Node
{
    public: 
        LaserScanBroadcaster() : Node("laser_scan_broadcaster"){
            scan_pub = this->create_publisher<sensor_msgs::msg::LaserScan>("scan", 10);
            timer_ = this->create_wall_timer(std::chrono::milliseconds(100), std::bind(&LaserScanBroadcaster::publishScan, this));
        }

    private:
        void publishScan(){
            std::ifstream infile("laser_pointcloud.txt");
            if(!infile.is_open()){
                RCLCPP_ERROR(this->get_logger(), "Could not open file");
                return;
            }
        std::vector<float> ranges;
        std::vector<float> intensities;
        float angle_min = -1.570796;
        float angle_max = 1.570796;
        float angle_increment = 0.0174533;
        float range_max = 10.0; // Set your desired max range limit here

        std::string line;
        while(std::getline(infile, line)){
            std::istringstream iss(line);
            float x, y, z, angle;
            if(!(iss >> x >> y >> z >> angle)){
                RCLCPP_ERROR(this->get_logger(), "Error reading file");
                break;
            }
            float angle_rad = angle*(M_PI/180.0); //degree to rad
            float range = std::sqrt(x*x + y*y + z*z);

            if(ranges.empty()){
                angle_min = angle_rad;
            }
            ranges.push_back(range);
            intensities.push_back(0.0);
            if(angle_min == 0.0){
                angle_min = angle_rad;
            }
            angle_max = angle_rad;
            ranges.push_back(range);
            intensities.push_back(0.0);
        }   
            if(ranges.empty()){
                RCLCPP_WARN(this->get_logger(), "No points in file");
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
            /*MESSAGE
            std_msgs/msg/Header header
            float angle_min
            float angle_max
            float angle_increment
            float time_increment
            float scan_time
            float range_min
            float range_max
            float[] ranges
            float[] intensities
            */
           scan_pub->publish(*scan_msg);
        //    RCLCPP_INFO(this->get_logger(), "Published scan message");
        RCLCPP_INFO(this->get_logger(), "Published scan message with %zu ranges", scan_msg->ranges.size());
        }
        rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr scan_pub;
        rclcpp::TimerBase::SharedPtr timer_;
};
int main(int argc, char *argv[]){
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LaserScanBroadcaster>());
    rclcpp::shutdown();
    return 0;
}