#include "read_lio.h"

DECLARE_bool(verbose);
DECLARE_bool(imuskip);
DECLARE_int32(start_idx);

vector<string> load_dir_files(const char *dir)
{
   //linux下目录文件信息   打开当前目录，ep readdir记录目录下所有文件信息，使用d_那么进行存储
    DIR *dp;
    struct dirent *ep;
    char filename[PATH_MAX];
    dp = opendir(dir);
    CHECK(dp != NULL) << dir << " not exists";
    vector<string> files;
    while((ep = readdir(dp)))
    {
        if(ep->d_name[0] == '.')
            continue;
        sprintf(filename, "%s/%s", dir, ep->d_name);
        files.push_back(string(filename));
    }
    closedir(dp);
    sort(files.begin(), files.end());
    return files;
}

vector<sensor_msgs::Imu> load_imu_data(const char * imufile, double first_timestamp)
{
    vector<sensor_msgs::Imu> imus;
    ifstream imuf(imufile);
    CHECK(imuf) << imufile << " not exists";
    int idx = -1;
    while(!imuf.eof())
    {
        sensor_msgs::Imu imu;
	imu.orientation_covariance={-1,-1,-1,-1,-1,-1,-1,-1,-1};
        string skip;
        double ts;
        imuf >> skip >> ts;
        ++idx;
        if(ts < first_timestamp)
        {
            ROS_WARN("ts < first_times tampskip the %d-th imu data", idx);
            continue;
        }
        float a,b,c;
	imuf >> skip
	     >> a
	     >> b
	     >> c;
        imu.header.stamp = ros::Time(ts - first_timestamp);
        imu.header.frame_id = "/base_link";
        imuf >> skip
             >> imu.angular_velocity.x
             >> imu.angular_velocity.y
             >> imu.angular_velocity.z;
        imuf >> skip
             >> imu.linear_acceleration.x
             >> imu.linear_acceleration.y
             >> imu.linear_acceleration.z;

/*        if(FLAGS_imuskip != 0 && idx % FLAGS_imuskip != 0)
        {
            ROS_WARN("skip the %d-th imu data", idx);
            continue;
        }
*/
        imus.push_back(imu);
/*
//读入imu信息第一行
//         if(FLAGS_verbose)
//             printf("imu ts: %.6f vels: %.3f %.3f %.3f accs: %.3f %.3f %.3f\n",
//                    ts,
//                    imu.angular_velocity.x,
//                    imu.angular_velocity.y,
//                    imu.angular_velocity.z,
//                    imu.linear_acceleration.x,
//                    imu.linear_acceleration.y,
//                    imu.linear_acceleration.z);
*/      
    }
    reverse(imus.begin(), imus.end());//for the pop_back in feed_lio 9999.
    return imus;
}

/*// sensor_msgs::LaserScan pointcloud_to_laserscan(pcl::PointCloud<PointXYZI>::Ptr cloud)
// {
//     sensor_msgs::LaserScan scan;
//     // scan.header.stamp = curr_clock;
//     scan.header.frame_id = "laser_local";//"horizontal_laser_link";
//     // fake data start
//     scan.angle_min = -M_PI;
//     scan.angle_max =  M_PI;
//     scan.angle_increment = M_PI / 360.0;
//     scan.time_increment = 0.0;
//     scan.range_min = 0.1;
//     scan.range_max = 16.0;
//     // fake data end
//     // hack: hide point cloud in ranges
//     scan.ranges.resize(3 * cloud->size());
//     for(size_t i = 0; i < cloud->size(); i++) {
//         scan.ranges[3*i] = cloud->points[i].x;
//         scan.ranges[3*i+1] = cloud->points[i].y;
//         scan.ranges[3*i+2] = cloud->points[i].z;
//     }
//     return scan;
// }*/