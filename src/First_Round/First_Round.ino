#include <NewPing.h>
#include <PIDController.h>
#include <ESP32Servo.h>
#include <HUSKYLENS.h>
#include <MPU6050_light.h>
MPU6050 mpu(Wire);
int yawAngle = 0, startAngle = 0;
#define endAngle 1080
unsigned long MPUtimer = 0, lapTimer = 0;

Servo myservo;
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
PIDController pid; // Create an instance of the PID controller class, called "pid"
HUSKYLENS huskylens;


NewPing sonarL(15, 15, MAX_DISTANCE), sonarR(16, 16, MAX_DISTANCE);     // 2 = right sensor, 3 = left
#define IN1 4
#define IN2 25
#define PWM 2
#define stdby 26
#define servo 33
#define button 14

int mainSpeed = 120;
bool start = false;
int dif, ID;
int angle;
int cal, pwm;
long duration, distance, RD, FD, LD;
int turndis = 110;
char dir;
float p = 25, i = .1, d = 8;


int sensorValue, output, lastOutput;
unsigned long lastMillis = 0, zeroStartMillis = 0, highSpeedMillis = 0, lastTurnMillis = 0, stopMillis = 0;
float alpha = 0.6;
int speed = 120;
bool justTurned = false;
byte turnCount = 0, orangeCount = 0, blueCount = 0;
int timeToIgnore = 1000;

void setup () {
  Serial.begin(115200);  // Some methods require the Serial.begin() method to be called first'
  //Serial.begin("ESP32test");  // Some methods require the Serial.begin() method to be called first'

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(stdby, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  Wire.begin();
  while (mpu.begin() != 0) {
    Serial.print(F("MPU6050 failed to initialize!"));
  }
  mpu.calcOffsets(); // gyro and accelero
  lapTimer = millis();
  while (!huskylens.begin(Wire))
  {
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }


  pid.begin();          // initialize the PID instance
  pid.setpoint(29);    // The "goal" the PID controller tries to "reach"
  pid.tune(p, i, d);  // Tune the PID, arguments: kP, kI, kD
  myservo.attach(servo);
  myservo.write(90); // 50, 90, 140
  dir = 'L';
  waitForN();
  drive(70);
  byte leftCount = 0, rightCount = 0;
  while (1) {
    MPU();
    delay(2);
    LD = sonarL.ping_cm();
    if (LD == 0 || LD > 200) {
      leftCount++;
      LD = MAX_DISTANCE;
    }
    RD = sonarR.ping_cm();
    if (RD == 0 || RD > 200) {
      rightCount++;
      RD = MAX_DISTANCE;
    }
    if (leftCount > 10) {
      dir = 'L';
      break;
    }
    if (rightCount > 10) {
      dir = 'R';
      break;
    }
    int error = LD - RD;
    myservo.write(constrain(map(error, -80, 80, 50, 140), 50, 140));
    //    Serial.print(LD);
    //    Serial.print("    ");
    //    Serial.print(RD);
    //    Serial.print("    ");
    //    Serial.println(error);
    //    if (huskylens.request(3) || huskylens.request(4)) {
    //      if (huskylens.count(3) > 0 || huskylens.count(4) > 0) {
    //        turnCount++;
    //        lastTurnMillis = millis();
    //        justTurned = true;
    //        //        break;
    //      }
    //    }
  }
}
int endAngleReached = 0;
void loop () {
  MPU();
  if (yawAngle > endAngle - 18 && yawAngle < endAngle + 18) {
    endAngleReached++;
  }
  if (endAngleReached > 5 && (huskylens.request(3) || huskylens.request(4))) {
//    if (huskylens.count(3) > 0 || huskylens.count(4) > 0) {
      myservo.write(90);
      drive(-150);
      delay(100);
      drive(0);
      Serial.println("3 laps completed!\n");
      Serial.print("Lap time: ");
      int lapTime = (millis() - lapTimer) / 1000;
      Serial.print(lapTime);
      Serial.println(" seconds");
      while (1);
//    }
  }
  if (justTurned == true && abs(output) < 15) {
    Serial.println("\nSpeeding up!");
    if (turnCount < 12)speed = 255;
    highSpeedMillis = millis();
    justTurned = false;
  }
  if (millis() - highSpeedMillis > 50 && speed > mainSpeed) {
    speed = mainSpeed;
  }
  //  if (turnCount >= 12) {
  //    myservo.write(95);
  //    drive(-200);
  //    delay(100);
  //    drive(0);
  //    while (turnCount != 0) {
  //      getInputs();
  //    }
  //  }
  delay(1);
  //   pid.setpoint(19);
  //  Serial.print(dir);
  //  Serial.print("\t");
  //  Serial.print(sensorValue);
  //  Serial.print("\t");
  //  Serial.print(output);
  //  Serial.print("\t");
  //  Serial.print(orangeCount);
  //  Serial.print("\t");
  //  Serial.print(blueCount);
  //  Serial.print("\t");
  //  Serial.print(turnCount);
  //  Serial.print("\t");


  if (dir == 'R') {
    pid.limit(-40, 50);   // Limit the PID output between 0 and 255, this is important to get rid of integral windup!
    RD = sonarR.ping_cm();
    if (RD == 0) {
      RD = MAX_DISTANCE;
      drive(60);
      delay(2);
    }

    sensorValue = RD;  // Read the value from the sensor
    output = pid.compute(sensorValue);   // Let the PID compute the value, returns the optimal output
    output = constrain(output, -40, 50);

    myservo.write(92 + output);
    if (turnCount > 11) {
      drive(speed);
    } else {
      drive(speed - (abs(output) * 1.3));
    }
  } else {
    pid.limit(-50, 40);   // Limit the PID output between 0 and 255, this is important to get rid of integral windup!
    LD = sonarL.ping_cm();
    if (LD == 0) {
      LD = MAX_DISTANCE;
      drive(60);
      delay(2);
    }
    //    else
    //      sensorValue = alpha * LD + (1 - alpha) * sensorValue;
    sensorValue = LD;
    output = pid.compute(sensorValue);   // Let the PID compute the value, returns the optimal output
    output = constrain(output, -50, 40);

    myservo.write(92 - output);
    if (turnCount > 11) {
      drive(speed);
    } else {
      drive(speed - (abs(output) * 1.3));
    }
  }

  if (sensorValue > 120 && millis() - lastTurnMillis > 1000) {
    turnCount++;
    lastTurnMillis = millis();
    justTurned = true;
  }
  if (turnCount > 11) {
    mainSpeed = 80;
    speed = mainSpeed;
  }

  Serial.println(yawAngle);
  //  const float alpha2 = 0.9;
  //  output = alpha2 * output + (1 - alpha2) * lastOutput;
  //  lastOutput = output;
  /*if(LD>RD && LD>170)
    {
      myservo.write(104-60);
      drive(190);
      delay(700);
      myservo.write(104+60);
      drive(190);
      delay(220);
    } */

  //  Serial.print("RD = ");
  //  Serial.print(RD);
  //  Serial.print(" - ");
  //  Serial.print("LD = ");
  //  Serial.print(LD);
  //  Serial.print(" - ");
  //  Serial.print("PID OUTPUT=");
  //  Serial.println(output);

  /* if(output<-25){
     myservo.write(104-40);
     drive(255);
     //delay(1500);
     myservo.write(104+40);
     drive(200);
     //delay(1000);
     myservo.write(104);
    }

    else (FD<10){
     drive(0);
    }    */

  Serial.println(millis() - lastMillis);
  lastMillis = millis();
  getInputs();
}




void drive(int pwm) {
  if (pwm < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(PWM, constrain(abs(pwm), 0, 255));
    digitalWrite(stdby, HIGH);
  } else if (pwm > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(PWM, constrain(abs(pwm), 0, 255));
    digitalWrite(stdby, HIGH);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(PWM, pwm);
    digitalWrite(stdby, HIGH);
  }
}

void getInputs() {
  if (Serial.available() > 0) {
    int received_char = Serial.read();
    //    Serial.println(char(received_char));
    switch (received_char) {
      case 112://p
        p = Serial.parseFloat();
        pid.tune(p, i, d);  // Tune the PID, arguments: kP, kI, kD
        break;
      case 105://i
        i = Serial.parseFloat();
        pid.tune(p, i, d);  // Tune the PID, arguments: kP, kI, kD
        break;
      case 100://d
        d = Serial.parseFloat();
        pid.tune(p, i, d);  // Tune the PID, arguments: kP, kI, kD
        break;
      case 109://m
        speed = Serial.parseInt();
        break;
      case 110://n
        if (start != true) {
          //          startAngle = abs(mpu.getAngleZ());
          lapTimer = millis();
          drive(150);
          delay(50);
          start = true;
        }
        break;
      case 111://o
        ESP.restart();
        break;

      case 113://q
        timeToIgnore = Serial.parseInt();
        Serial.print("set to: ");
        Serial.println(timeToIgnore);
        break;

      case 114://r
        Serial.print("Current degree");
        Serial.println(yawAngle);
        drive(-200);
        delay(100);
        drive(0);
        waitForN();
        break;
    }
  }
}

void MPU() {
  mpu.update();
  if ((millis() - MPUtimer) > 50) {
    if (start == false) {
      startAngle = abs(mpu.getAngleZ());
      Serial.println(startAngle);
    } else {
      yawAngle = abs(mpu.getAngleZ()) - startAngle;
    }
    MPUtimer = millis();
  }
}

void waitForN() {
  while (start == false) {
    MPU();
    //    startAngle = abs(mpu.getAngleZ());
    getInputs();
    if (digitalRead(button) == LOW) {
      //          startAngle = abs(mpu.getAngleZ());
      lapTimer = millis();
      drive(150);
      delay(50);
      start = true;
    }
  }
}