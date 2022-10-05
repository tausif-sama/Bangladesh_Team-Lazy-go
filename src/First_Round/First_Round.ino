#include <NewPing.h>
//#include <Wire.h>
//#include <PIDController.h>
#include <Servo.h>
Servo myservo;

// Objects
//PIDController pid; // Create an instance of the PID controller class, called "pid"

#define MAX_DISTANCE 400 // Maximum distance (in cm) to ping.
NewPing sonarL(12, 12, MAX_DISTANCE), sonarR(2, 2, MAX_DISTANCE);
#define IN1 7 //assigning the motor pin
#define IN2 8
#define PWM 5
#define stdby 4
int dif, ID, LDF;   //assigning global variables
int angle, w;
int cal, pwm;
long duration, wallDist=40, mid_pos=90, max_steering=34;
char dir;
int turndis = 110;
int count = 0;

unsigned long timenow;

int autoSteer(char); //autosteer function call(this steers the servo on basis of left and right sonar)
void setup(void)
{
  Serial.begin(9600);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(stdby, OUTPUT);
  pinMode(A3, OUTPUT);
  if(digitalRead(A1)==LOW){
    dir='L';  // assigning the switch mechanism    dir='L';
    digitalWrite(A3,LOW);
  }else{
    dir='R';
    digitalWrite(A3,HIGH);
  }
  myservo.attach(10);
  if(sonarL.ping_cm()<sonarR.ping_cm() && sonarR.ping_cm()>40){    //this is the manual control of the bot to go at 19 cm distance
    myservo.write(125);
    drive(130);
    delay(600);
    myservo.write(50);
    drive(130);
    delay(500);
    myservo.write(90);
    delay(30);
    drive(0);
  }

  
 
}
void loop(void)
{


  timenow = millis();
  int angle = autoSteer(dir);
 //Serial.println(angle);
  

  drive(80+abs(angle));  //driving the motors with appropiate speed

  if(timenow>31000){
    drive(0);
  }
}

void drive(int pwm) { //driving function
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
