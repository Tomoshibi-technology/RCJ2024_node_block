#include <Arduino.h>
#include <SCServo.h>

#define servo_RX PC7
#define servo_TX PC6

HardwareSerial ServoSerial(servo_RX, servo_TX); //Serial6

SMS_STS st;


void setup()
{
  ServoSerial.begin(1000000);
  st.pSerial = &ServoSerial;
  delay(1000);
}

void loop()
{
  for (int i = 0; i < 4096; i = i + 10)
  {
    st.WritePosEx(1, i, 2048, 100);
    st.WritePosEx(2, i, 2048, 100);
    st.WritePosEx(3, i, 2048, 100);
    st.WritePosEx(11, i, 2048, 100);
    st.WritePosEx(12, i, 2048, 100);
    st.WritePosEx(13, i, 2048, 100);
    delay(20);
  }
  delay(2000);
}