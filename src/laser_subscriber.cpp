#include "ros/subscriber.h"
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>


class LaserSubscriberNode {
    ros::NodeHandle nh_;
    ros::Subscriber sub_;
    const sensor_msgs::LaserScan *scan_ptr_;

    void laserScanCallback(const sensor_msgs::LaserScan::ConstPtr &msg) {
        scan_ptr_ = msg.get();

        if (! scan_ptr_->ranges.empty()) {
            float range = scan_ptr_->ranges[360];
            ROS_INFO("Range in front: %f", range);
        }
    }

public:
    LaserSubscriberNode() : sub_{nh_.subscribe("/kobuki/laser/scan", 10, &LaserSubscriberNode::laserScanCallback, this)},
    scan_ptr_{nullptr} {}
    ~LaserSubscriberNode() { delete scan_ptr_; }
};

int main(int argc, char **argv) {
    ros::init(argc, argv, "laser_subscriber_node");

    LaserSubscriberNode node;

    ros::spin();

    return 0;
    
}