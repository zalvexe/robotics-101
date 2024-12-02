# Iterative Closest Point - ICP

ICP, or Iterative Closest Point, is an algorithm used in computer vision and robotics for aligning two point clouds. It works by iteratively minimizing the difference between a source point cloud and a target point cloud through the following steps:   

- Matching: Find the closest points in the target cloud for each point in the source cloud.   
- Transformation: Compute the transformation (rotation and translation) that minimizes the distance between the matched points.   
- Update: Apply the transformation to the source cloud.   
- Repeat: Iterate until convergence or a specified error threshold is met.   
- ICP is commonly used in tasks like object recognition, 3D mapping, and localization.

Jadi singkatnya, ICP itu buat ngehubungin point" hasil scan lidar dan disesuaiin dengan Fixed Map.  

## STEP:
1. Subscribe ke /laserscan buat dapetin hasil scan lidar (ini datanya masih dalam bentuk angle dan jaraknya).   
   dari data itu, kita perlu ubah ke PointCloud (XYZ)  
2. Buat dapetin data pointcloud, kita publish laserscan dengan bentuk pointcloud dari msg sensor_msgs    
3. Dari data pointcloud, kita pake perhitungan dari [orang ini](https://github.com/AtsushiSakai/PythonRobotics/blob/master/SLAM/iterative_closest_point/iterative_closest_point.py)
   atau pake library PCL. Dari sini, kita dapetin bentuk R (rotasi, XYZW) dan T (Translasi, XYZ (ini udah dari pointcloud tadi))
4. Kalo udah, kita coba fusion odometry dengan lidar, mau coba pake Complementary Filter dulu (yg paling gampang.. katanya..)


### catatan: riset ini masih ongoing, mungkin ada penjelasan/step yg kurang masih.. :]
     

