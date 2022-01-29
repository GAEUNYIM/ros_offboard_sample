# ros_offboard_sample
The 'ros_offboard_sample' package firstly made for simple offboarding c++ code. And now it is currently being updated with new functions which can shot pictures through automated control system.
#### ver1.0.0
Your drone in gazebo simulation will be set to the certain point, (0, 0, 2).
#### ver1.0.1
Now your new drone has a fpv camera. 
#### ver1.0.2
It takes 5 pictures through the attatched cam by increasing 1 units on x-axis from the initial position (4, 0, 2).

## Prerequisites
All requirements should be installed inside Docker.
- gazebo
- px4
- ROS
- QGC

## How to simulate?
To open multiple Dokcer terminal, you can use this commands to bash the docker host. In my case, my docker container has the name 'exciting_cartwright'. You need to replace it with your own docker container's name. Each terminal should be opedn by this command below.
```
  docker exec -it exciting_cartwright bash
```
### Terminal 1 - Run PX4
```
  ./px4_run.sh
```

### Terminal 2 - Run QGC
```
  ./QGroundControl.AppImage 
```

### Terminal 3 - Integrated roslaunch for mavros and sample code
```
  roslaunch ros_offboard_sample my_mavros
```

### TODO : arming & Off-boarding
You need to prepare the first drone setup by arming and changing option to off-board through the QGC GUI.

## Captures
Video_ver1.0.0
https://user-images.githubusercontent.com/59522019/148004927-353244bd-b541-4727-8563-5a222355faed.mp4

Video_ver1.0.1
https://user-images.githubusercontent.com/59522019/149767535-f14935c3-fff0-4c8e-acbb-8508a884b36d.mp4


## Further TODO?
Handling the captured images by the ros_cv_pkg.

## Refereneces
https://docs.px4.io/master/en/ros/mavros_offboard.html
