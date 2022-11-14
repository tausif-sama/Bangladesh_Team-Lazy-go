#include <NewPing.h>
#include <PIDController.h>
#include <ESP32Servo.h>

Servo myservo;
#define MAX_DISTANCE 400 // Maximum distance (in cm) to ping.
PIDController pid; // Create an instance of the PID controller class, called "pid"
NewPing sonarL(13, 13, MAX_DISTANCE), sonarR(14, 14, MAX_DISTANCE);     // 2 = right sensor, 3 = left

#define IN1 4
#define IN2 25
#define PWM 2
#define stdby 26
int dif, ID;
int angle;
int cal, pwm;
long duration, distance, RD, FD, LD;
int turndis = 110;
char dir;
float p = 26, i = .1, d = 8;

void setup () {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(stdby, OUTPUT);

  if (digitalRead(18) == LOW) {
    dir = 'R';
    digitalWrite(19, LOW);
  } else {
    dir = 'L';
    digitalWrite(19, HIGH);
  }

  pid.begin();          // initialize the PID instance
  pid.setpoint(27);    // The "goal" the PID controller tries to "reach"
  pid.tune(p, i, d);  // Tune the PID, arguments: kP, kI, kD
  myservo.attach(33);
  myservo.write(95); // 57, 95, 140
  dir = 'L';

}
int sensorValue, output, lastOutput;
unsigned long lastMillis = 0, zeroStartMillis = 0;
float alpha = 0.6;
int speed = 110;
void loop () {
  delay(2);
  //   pid.setpoint(19);
  Serial.print(dir);
  Serial.print("\t");
  Serial.print(sensorValue);
  Serial.print("\t");
  Serial.print(output);
  Serial.print("\t");


  if (dir == 'R') {
    pid.limit(-42, 50);   // Limit the PID output, this is important to get rid of integral windup!
    RD = sonarR.ping_cm();
    if (RD == 0) RD = 200;

    sensorValue = RD;  // Read the value from the sensor
    output = pid.compute(sensorValue);   // Let the PID compute the value, returns the optimal output
    //    output = constrain(output, -42, 50);

    myservo.write(95 + output);
    drive(speed - (abs(output) / 1));
  } else {
    pid.limit(-42, 40);   // Limit the PID output, this is important to get rid of integral windup!
    LD = sonarL.ping_cm();
    if (LD == 0)
      LD = 200;
    //    else
    //      sensorValue = alpha * LD + (1 - alpha) * sensorValue;
    sensorValue = LD;
    output = pid.compute(sensorValue);   // Let the PID compute the value, returns the optimal output
    output = constrain(output, -42, 40);

    myservo.write(95 - output);
    if (abs(output) > 5) {
      drive(speed - map(abs(output), 5, 42, 25, 60));
    }else{
      drive(speed);
    }
  }
  Serial.write(sensorValue);
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
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(PWM, constrain(pwm, 0, 255));
  digitalWrite(stdby, HIGH);
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
    }
  }
}
