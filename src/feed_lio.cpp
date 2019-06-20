#include "feed_lio.h"
#include "read_lio.h"

DEFINE_string(pcldir, "/home/uisee/linshu/fjy_second/lidar_20180101015812/renamed_pcd", "pcl dir");
//DEFINE_string(scandir, "", "scan dir");
DEFINE_string(imufile, "/home/uisee/linshu/fjy_second/imu-2-new.txt", "imu file");
//DEFINE_string(pcldir, "/home/uisee/linshu/diku_xiaoquan/lidar_20190530100729/renamed_pcd", "pcl dir");
//DEFINE_string(imufile, "/home/uisee/linshu/diku_xiaoquan/imu.txt", "imu file");
DEFINE_bool(verbose, true, "wtf");
DEFINE_int32(ms, 200, "sleep microseconds");
//DEFINE_int32(imuskip, 0, "send imu every N times");
DEFINE_int32(cnt, 100000, "frame limit");
DEFINE_double(yaw, -90, "yaw");
DEFINE_double(pitch, -5, "pitch");
DEFINE_double(roll, 0, "roll");
DEFINE_double(sensor_x_offset, 0, "x");
DEFINE_double(sensor_y_offset, 0.46, "y");
DEFINE_double(sensor_z_offset, 1.8, "z");
DEFINE_int32(start_idx, 0, "pcd read frame");
DEFINE_int32(end_idx, 10000, "pcd read end frame");

int main(int argc,char **argv){
    FLAGS_logtostderr = 1;
    google::InitGoogleLogging(argv[0]);
    google::ParseCommandLineFlags(&argc, &argv, true);
    
    ros::init(argc,argv,"feed_lio");
    ros::NodeHandle n;
    
    ros::Publisher clock_pub = n.advertise<rosgraph_msgs::Clock>("clock", 100);
    //ros::Publisher scan_pub = n.advertise<sensor_msgs::LaserScan>("scan", 100);
    ros::Publisher points2_pub = n.advertise<sensor_msgs::PointCloud2>("/velodyne_points", 100);
    ros::Publisher imu_pub = n.advertise<sensor_msgs::Imu>("/imu/data", 100);
    
    vector<string> pcl_files;
    static double first_timestamp = 0;
 
//1111    //load path   dir ----  pcdfiles
    if (FLAGS_pcldir[0] != '\0')
        pcl_files = load_dir_files(FLAGS_pcldir.c_str());
   // else if (FLAGS_scandir[0] != '\0')
     //   pcl_files = load_dir_files(FLAGS_scandir.c_str());
    else
        CHECK(false) << "pcldir or scandir required";
    ROS_INFO("%zu files", pcl_files.size());

    
//2222    //欧拉角转换成旋转矩阵
    Eigen::Isometry3f tran = Eigen::Isometry3f::Identity();
//     /*tran.rotate(Eigen::AngleAxisf(FLAGS_roll / 180.0 * M_PI, Eigen::Vector3f::UnitX()) *
//                 Eigen::AngleAxisf(-FLAGS_pitch / 180.0 * M_PI, Eigen::Vector3f::UnitY()) *
//                 Eigen::AngleAxisf((FLAGS_yaw-90) / 180.0 * M_PI, Eigen::Vector3f::UnitZ()));
// 
//     tran.pretranslate(Eigen::Vector3f(FLAGS_sensor_y_offset,  -FLAGS_sensor_x_offset, FLAGS_sensor_z_offset));*/
    tran.rotate(Eigen::AngleAxisf(FLAGS_roll / 180.0 * M_PI, Eigen::Vector3f::UnitX()) *
                Eigen::AngleAxisf(FLAGS_pitch / 180.0 * M_PI, Eigen::Vector3f::UnitY()) *
                Eigen::AngleAxisf((FLAGS_yaw)/ 180.0 * M_PI, Eigen::Vector3f::UnitZ()));
   // tran.pretranslate(Eigen::Vector3f(FLAGS_sensor_x_offset,  FLAGS_sensor_y_offset, FLAGS_sensor_z_offset));
    cout << "mat:" << endl << tran.matrix() << endl;   
    
//3333 // load imu data;    多余一行0数据
    vector<sensor_msgs::Imu> imus;
    if (FLAGS_imufile[0] != '\0')
    {
        ROS_INFO("loading imu data from %s", FLAGS_imufile.c_str());
        imus = load_imu_data(FLAGS_imufile.c_str(), first_timestamp);
        ROS_INFO("loaded %zu data", imus.size());
    }
    
//4444    enter control pause and continue
    size_t cnt = FLAGS_start_idx;
    //get pcd frame numbers;
    bool pause = true;
    LOG(INFO) << "[ENTER] to pause and continue.";
    boost::thread pause_th([&]() {
        while (1)
            if (getchar() == 10)
                pause = !pause;
    });
    int end_idx;
    end_idx = pcl_files.size() - 1;
/*//     if(FLAGS_end_idx > 0)
//         end_idx = FLAGS_end_idx;
//     else
//     {
//         std::cout << " end_idx less than zero" <<std::endl;
//     }*/
    std::cout << " end_idx:"<< end_idx <<std::endl;
 
        
//5555    //loop
    while(ros::ok()&& cnt< end_idx){ 
      //%zu是size_t类型
      if(pause)
	continue;
      ROS_INFO("sending %zu/%zu",cnt+1,min((size_t)FLAGS_cnt,pcl_files.size()));  //frame limit and read;
//6666 // load each pcd file,and get point cloud
      pcl::PointCloud<PointXYZI>::Ptr cloud(new PointCloud<PointXYZI>());
      CHECK(io::loadPCDFile(pcl_files[cnt], *cloud) != -1)
            << "read " << pcl_files[cnt] << " failed";
      if(FLAGS_verbose)
      {
          ROS_INFO("%zu points", cloud->size());
      }     
//7777 //transformPointCloud(input,output,tran)
      transformPointCloud(*cloud, *cloud, tran);
      
 /*  点云降采样
      pcl::PointCloud<PointXYZI>::Ptr cloud_cut(new PointCloud<PointXYZI>());
        pcl::PointCloud<PointXYZI>::Ptr gr_cloud_ptr(new PointCloud<PointXYZI>());
        pcl::PointCloud<PointXYZI>::Ptr ngr_cloud_ptr(new PointCloud<PointXYZI>());

        for(size_t i =0 ; i< cloud->size();i++)
        {
           if(cloud->points[i].z< 0.3)
               gr_cloud_ptr ->push_back(cloud->points[i]);
           else
               ngr_cloud_ptr ->push_back(cloud->points[i]);
            cloud_cut->push_back(cloud->points[i]);
        }

        pcl::PointCloud<PointXYZI>::Ptr gr_cloud(new PointCloud<PointXYZI>());
        pcl::VoxelGrid<pcl::PointXYZI> filter;

        filter.setInputCloud(gr_cloud_ptr);
        filter.setLeafSize(0.45, 0.45, 0.45);
        filter.filter(*gr_cloud);
        gr_cloud ->insert(gr_cloud->end(), ngr_cloud_ptr->begin(), ngr_cloud_ptr->end());

        cloud = cloud_cut;
//        cloud = gr_cloud ;
        ROS_INFO("cut %zu points", cloud->size());
*/	
 
//8888   //load timestamp from pcl filename
      double timestamp = atof(pcl_files[cnt].substr(pcl_files[cnt].rfind("1514")).c_str());
   //double timestamp = atof(pcl_files[cnt].substr(pcl_files[cnt].rfind("1559")).c_str());
       //返回字符串第一次出现的位置，如果没有匹配项则返回-1。rfind  substr(start,length) length 么有就是直到末尾
       //ROS_INFO("timestampt %f", timestamp);
      ros::Time curr_clock = ros::Time(timestamp - first_timestamp);
//    ROS_INFO("curr clock %f", curr_clock.toSec());
//    double timestamp = cnt / 5;
//    ros::Time curr_clock = ros::Time(timestamp);
       // publish the clock
      rosgraph_msgs::Clock clock;
      clock.clock = curr_clock;
      clock_pub.publish(clock);

         
//9999//publish imu_pub   time process???
      while(!imus.empty()&& imus.back().header.stamp <= curr_clock )
        {
           //ROS_INFO("imu %f", odoms.back().header.stamp.toSec());
            imu_pub.publish(imus.back());
	 //    ROS_INFO("%f",imus.back().angular_velocity.x);
	   //ROS_INFO("%zu",imus.size());
            imus.pop_back();	
	    if(imus.size()==1)
	      break;
        }        
     // publish point cloud
      if (FLAGS_pcldir[0] != '\0')
      {
          sensor_msgs::PointCloud2 points2;
          pcl::toROSMsg(*cloud, points2); //Convert the cloud to ROS message
          points2.header.stamp = curr_clock;
          points2.header.frame_id = "/base_link";//"horizontal_laser_link";
          points2_pub.publish(points2);
          ROS_INFO("points2 %f",points2.header.stamp.toSec());
      }
/*//       else if (FLAGS_scandir[0] != '\0')
//       {
//           sensor_msgs::LaserScan scan = pointcloud_to_laserscan(cloud);
//           scan.header.stamp = curr_clock;
//           scan_pub.publish(scan);
//       }*/
      else
          CHECK(false) << "no point cloud?";

      ros::spinOnce();
      usleep(FLAGS_ms * 1000);
      cnt++;
      if (cnt == FLAGS_cnt)
         break;
    }
}
