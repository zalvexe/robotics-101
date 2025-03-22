# Iterative Closest Point - ICP

ICP, or Iterative Closest Point, is an algorithm used in computer vision and robotics with lidar for aligning two point clouds. It works by iteratively minimizing the difference between a source point cloud and a target point cloud through the following steps:   

- Matching: Find the closest points in the target cloud for each point in the source cloud.   
- Transformation: Compute the transformation (rotation and translation) that minimizes the distance between the matched points.   
- Update: Apply the transformation to the source cloud.   
- Repeat: Iterate until convergence or a specified error threshold is met.   
- ICP is commonly used in tasks like object recognition, 3D mapping, and localization  
  
![image](https://github.com/user-attachments/assets/f52bd3c9-83e7-41ef-8eb4-3ccb1c89af84)

Lidar used: Hokuyo, lslidar     
**urg node2: https://github.com/Hokuyo-aut/urg_node2.git**

## PROGRESS:
[✅] Accessing Lidar    
[✅] Publishing `/scan` from LaserScan to Pointcloud2 as XYZ   
[✅] Saving the PCL result to `pointcloud.txt` as fixed map   
[✅] Getting all lidar angle data (360 deg)  
[✅] Publishing fixed map to rviz  
[✅] Implementing ICP calculation   
[✅] Calculating R and T data and publishing them   
[✅] Testing in hokuyo lidar   
[✅] Quarternion localization (x,y,z,w)      
[✅] Euler localization (x,y,yaw)     
[✅] Complementary fusion with Odometry   
[✅] Tryin out irl robot  
## ---------------------------------------------- '_' ----------------------------------------------------
### Notes on 7 Dec 24:  
Implemented program in Lslidar N10_P. Some problem faced (and solved):     
- Issue: Received only ~270 LiDAR angle data points.  
  - **Solution**: Limited the `max_range` to 10 meters.  
- Issue: Mismatched LiDAR driver, resulting in incorrect scan results.  
  - **Solution**: Updated the YAML parameters from `N10` to `N10_P`.

### Summary on 21 Jan 2025:
- ```laser_to_pointcloud.cpp```: Converts laser scan data to a point cloud and publishes it.    
- ```fixed_map_rviz.cpp```: Reads a fixed map from a file, converts it to a LaserScan message, and publishes it.    
- ```icp_publish.cpp```: Subscribes to both the current LiDAR scan data and the fixed map, performs ICP alignment, and publishes the aligned point cloud.    


### Summary on 4 Feb 2025
- icp converged, rviz:

https://github.com/user-attachments/assets/0fc575ad-c8ee-4c1c-ab90-5dc0eab581e5

  ![Screenshot from 2025-02-03 14-32-34](https://github.com/user-attachments/assets/bbac5646-536e-45eb-8464-e61868155efc)
> red: fixed map   
> white: current lidar scan    
> green: icp pointcloud    

### Summary on 20 feb 2025
- Quarternion pose:
![Screenshot from 2025-02-18 15-13-18](https://github.com/user-attachments/assets/cf4909f4-cec2-4544-963c-d4a324864a36)
- Turns quarternion into euler and matched it with irl robot coordinate
- Complementary filter (fuse with odom pose)  
![image](https://github.com/user-attachments/assets/ef026430-f104-4749-8e68-f400ec60e03c)

(+) PCL directory problem:
```
CMake Error at CMakeLists.txt:20 (find_package):
  By not providing "Findpcl.cmake" in CMAKE_MODULE_PATH this project has
  asked CMake to find a package configuration file provided by "pcl", but
  CMake did not find one.

  Could not find a package configuration file provided by "pcl" with any of
  the following names:

    pclConfig.cmake
    pcl-config.cmake

  Add the installation prefix of "pcl" to CMAKE_PREFIX_PATH or set "pcl_DIR"
  to a directory containing one of the above files.  If "pcl" provides a
  separate development package or SDK, be sure it has been installed.
```
can be solved by:   
```cmake
set(pcl_DIR "/opt/ros/humble/share/perception_pcl/cmake/")
set(CMAKE_PREFIX_PATH "/opt/ros/humble/share/perception_pcl/cmake/" ${CMAKE_PREFIX_PATH}) #THE SOLVER
find_package(PCL REQUIRED)
find_package(pcl_conversions REQUIRED)
find_package(PCL REQUIRED COMPONENTS <components>)
find_package(pcl_ros REQUIRED)
```

> Note: Research on progress.. :]   
