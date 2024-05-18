#include "ros/subscriber.h"
#include <cmath>
#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>

class LaserSubscriberNode {
  ros::NodeHandle nh_;
  ros::Subscriber sub_;
  const sensor_msgs::LaserScan *scan_ptr_;
  float closest_obstacle_angle_;
  float closest_obstacle_distance_;

  void laserScanCallback(const sensor_msgs::LaserScan::ConstPtr &msg) {
    scan_ptr_ = msg.get();

    if (!scan_ptr_->ranges.empty()) {
      float range = scan_ptr_->ranges[360];
      ROS_INFO("Range in front: %f", range);
    }

    int index = 0;
    for (int i = 0; i < scan_ptr_->ranges.size(); ++i)
      if (!std::isinf(scan_ptr_->ranges[i]) &&
          scan_ptr_->ranges[i] < closest_obstacle_distance_) {
        closest_obstacle_distance_ = scan_ptr_->ranges[i];
        index = i;
      }
    closest_obstacle_angle_ = msg->angle_min + index * msg->angle_increment;

    printClosestObstacleDistance();
  }

  void printClosestObstacleDistance() const {
    ROS_INFO("Distance to closest obstacle (m): %f", closest_obstacle_distance_);
    ROS_INFO("Angle to closest obstacle (rad): %f", closest_obstacle_angle_);
  };

public:
  LaserSubscriberNode()
      : sub_{nh_.subscribe("/kobuki/laser/scan", 10,
                           &LaserSubscriberNode::laserScanCallback, this)},
        scan_ptr_{nullptr}, closest_obstacle_distance_{1000} {}

  ~LaserSubscriberNode() { delete scan_ptr_; }
};

int main(int argc, char **argv) {
  ros::init(argc, argv, "laser_subscriber_node");

  LaserSubscriberNode node;

  ros::spin();

  return 0;
}