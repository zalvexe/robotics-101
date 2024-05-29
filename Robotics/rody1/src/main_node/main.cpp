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

int main(int argc, char **argv)
{
    ros::init(argc, argv, "controller_node");
    ros::NodeHandle nh;
    ros::Publisher pub_controller = nh.advertise<rody1_messages::Controller>("controller", 1000);
    ros::Subscriber sub = nh.subscribe("joy", 1000, node_callback);
    

    ros::spin();
    return 0;
}