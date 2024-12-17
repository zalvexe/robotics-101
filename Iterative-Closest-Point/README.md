# Iterative Closest Point - ICP

ICP, or Iterative Closest Point, is an algorithm used in computer vision and robotics with lidar for aligning two point clouds. It works by iteratively minimizing the difference between a source point cloud and a target point cloud through the following steps:   

- Matching: Find the closest points in the target cloud for each point in the source cloud.   
- Transformation: Compute the transformation (rotation and translation) that minimizes the distance between the matched points.   
- Update: Apply the transformation to the source cloud.   
- Repeat: Iterate until convergence or a specified error threshold is met.   
- ICP is commonly used in tasks like object recognition, 3D mapping, and localization  
  
![image](https://github.com/user-attachments/assets/f52bd3c9-83e7-41ef-8eb4-3ccb1c89af84)

Lidar used: Hokuyo, lslidar     

## PROGRESS:
[✅] Accessing Lidar    
[✅] Publishing `/scan` from LaserScan to Pointcloud2 as XYZ   
[✅] Saving the PCL result to `pointcloud.txt` as fixed map   
[✅] Getting all lidar angle data (360 deg)  
[✅] Publishing fixed map to rviz  
[❌] Implementing ICP calculation   
[❌] Calculating R and T data and publishing them   
[❌] Complementary fusion with Odometry   
## ---------------------------------------------- '_' ----------------------------------------------------
### Notes on 7 Dec 24:  
Implemented program in Lslidar N10_P. Some problem faced (and solved):     
- Issue: Received only ~270 LiDAR angle data points.  
  - **Solution**: Limited the `max_range` to 10 meters.  
- Issue: Mismatched LiDAR driver, resulting in incorrect scan results.  
  - **Solution**: Updated the YAML parameters from `N10` to `N10_P`.

> Notes: Research on progress.. :]   


