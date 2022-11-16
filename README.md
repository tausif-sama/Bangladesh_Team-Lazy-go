# Engineering Documentation | Team Lazy-go | Bangladesh
----

This repository contains engineering materials of Lazy-bot, a self-driven vehicle's model from Team Lazy-go, Bangladesh, participating in the WRO Future Engineers competition in the season 2022.

----

## Team Members:

- Tausif Samin - email: <tausifsamin.dhk@gmail.com>, Discord: Ikigai#8742
- Iqbal Samin Prithul - email: <prithul0218@gmail.com>, Discord: Prithul#3957

----

## Repository Overview


+ `chassis` - this folder contains details of the chassis that we've used
+ `models` - contains 3D Printable files designed by us
+ `schemes` - contains the electrical systems schematics 
+ `src` - contains the main programs and control software and 
+ `t-photos` - contains one serious and one funny photo
+ `v-photos` - contains the photos of the robot from all required directions
+ `video` - contains youtube link of the video where you can see our robot in action
+ `others` - other essential photos

----



# Program arrangement and Algorithm Planning

The robot is running on an ESP32-based development board called the JRC board locally made in our country to run all the actuators and sense the environment. It is using the Huskylens to see the red/green obstacles as well as the corner lines (blue/orange). An MPU6050 gyroscope and accelerometer sensor are used to detect the orientation of the robot and count laps.

When the robot is first powered on with the battery, it initializes the chip and starts communicating with the Huskylens and IMU sensor. Once it is ready, the servo motor gets centered and it waits for the user to push the button. The code is then divided into three major parts - 

### Part 1 - Detecting run direction:

The first part determines the direction of the robot run. The robot moves slowly through the first straightforward section to ensure it can safely detect the direction of the run with its sonar sensors. Once it detects a large distance (>90cm) from one of the sensors, it marks the direction as “L” - clockwise, or “R”  - anti-clockwise accordingly.

### Part 2 - Completing the run as fast as possible:

In this part, we run a PID loop (1st round) or a proportional+obstacle avoiding code(2nd round) to complete the run as fast as possible. 

### **Wall avoidance:**

In the first round, we only check the sonar on the inner wall side (based on the direction of run detected from Part 1) to run a PID (proportional, integral, derivative) code. The code tries to keep the robot at a fixed distance from the wall while slowing down the rear axle drive motor whenever a turn is initiated.
To complete this part as fast as possible, we use smart algorithms to detect when the robot has just finished a turning section and is about to enter a straight section. The robot then spins the drive motor at maximum speed for a short period of time (40-50mS) to give the robot a great acceleration or “boost”.

In the 2nd round, however, we decided to disable the boost and changed PID control to just P or proportional to keep the robot centered on the track, which is sufficient at the slow speed of the robot.

### **Obstacle avoidance:**

When the huskylens detects an object, the ESP32 gets the object's color and location in the X and Y axis, as well as its height (to get the distance). The robot then follows the obstacle with an offset. Meaning it follows slightly on the right side of red obstacles, and slightly to the left of green obstacles. And once it reaches a distance of less than 30 cm, it turns the front wheel proportionally to the distance of the object (calculated from height) and the position of the object on the screen's horizontal axis.

### Wall collision protection:

If the robot detects a wall within 8cm of one of the sides, it rotates the servo in the other direction for a few milliseconds to keep the robot from hitting the wall.

### Part 3 - Detecting the end of 3 laps:

The robot equips a gyroscope sensor to measure the yaw angle by integrating the rotational acceleration values on the Z axis over time. Gyroscope sensors are infamous for drift in the angle over time, but our robot runs for such a short time that it does not affect its performance.

When the start button is pressed, the robot is at a zero-degree angle starting position. After each lap of the track, the robot spins 360 degrees. So we can detect the completion of three laps as soon as the robot completes 3x360 or 1080 degrees turn (+/- 20 degrees to compensate for errors). 
However, the robot is not instantly stopped, since it may not have reached the straightforward section. We start waiting for the Huskylens to detect a blue or orange line in front of it and stop instantly with a short backward pulse to the drive motor (hard break).

----



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

![brd1](https://github.com/tausif-sama/Bangladesh_Team-Lazy-go/blob/main/others/brd1.jpg.jpg)
![brd2](https://github.com/tausif-sama/Bangladesh_Team-Lazy-go/blob/main/others/brd2.jpg)


----


# Mechanical Design

We're using a ready made Ackermann based chassis kit from YF robot and have significantly modified each aspect of the chassis. The chassis base plate is made of Aluminum and the rest of the parts involve brass spacers, acrylic top plates, front articulating axel parts and some other miscellaneous parts. We've also designed and 3D Printed some parts for making our chassis more practical.

## Design Decisions
We setteled on the YF Robot chassis because it provided a robust platform to work on and featured ackermann steering mechanism. We've deigned and 3D printed some parts which were required for the optimal operation of the robot. 




## 3D Printed parts designed by us

- We've designed a sonar mount which is mounted in the front of the robot where the sonar sensors are mounted at an angle of 20 deg. We've tested this to be the optimal angle for the sonars to detect walls before hand, providing ample time for the bot to react.
- An elevated and angled mount for the huskylens was designed for the camera to get optimal vision of the track.
- The wheels were completely 3D printed, except for the tires. After extensive testing, we've found that thin twheels perfom better in cases where precise movement is required.
- A mounting plate for the veroboard was designed so that the solder joints under the board do not make any contact with the motor.

----


# Software Setup
![img0](https://github.com/tausif-sama/Bangladesh_Team-Lazy-go/blob/main/others/img0.jpg)
- First we need to install the latest Arduino IDE on our computer from: arduino.cc/en/Main/Software. If you already have an older version, uninstall it and install the latest one again. Otherwise, it may not work.

![img1](https://github.com/tausif-sama/Bangladesh_Team-Lazy-go/blob/main/others/img1.jpg)
- In your Arduino IDE, go to File > Preferences, and enter the following into the "Additional Board Manager URLs" field, and click OK:
`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`

![img2](https://github.com/tausif-sama/Bangladesh_Team-Lazy-go/blob/main/others/img2.jpg)
- Open the Arduino Boards Manager: Tools > Board > Boards Manager... and search for ESP32. Press install button for the "ESP32 by Espressif Systems".

![img3](https://github.com/tausif-sama/Bangladesh_Team-Lazy-go/blob/main/others/img3.jpg)
- Now we try to upload our code to the ESP32. Select your Board in Tools > Board menu (in our case it is the DOIT ESP32 DEVKIT V1).

![img4](https://github.com/tausif-sama/Bangladesh_Team-Lazy-go/blob/main/others/img4.jpg)
- Select the COM port, and open the sketch.

![img5](https://github.com/tausif-sama/Bangladesh_Team-Lazy-go/blob/main/others/img5.jpg)
- Press the Upload button in the Arduino IDE. Wait a few seconds while the code compiles and uploads to your board. It should say "Done Uploading" if the upload completes successfully.

![img6](https://github.com/tausif-sama/Bangladesh_Team-Lazy-go/blob/main/others/img6.jpg)
- Now you can place the robot on the track, power it up, and wait until the front servo centers.

- Press the button and it will start doing the laps.
