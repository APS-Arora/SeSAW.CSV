#include <ros.h>
#include <std_msgs/Empty.h>

byte liftingMotorDirPin=31;
byte liftingMotorControlPin=7;
byte liftingMotorBrakePin=27;
byte liftingSpeed=200;
byte returnSpeed=0;
int liftingDelay=2000;
int returnDelay=500;

ros::NodeHandle nh;

void liftGarbage(const std_msgs::Empty& motor_msg)
{
  //Lift Garbage
  digitalWrite(liftingMotorBrakePin,LOW);
  digitalWrite(liftingMotorDirPin,LOW);
  //digitalWrite(liftingMotorControlPin,HIGH);
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
  //analogWrite(liftingMotorControlPin,0);
  digitalWrite(liftingMotorBrakePin,HIGH);
}

ros::Subscriber<std_msgs::Empty> motor_sub("/sesaw/test/liftingMech", &liftGarbage );

void setup() {
	nh.getHardware()->setBaud(57600);
	nh.initNode();
  nh.subscribe(motor_sub);
  pinMode(liftingMotorDirPin,OUTPUT);
  //pinMode(liftingMotorControlPin,OUTPUT);
  pinMode(liftingMotorBrakePin,OUTPUT);
}

void loop() {
nh.spinOnce();
delay(1);
}
