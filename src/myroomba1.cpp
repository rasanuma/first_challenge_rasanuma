#include<myroomba1.h>

Room::Room():private_nh("~")
{
    private_nh.param("hz",hz,{10});

    dt = 1/float(hz);

    sub_pose = nh.subscribe("roomba/odometry",10,&Room::pose_callback,this);

    pub_twist = nh.advertise<roomba_500driver_meiji::RoombaCtrl>("/roomba/control",1);

    longsize_ = 0;

    kyori = 1;
}

void Room::pose_callback(const nav_msgs::Odometry::ConstPtr &msg)
{
    now_pose = *msg;
}

void Room::roomba()
{
    double roll = 0;
    double pitch = 0;
    double yaw =0;
    longsize_ = now_pose.pose.pose.position.x;
    roomba_500driver_meiji::RoombaCtrl v;
    time += dt;
    //theta += v.angular.z;
    v.cntl.linear.x = 0.1;
    //printf("%f\n", v.cntl.linear.x);
    if(longsize_>=kyori && !(theta >= 2*M_PI)) {
        v.cntl.linear.x = 0;
        tf::Quaternion quat(now_pose.pose.pose.orientation.x,now_pose.pose.pose.orientation.y,now_pose.pose.pose.orientation.z,now_pose.pose.pose.orientation.w);
        tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
        //v.cntl.angular.z =M_PI/10;
        //Room::theta += dt*v.cntl.angular.z;
        if(theta >= 2*M_PI){
            v.cntl.linear.x = 0.1;
            //time=0;
            //theta=0;
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
