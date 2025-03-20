# Personal Documentation of Livox MID360

## Important Links
1. Read datasheet (for power supply, ethernet, Livox Viewer 2)   
   https://terra-1-g.djicdn.com/851d20f7b9f64838a34cd02351370894/Livox/Livox_Mid-360_User_Manual_EN.pdf 
2. To access more of this lidar, follow this (Like really, just follow all this step)   
   https://github.com/Livox-SDK/Livox-SDK2/blob/master/README.md
3. Configuring with ros 2    
   https://github.com/Livox-SDK/livox_ros_driver2
      > Note: you'll get somekind of stderr when building the pkg but its not a necessary error, just ignore it
   ```
    Starting >>> livox_ros_driver2
    --- stderr: livox_ros_driver2                                
    /usr/include/apr-1.0
    apr-1
    ---
    Finished <<< livox_ros_driver2 [10.6s]

    Summary: 1 package finished [10.8s]
    1 package had stderr output: livox_ros_driver2
    ```

## How to Access
1. Turn on lidar
   Supply: 12V, 0.45 A
2. Check its ethernet interface with ```ifconfig```    
   <img src="https://github.com/user-attachments/assets/37026e04-8c4a-4851-83a7-0db9d751cb24" width="480">   
3. To find the ip, you can use Wireshark or nmap    
   <img src="https://github.com/user-attachments/assets/84d622ca-859b-4503-b2ff-97154aea3c7d" width="480">   
4. Based on datasheet, we need to make our pc's ip static in ```192.168.1.50``` BUT in my case, the wireshark capture an ip of ```192.168.2.50``` so i changed the subnet from 1 into 2. For this, i used this command    
   ```
   sudo ip addr add 192.168.2.100/24 dev enx68e43b30881a
   ```
5. For lidar's ip, check the serial number (near a QR code), ex:  ```47MDMXXXX273```, the last 2 digits you can add to ```192.168.1.1XX```, but again wireshark showed that the subnet is 2, so my lidar's ip would be
   ```192.168.2.173```
then to make sure you can access the lidar, try pinging it   
   ```
   ping 192.168.2.173
   ```
6. To build the ROS pkg, you need to change few config:
   - ```MID360_config.json```
     
     ```json
     {
      "lidar_summary_info" : {
        "lidar_type": 8
      },
      "MID360": {
        "lidar_net_info" : {
          "cmd_data_port": 56100,
          "push_msg_port": 56200,
          "point_data_port": 56300,
          "imu_data_port": 56400,
          "log_data_port": 56500
        },
        "host_net_info" : {
          "cmd_data_ip" : "192.168.2.50",
          "cmd_data_port": 56101,
          "push_msg_ip": "192.168.2.50",
          "push_msg_port": 56201,
          "point_data_ip": "192.168.2.50",
          "point_data_port": 56301,
          "imu_data_ip" : "192.168.2.50",
          "imu_data_port": 56401,
          "log_data_ip" : "",
          "log_data_port": 56501
        }
      },
      "lidar_configs" : [
        {
          "ip" : "192.168.2.173",
      ```
- ```msg_MID360_launch.py```
  
    ```py
    cmdline_bd_code = '47MDM6E0020273'
    ```
- ```rviz_MID360_launch.py```
  
    ```py
    cmdline_bd_code = '47MDM6E0020273'
    ```
7. Make the lidar ip static in Wired Connection setting:
   - Ip4 :
     - Ip Address: 192.168.2.50
     - Netmask: 255.255.255.0
     - Gateaway: 192.168.2.50

8. Launch ROS:
   - cd to livox workspace (ws_livox)    
   - source while on that workspace   
     ```source ./install/setup.zsh```   
   - ```ros2 launch livox_ros_driver2 msg_MID360_launch.py```
   - ```ros2 launch livox_ros_driver2 rviz_MID360_launch.py```
  
### Screenshots
- Livox Viewer 2    
  <img src="https://github.com/user-attachments/assets/316525d1-12c3-4afb-9bb5-ef0cd835a185" width="480">    
- Rviz2    
  <img src="https://github.com/user-attachments/assets/171fed84-df33-4b34-9bcd-4e10376d5b2c" width="480">


