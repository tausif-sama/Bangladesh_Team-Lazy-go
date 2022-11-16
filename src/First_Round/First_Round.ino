#include <NewPing.h>
#include <PIDController.h>
#include <ESP32Servo.h>
#include <HUSKYLENS.h>

Servo myservo;
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
NewPing sonarL(13, 13, MAX_DISTANCE), sonarR(14, 14, MAX_DISTANCE);     // 2 = right sensor, 3 = left

PIDController pid; // Create an instance of the PID controller class, called "pid"
HUSKYLENS huskylens;

#define IN1 4
#define IN2 25
#define PWM 2
#define stdby 26
#define button 14
bool start = false;
int dif, ID;
int angle;
int cal, pwm;
long duration, distance, RD, FD, LD;
int turndis = 110;
char dir;
float p = 26, i = .1, d = 8;


int sensorValue, output, lastOutput;
unsigned long lastMillis = 0, zeroStartMillis = 0, highSpeedMillis = 0, lastTurnMillis = 0, stopMillis = 0;
float alpha = 0.6;
int speed = 90;
bool justTurned = false;
byte turnCount = 0, orangeCount = 0, blueCount = 0;

void setup () {
  Serial.begin(115200);  // Some methods require the Serial.begin() method to be called first'

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(stdby, OUTPUT);
  pinMode(button, INPUT_PULLUP);

  Wire.begin();
  while (!huskylens.begin(Wire)) {
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }


  pid.begin();          // initialize the PID instance
  pid.setpoint(30);    // The "goal" the PID controller tries to "reach"
  pid.tune(p, i, d);  // Tune the PID, arguments: kP, kI, kD
  myservo.attach(33);
  myservo.write(95); // 57, 95, 140
  dir = 'L';
  waitForN();
  drive(68);
  byte leftCount = 0, rightCount = 0;
  while (1) {
    delay(2);
    LD = sonarL.ping_cm();
    if (LD == 0 || LD > 100) {
      leftCount++;
      LD = 200;
    }
    RD = sonarR.ping_cm();
    if (RD == 0 || RD > 100) {
      rightCount++;
      RD = 200;
    }
    if (leftCount > 6) {
      dir = 'L';
      break;
    }
    if (rightCount > 6) {
      dir = 'R';
      break;
    }
    int error = LD - RD;
    myservo.write(constrain(map(error, -80, 80, 95 - 39, 95 + 39), 95 - 39, 95 + 39));
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

void loop () {
  if (huskylens.request(3) && millis() - lastTurnMillis > 1000) {
    if (huskylens.count(3) > 0)
      orangeCount++;
  }
  if (huskylens.request(4) && millis() - lastTurnMillis > 1100) {
    if (huskylens.count(4) > 0)
      blueCount++;
  }
  if (orangeCount > 0 || blueCount > 0) {
    orangeCount = 0;
    blueCount = 0;
    turnCount++;
    lastTurnMillis = millis();
    justTurned = true;
  }
  if (justTurned == true && abs(output) < 15) {
    Serial.println("\nSpeeding up!");
    speed = 200;
    highSpeedMillis = millis();
    justTurned = false;
  }
  if (millis() - highSpeedMillis > 40 && speed > 120) {
    speed = 90;
  }
  if (turnCount >= 12) {
    myservo.write(95);
    drive(-200);
    delay(100);
    drive(0);
    while (turnCount != 0) {
      getInputs();
    }
  }
  delay(1);
  //   pid.setpoint(19);
  //  Serial.print(dir);
  //  Serial.print("\t");
  //  Serial.print(sensorValue);
  //  Serial.print("\t");
  //  Serial.print(output);
  //  Serial.print("\t");
  Serial.print(orangeCount);
  Serial.print("\t");
  Serial.print(blueCount);
  Serial.print("\t");
  Serial.print(turnCount);
  Serial.print("\t");


  if (dir == 'R') {
    pid.limit(-40, 42);   // Limit the PID output between 0 and 255, this is important to get rid of integral windup!
    RD = sonarR.ping_cm();
    if (RD == 0) RD = 200;

    sensorValue = RD;  // Read the value from the sensor
    output = pid.compute(sensorValue);   // Let the PID compute the value, returns the optimal output
    output = constrain(output, -40, 42);

    myservo.write(95 + output);
    drive(speed - (abs(output) / 1.9));
  } else {
    pid.limit(-42, 40);   // Limit the PID output between 0 and 255, this is important to get rid of integral windup!
    LD = sonarL.ping_cm();
    if (LD == 0)
      LD = 200;
    //    else
    //      sensorValue = alpha * LD + (1 - alpha) * sensorValue;
    sensorValue = LD;
    output = pid.compute(sensorValue);   // Let the PID compute the value, returns the optimal output
    output = constrain(output, -42, 40);

    myservo.write(95 - output);
    drive(speed - (abs(output) / 1.9));
  }
  Serial.println(sensorValue);
  //  const float alpha2 = 0.9;
  //  output = alpha2 * output + (1 - alpha2) * lastOutput;
  //  lastOutput = output;

  //  Serial.print("RD = ");
  //  Serial.print(RD);
  //  Serial.print(" - ");
  //  Serial.print("LD = ");
  //  Serial.print(LD);
  //  Serial.print(" - ");
  //  Serial.print("PID OUTPUT=");
  //  Serial.println(output);


  Serial.println(millis() - lastMillis);
  lastMillis = millis();
  getInputs();
}




void drive(int pwm) {
  if (pwm >= 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(PWM, constrain(abs(pwm), 0, 255));
    digitalWrite(stdby, HIGH);
  } else {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(PWM, constrain(abs(pwm), 0, 255));
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
        drive(150);
        delay(50);
        start = true;
        break;
      case 111://o
        ESP.restart();
        turnCount = 0;
        drive(255);
        delay(10);
        break;
    }
  }
}

void waitForN() {
  while (start == false) {
    getInputs();
    if (digitalRead(button) == LOW) {
      drive(150);
      delay(50);
      start = true;
      break;
    }
  }
}
