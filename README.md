# Engineering Documentation | Team Lazy-go | Bangladesh
----

This repository contains engineering materials of Lazy-bot, a self-driven vehicle's model from Team Lazy-go, Bangladesh, participating in the WRO Future Engineers competition in the season 2022.

----

## Team Members:

- Tausif Samin - email: <tausifsamin.dhk@gmail.com>, Discord: Ikigai#8742
- Iqbal Samin Prithul - email: <prithul0218@gmail.com>, Discord: Prithul#3957

----

## Repository Index


+ `chassis` - this folder contains details of the chassis that we've used
+ `models` - contains 3D Printable files designed by us
+ `schemes` - contains the electrical systems schematics 
+ `src` - contains the main programs and control software and 
+ `t-photos` - contains one serious and one funny photo
+ `v-photos` - contains the photos of the robot from all required directions
+ `video` - contains youtube link of the video where you can see our robot in action

----

## Repository Overview

`chassis`: We're using a ready made Ackermann based chassis kit from YF robot and have significantly modified each aspect of the chassis.
   - `baring placement`: This shows the unique chassis's bearing arrangement on the front axel wheels.
   - `chassis parts list`: This shows the number of parts and the kit contains.
   - `chassis.png`: Shows overall image of the chassis
   - `front axel wheels assembly`: This shows the assembly mechanism of the front axel.
   - `front axel.png`: This shows the placement of the front steering mechanism on the baseplate
   - `main motor.png`: This shows the placement of the motor and it's mounting mechanism on the baseplate.
   - `servo.png`: This shows the placement of the servo motor and it's mounting mechanism with the linkages.

`models`: Folder contains all the essential parts of our robot which was 3D Printed. All the parts were modeled and designed in Autodesk Fusion 360 and printed with a self-made custom 3D Printer.

   - `Motor Driver Mount.stl`: This model helps us mount the VNH2SP30 Motor Driver to the chassis.
   - `PCB mounting plate.stl`: This model is used to mount our veroboard to the robot.
   - `Sonar mount 20 degree.stl`: This model is used to mount our front two sensors at an angle.
   - `Wheel A.stl`: All our wheels are fully 3D Printed. This model is one part of the wheel.
   - `Wheel B.stl`: All our wheels are fully 3D Printed. This model is the other of the wheel.
   - `Older designs`: This file contains older iterations of our models.

`schemes`: Folder contains the electrical design schematics, designed in EasyEDA.

   - `Schematic_Lazy-bot_v0.1.4.json`: this json file can be imported in EasyEDA to edit and make changes.
   - `Schematic_Lazy-bot_v0.1.4.pdf`: this is the pdf of the schematic, good for printing purposes.
   - `Schematic_Lazy-bot_v0.1.4.png`: this is the image of the schematic.
   - `Schematic_Lazy-bot_v0.1.4.schdoc`: this can be imported to Altium Designer
   - `Schematic_Lazy-bot_v0.1.4.svg`: this is the svg of the schematic, good for online publishing.


# Electrical Systems Design

We've carefully selected the best possible parts for our robot after several hundred hours of R&D and have tried to achieve highest possible efficiency and reliability. The electrical systems design is explained in the following paragraph.

## Parts List

+ JRC Board: An ESP32 based development board (Made in Bangladesh)
+ Huskylens: Embedded Vision Sensor
+ Adafruit HC-SR04 Ultrasonic Sensors
+ 12V 300RPM DC Geared Motor
+ 9imod DS20MG Servo Motor
+ VNH2SP30 Motor Driver
+ MPU6050
+ 2x MP1584 Buck Converter
+ Pololu U3V70A Boost Converter
+ 3S LiPo battery input through XT60 Connector

## Design decisions

+  JRC Board
    - We're using the JRC Board as it is equipped with an ESP-32 microcontroller at a clock speed of 240Hz. This is the fastest development platform for the price currently available. As the ESP-32 is dual-core, we can run two loops simaltaneously which is very benificial. 
    - There's a brief but impressive backsotry of the JRC Board. The abbreviation of 'JRC' is Jamilur Reza Chowdhury. Dr. Jamilur Reza Chowdhury was a Bangladeshi Civil Engineer, Professor, Researcher, and Education Advocate. He is also the pioneer of computers in Bangladesh and brought the first ever computers in Bangladesh. This board was designed and developed in the memory of Dr. Jamilur Reza Chowdhury to spread the light of IoT and hardware level electronics education to kids in Bangladesh.
- We're using the Huskylens which is an embedded machine vision sensor based on Kendryte K210 that can recognize and track colors and objects in real-time. We're using I2C to receive tracked color position data.
- We're using a 12V 500RPM geared motor with a modified gearbox that provides ample Torque and Speed to drive our car weiging about 900g.
- The front Axel is being articulated by a 9imod DS20MG Servo Motor. This Servo boasts a coreless motor inside and operates at a speed of 0.09sec/60°@7.4V.
- We've used a VNH2SP30 Motor driver replacing a TB6612FNG because of lack of current. VNH2SP30 has no problems communicating with the 3.3v ESP-32 and provides ample current to the motor.
- We're using 2x HC-SR04 sonar sensors to measure distance from both the left and right walls.
- The Pololu U3V70A Boost Converter was used because providing sustained voltage to the motor driver is an absolute necessity for achieving consistent performance. U3V70A Provides a constant 12v to the Motor driver regardless of the battery voltage.
- 2x MP1584 buck converters were used, one for powering all the 5v peripherals and on-board devices and the other one for providing 7.4v to the servo motor.
- A MPU6050 was used to know the orientation of the robot in real-time. This data is later used to count laps.
- A 3S 12.6V 1500mAh battery is powering this entire system. We're getting a runtime of average 3 hours.

## Steps to replicate the exact same electrical design

+  Replicating the Power system
    - Connect the main power from the XT60 connector to the both MP1584 buck converters and U3V70A Boost Converter. Set the output voltage of the boost converter to 12v (Whatever voltage your motor needs) and one MP1584 to 7.4v for the servo and the other one to 5V for all of the other onboard 5v devices.
    - Connect the Output of the U3V70A to the PWR_IN and GND Pins of the VNH2SP30 Motor Driver.
    -  

+  Connecting all the logic wires
    - We're communicating with the huskylens and MPU6050 through I2C. 
    - We're taking sonar input on digital pins 13, 15.
    - VNH2SP30 
    - Please follow the Schematics for detailed wiring guidelines.




---
