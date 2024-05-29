#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include <chrono>
#include <iostream>
// #include <libusbp.hpp>
#include <memory>
#include <streambuf>
#include <thread>
 
#include "ros/ros.h"
#include "rody1_messages/Controller.h"

// #include "rclcpp/rclcpp.hpp"
// #include "interfaces/msg/motion.hpp"
// #include "controller.h"

enum Axes {
    XL,
    YL,
    XR,
    L2_FLOAT,
    R2_FLOAT,
    YR,
    X_BTN_FLOAT,
    Y_BTN_FLOAT
};

enum Buttons {
    SQUARE,
    CROSS,
    CIRCLE,
    TRIANGLE,
    L1,
    R1,
    L2,
    R2,
    SHARE,
    OPTIONS,
    X_BTN,
    Y_BTN,
    PS
};

struct Joystick {
    float axes[8];  // Adjust the size according to the number of axes you are using
    int buttons[13];// Adjust the size according to the number of buttons you are using
};

Joystick jb;
// ros::Publisher publisher;
ros::Publisher pub_controller;
float max_linear_speed = 1.0;  // Max linear speed in m/s
float max_angular_speed = 1.0; // Max angular speed in rad/s
double map = 0.5;
void node_callback(const sensor_msgs::Joy::ConstPtr& msg)
{
    // Map the input from the joystick message to our joystick state
    jb.axes[XL] = msg->axes[0];
    jb.axes[YL] = msg->axes[1];
    jb.axes[XR] = msg->axes[2];
    jb.axes[YR] = msg->axes[3];
    jb.axes[L2_FLOAT] = msg->axes[3];
    jb.axes[R2_FLOAT] = msg->axes[4];
    jb.axes[X_BTN] = msg->axes[6];
    jb.axes[Y_BTN] = msg->axes[7];

    jb.buttons[CROSS] = msg->buttons[0];
    jb.buttons[CIRCLE] = msg->buttons[1];
    jb.buttons[TRIANGLE] = msg->buttons[2];
    jb.buttons[SQUARE] = msg->buttons[3];
    jb.buttons[L1] = msg->buttons[4];
    jb.buttons[R1] = msg->buttons[5];
    jb.buttons[L2] = msg->buttons[6];
    jb.buttons[R2] = msg->buttons[7];
    jb.buttons[SHARE] = msg->buttons[8];
    jb.buttons[OPTIONS] = msg->buttons[9];
    jb.buttons[X_BTN] = msg->buttons[10];
    jb.buttons[Y_BTN] = msg->buttons[11];
    jb.buttons[PS] = msg->buttons[12];

    geometry_msgs::Twist cmd_vel;
    rody1_messages::Controller Controller_msg;
    // cmd_vel.linear.x = jb.axes[XL] * max_linear_speed;  // Forward/backward motion
    // cmd_vel.linear.y = jb.axes[YL] * max_linear_speed;  // right/left motion
    // cmd_vel.angular.z = jb.axes[XR] * max_angular_speed;  // rotation
    if(jb.axes[XL] > 0.1 || jb.axes[XL] < -0.1)
        Controller_msg.vely = jb.axes[XL] * 100;
    else
        Controller_msg.vely = 0;

    if(jb.axes[YL] > 0.1 || jb.axes[YL] < -0.1)
        Controller_msg.velx = jb.axes[YL] * 100;
    else
        Controller_msg.velx = 0;

    if(jb.axes[YR] > 0.1 || jb.axes[YR] < -0.1)
        Controller_msg.angvel = jb.axes[YR] * 4;
    else
        Controller_msg.angvel = 0;
    // Publish the velocity commands
    pub_controller.publish(Controller_msg);

    // printf("X: %f, Y: %f, Z: %f\n", cmd_vel.linear.x, cmd_vel.linear.y, cmd_vel.angular.z);
    printf("X: %d, Y: %d, Z: %d\n", Controller_msg.velx, Controller_msg.vely, Controller_msg.angvel);
    printf("size sent : %ld\n", sizeof(Controller_msg));
}

ros::Time start_time;

int main(int argc, char **argv)
{
    ros::init(argc, argv, "Controller_node");
    ros::NodeHandle nh;
    ROS_INFO("Controller node has started.");

    // publisher = nh.advertise<interfaces::Motion>("motion_command", 10);
    // vel_pub = nh.advertise<geometry_msgs::Twist>("cmd_vel", 10);
    pub_controller = nh.advertise<rody1_messages::Controller>("Controller", 10);
    ros::Subscriber sub1 = nh.subscribe<sensor_msgs::Joy>("joy", 10,node_callback);
    // start_time = ros::Time::now();
    ros::MultiThreadedSpinner spinner(4);
    spinner.spin();

    // ros::spin();
    // ros::Timer timer = nh.createTimer(ros::Duration(0.01), robot_loop);
}

