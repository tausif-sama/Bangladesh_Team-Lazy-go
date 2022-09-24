Engineering materials
====

This repository contains engineering materials of a self-driven vehicle's model participating in the WRO Bangladesh Future Engineers competition in the season 2022.

## Content

* `t-photos` contains 2 photos of the team (an official one and one funny photo with all team members)
* `v-photos` contains 6 photos of the vehicle (from every side, from top and bottom)
* `video` contains the video.md file with the link to a video where driving demonstration exists
* `schemes` contains one or several schematic diagrams in form of JPEG, PNG or PDF of the electromechanical components illustrating all the elements (electronic components and motors) used in the vehicle and how they connect to each other.
* `src` contains code of control software for all components which were programmed to participate in the competition
* `other` is for other files which can be used to understand how to prepare the vehicle for the competition. It may include documentation how to connect to a SBC/SBM and upload files there, datasets, hardware specifications, communication protocols descriptions etc. If there is nothing to add to this location, the directory can be removed.

## Introduction

_This part must be filled by participants with the technical clarifications about the code: which modules the code consists of, how they are related to the electromechanical components of the vehicle, and what is the process to build/compile/upload the code to the vehicle’s controllers._

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




