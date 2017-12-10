#include <ros.h>
#include <sesaw_msgs/ThrusterCommand.h>
#include <sesaw_msgs/CollectCommand.h>
#include <Servo.h>

//Creating Node Handle
ros::NodeHandle nh;

// Preparing Thruster Code
byte leftThrusterPin = 2;
byte rightThrusterPin = 12;
Servo leftThruster;
Servo rightThruster;

void runThrusters( const sesaw_msgs::ThrusterCommand& thrust_msg){
	leftThruster.writeMicroseconds(constrain((thrust_msg.leftThrust)>=0?(thrust_msg.leftThrust+1525):(thrust_msg.leftThrust+1475),1100,1900));
  rightThruster.writeMicroseconds(constrain((thrust_msg.rightThrust)>=0?(thrust_msg.rightThrust+1525):(thrust_msg.rightThrust+1475),1100,1900));
}

ros::Subscriber<sesaw_msgs::ThrusterCommand> thrust_sub("/sesaw/control/Thrust", &runThrusters );

//Preparing Lifting Mechanism Code
byte liftingMotorDirPin=31;
byte liftingMotorControlPin=7;
byte liftingMotorBrakePin=27;
byte liftingSpeed=200;
byte returnSpeed=0;
int liftingDelay=2000;
int returnDelay=500;

void liftGarbage(const sesaw_msgs::CollectCommand::Request& lift_request,
								sesaw_msgs::CollectCommand::Response& collection_response)
{
  //Lift Garbage
  digitalWrite(liftingMotorBrakePin,LOW);
  digitalWrite(liftingMotorDirPin,LOW);
  analogWrite(liftingMotorControlPin,liftingSpeed);
  //Wait for collection to complete
  delay(liftingDelay);
  //Return Lifting Mechanism to Previous State
  analogWrite(liftingMotorControlPin,0);
  digitalWrite(liftingMotorDirPin,HIGH);
  analogWrite(liftingMotorControlPin,returnSpeed);
  //Wait for return to complete
  delay(returnDelay);
  //Stop Lifting Mechanism
  analogWrite(liftingMotorControlPin,0);
  digitalWrite(liftingMotorBrakePin,HIGH);
}

ros::ServiceServer<sesaw_msgs::CollectCommand::Request,sesaw_msgs::CollectCommand::Response> collection_server("/sesaw/actuators/Collect", &liftGarbage);

void setup() {
	//Initialising Node
	nh.getHardware()->setBaud(57600);
	nh.initNode();

	//Preparing Thruster Code
  nh.subscribe(thrust_sub);
	leftThruster.attach(leftThrusterPin);
  rightThruster.attach(rightThrusterPin);
	leftThruster.writeMicroseconds(1500); // send "stop" signal to ESC.
	rightThruster.writeMicroseconds(1500);
  delay(1000); // delay to allow the ESC to recognize the stopped signal

	//Preparing Lifting Mechanism Code
	nh.advertiseService(collection_server);
	pinMode(liftingMotorDirPin,OUTPUT);
	pinMode(liftingMotorBrakePin,OUTPUT);
}

void loop() {
nh.spinOnce();
delay(1);
}
