#include <Arduino.h>
#include <SCServo.h>

#define servo_RX PC7
#define servo_TX PC6

#include <HardwareSerial.h>
HardwareSerial ServoSerial(servo_RX, servo_TX); //Serial6

SMS_STS st;


void setup()
{
  ServoSerial.begin(1000000);
  // ServoSerial.begin(115200);
  st.pSerial = &ServoSerial;
  delay(1000);
}

void loop()
{
  // ServoSerial.write(150);
  // delay(100);
  // ServoSerial.write(100);
  // delay(100);
  
  // st.WritePosEx(1, 4095, 4095, 100);
  // delay(2000);
  // st.WritePosEx(1, 0, 4095, 100);
  // delay(2000);


  for (int i = 0; i < 4096; i = i + 10)
  {
    st.WritePosEx(1, i, 4095, 100);
    st.WritePosEx(2, i, 4095, 100);
    st.WritePosEx(3, i, 4095, 100);
    st.WritePosEx(11, i, 4095, 100);
    st.WritePosEx(12, i, 4095, 100);
    st.WritePosEx(13, i, 4095, 100);
    delay(20);
  }
}