// #include <Arduino.h>
// #include <SCServo.h>

// #define servo_RX PC7
// #define servo_TX PC6

// HardwareSerial ServoSerial(servo_RX, servo_TX); //Serial6

// SMS_STS st;


// void setup()
// {
//   ServoSerial.begin(1000000);
//   st.pSerial = &ServoSerial;
//   delay(1000);
// }

// void loop()
// {
//   for (int i = 0; i < 4096; i = i + 10)
//   {
//     st.WritePosEx(11, 2047, 2048, 100);
//     st.WritePosEx(12, 2047, 2048, 100);
//     st.WritePosEx(13, 2047, 2048, 100);
//     st.WritePosEx(21, 2047, 2048, 100);
//     st.WritePosEx(22, 2047, 2048, 100);
//     st.WritePosEx(23, 2047, 2048, 100);
//     delay(20);
//   }
//   delay(2000);
// }

#include <Arduino.h>
#include <SCServo.h>

#define servo_RX PC7
#define servo_TX PC6

HardwareSerial Serial1(PA10, PA9);
HardwareSerial ServoSerial(servo_RX, servo_TX); // Serial6

SMS_STS st;

void exe1();
void exe2();
void exe3();
void exe4();
void exe5();
void exe6();
void exe7();
void apply(int id, int target, int speed, int acceleration);
int RCV_DATA(int id);

void setup()
{
  ServoSerial.begin(1000000);
  Serial1.begin(115200);
  st.pSerial = &ServoSerial;
  delay(1000);
}

void loop()
{
  unsigned long now = millis();
  if (10000 <= now && now < 15000)exe1();
  if (15000 <= now && now < 16000)exe2();
  if (16500 <= now && now < 17500)exe3();
  if (100000 <= now && now < 16000)exe4();
  Serial1.println(millis());
}

void apply(int id, int target, int speed, int acceleration)
{
  st.WritePosEx(id + 10, (int)(target * 4000 / 360) + 50, speed, acceleration);
  st.WritePosEx(id + 20, (int)((360 - target) * 4000/ 360) + 50, speed, acceleration);
}

int RCV_DATA(int id)
{
  if (st.FeedBack(id + 10) != -1)
  {
    int pos = st.ReadPos(id + 10);
    return pos;
  }
  else
  {
    return -1;
  }
}

void exe1()
{
  apply(1, 90, 500, 20);
  apply(2, 180, 500, 20);
  apply(3, 0, 500, 20);
  Serial1.println("exe1");
}

void exe2()
{
  apply(1, 180, 4095, 100);
  Serial1.print("exe2");
}

void exe3()
{
  apply(2, 90, 4095, 100);
  Serial1.print("exe3");
}