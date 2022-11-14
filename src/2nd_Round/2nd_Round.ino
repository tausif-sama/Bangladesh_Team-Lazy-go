#include "HUSKYLENS.h"
#include <BluetoothSerial.h>
#include <NewPing.h>
#include <Wire.h>
#include <ESP32Servo.h>
#include <MPU6050_light.h>
MPU6050 mpu(Wire);
int yawAngle = 0, startAngle = 0;
#define endAngle 1080
unsigned long MPUtimer = 0, lapTimer = 0;

HUSKYLENS huskylens;
BluetoothSerial mySerial;
Servo myservo;

#define MAX_DISTANCE 140 // Maximum distance (in cm) to ping.
NewPing sonarL(16, 16, MAX_DISTANCE), sonarR(17, 17, MAX_DISTANCE);
#define IN1 26
#define IN2 25
#define PWM 2
#define stdby 33
#define button 32
#define servo 4

int lastSpeed = 0;
int dif, ID, LDF;
int cal, pwm;
long duration, Ldistance, Rdistance, wallDist = 38, mid_pos = 90, max_steering = 40, obj_h = 60;
int turndis = 110;
int count = 0;
bool just_avoided = false;
char dir = '0';
bool start = false, firstIntersection = true;
int LRiskCount = 0, RRiskCount = 0;

#define objectHeightThres 40
#define objectWidthThres 20

int autoSteer(char);
int speed = 50;
int turnCount = 0, noObjectCount = 0, leftCount = 0, rightCount = 0;
byte priority = 0;

void setup(void) {
  Serial.begin(115200);
  Serial.print("Starting...\n");
  mySerial.begin("ESP32test");
  Wire.begin();
  while (mpu.begin() != 0) {
    Serial.print(F("MPU6050 failed to initialize!"));
  }
  while (!huskylens.begin(Wire)) {
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }

  huskylens.writeAlgorithm(ALGORITHM_COLOR_RECOGNITION);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(stdby, OUTPUT);
  pinMode(button, INPUT_PULLDOWN);
  //  pinMode(A3, OUTPUT);
  //  if (digitalRead(A1) == LOW) {
  //    dir = 'L';
  //    digitalWrite(A3, LOW);
  //  } else {
  //    digitalWrite(A3, HIGH)
  //    ;
  //  }
  dir = 'L';
  myservo.attach(servo);
  myservo.write(90);
  waitForN();
  //  delay(500);
}
int followThreshold = 50;

int endAngleReached = 0;
void loop(void) {
  if (yawAngle > endAngle - 5 && yawAngle < endAngle + 30) {
    endAngleReached++;
  }
  check3Lap();
  int angle, g_x, g_h, g_w, g_area;
  delay(1);
  MPU();

  if (!huskylens.requestBlocksLearned()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
  else if (!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  else if (huskylens.requestBlocksLearned()) {
    while (huskylens.request(1) && huskylens.count(1) > 0 && priority != 2) {
      Rdistance = sonarR.ping_cm();
      Ldistance = sonarL.ping_cm();
      MPU();
      //      checkSize();
      checkSonar();
      check3Lap();
      //      if (Ldistance == 0) {
      //        drive(60);
      //        myservo.write(90);
      //        delay(80);
      //        myservo.write(140);
      //        delay(550);
      //      }
      HUSKYLENSResult result = huskylens.get(1, 0);
      g_h = result.height;
      //      Serial.println(g_h);
      g_w = result.width;
      //      g_x = result.xCenter + (g_w / 2);
      g_x = result.xCenter;
      if (g_h < 16) break;
      if (huskylens.request(2)) {
        HUSKYLENSResult resultHere = huskylens.get(2, 0);
        if (resultHere.height > result.height) break;
      }
      Serial.println(g_h);
      Serial.print(". Green. Center: ");
      Serial.print(g_x);
      float turnAmount = 30 + map(g_h, 50, 90, 0, 20);
      if (g_h < followThreshold && g_x > 0) {
        drive(speed);
        myservo.write(constrain(map(g_x - 40, 320, 0, 90 - 30, 90 + 30), 50, 140)); // follow
        Serial.print(". Servo: ");
        Serial.print(constrain(map(g_x - 40, 320, 0, 90 - 30, 90 + 30), 50, 140));
      } else {
        drive(speed * 1.1);
        myservo.write(constrain(map(g_x, 320 / 2, 320, 90 + turnAmount, 100), 50, 140)); // avoid
        Serial.print(". Servo: ");
        Serial.print(constrain(map(g_x, 320 / 2, 320, 90 + turnAmount, 100), 50, 140));
        just_avoided = true;
      }
      Serial.println();
    }
    while (huskylens.request(2) && huskylens.count(2) > 0 && priority != 1) {
      Rdistance = sonarR.ping_cm();
      Ldistance = sonarL.ping_cm();
      MPU();
      checkSonar();
      check3Lap();
      //      if (Rdistance == 0) {
      //        drive(60);
      //        myservo.write(90);
      //        delay(50);
      //        myservo.write(50);
      //        delay(200);
      //      }
      HUSKYLENSResult result = huskylens.get(2, 0);
      g_h = result.height;
      g_w = result.width;
      //      g_x = result.xCenter + (g_w / 2);
      g_x = result.xCenter;
      if (g_h < 16) break;

      if (huskylens.request(1)) {
        HUSKYLENSResult resultHere = huskylens.get(1, 0);
        if (resultHere.height > result.height) break;
      }
      Serial.println(g_h);
      Serial.print(". Red. Center: ");
      Serial.print(g_x);
      float turnAmount = 20 + map(g_h, 50, 90, 0, 20);
      if (g_h < followThreshold && g_x < 320) {
        drive(speed);
        myservo.write(constrain(map(g_x + 30, 320, 0, 90 - 40, 90 + 00), 50, 140)); // follow
        Serial.print(". Servo: ");
        Serial.print(constrain(map(g_x + 30, 320, 0, 90 - 30, 90 + 30), 50, 140));
      } else {
        drive(speed * 1.1);
        myservo.write(constrain(map(g_x, 0 , 320 / 2, 80, 90 - turnAmount), 50, 140)); // avoid
        Serial.print(". Servo: ");
        Serial.print(constrain(map(g_x, 0 , 320 / 2, 80, 90 - turnAmount), 50, 140));
        just_avoided = true;
      }
      Serial.println();
    }
  }
  //  HUSKYLENSResult result = huskylens.read();
  //  Serial.print(result.ID);
  Serial.print("\t");
  Serial.print(F("No objects detected"));
  if (just_avoided) {

    int here = lastSpeed;
    drive(1);
    delay(100);
    drive(here);
    just_avoided = false;
  }
  //  if (huskylens.request())
  //    if (huskylens.count(1) == 0 && huskylens.count(2) == 0)
  //      noObjectCount++;
  if (noObjectCount > 10) {
    noObjectCount = 0;
    bool breakable = false;
    mySerial.println("going straight");
    myservo.write(90);
    drive(45);
    while (huskylens.count(1) == 0 && huskylens.count(2) == 0) {
      huskylens.request();
      MPU();
      angle = autoSteer(dir);
      // Serial.println(angle);
      drive(speed + abs(angle) / 3);
      if (huskylens.count(1) != 0 || huskylens.count(2) != 0) {
        breakable = true;
        break;
      }
      mySerial.println("waiting for infinity");
      int dist = sonarL.ping_cm();
      if (dist == 0 || dist > 90) {
        MPU();
        myservo.write(90);
        delay(100);
        dist = sonarL.ping_cm();
        if (dist == 0 || dist > 100) break;
      }
    }
    int thisTurnCount = turnCount;
    while (!breakable && yawAngle > (90 * thisTurnCount + 1) - 10) {
      huskylens.request();
      if (huskylens.count(1) != 0 || huskylens.count(2) != 0) {
        breakable = true;
        break;
      }

      mySerial.println("waiting to reach angle");
      MPU();
      if (dir == 'L') {
        myservo.write(90 - 40);
      } else {
        myservo.write(90 + 40);
      }
    }
    while (!breakable && huskylens.count(1) == 0 && huskylens.count(2) == 0) {
      huskylens.request();
      mySerial.println("waiting to for object");
      myservo.write(90);
      drive(45);
    }
  } else {
    //    mySerial.println("detected object 1/2!");
  }
  angle = autoSteer(dir);
  // Serial.println(angle);
  drive(speed + abs(angle) / 3);
  Serial.println();
  getInputs();
}

void drive(int pwm) {
  lastSpeed = constrain(abs(pwm), 0, 255);
  if (pwm < 0) {
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

void printResult(HUSKYLENSResult result) {
  if (result.command == COMMAND_RETURN_BLOCK) {
    Serial.println(String() + F("Block:xCenter=") + result.xCenter + F(",yCenter=") + result.yCenter + F(",width=") + result.width + F(",height=") + result.height + F(",ID=") + result.ID);
  }
  else if (result.command == COMMAND_RETURN_ARROW) {
    Serial.println(String() + F("Arrow:xOrigin=") + result.xOrigin + F(",yOrigin=") + result.yOrigin + F(",xTarget=") + result.xTarget + F(",yTarget=") + result.yTarget + F(",ID=") + result.ID);
  }
  else {
    Serial.println("Object unknown!");
  }
}

void getInputs() {
  if (mySerial.available() > 0) {
    int received_char = mySerial.read();
    //    Serial.println(char(received_char));
    switch (received_char) {
      case 108://t
        followThreshold = mySerial.parseInt();
        break;
      case 109://m
        speed = mySerial.parseInt();
        break;
      case 110://n
        lapTimer = millis();
        drive(150);
        delay(5);
        start = true;
        break;
      case 111://o
        ESP.restart();
        turnCount = 0;
        drive(255);
        delay(10);
        break;

      case 113://q
        //        timeToIgnore = mySerial.parseInt();
        //        mySerial.println(
        break;
    }
  }
}

void waitForN() {
  while (start == false) {
    MPU();
    getInputs();
    if (digitalRead(button) == HIGH) {
      //          startAngle = abs(mpu.getAngleZ());
      lapTimer = millis();
      drive(150);
      delay(50);
      start = true;
    }
  }
}


void checkSize() {
  if (huskylens.request(1) && huskylens.request(2)) {
    HUSKYLENSResult resultGreen = huskylens.get(1, 0);
    HUSKYLENSResult resultRed = huskylens.get(2, 0);
    // priority = green:1 red:2 none:0
    if (resultGreen.height > resultRed.height) priority = 1;
    if (resultRed.height > resultGreen.height) priority = 2;
  } else {
    priority = 0;
  }
}

void checkSonar() {
  if (Ldistance < 8 && Ldistance != 0) LRiskCount++;
  if (Rdistance < 8 && Rdistance != 0) RRiskCount++;
  if (LRiskCount > 9) {
    mySerial.println("Left risk");
    int here = lastSpeed;
    myservo.write(50);
    drive(40);
    delay(150);
    drive(here);
    LRiskCount = 0;
  }

  if (RRiskCount > 9) {
    mySerial.println("Right risk");
    int here = lastSpeed;
    myservo.write(140);
    drive(40);
    delay(150);
    drive(here);
    RRiskCount = 0;
  }
}


void MPU() {
  mpu.update();
  if ((millis() - MPUtimer) > 50) {
    if (start == false) {
      startAngle = abs(mpu.getAngleZ());
      mySerial.println(startAngle);
    } else {
      yawAngle = abs(mpu.getAngleZ()) - startAngle;
    }
    MPUtimer = millis();
  }
  if (yawAngle > (90 * turnCount + 1) - 15) {
    turnCount++;
    mySerial.println(turnCount);
  }
}

void check3Lap() {
  if (endAngleReached > 6 && (huskylens.request(3) || huskylens.request(4))) {
    if (huskylens.count(3) > 0 || huskylens.count(4) > 0) {
      myservo.write(90);
      drive(-150);
      delay(100);
      drive(0);
      mySerial.println("\n3 laps completed!\n");
      mySerial.print("Lap time: ");
      int lapTime = (millis() - lapTimer) / 1000;
      mySerial.print(lapTime);
      mySerial.println(" seconds");
      while (1)
        getInputs();
    }
  }
}
