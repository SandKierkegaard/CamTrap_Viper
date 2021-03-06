#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
#include <iostream>
#include <time.h>

#include <sys/statvfs.h>
#include <phidget21.h>
#include "CamTrap_Viper/CvService.h"

void hvCallback(const std_msgs::String::ConstPtr& msg)
{
	//Do stuff when the hypervisor says so
}

void comCallback(const std_msgs::String::ConstPtr& msg)
{
	//Do stuff when the com node says so
}


int main(int argc, char** argv){
	
	ros::init(argc, argv, "pan");
	ros::NodeHandle n;
	
	//connect to Hypervisor and com nodes
	ros::Subscriber hv_sub = n.subscribe("Hypervisor_Output", 10, hvCallback);	
	ros::Subscriber com_sub = n.subscribe("Com_Commands", 10, comCallback);
	
	//Open connection for Log node to connect to
	ros::Publisher pub = n.advertise<std_msgs::String>("Motor_Movement",10);
	
	//Connect to CV service
	ros::ServiceClient client = n.serviceClient<CamTrap_Viper::CvService>("cv_service");
	CamTrap_Viper::CvService srv;
	
	int x_offset = 0;
	float x_degree = 0; 
	while (ros::ok())
	{
	//	ROS_INFO("making service request...\n");
		srv.request.localization_request = 0;
		if (client.call(srv))
		{
			ROS_INFO("[x,y] = [%d,%d]\n", (int) srv.response.x_offset, srv.response.y_offset);
			x_offset = srv.response.x_offset;
			x_degree = srv.response.x_degree;	
			ROS_INFO("motors- Turn %f degrees to re-align center", x_degree);
			
		}
		else
		{
			ROS_ERROR("cv service call failed");
		}
		ros::spinOnce();
		ros::Duration(1.0).sleep();
	}	
}
