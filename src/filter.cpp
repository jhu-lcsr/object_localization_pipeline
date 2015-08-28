#include <iostream>
#include <pcl/point_types.h>
#include <pcl/filters/passthrough.h>

// ros stuff
#include <sensor_msgs/PointCloud2.h>
#include <ros/ros.h>

// include to convert from messages to pointclouds and vice versa
#include <pcl_conversions/pcl_conversions.h>


ros::Publisher pub;
ros::Subscriber sub;

void callback(sensor_msgs::PointCloud2 &msg) {

  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGBA>());
  pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZRGBA>());

  pcl::PCLPointCloud2 pcl_pc;
  pcl_conversions::toPCL(pc, pcl_pc);

  pcl::fromPCLPointCloud2(pcl_pc, *cloud);

  if( scene_pc->empty() == true ) {
    ROS_ERROR("Recieved empty point cloud message!");
    return;
  }

  std::cerr << "Cloud before filtering: " << std::endl;
  for (size_t i = 0; i < cloud->points.size (); ++i)
    std::cerr << "    " << cloud->points[i].x << " " 
                        << cloud->points[i].y << " " 
                        << cloud->points[i].z << std::endl;

  // Create the filtering object
  pcl::PassThrough<pcl::PointXYZRGBA> pass;
  pass.setInputCloud (cloud);
  pass.setFilterFieldName ("z");
  pass.setFilterLimits (0.0, 1.0);
  //pass.setFilterLimitsNegative (true);
  pass.filter (*cloud_filtered);

  std::cerr << "Cloud after filtering: " << std::endl;
  for (size_t i = 0; i < cloud_filtered->points.size (); ++i)
    std::cerr << "    " << cloud_filtered->points[i].x << " " 
                        << cloud_filtered->points[i].y << " " 
                        << cloud_filtered->points[i].z << std::endl;

    sensor_msgs::PointCloud2 output_msg;
    toROSMsg(*clout_filtered,output_msg);
    output_msg.header.frame_id = pc.header.frame_id;
    pub.publish(output_msg);
}

int main (int argc, char** argv) {

  ros::init("pc_filter_node",argc,argv);
  ros::NodeHandle nh;

  sub = nh.subscribe("points_in",1,callback);
  pub = nh.advertise<sensor_msgs::PointCloud2>("points_out");

  ros::spin();

  return (0);
}