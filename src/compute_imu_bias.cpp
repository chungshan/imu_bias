#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <eigen3/Eigen/Dense>
sensor_msgs::Imu imu_data;

void imu_cb(const sensor_msgs::Imu::ConstPtr &msg){
  imu_data = *msg;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "compute_imu_bias");
  ros::NodeHandle nh;

  ros::Subscriber imu_sub = nh.subscribe<sensor_msgs::Imu>("/drone1/mavros/imu/data", 10, imu_cb);

  ros::Rate rate(50);
  float sum_x = 0,sum_y = 0, sum_z = 0, avg_x = 0, avg_y = 0, avg_z = 0;
  int count = 0, i = 0;
  Eigen::VectorXd linear_x;
  while(ros::ok()){

  if (imu_data.linear_acceleration.x !=0 && imu_data.linear_acceleration.y !=0 ){
    linear_x(i) = imu_data.linear_acceleration.x;
    i += 1;
    sum_x += imu_data.linear_acceleration.x;
    sum_y += imu_data.linear_acceleration.y;
    sum_z += imu_data.linear_acceleration.z;
    count += 1;
    avg_x = sum_x / count;
    avg_y = sum_y / count;
    avg_z = sum_z / count;
  }

  ROS_INFO("Bias: ax = %f, ay = %f, az = %f", avg_x, avg_y, avg_z);
*/
    if(abs(imu_data.linear_acceleration.x) > 1 ){
      ROS_INFO("a_x > 1");
      count += 1;
      ROS_INFO("Numbers = %d", count);
    }
  ros::spinOnce();
  rate.sleep();
  }


}
