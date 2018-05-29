#include <ros/ros.h>
#include <sensor_msgs/Imu.h>
#include <eigen3/Eigen/Dense>
#include <UKF/output.h>
sensor_msgs::Imu imu_data;
UKF::output output;
void imu_cb(const sensor_msgs::Imu::ConstPtr &msg){
  imu_data = *msg;
}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "compute_imu_bias");
  ros::NodeHandle nh;
  output.force.x = 1;
  ros::Subscriber imu_sub = nh.subscribe<sensor_msgs::Imu>("/drone1/mavros/imu/data", 10, imu_cb);

  ros::Rate rate(50);
  float sum_x = 0,sum_y = 0, sum_z = 0, avg_x = 0, avg_y = 0, avg_z = 0;
  int count = 0, i = 0 , n = 401;
  std::vector< float > x, y, z;
  float square_x, square_y, square_z;
  float avg_sigma_x, avg_sigma_y, avg_sigma_z;
  float sigma_x, sigma_y, sigma_z;
  //Eigen::VectorXd linear_x;
  while(ros::ok()){

  if (imu_data.linear_acceleration.x !=0 && imu_data.linear_acceleration.y !=0 ){
    x.push_back(imu_data.linear_acceleration.x);
    y.push_back(imu_data.linear_acceleration.y);
    z.push_back(imu_data.linear_acceleration.z);
    i += 1;
    sum_x += imu_data.linear_acceleration.x;
    sum_y += imu_data.linear_acceleration.y;
    sum_z += imu_data.linear_acceleration.z;
    count += 1;
    avg_x = sum_x / count;
    avg_y = sum_y / count;
    avg_z = sum_z / count;
  }
  if(i == n-1){
    ROS_INFO("avg_x = %f, avg_y = %f, avg_z = %f", avg_x, avg_y, avg_z);
    for(int k = 0 ; k < n ; k++){
      square_x = pow((x[k] - avg_x),2);
      square_y = pow((y[k] - avg_y),2);
      square_z = pow((z[k] - avg_z),2);
      avg_sigma_x += square_x;
      avg_sigma_y += square_y;
      avg_sigma_z += square_z;
    }

    sigma_x = sqrt(avg_sigma_x / n);
    sigma_y = sqrt(avg_sigma_y / n);
    sigma_z = sqrt(avg_sigma_z / n);
    ROS_INFO("sigma_x= %f, sigma_y= %f, sigma_z= %f", sigma_x, sigma_y, sigma_z);
    i = i+1;
    ROS_INFO("Finish");
  }

  //ROS_INFO("Bias: ax = %f, ay = %f, az = %f", avg_x, avg_y, avg_z);
/*
    if(abs(imu_data.linear_acceleration.x) > 1 ){
      ROS_INFO("a_x > 1");
      count += 1;
      ROS_INFO("Numbers = %d", count);
    }
    */
  ros::spinOnce();
  rate.sleep();
  }


}
