# Personal Documentation of DLIO SLAM
DLIO (Direct LiDAR-Inertial Odometry) is a SLAM algorithm that fuses LiDAR and IMU data to estimate the robot's pose in real time. Unlike traditional SLAM methods that rely on scan-matching, DLIO uses direct point cloud alignment and IMU preintegration for improved robustness and efficiency.   

## Sources
- DLIO ROS 2 Package: https://github.com/vectr-ucla/direct_lidar_inertial_odometry/tree/feature/ros2
- Livox SDK: https://github.com/Livox-SDK/livox_ros_driver2

## Livox MID360
<img src="https://github.com/user-attachments/assets/67184c80-0762-4687-a051-c554beff52eb" width="300">

## How To
1. Follow all instructions in DLIO README until the package built succesfully
2. Before running the launch file, make sure to launch the livox launch:
```
ros2 launch livox_ros_driver2 rviz_MID360_launch.py
```
and 
```
ros2 launch livox_ros_driver2 msg_MID360_launch.py
```
then we can launch  
```
ros2 launch direct_lidar_inertial_odometry dlio.launch.py rviz:=true pointcloud_topic:=/livox/lidar imu_topic:=/livox/imu
```
yes, change the pointcloud topic and imu topic based on what the lidar publishes, for livox its /livox/lidar and /livox/imu  

What cool about this package is how it'll callibrate the imu with lidar for the first 3 seconds before it starts the SLAM. Stuff that will show up in terminal if you're succeed is   
<img src="https://github.com/user-attachments/assets/42f015e5-31c0-4d00-9c23-719014fa8bd7" width="680">

**Here is the mapping result of ITS Robotics (speed 22x)**    


https://github.com/user-attachments/assets/b18fa7cc-39c7-4905-ad1a-c79553003aab



<img src="https://github.com/user-attachments/assets/7541b98d-8ea4-47c7-81c0-f6d235705db8" width="680">

## Summary
- The longer the lidar stays in one point, the mapping scan result around that point will be clearer
- Localization and IMU were reallllyy accurate
- Comparing with FAST-LIO, this DLIO gives more accurate and more detailed mapping result
- Still need to find out how to save the scanned map, cuz the service call didnt work


