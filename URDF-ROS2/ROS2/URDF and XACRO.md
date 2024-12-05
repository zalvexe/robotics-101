# URDF 🤯
## Joints and Links  
URDF will later be published with ```robot_state_publisher``` node and can be use in topic ```/robot_description```
Later in URDF (xml file) it will contains tags for robot description.  
The very basic tags include :

```xacro
<visual> <!--(for robot model) : -->
  <geometry>
  <origin>
  <material>
</visual>  

<collision>
  <geometry>
  <origin>
<!-- the geometry and origin usually just the same as visual, but for more complex visual you can just use a box to represent the model.  -->
</collision>   
    
<inertial> (how the link will respond)
  <mass> (mass of the link)
  <origin> (center of mass)
  <inertia> (how the mass distribution will effect), using ixx,ixy,iyy,iyz,izz 
</inertial>

```

### Joints

Joint types :
- fixed : static  
- revolute : like a servo, has limit
- continuous : like a motor
- prismatic : linear  
```xacro
<joint name = "arm_joint" type="revolute"> 
  <parent link="slider_link" />
  <child link="arm_link"/>
  <origin xyz="0.1 0.15 0.4" rpy="0 0 0" /> (rpy = roll pitch yaw)
  (origin = position of child to parent link before any motion applied (origin pos))

 ---------- thats it for fixed joint, below is an extra for non fixed joint ---
  <axis xyz="0 -1 0" />
  <limit lower="0" upper="${pi/2}" velocity="100" effort="100" />
</joint>
```

# XACRO
! important to use
```xacro
<?xml version="1.0"?>
<robot xmlns:xacro="http://www.ros.org/wiki/xacro">
  <xacro:include filename="my_materials.xacro" /> (to add another xacro file)
  <link></link>
  <joint></joint>
  <link></link>
  <joint></joint>
  -- and add another xacro file again if needed
```
(ye i was too lazy to write this down, but this is actually sooo important  
![Screenshot from 2024-07-16 06-43-28](https://github.com/user-attachments/assets/805730e7-96de-4822-944d-a3e22f51a90a)

## HOW TO RUN XACRO FILE IN RVIZ
- ```$ roscore```
- sourcing ROS 2 :
  ```source /opt/ros/galactic/setup.zsh``` (for zsh)  
- ```$ rviz2```
- ros controller publisher (if needed)  
   ```$ ros2 run joint_state_publisher_gui joint_state_publisher_gui```
- ```$ ros2 run robot_state_publisher robot_state_publisher --ros-args -p robot_description:="$(xacro ~robot_urdf.urdf.xacro)"```
### IN RVIZ :
- check the TF option
- in Description Topic add /robot_description to show the model  
this is how it should be looks like
![Screenshot from 2024-07-16 19-13-39](https://github.com/user-attachments/assets/18e045a2-5302-4c24-a5f4-47ce9374f0b2)

### THIS MIGHT HELP
https://ni.www.techfak.uni-bielefeld.de/files/URDF-XACRO.pdf
> to be continued..

  
