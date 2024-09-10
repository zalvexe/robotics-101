#!/usr/bin/env python3

import rospy
from geometry_msgs.msg import Twist
from turtlesim.srv import TeleportAbsolute, SetPen

def move_turtle(linear_speed, angular_speed, duration, description):
    vel_msg = Twist()
    vel_msg.linear.x = linear_speed
    vel_msg.angular.z = angular_speed

    rate = rospy.Rate(10)
    start_time = rospy.Time.now().to_sec()
    print(f"Starting movement: {description}")
    while rospy.Time.now().to_sec() - start_time < duration:
        velocity_publisher.publish(vel_msg)
        rate.sleep()

    vel_msg.linear.x = 0
    vel_msg.angular.z = 0
    velocity_publisher.publish(vel_msg)
    print(f"Finished movement: {description}")

def set_pen(r, g, b, width, off):
    rospy.wait_for_service('/turtle1/set_pen')
    set_pen_service = rospy.ServiceProxy('/turtle1/set_pen', SetPen)
    set_pen_service(r, g, b, width, off)

def teleport_turtle(x, y, theta, description):
    set_pen(0, 0, 0, 1, 1)  # Disable the pen
    rospy.wait_for_service('/turtle1/teleport_absolute')
    teleport_service = rospy.ServiceProxy('/turtle1/teleport_absolute', TeleportAbsolute)
    teleport_service(x, y, theta)
    set_pen(0, 0, 0, 1, 0)  # Enable the pen
    print(f"Turtle teleported: {description}")

def draw_letter_r():
    teleport_turtle(2, 2, 0, "Teleport to start drawing R")
    move_turtle(0,1.57,1.6, "rotate upward")
    move_turtle(1, 0, 2, "Drawing R: vertical line")
    move_turtle(0,-1.57,1, "rotate right")
    move_turtle(1,0,1,"up horizontal")
    move_turtle(0,-1.57,1, "rotate right")
    move_turtle(1,0,1,"down horizontal")
    move_turtle(0,-1.57,1, "rotate right")
    move_turtle(1,0,1,"left horizontal")
    move_turtle(0,1.7,1, "rotate left")
    move_turtle(1,1,1,"diagonal line")

def draw_letter_i():
    teleport_turtle(4, 4, 0, "Teleport to start drawing I")
    move_turtle(0,1.57,1.6, "rotate upward")
    move_turtle(1, 0, 2, "Drawing I: vertical line")

def draw_letter_v():
    teleport_turtle(5, 5, 0, "Teleport to start drawing V")
    move_turtle(0,2.57,1,"rotate left")
    move_turtle(1, 0, 2, "Drawing V: left diagonal")
    teleport_turtle(5, 5, 0, "Teleport to start drawing V")
    move_turtle(0,1.57,1,"rotate left")
    move_turtle(1, 0, 2, "Drawing V: left diagonal")

def draw_letter_a():
    teleport_turtle(7, 7, 0, "Teleport to start drawing A")
    move_turtle(0,-2,1,"rotate right")
    move_turtle(1, 0, 2, "Drawing V: left diagonal")
    teleport_turtle(7, 7, 0, "Teleport to start drawing A")
    move_turtle(0,-1,1,"rotate right")
    move_turtle(1, 0, 2, "Drawing V: left diagonal")

def draw_letter_l():
    teleport_turtle(8, 8, 0, "Teleport to start drawing L")
    move_turtle(0,-0.6,1, "rotate upward")
    move_turtle(1, 0, 2, "Drawing L: vertical line")
    move_turtle(0,1,1.6, "rotate upward")
    move_turtle(1, 0, 2, "Drawing L: horizontal line")
    # move_turtle(1, 0, 2, "Drawing L: vertical line")
    # move_turtle(1, 0, 1, "Drawing L: horizontal line")

def draw_text_rival():
    draw_letter_r()
    draw_letter_i()
    draw_letter_v()
    draw_letter_a()
    draw_letter_l()

if __name__ == '__main__':
    rospy.init_node('draw_text_rival', anonymous=True)
    velocity_publisher = rospy.Publisher('/turtle1/cmd_vel', Twist, queue_size=10)
    draw_text_rival()