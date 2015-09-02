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

using namespace pcl_conversions;

// Create the filtering object
pcl::PassThrough<pcl::PCLPointCloud2> pass;

void callback(const sensor_msgs::PointCloud2 &pc) {

  pcl::PCLPointCloud2::Ptr cloud(new pcl::PCLPointCloud2);;
  pcl::PCLPointCloud2 cloud_filtered;

  pcl_conversions::toPCL(pc, *cloud);

  pass.setInputCloud (cloud);
  pass.setFilterFieldName ("z");
  pass.setFilterLimits (0.2, 2.0);
  //pass.setFilterLimitsNegative (true);
  pass.filter (cloud_filtered);

  sensor_msgs::PointCloud2 output_msg;
  pcl_conversions::moveFromPCL(cloud_filtered,output_msg);
  output_msg.header.frame_id = pc.header.frame_id;
  pub.publish(output_msg);
}

int main (int argc, char** argv) {

  ros::init(argc,argv,"pc_filter_node");
  ros::NodeHandle nh;

  sub = nh.subscribe("points_in",1,callback);
  pub = nh.advertise<sensor_msgs::PointCloud2>("points_out",1000);

  ros::spin();

  return (0);
}
