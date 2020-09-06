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
double kp = 0.98;
double ki = 0;
double kd = 0.09;

bool fwdbkwdflag = true;

PID myPID(&pidIn, &pidOut, &pidSet, kp, ki, kd, DIRECT);

void setup()
{
    pinMode(senPinA, INPUT);
    pinMode(senPinB, INPUT);
    attachInterrupt(digitalPinToInterrupt(senPinA), senchanged, CHANGE); //设置中断0的处理函数，电平变化触发
    attachInterrupt(digitalPinToInterrupt(senPinB), senchanged, CHANGE); //设置中断0的处理函数，电平变化触发
    motor1.setPin(5, 6);
    motor1.setDirection(1);
    Serial.begin(9600);
    pidSet = 224;
    myPID.SetOutputLimits(-255, 255);
    myPID.SetMode(AUTOMATIC);
}

void loop()
{

    goTo(224);
    delay(1000);
    goTo(0);
    delay(1000);

    /*     int i;
    for (i = -1; i < 2; i++)
    {
        motor1.updateSpeed(200);
        motor1.setDirection(i);
        delay(1000);
        Serial.println(count);
    }
    for (i = 1; i > -2; i--)
    {
        motor1.updateSpeed(200);
        motor1.setDirection(i);
        delay(1000);
        Serial.println(count);
    } */
}

//中断处理函数
void senchanged()
{
    count += motor1.addValue;
}

bool goTo(int dest)
{
    pidSet = dest;
    int destcount = 0;
    int stopcount = 0;
    int countlast = 0;
    while (true)
    {
        countlast = count;
        Serial.println(count);
        pidIn = count;
        myPID.Compute();
        motor1.pidInput(pidOut);
        if (abs(dest - count) < 20)
        {
            destcount += 1;
        }
        else
        {
            destcount = 0;
        }

        if (destcount > 50)
        {
            return (true);
        }
        
        stopcount += 1;
        if (count - countlast != 0)
        {
            stopcount = 0;
        }
        if (stopcount > 50)
        {
            return (true);
        }
    }
}
