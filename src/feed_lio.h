#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <thread>


#include <ros/ros.h>
#include <std_msgs/String.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/LaserScan.h>
#include <rosgraph_msgs/Clock.h>
#include <geometry_msgs/Vector3.h>
#include <tf/transform_broadcaster.h>
#include <pcl_conversions/pcl_conversions.h>
#include <Eigen/Core>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
    //#include <cartographer_ros_msgs/FinishTrajectory.h>
    //#include <cartographer_ros_msgs/FixPose.h>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <pcl/filters/voxel_grid.h>

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <nav_msgs/Odometry.h>
#include <nav_msgs/Path.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Quaternion.h>
#include <geometry_msgs/PoseStamped.h>
#include <tf/transform_broadcaster.h>

#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/common/transforms.h>

using namespace std;
using namespace pcl;