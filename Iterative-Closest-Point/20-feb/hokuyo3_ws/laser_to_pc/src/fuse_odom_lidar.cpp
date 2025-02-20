#include "rclcpp/rclcpp.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"
#include "romusa2_messages/msg/fused_pos.hpp"
#include "romusa2_messages/msg/euler_pose.hpp"
#include "romusa2_messages/msg/global_pos.hpp"

// #include "romusa2_messages/msg/global_pos.hpp"

class ComplementaryFilterNode : public rclcpp::Node
{
public:
    ComplementaryFilterNode() : Node("complementary_filter_node")
    {
        // odom_sub = this->create_subscription<nav_msgs::msg::Odometry>(
        //     "odom", 10, std::bind(&ComplementaryFilterNode::odomCallback, this, std::placeholders::_1));  
        global_pos_sub = this->create_subscription<romusa2_messages::msg::GlobalPos>(
            "GlobalPos", 10, std::bind(&ComplementaryFilterNode::globalposCallback, this, std::placeholders::_1));           
        // icp_pose_sub = this->create_subscription<geometry_msgs::msg::PoseStamped>(
        //     "icp_pose", 10, std::bind(&ComplementaryFilterNode::icpPoseCallback, this, std::placeholders::_1));
        icp_euler_sub = this->create_subscription<romusa2_messages::msg::EulerPose>(
            "euler_pose", 10, std::bind(&ComplementaryFilterNode::icpEulerCallback, this, std::placeholders::_1));
        // fused_pose_pub = this->create_publisher<geometry_msgs::msg::PoseStamped>("fused_pose", 10);
        fused_pose_pub = this->create_publisher<romusa2_messages::msg::FusedPos>("fused_position",10);
        
    }

private:
    // rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub;
    // rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr icp_pose_sub;
    // rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr fused_pose_pub;
    rclcpp::Publisher<romusa2_messages::msg::FusedPos>::SharedPtr fused_pose_pub;
    rclcpp::Subscription<romusa2_messages::msg::EulerPose>::SharedPtr icp_euler_sub;
    rclcpp::Subscription<romusa2_messages::msg::GlobalPos>::SharedPtr global_pos_sub;

    double odom_pos_x, odom_pos_y, odom_pos_z;
    double odom_roll, odom_pitch, odom_yaw;
    bool odom_data_received_ = false;

    double icp_pos_x, icp_pos_y, icp_pos_z;
    double icp_roll, icp_pitch, icp_yaw;
    bool icp_data_received_ = false;

    double euler_x, euler_y, euler_yaw;
    bool euler_data_received_ = false;

    double global_x, global_y, global_yaw;
    bool global_data_received = false;

    // void odomCallback(const nav_msgs::msg::Odometry::SharedPtr msg)
    // {
    //     odom_pos_x = msg->pose.pose.position.x;
    //     odom_pos_y = msg->pose.pose.position.y;
    //     odom_pos_z = msg->pose.pose.position.z;

    //     tf2::Quaternion q(
    //         msg->pose.pose.orientation.x,
    //         msg->pose.pose.orientation.y,
    //         msg->pose.pose.orientation.z,
    //         msg->pose.pose.orientation.w);
    //     tf2::Matrix3x3(q).getRPY(odom_roll, odom_pitch, odom_yaw);

    //     odom_data_received_ = true;
    //     // fuseData();
    // }

    void globalposCallback(const romusa2_messages::msg::GlobalPos::SharedPtr msg){
        global_x = msg->pos_x;
        global_y = msg->pos_y;
        global_yaw = msg->pos_z;
        global_data_received = true;
        fuseData();
    }

    void icpEulerCallback(const romusa2_messages::msg::EulerPose::SharedPtr msg)
    {
        euler_x = msg->x;
        euler_y = msg->y;
        euler_yaw = msg->yaw;

        euler_data_received_ = true;
        fuseData();
    }

    // void icpPoseCallback(const geometry_msgs::msg::PoseStamped::SharedPtr msg)
    // {
    //     icp_pos_x = msg->pose.position.x;
    //     icp_pos_y = msg->pose.position.y;
    //     icp_pos_z = msg->pose.position.z;

    //     tf2::Quaternion q(
    //         msg->pose.orientation.x,
    //         msg->pose.orientation.y,
    //         msg->pose.orientation.z,
    //         msg->pose.orientation.w);
    //     tf2::Matrix3x3(q).getRPY(icp_roll, icp_pitch, icp_yaw);

    //     icp_data_received_ = true;
    //     // fuseData();
    // }

    void fuseData()
    {
        // RCLCPP_INFO(this->get_logger(), "Fusing data...");
        if (global_data_received && euler_data_received_)
        {
            RCLCPP_INFO(this->get_logger(), "Data received...");
            float alpha = 0.98;

            float fused_pos_x = alpha * global_x + (1 - alpha) * euler_x;
            float fused_pos_y = alpha * global_y + (1 - alpha) * euler_y;
            float fused_yaw = alpha * global_yaw + (1 - alpha) * euler_yaw;

            // tf2::Quaternion fused_q;
            // fused_q.setRPY(0, 0, fused_yaw);

            // geometry_msgs::msg::PoseStamped fused_pose_msg;
            romusa2_messages::msg::FusedPos fused_position_msg;

            // fused_pose_msg.header.stamp = this->get_clock()->now();
            // fused_pose_msg.header.frame_id = "laser";
            // fused_pose_msg.pose.position.x = fused_pos_x;
            // fused_pose_msg.pose.position.y = fused_pos_y;
            // fused_pose_msg.pose.position.z = fused_pos_z;
            // fused_pose_msg.pose.orientation.x = fused_q.x();
            // fused_pose_msg.pose.orientation.y = fused_q.y();
            // fused_pose_msg.pose.orientation.z = fused_q.z();
            // fused_pose_msg.pose.orientation.w = fused_q.w();
            fused_position_msg.fused_x = fused_pos_x;
            fused_position_msg.fused_y = fused_pos_y;
            fused_position_msg.fused_yaw = fused_yaw;
            fused_pose_pub->publish(fused_position_msg);

            RCLCPP_INFO(this->get_logger(), "FUSED POS: [%f, %f, %f]", fused_pos_x, fused_pos_y, fused_yaw);
        }
        else{
            RCLCPP_INFO(this->get_logger(), "Data not received yet...");
        }
    }


};

int main(int argc, char **argv)
{
    RCLCPP_INFO(rclcpp::get_logger("complementary_filter_node"), "Starting complementary_filter_node...");
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ComplementaryFilterNode>());
    rclcpp::shutdown();
    return 0;
}