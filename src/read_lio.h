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

using namespace std;

#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/common/transforms.h>
using namespace pcl;

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

          //#include <cartographer_ros_msgs/FinishTrajectory.h>
           //#include <cartographer_ros_msgs/FixPose.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include <Eigen/Core>
#include <Eigen/Eigen>
#include <Eigen/Geometry>
vector<string> load_dir_files(const char *dir);
vector<sensor_msgs::Imu> load_imu_data(const char * imufile, double first_timestamp);
//sensor_msgs::LaserScan pointcloud_to_laserscan(pcl::PointCloud<PointXYZI>::Ptr cloud);