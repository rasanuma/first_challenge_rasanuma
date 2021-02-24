#ifndef MYTURTLE_H
#define MYTURTLE_H

#include<ros/ros.h>
#include "nav_msgs/Odometry.h"
#include "geometry_msgs/Twist.h"
#include "roomba_500driver_meiji/RoombaCtrl.h"
#include "tf/tf.h"

class Room
{
public:
   Room();
   void process();

private:
   void roomba();
   void pose_callback(const nav_msgs::Odometry::ConstPtr &msg);

   int hz;
   int check;
   float longsize_;
   float dt;
   float theta=0;
   float kyori;
   float time=0.0;

   ros::NodeHandle nh;
   ros::NodeHandle private_nh;
   ros::Subscriber sub_pose;
   ros::Publisher pub_twist;
   nav_msgs::Odometry now_pose;
};
#endif
