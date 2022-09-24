Electrical Systems Design
====

Badur Gaddi is a self driving car, modeled to work on the WRO FE track. The electrical parts, components and schematic diagrams are as follows.

## Parts List

+ Arduino Nano
+ PixyCAM - v2.1 Vision Sensor
+ TB6612FNG Motor Driver 
+ 12V 300RPM DC Geared Motor
+ MG996R Servo Motor
+ HC-SR04 Sonar Sensors
+ XL6009 Buck Converter
+ Mini360 Buck Converter
+ 3S LiPo battery output through T-Plug Connector
+ GY-271 HMC5883L Compass

## Electrical Design decisions

- We've built a hand soldered pcb for ease of operation and efficient use of space.
- We're using the Arduino Nano for it's compact size and ease of use.
- PixyCAM V2 talks really good with the Nano and Arduino ecosystem. We're using I2C communication protocol.
- We're using a 12V 300RPM geared motor that provides ample Torque and Speed.
- The front Axel is being articulated by a MG996R Metal Gear servo.
- We've used a TB6612FNG Motor driver. This is a more efficient alternative to L298N and it's overall compact size helps it sit rightly on our hadmande pcb.
- A GY-271 HMC5883L Compass module was used to find the heading and and overall 2D Position 
- We're using 2x HC-SR04 sonar sensors to measure distance from both the left and right walls.
- A boost converter were used because providing sustained voltage to the motor driver is an absolute necessity for achieving consistent performance. XL6009 Provides a constant 12v to the Motor driver regardless of the battery voltage.
- A Mini360 buck converter was used to power all the 5v devices.
- A 3S 12.6V 1500mAh battery is powering this entire system. We're getting a runtime of average 40min.


      



