#define I2C
//#define UART
//#define SPI_SS
   
#ifdef I2C

#include <Pixy2I2C.h>
Pixy2I2C pixy;

#else 
#ifdef UART

#include <Pixy2UART.h>
Pixy2UART pixy;

#else 
#ifdef SPI_SS

#include <Pixy2SPI_SS.h>
Pixy2SPI_SS pixy;

#else

#include <Pixy2.h>
Pixy2 pixy;

#endif
#endif
#endif


// This is the main Pixy object 


#include <NewPing.h>
#include <Wire.h>

#include <Servo.h>
Servo myservo;


#define MAX_DISTANCE 400 // Maximum distance (in cm) to ping.
NewPing sonarL(12, 12, MAX_DISTANCE), sonarR(2, 2, MAX_DISTANCE);
#define IN1 7
#define IN2 8
#define PWM 5
#define stdby 4
int dif, ID, LDF;
int cal, pwm;
long duration, wallDist=40, mid_pos=90, max_steering=32, obj_h=60;
int turndis = 110;
int count = 0;
bool just_avoided=false;
char dir;

int autoSteer(char);
void setup(void)
{
  //Serial.begin(9600);
  //Serial.print("Starting...\n");
  pixy.init();
  pixy.setLamp(1,0);
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
  myservo.attach(10);
  myservo.write(90);
  delay(500);
}
void loop(void)
{
  int angle,g_x,g_h,g_w,g_area;
  pixy.ccc.getBlocks();
  if (pixy.ccc.numBlocks)
  {
    drive(70);
    for (int i=0; i<pixy.ccc.numBlocks; i++)
    {
      if(pixy.ccc.blocks[i].m_signature==4){ // Green obj
        g_h = pixy.ccc.blocks[i].m_height;
        g_w = pixy.ccc.blocks[i].m_width;
        g_x=pixy.ccc.blocks[i].m_x+(g_w/2);
        Serial.println(g_h);
        if(g_h<48 && g_x<290){
          myservo.write(map(g_x,0,320,44,115)); // follow
        }else{
          myservo.write(map(g_x,150,350,50,70)); // avoid
        }
      }else if(pixy.ccc.blocks[i].m_signature==1){ // red obj
        g_h = pixy.ccc.blocks[i].m_height;
        g_w = pixy.ccc.blocks[i].m_width;
        g_x=pixy.ccc.blocks[i].m_x+(g_w/2);
        Serial.println(g_x);
        if(g_h<48 && g_x>30){
          myservo.write(map(g_x,0,300,47,120)); // follow
        }else{
          myservo.write(map(g_x,-30,130,95,120)); // avoid
          just_avoided=true;
        }
      }
    }
  }else{
    if (just_avoided){
      delay(200);
      just_avoided=false;
    }
    angle = autoSteer(dir);
   // Serial.println(angle);
    drive(80+abs(angle));
  }
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
