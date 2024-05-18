#include "laser_reader/laser_reader.h"
// #include "robot_management/laser_turtle.h"
#include "sensor_msgs/LaserScan.h"
#include <cmath>
#include <iostream>
#include <ros/ros.h>

// TurtleClass constructor make changes in the constructor as per definition
TurtleClass::TurtleClass() {
  n = ros::NodeHandle("~");
  laser_sub =
      n.subscribe("/kobuki/laser/scan", 10, &TurtleClass::laser_callback, this);
  ROS_INFO("Initializing node.");
  usleep(2000000);
  // an array of 720 pointers to float
  last_laser_ranges = new float[720];
  for (int i = 0; i < 10; ++i)
    last_ten_scan[i] = new float[720];
}

/* CREATE THE CLASS DESTRUCTOR HERE */

void TurtleClass::laser_callback(
    const sensor_msgs::LaserScan::ConstPtr &laser_msg) {

  /* WRITE HERE YOUR CODE */
  for (int i = 0; i < laser_msg->ranges.size(); ++i) {
    last_laser_ranges[i] = laser_msg->ranges[i];
    // assign round-robin to last-10 array
    last_ten_scan[ptr_array_index][i] = last_laser_ranges[i];
  }
  ptr_array_index = (ptr_array_index + 1) % 10; // round-robin

  // with two robots, two different value alternate!!!
  std::cout << "Last distance in front of robot with laser scan is ";
  std::cout << *(last_laser_ranges + 360) << '\n';

  float min = 1000, max = 0, tmp;
  for (int i = 0; i < 10; ++i)
    if (!std::isinf(last_ten_scan[i][360])) { // ignore inf
      tmp = last_ten_scan[i][360];
      if (tmp < min)
        min = tmp;
      if (tmp > max)
        max = tmp;
    }

  std::cout << "The range of the non-inf laser readings in front of a robot "
               "with laser scan is [";
  std::cout << min << ", " << max << "]\n";
}

TurtleClass::~TurtleClass() {
  delete[] last_laser_ranges;
  for (int i = 0; i < 10; ++i)
    delete[] last_ten_scan[i];
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "rosbot_class_node");

  TurtleClass tc = TurtleClass();
  ros::spin();

  return 0;
}