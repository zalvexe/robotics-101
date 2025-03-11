# Personal Documentation of SLAM (FAST-LIO)
SLAM (Simultaneous Localization And Mapping) is a computational technique that enables robots or autonomous devices to create a map of an unknown environment while simultaneously tracking their own position within that environment.    

### Lidar used: Livox MID-360   

<img src="https://github.com/user-attachments/assets/0f987d5a-3b17-435a-869b-9666af51bc0a" width="250">

### Sources:
- FAST LIO ROS2: https://github.com/Ericsii/FAST_LIO_ROS2
- Livox driver 2: https://github.com/Livox-SDK/livox_ros_driver2

### How to?
1. Just follow every instruction on fast-lio github, its pretty straightforward
2. launch msg from livox driver
```
ros2 launch livox_ros_driver2 msg_MID360_launch.py
```
3. launch fast-lio
```
ros2 launch fast_lio mapping.launch.py config_file:=mid360.yaml
```
4. Put the lidar in a static position and just move the robot base normally
> ITS robotic area mapping (2x speed)


https://github.com/user-attachments/assets/44490f40-1ef2-418b-a967-a85b26e160d4


<img src="https://github.com/user-attachments/assets/6f6f40ae-e5ac-45e6-b54c-0e6e53fcefe4" width="480"> 


6. how to save pcd:    
```
ros2 service call /map_save std_srvs/srv/Trigger
```
it'll be saved in ws/test.pcd   

6. open pcd file with pcl_viewer
```
pcl_viewer test.pcd
```
