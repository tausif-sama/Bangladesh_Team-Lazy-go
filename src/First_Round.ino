#include <NewPing.h>
//#include <Wire.h>
//#include <PIDController.h>
#include <Servo.h>
Servo myservo;

// Objects
//PIDController pid; // Create an instance of the PID controller class, called "pid"

#define MAX_DISTANCE 400 // Maximum distance (in cm) to ping.
NewPing sonarL(12, 12, MAX_DISTANCE), sonarR(11, 11, MAX_DISTANCE), sonarF(10, 10, MAX_DISTANCE);
#define IN1 7
#define IN2 8
#define PWM 5
#define stdby 4
int dif, ID, LDF;
int angle, w;
int cal, pwm;
long duration, wallDist=20, mid_pos=82, max_steering=38;
char dir;
int turndis = 110;
int count = 0;

int autoSteer(char);
void setup(void)
{
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(stdby, OUTPUT);
  pinMode(A3, OUTPUT);
  if(digitalRead(A1)==LOW){
    dir='R';
    digitalWrite(A3,LOW);
  }else{
    dir='L';
    digitalWrite(A3,HIGH);
  }
  myservo.attach(2);
  myservo.write(82);
  delay(500);
}
void loop(void)
{
  int angle = autoSteer(dir);
//  Serial.println(angle);
  drive(90+abs(angle));
}

void drive(int pwm) {
  if (pwm >= 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(PWM, pwm);
    digitalWrite(stdby, HIGH);
  } else {
    digitalWrite(IN2, HIGH);
    digitalWrite(IN1, LOW);
    analogWrite(PWM, -pwm);
    digitalWrite(stdby, HIGH);
  }
}
