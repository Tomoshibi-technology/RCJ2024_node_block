#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <SCServo.h>

#include "./fled/fled.h"
#include "./control/control.h"

#define servo_RX PC7
#define servo_TX PC6
#define ledpin1 PA5
#define ledpin2 PA6
#define numpixels 24
#define led_count 12

int now = 0;
int once = 0;
int start = 0;

HardwareSerial Serial1(PA10, PA9);
HardwareSerial ServoSerial(servo_RX, servo_TX);

Adafruit_NeoPixel raw_led1(numpixels, ledpin1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel raw_led2(numpixels, ledpin2, NEO_GRB + NEO_KHZ800);
FLED led1(&raw_led1, numpixels);
FLED led2(&raw_led2, numpixels);

SMS_STS st;

void calc_led_vector(int led_id, double led_vector[]);
void calc_target_vector(int target_center, double target_vector[]);
void calc_cossim(double led_vector[], double target_vector[], double *cossim);
double calc_blur(int width);
int calc_diff(double cossim, int led_width, int brightness);
void exe_led(int id, int h, int s, int target_center, int target_width, int brightness);

void apply(int id, int target, int speed, int acceleration);

void exe1();
void exe2();
void exe3();
void exe4();
void exe5();
void exe6();
void exe7();
void exe8();
void exe9();
void exe10();
void exe11();
void exe12();
void exe13();
void exe14();
void exe15();
void exe16();

void setup()
{
  control_init();
  led1.init();
  led2.init();
  raw_led1.clear();
  raw_led2.clear();
  raw_led1.show();
  raw_led2.show();
  ServoSerial.begin(1000000);
  Serial1.begin(115200);
  st.pSerial = &ServoSerial;
  delay(1000);
}

void loop()
{
  control_loop();
  if (twelite.receive_data[2] == 200 && once == 0)
  {
    start = int(millis() / 100);
    once += 1;
    Serial1.println("!!!!!!!");
    Serial1.println(start);
  }
  if (twelite.read())
  { // tweliteから受信成功したら1を返す
    // PC.print(micros() - loop_timer);
    //  for(int i=0; i<4; i++){
    //  	Serial1.print("  :");
    //  	Serial1.print(twelite.receive_data[i]);
    //  }
    //  Serial1.println();
  }
  //---------------------
  // unsigned long now = millis();
  if (once == 1)
  {
    now = int(millis() / 100) - start;
    if (40 <= now && now < 100)
      exe1();
    if (100 <= now && now < 110)
      exe2();
    if (115 <= now && now < 125)
      exe3();
    if (130 <= now && now < 150)
      exe4();
    if (140 <= now && now < 160)
      exe5();
    if (160 <= now && now < 180)
      exe6();
    if (180 <= now && now < 200)
      exe7();
    if (245 <= now && now < 255)
      exe8();
    if (255 <= now && now < 275)
      exe9();
    if (285 <= now && now < 305)
      exe10();
    if (310 <= now && now < 350)
      exe11();
    if (350 <= now && now < 400)
      exe12();
    if (450 <= now && now < 500)
      exe13();
    if (500 <= now && now < 550)
      exe14();
    if (555 <= now && now < 570)
      exe15();
    if (580 <= now && now < 590)
      exe16();
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

void exe4()
{
  for (int i = 0; i < 700; i++)
  {
    exe_led(12, 0, 0, 90, i, 255);
    exe_led(22, 0, 0, 270, i, 255);
  }
}

void exe5()
{
  for (int i = 0; i < 700; i++)
  {
    exe_led(11, 0, 0, 0, i, 255);
    exe_led(21, 0, 0, 0, i, 255);
  }
}

void exe6()
{
  for (int i = 0; i < 256; i++)
  {
    exe_led(11, 120, i, 0, 700, 255);
    exe_led(12, 180, i, 0, 700, 255);
    exe_led(21, 120, i, 0, 700, 255);
    exe_led(22, 180, i, 0, 700, 255);
    delay(5);
  }
}

void exe7()
{
  for (int i = 0; i < 256; i++)
  {
    exe_led(11, (120 + i) % 256, 255, 0, 700, 255);
    exe_led(12, (180 + i) % 256, 255, 0, 700, 255);
    exe_led(21, (120 + i) % 256, 255, 0, 700, 255);
    exe_led(22, (180 + i) % 256, 255, 0, 700, 255);
    delay(10);
  }
}

void exe8()
{
  apply(3, 360, 2047, 20);
}

void exe9()
{
  apply(3, 0, 2047, 20);
}

void exe10()
{
  for (int i = 0; i < 700; i++)
  {
    exe_led(11, 120, 255, 0, 800 - i, 255);
    exe_led(12, 180, 255, 90, 800 - i, 255);
    exe_led(21, 120, 255, 0, 800 - i, 255);
    exe_led(22, 180, 255, 270, 800 - i, 255);
  }
}

void exe11()
{
  for (int i = 0; i < 1800; i += 4)
  {
    exe_led(11, (120 + i * 256 / 1800) % 256, 255, i % 360, 100, 255);
    exe_led(12, (180 + i * 256 / 1800) % 256, 255, i % 360, 100, 255);
    exe_led(21, (120 + i * 256 / 1800) % 256, 255, 360 - i % 360, 100, 255);
    exe_led(22, (180 + i * 256 / 1800) % 256, 255, i % 360, 100, 255);
    apply(1, 180 + 180 * sin(i / 300), 1500, 50);
    apply(2, 180 + 180 * sin(i / 300), 1500, 50);
    apply(3, 90 + 90 * sin(i / 300), 1500, 50);
  }
}

void exe12()
{
  apply(1, 360, 400, 50);
  apply(2, 270, 300, 50);
  apply(3, 0, 400, 50);
  for (int i = 0; i < 256; i++)
  {
    exe_led(11, 120, 255 - i, 0, 100 + 2 * i, 255);
    exe_led(12, 180, 255 - i, 0, 100 + 2 * i, 255);
    exe_led(21, 120, 255 - i, 0, 100 + 2 * i, 255);
    exe_led(22, 180, 255 - i, 0, 100 + 2 * i, 255);
    delay(20);
  }
}

void exe13()
{
  for (int i = 0; i < 900; i++)
  {
    apply(1, 180 + 180 * sin(i / 100), 2000, 100);
    apply(2, 180 + 180 * sin(i / 100), 2000, 100);
    apply(3, 180 + 180 * sin(i / 100), 2000, 100);
    exe_led(11, i, i / 4, 0, 500, (i % 60) * 4);
    exe_led(12, i, i / 4, 0, 500, (i % 60) * 4);
    exe_led(21, i, i / 4, 0, 500, (i % 60) * 4);
    exe_led(22, i, i / 4, 0, 500, (i % 60) * 4);
  }
}

void exe14()
{
  for (int i = 0; i < 900; i++)
  {
    apply(1, 0, 700, 50);
    apply(2, 270, 700, 50);
    apply(3, 360, 700, 50);
    exe_led(11, i, 255, 0, 500, (i % 30) * 8);
    exe_led(12, i, 255, 0, 500, (i % 30) * 8);
    exe_led(21, i, 255, 0, 500, (i % 30) * 8);
    exe_led(22, i, 255, 0, 500, (i % 30) * 8);
  }
}

void exe15()
{
  apply(1, 270, 4095, 255);
  apply(2, 180, 4095, 255);
  apply(1, 270, 4095, 255);
  exe_led(11, 0, 0, 0, 500, 255);
  exe_led(12, 0, 0, 0, 500, 255);
  exe_led(21, 0, 0, 0, 500, 255);
  exe_led(22, 0, 0, 0, 500, 255);
}

void exe16()
{
  raw_led1.clear();
  raw_led2.clear();
  raw_led1.show();
  raw_led2.show();
}

//-------------------------------------------------------------
void apply(int id, int target, int speed, int acceleration)
{
  st.WritePosEx(id + 10, (int)(target * 4000 / 360) + 50, speed, acceleration);
  // st.WritePosEx(id + 20, (int)((360 - target) * 4000 / 360) + 50, speed, acceleration);
}
//-------------------------------------------------------------

void calc_led_vector(int led_id, double *led_vector)
{
  double degree = (15 + led_id * (360 / led_count)) * 2 * PI / 360;
  led_vector[0] = sin(degree);
  led_vector[1] = cos(degree);
}

void calc_target_vector(int target_center, double *target_vector)
{
  double degree = target_center * 2 * PI / 360;
  target_vector[0] = sin(degree);
  target_vector[1] = cos(degree);
}

void calc_cossim(double *led_vector, double *target_vector, double *cossim)
{
  *cossim = ((led_vector[0] * target_vector[0] + led_vector[1] * target_vector[1]) + 1) / 2;
};

double calc_blur(int width)
{
  double rad = width * 2 * PI / 360;
  return 1 / (1 - cos(rad / 4));
}

int calc_diff(double cossim, int led_width, int brightness)
{
  double blur = calc_blur(led_width);
  if ((cossim - 1) * blur + 1 > 0)
  {
    return (int)(((cossim - 1) * blur + 1) * brightness);
  }
  else
  {
    return 0;
  }
}

void exe_led(int id, int h, int s, int target_center, int target_width, int brightness)
{
  switch (id)
  {
  case 11:
    for (int j = 0; j < 12; j++)
    {
      double led_vector[] = {0, 0};
      double target_vector[] = {0, 0};
      double cossim;
      calc_led_vector(j, led_vector);
      calc_target_vector(target_center, target_vector);
      calc_cossim(led_vector, target_vector, &cossim);
      led1.set_color_hsv(j, h, s, calc_diff(cossim, target_width, brightness));
    }
    led1.show();
    break;

  case 12:
    for (int j = 12; j < 24; j++)
    {
      double led_vector[] = {0, 0};
      double target_vector[] = {0, 0};
      double cossim;
      calc_led_vector(j, led_vector);
      calc_target_vector(target_center, target_vector);
      calc_cossim(led_vector, target_vector, &cossim);
      led1.set_color_hsv(j, h, s, calc_diff(cossim, target_width, brightness));
    }
    led1.show();
    break;

  case 21:
    for (int j = 0; j < 12; j++)
    {
      double led_vector[] = {0, 0};
      double target_vector[] = {0, 0};
      double cossim;
      calc_led_vector(j, led_vector);
      calc_target_vector(target_center, target_vector);
      calc_cossim(led_vector, target_vector, &cossim);
      led2.set_color_hsv(j, h, s, calc_diff(cossim, target_width, brightness));
    }
    led2.show();
    break;

  case 22:
    for (int j = 12; j < 24; j++)
    {
      double led_vector[] = {0, 0};
      double target_vector[] = {0, 0};
      double cossim;
      calc_led_vector(j, led_vector);
      calc_target_vector(target_center, target_vector);
      calc_cossim(led_vector, target_vector, &cossim);
      led2.set_color_hsv(j, h, s, calc_diff(cossim, target_width, brightness));
    }
    led2.show();
    break;

  default:
    break;
  }
}