Electrical Systems Design
====

Folder contains the electrical design schematics, designed in EasyEDA.

## Overview

   - `Schematic_Lazy-bot_v0.1.4.json`: this json file can be imported in EasyEDA to edit and make changes.
   - `Schematic_Lazy-bot_v0.1.4.pdf`: this is the pdf of the schematic, good for printing purposes.
   - `Schematic_Lazy-bot_v0.1.4.png`: this is the image of the schematic.
   - `Schematic_Lazy-bot_v0.1.4.schdoc`: this can be imported to Altium Designer
   - `Schematic_Lazy-bot_v0.1.4.svg`: this is the svg of the schematic, good for online publishing.

## Parts List

+ JRC Board: An ESP32 based development board (Made in Bangladesh)
+ Huskylens: Embedded Vision Sensor
+ Adafruit HC-SR04 Ultrasonic Sensors
+ 12V DC Geared Motor
+ 9imod DS20MG Servo Motor
+ VNH2SP30 Motor Driver
+ MPU6050
+ 2x MP1584 Buck Converter
+ Pololu U3V70A Boost Converter
+ 3S LiPo battery output through XT60 Connector

## Electrical Schematics Explanation

- Connections
   - We've shorted the TRIG and ECHO pin of each sonar and using only one wire to communicate with the Sonar. This reduces clutter without any performance loss whatsoever.
   - Huskylens and MPU6050 are connected via I2C to the JRC Board.
   - The servo signal is connected to GPIO 4.
   - VNH2SP30 is connected to the JRC board through a jumper pins, where: INA, INB, PWM, EN are connected to GPIO 25, 26, 2 and 33 respectively.
   - A push button is connected to PIN 32, when the button is pressed, the PIN is pulled down to ground.
   - All the spare GPIO are laid out on a female header for convenience.
- PSU
   - We're using two MP1584 so that we can use our servo at our full potential, i.e 0.09sec/60°@7.4V. Two MP1584 provides ample current to all of the onboard peripherals.
   - U1: MP1584EN Provides 7.4v to the servo.
   - U4: MP1584EN Provides 5v to the the all 5v devices on board.
   - Pololu U3V70A is receiving power from the battery at about 11.1V and later keeping the voltage constant at 12V for the motor.
   - The BATT_IN hoasts a solid XT60 connector for the battery input. 
   - All connections and solder joints are reinforced for high current demand scenarios.
- Board
    - We're using the JRC Board as it is equipped with an ESP-32 microcontroller at a clock speed of 240Hz. This is the fastest development platform for the price currently available. As the ESP-32 is dual-core, we can run two loops simaltaneously which is very benificial. 
    - There's a brief but impressive backsotry of the JRC Board. The abbreviation of 'JRC' is Jamilur Reza Chowdhury. Dr. Jamilur Reza Chowdhury was a Bangladeshi Civil Engineer, Professor, Researcher, and Education Advocate. He is also the pioneer of computers in Bangladesh and brought the first ever computers in Bangladesh. This board was designed and developed in the memory of Dr. Jamilur Reza Chowdhury to spread the light of IoT and hardware level electronics education to kids in Bangladesh. We're joining in to support their cause out of respect for Dr. Jamiulur Reza Chowdhury Sir and spreding the love for robotics and electronics throughout Bangladesh.
    
## Steps to replicate the exact same electrical design

+  Replicating the Power system
    - Connect the main power from the XT60 connector to the both MP1584 buck converters and U3V70A Boost Converter. Set the output voltage of the boost converter to 12v (Whatever voltage your motor needs) and one MP1584 to 7.4v for the servo and the other one to 5V for all of the other onboard 5v devices.
    - Connect the Output of the U3V70A to the PWR_IN and GND Pins of the VNH2SP30 Motor Driver.
  

+  Connecting all the logic wires
    - We're communicating with the huskylens and MPU6050 through I2C, connecting SDA and SCL of the JRC Board to the respective SCL and SDA Pins of MPU6050 and huskylens.
    - We're taking sonar input on digital pins 16, 17.
    - VNH2SP30 INA, INB, PWM, EN are connected to pins 25, 26, 2 and 33 respectively.
    - A push button is wired to pin 32 and ground.
    - Servo Signal pin to GPIO 4.
    - Please follow the Schematics for detailed wiring guidelines.
      



