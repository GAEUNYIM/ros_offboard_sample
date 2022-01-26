#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <std_msgs/Empty.h>

mavros_msgs::State current_state;
void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "offb_node");
    ros::NodeHandle nh;

    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>
            ("mavros/state", 10, state_cb);
    ros::Publisher local_pos_pub = nh.advertise<geometry_msgs::PoseStamped>
            ("mavros/setpoint_position/local", 10);
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>
            ("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>
            ("mavros/set_mode");
    ros::Publisher reach_signal_pub = nh.advertise<std_msgs::Empty>
    		("reached_signal", 1);

    //the setpoint publishing rate MUST be faster than 2Hz
    ros::Rate rate(20.0);

    // wait for FCU connection
    while(ros::ok() && !current_state.connected){
	ROS_WARN("im here");
        ros::spinOnce();
        rate.sleep();
    }

    int FORWARD_HEIGHT = 2;
	
    geometry_msgs::PoseStamped pose;
    // Set a customized position & orientation of my drone
    // The drone is watching the panorama of the warehouse.world
    pose.pose.position.x = 4;
    pose.pose.position.y = 0;
    pose.pose.position.z = FORWARD_HEIGHT;
    pose.pose.orientation.z = 3.14;

	// How many shots wil be taken?
	int shots = 1;
	
	// OFFBOARD & ARMING is done
	bool takeoff_done = false;
	bool filming_done = false;

    //send a few setpoints before starting
    for(int i = 100; ros::ok() && i > 0; --i){
        local_pos_pub.publish(pose);
        ros::spinOnce();
        rate.sleep();
    }

    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD";

    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;
    
    

    ros::Time last_request = ros::Time::now();

    while(ros::ok()){
        if (ros::Time::now() - last_request > ros::Duration(5.0))
        {
        	if(current_state.mode != "OFFBOARD") {
        		if(set_mode_client.call(offb_set_mode) 
        				&& offb_set_mode.response.mode_sent){
                	ROS_INFO("Offboard enabled");
            	}	
        	} else if (!current_state.armed ) {
        		if( arming_client.call(arm_cmd) &&
                    	arm_cmd.response.success){
                    ROS_INFO("Vehicle armed");
                }
        	} else {
        		if (shots <= 5){
        			ROS_INFO("shots!! %d", shots++);
            		pose.pose.position.x += 1;
            		
            		std_msgs::Empty m;
            		reach_signal_pub.publish(m);
            		
        		}
        	}
        	last_request = ros::Time::now();
        }

        local_pos_pub.publish(pose);

        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
