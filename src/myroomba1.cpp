#include<myroomba1.h>

Room::Room():private_nh("~")
{
    private_nh.param("hz",hz,{10});

    dt = 1/float(hz);

    sub_pose = nh.subscribe("roomba/odometry",10,&Room::pose_callback,this);

    sub_range = nh.subscribe("/scan",10,&Room::ranges_callback,this);

    pub_twist = nh.advertise<roomba_500driver_meiji::RoombaCtrl>("/roomba/control",1);

    longsize_ = 0;
    distance = 1;
    stop_distance = 0.5;
}

void Room::pose_callback(const nav_msgs::Odometry::ConstPtr &msg)
{
    now_pose = *msg;
}

void Room::ranges_callback(const sensor_msgs::LaserScan::ConstPtr &msg)
{
    ranges = *msg;
}

void Room::roomba()
{
    int check;
    double roll = 0;
    double pitch = 0;
    double yaw =0;
    longsize_ = now_pose.pose.pose.position.x;
    roomba_500driver_meiji::RoombaCtrl v;
    time += dt;

    v.cntl.linear.x = 0.2;
    //printf("%f\n", v.cntl.linear.x);
    if(longsize_>=distance) {
        if(-0.1<=theta&&theta <= -0.01) {
            check = 0;
            v.cntl.linear.x = 0.2;
            v.cntl.angular.z = 0;
        }
        else {
            check = 1;
            v.cntl.linear.x = 0;
            v.cntl.angular.z =0.2;
            tf::Quaternion quat(now_pose.pose.pose.orientation.x,now_pose.pose.pose.orientation.y,now_pose.pose.pose.orientation.z,now_pose.pose.pose.orientation.w);
            tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
        //v.cntl.angular.z =M_PI/10;
        //Room::theta += dt*v.cntl.angular.z;
            theta = float(yaw);
        }
    }
    if(ranges.ranges.size()>0) {
            printf("%f\n", ranges.ranges[ranges.ranges.size()/2]);
        if((ranges.ranges[ranges.ranges.size()/2])<=stop_distance && check != 1) {
            v.cntl.linear.x = 0;
            longsize_ = 0;
        }
    }
    v.mode = 11;
     //printf("%f\n", theta);
pub_twist.publish(v);
}

void Room::process()
{
    ros::Rate loop_rate(hz);
    while (ros::ok())
    {
        // go_straight();
        roomba();
        ros::spinOnce();
        loop_rate.sleep();
    }
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "roomba");
    Room room;
    room.process();
    return 0;
}
