#include <ros.h>
#include <sesaw_msgs/ThrusterCommand.h>
#include <Servo.h>

byte leftThrusterPin = 2;
byte rightThrusterPin = 12;
Servo leftThruster;
Servo rightThruster;

ros::NodeHandle nh;

void runThrusters( const sesaw_msgs::ThrusterCommand& thrust_msg){
	leftThruster.writeMicroseconds(constrain(thrust_msg.leftThrust+1500,1100,1900));
  rightThruster.writeMicroseconds(constrain(thrust_msg.rightThrust+1500,1100,1900));
}
ros::Subscriber<sesaw_msgs::ThrusterCommand> thrust_sub("/sesaw/control/Thrust", &runThrusters );

void setup() {
	nh.getHardware()->setBaud(57600);
	nh.initNode();
  nh.subscribe(thrust_sub);
	leftThruster.attach(leftThrusterPin);
  rightThruster.attach(rightThrusterPin);
	leftThruster.writeMicroseconds(1500); // send "stop" signal to ESC.
	rightThruster.writeMicroseconds(1500);
  delay(1000); // delay to allow the ESC to recognize the stopped signal
}

void loop() {
nh.spinOnce();
delay(1);
}
