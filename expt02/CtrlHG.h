#define FORWARD 1
#define BACKWARD -1
#define STOP 0

class HGMotor
{
public:
    int currentSpeed = 0;
    int currentDirection = 0;
    int pin1, pin2;
    volatile int currentPos;
    double s = 0;
    char addValue = 0;

    void setPin(int p1, int p2)
    {
        //设置引脚
        pinMode(p1, OUTPUT);
        pinMode(p2, OUTPUT);
        pin1 = p1;
        pin2 = p2;
    }

    void updateSpeed(int speed)
    {
        //更新速度
        if (currentDirection == FORWARD)
        {
            analogWrite(pin1, speed);
            digitalWrite(pin2, LOW);
        }
        else if (currentDirection == BACKWARD)
        {
            analogWrite(pin2, speed);
            digitalWrite(pin1, LOW);
        }
        else if (currentDirection == STOP)
        {
            digitalWrite(pin1, HIGH);
            digitalWrite(pin2, HIGH);
        }
        currentSpeed = speed;
    }

    void setDirection(int dirc)
    {
        currentDirection = dirc;
        if (dirc != STOP)
        {
            addValue = currentDirection;
        }

    }

    void pidInput(int speed)
    {

        double lowlimtfwd = 40;
        double lowlimtbkwd = 40;
        if (speed > 0)
        {
            s = (255 - lowlimtfwd) / 255 * speed + lowlimtfwd;
            this->setDirection(FORWARD);
            this->updateSpeed(s);
        }
        else
        {
            s = (255 - lowlimtbkwd) / 255 * (-speed) + lowlimtbkwd;
            this->setDirection(BACKWARD);
            this->updateSpeed(s);
        }
    }

    void fwdPidIn(int speed)
    {
        if (speed < 40)
        {
            speed = 0;
        }
        this->setDirection(FORWARD);
        this->updateSpeed(speed);
    }

    void bkwdPidIn(int speed)
    {
        if (speed > -40)
        {
            speed = 0;
        }
        this->setDirection(BACKWARD);
        this->updateSpeed(-speed);
    }
};
