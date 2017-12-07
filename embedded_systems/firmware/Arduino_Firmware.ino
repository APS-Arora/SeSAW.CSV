#include <Servo.h>

byte leftThrusterPin = 2;
byte rightThrusterPin = 12;
Servo leftThruster;
Servo rightThruster;

void setup() {
	leftThruster.attach(leftThrusterPin);
        rightThruster.attach(rightThrusterPin);

	leftThruster.writeMicroseconds(1500); // send "stop" signal to ESC.
	rightThruster.writeMicroseconds(1500);
        delay(1000); // delay to allow the ESC to recognize the stopped signal
}

void loop() {
	int signal = 1530; // Set signal value, which should be between 1100 and 1900

	leftThruster.writeMicroseconds(signal); // Send signal to ESC.
        rightThruster.writeMicroseconds(signal);
}

