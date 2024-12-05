# Iterative Closest Point - ICP

ICP, or Iterative Closest Point, is an algorithm used in computer vision and robotics with lidar for aligning two point clouds. It works by iteratively minimizing the difference between a source point cloud and a target point cloud through the following steps:   

- Matching: Find the closest points in the target cloud for each point in the source cloud.   
- Transformation: Compute the transformation (rotation and translation) that minimizes the distance between the matched points.   
- Update: Apply the transformation to the source cloud.   
- Repeat: Iterate until convergence or a specified error threshold is met.   
- ICP is commonly used in tasks like object recognition, 3D mapping, and localization  
  
![image](https://github.com/user-attachments/assets/f52bd3c9-83e7-41ef-8eb4-3ccb1c89af84)

Lidar used: Hokuyo  
Notes: Research on progress.. :] About to implementing the calculation from icp xyz to Rotation and Translation..   


