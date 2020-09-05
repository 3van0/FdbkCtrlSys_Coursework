const int interruptPin = 2;
volatile int count;

void myfun()
{
  count += 1;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), myfun, FALLING);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(count, DEC);
}
