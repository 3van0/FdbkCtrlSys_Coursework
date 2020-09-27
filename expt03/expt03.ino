#include <PID_v1.h>
#include "CtrlHG.h"

//定义引脚连接
int senPinA = 2;
int senPinB = 3;
int dirc = 1;
int value = 1;
volatile int count = 0;
HGMotor motor1;
double pidIn, pidOut, pidSet;
double pidIn2, pidOut2, pidSet2;
double pidIn3, pidOut3, pidSet3;
/*
double kp = 3.45;
double ki = 0;
double kd = 0.651;
double kp3 = 3.145;
double ki3 = 0;
double kd3 = 0.605;
double kp2 = 0.3;
double ki2 = 0;
double kd2 = 0;
 */
double kp = 3.6;
double ki = 0;
double kd = 0.68;
double kp3 = 3.55;
double ki3 = 0;
double kd3 = 0.64;
double kp2 = 0.25;
double ki2 = 0;
double kd2 = 0;
bool fwdbkwdflag = true;

unsigned long timelast = 0;
unsigned long timenow;

int countlast = 0;
double dcount = 0;
double dtime = 1;
double v;

PID myPID(&pidIn, &pidOut, &pidSet, kp, ki, kd, DIRECT);
PID myPID2(&pidIn2, &pidOut2, &pidSet2, kp2, ki2, kd2, DIRECT);
PID myPID3(&pidIn3, &pidOut3, &pidSet3, kp3, ki3, kd3, DIRECT);

void setup()
{
  pinMode(senPinA, INPUT);
  pinMode(senPinB, INPUT);
  attachInterrupt(digitalPinToInterrupt(senPinA), senchanged, CHANGE); //设置中断0的处理函数，电平变化触发
  attachInterrupt(digitalPinToInterrupt(senPinB), senchanged, CHANGE); //设置中断0的处理函数，电平变化触发
  motor1.setPin(5, 6);
  motor1.setDirection(FORWARD);

  Serial.begin(9600);
  //pidSet = 224;
  //pidSet2 = 400;
  myPID.SetOutputLimits(-900, 900);
  myPID.SetMode(AUTOMATIC);
  myPID2.SetOutputLimits(30, 190);
  myPID2.SetMode(AUTOMATIC);
  myPID3.SetOutputLimits(-800, 800);
  myPID3.SetMode(AUTOMATIC);
}

void loop()
{
  
  goTo2(224, FORWARD);
  delay(800);
  goTo2(0, BACKWARD);
  delay(800);

}


//中断处理函数
void senchanged()
{
  count += motor1.addValue;
}
/*
bool goTo(int dest, int dirc)
{
  pidSet = dest;
  int destcount = 0;
  int stopcount = 0;
  //int countlast = 0;
  while (true)
  {
    countlast = count;
    pidIn = count;
    myPID.Compute();
    //Serial.println(pidOut);
    if (dirc == 1)
    {
      pidSet2 = pidOut;
      motor1.setDirection(FORWARD);
    }
    else
    {
      pidSet2 =  - pidOut;
      motor1.setDirection(BACKWARD);
    }
    timenow = millis();
    dcount = count - countlast;
    dtime = timenow - timelast;
    timelast = timenow;
    countlast = count;
    v = dcount * 1000 / dtime;
    if (not isnan(v))
    {
      pidIn2 = v;
      myPID2.Compute();
      //Serial.println(pidOut2);
      motor1.updateSpeed(pidOut2);
      //Serial.println(v);
    }

    stopcount += 1;
    if (count - countlast != 0)
    {
      stopcount = 0;
    }
    if (stopcount > 100)
    {
      motor1.setDirection(STOP);
      return (true);
    }
    Serial.println(count);
  }

}
*/

bool goTo2(int dest, int dirc)
{
  pidSet = dest;
  pidSet3 = dest;
  int destcount = 0;
  int stopcount = 0;
  //int countlast = 0;
  while (true)
  {
    countlast = count;
    pidIn = count;
    pidIn3 = count;
    //Serial.println(pidOut);
    
    if (abs(dest - count) < 10)
    {
      motor1.setDirection(STOP);
      //return;
    }
    if (dirc == 1)
    {
      myPID.Compute();
      if (pidOut >= 0)
      {
        pidSet2 = pidOut;
        motor1.setDirection(FORWARD);
      }
      else
      {
        pidSet2 = 0;      
        //motor1.setDirection(STOP);
      }
    }
    else if (dirc == -1)
    {
      myPID3.Compute();
      if (pidOut3 >= 0)
      {
        pidSet2 = 0;
        //motor1.setDirection(STOP);
      }
      else
      {
        pidSet2 = - pidOut3;
        motor1.setDirection(BACKWARD);
      }
    }
    timenow = millis();
    dcount = count - countlast;
    dtime = timenow - timelast;
    timelast = timenow;
    countlast = count;
    v = dcount * 1000 / dtime;
     
    if (not isnan(v))
    {
      pidIn2 = v;
      myPID2.Compute();
      //Serial.println(pidOut2);
      motor1.updateSpeed(pidOut2);
    }
    stopcount += 1;
    if (count - countlast != 0)
    {
      stopcount = 0;
    }
    if (stopcount > 100)
    {
      motor1.setDirection(STOP);
      return (true);
    }
    Serial.println(count);
  }

}
