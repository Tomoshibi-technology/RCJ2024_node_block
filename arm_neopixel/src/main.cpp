// #include <Arduino.h>
// #include <Adafruit_NeoPixel.h>

// #include "./fled/fled.h"

// #define ledpin1 PA5
// #define ledpin2 PA6
// #define numpixels 24
// #define led_count 12
// #define led_brightness 255

// HardwareSerial Serial1(PA10, PA9);

// Adafruit_NeoPixel raw_led1(numpixels, ledpin1, NEO_GRB + NEO_KHZ800);
// Adafruit_NeoPixel raw_led2(numpixels, ledpin2, NEO_GRB + NEO_KHZ800);
// FLED led1(&raw_led1, numpixels);
// FLED led2(&raw_led2, numpixels);

// void calc_led_vector(int led_id, double led_vector[]);
// void calc_target_vector(int target_center, double target_vector[]);
// void calc_cossim(double led_vector[], double target_vector[], double *cossim);
// double calc_blur(int width);
// int calc_diff(double cossim, double led_width);
// void exe_led(int id, int h, int s, int target_center, int target_width);

// void setup()
// {
//   led1.init();
//   led2.init();
//   Serial1.begin(115200);
//   delay(1000);
// }

// void loop()
// {
//   for (int i = 0; i < 360; i++)
//   {
//     // exe_led(11, int(i * 255 / 360), 255, i, 3);
//     exe_led(12, int(360 * 0.87), 255, i, 0);
//     // exe_led(21, int(i * 255 / 360), 255, i, 3);
//     exe_led(22, int(360 * 0.87), 255, i, 0);
//   }
// }

// void calc_led_vector(int led_id, double *led_vector)
// {
//   double degree = (15 + led_id * (360 / led_count)) * 2 * PI / 360;
//   led_vector[0] = sin(degree);
//   led_vector[1] = cos(degree);
// }

// void calc_target_vector(int target_center, double *target_vector)
// {
//   double degree = target_center * 2 * PI / 360;
//   target_vector[0] = sin(degree);
//   target_vector[1] = cos(degree);
// }

// void calc_cossim(double *led_vector, double *target_vector, double *cossim)
// {
//   *cossim = ((led_vector[0] * target_vector[0] + led_vector[1] * target_vector[1]) + 1) / 2;
// };

// // double calc_blur(int width)
// // {
// //   double rad = width * 2 * PI / 360;
// //   return 1 / (1 - cos(rad / 4));
// // }

// // int calc_diff(double cossim, int led_width)
// // {
// //   double blur = calc_blur(led_width);
// //   if ((cossim - 1) * blur + 1 > 0)
// //   {
// //     return (int)(((cossim - 1) * blur + 1) * led_brightness);
// //   }
// //   else
// //   {
// //     return 0;
// //   }
// // }

// double calc_blur(double width)
// {
//   double rad = width * 2 * PI / 360;
//   return 1 / (1 - cos(rad / 4));
// }

// int calc_diff(double cossim, double blur)
// {
//   // double blur = calc_blur(led_width);
//   if ((cossim - 1) * blur + 1 > 0)
//   {
//     return (int)(((cossim - 1) * blur + 1) * led_brightness);
//   }
//   else
//   {
//     return 0;
//   }
// }

// void exe_led(int id, int h, int s, int target_center, int target_width)
// {
//   switch (id)
//   {
//   case 11:
//     for (int j = 0; j < 12; j++)
//     {
//       double led_vector[] = {0, 0};
//       double target_vector[] = {0, 0};
//       double cossim;
//       calc_led_vector(j, led_vector);
//       calc_target_vector(target_center, target_vector);
//       calc_cossim(led_vector, target_vector, &cossim);
//       led1.set_color_hsv(j, h, s, calc_diff(cossim, target_width));
//     }
//     led1.show();
//     break;

//   case 12:
//     for (int j = 12; j < 24; j++)
//     {
//       double led_vector[] = {0, 0};
//       double target_vector[] = {0, 0};
//       double cossim;
//       calc_led_vector(j, led_vector);
//       calc_target_vector(target_center, target_vector);
//       calc_cossim(led_vector, target_vector, &cossim);
//       led1.set_color_hsv(j, h, s, calc_diff(cossim, target_width));
//     }
//     led1.show();
//     break;

//   case 21:
//     for (int j = 0; j < 12; j++)
//     {
//       double led_vector[] = {0, 0};
//       double target_vector[] = {0, 0};
//       double cossim;
//       calc_led_vector(j, led_vector);
//       calc_target_vector(target_center, target_vector);
//       calc_cossim(led_vector, target_vector, &cossim);
//       led2.set_color_hsv(j, h, s, calc_diff(cossim, target_width));
//     }
//     led2.show();
//     break;

//   case 22:
//     for (int j = 12; j < 24; j++)
//     {
//       double led_vector[] = {0, 0};
//       double target_vector[] = {0, 0};
//       double cossim;
//       calc_led_vector(j, led_vector);
//       calc_target_vector(target_center, target_vector);
//       calc_cossim(led_vector, target_vector, &cossim);
//       led2.set_color_hsv(j, h, s, calc_diff(cossim, target_width));
//     }
//     led2.show();
//     break;

//   default:
//     break;
//   }
// }

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <SCServo.h>

#include "./fled/fled.h"

#define servo_RX PC7
#define servo_TX PC6
#define ledpin1 PA5
#define ledpin2 PA6
#define numpixels 24
#define led_count 12
#define led_brightness 255

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
int calc_diff(double cossim, double led_width);
void exe_led(int id, int h, int s, int target_center, int target_width);

void apply(int id, int target, int speed, int acceleration);
int RCV_DATA(int id);

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

void setup()
{
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
  unsigned long now = millis();
  if (4000 <= now && now < 10000)
    exe1();
  if (10000 <= now && now < 11000)
    exe2();
  if (11500 <= now && now < 12500)
    exe3();
  if (13000 <= now && now < 15000)
    exe4();
  if (14000 <= now && now < 16000)
    exe5();
  if (16000 <= now && now < 18000)
    exe6();
  if (18000 <= now && now < 20000)
    exe7();
  if (24500 <= now && now < 25500)
    exe8();
  if (25500 <= now && now < 27500)
    exe9();
  if (28000 <= now && now < 29000)
    exe10();
  if (29000 <= now && now < 31000)
    exe11();
  if (31500 <= now && now < 33500)
    exe12();
  if (34000 <= now && now < 41000)
    exe13();
  Serial1.println(millis());
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
    exe_led(12, 0, 0, 90, i);
    exe_led(22, 0, 0, 270, i);
  }
}

void exe5()
{
  for (int i = 0; i < 700; i++)
  {
    exe_led(11, 0, 0, 0, i);
    exe_led(21, 0, 0, 0, i);
  }
}

void exe6()
{
  for (int i = 0; i < 256; i++)
  {
    exe_led(11, 120, i, 0, 700);
    exe_led(12, 180, i, 0, 700);
    exe_led(21, 120, i, 0, 700);
    exe_led(22, 180, i, 0, 700);
    delay(5);
  }
}

void exe7()
{
  for (int i = 0; i < 256; i++)
  {
    exe_led(11, (120 + i) % 256, 255, 0, 700);
    exe_led(12, (180 + i) % 256, 255, 0, 700);
    exe_led(21, (120 + i) % 256, 255, 0, 700);
    exe_led(22, (180 + i) % 256, 255, 0, 700);
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
  apply(1, 360, 2047, 20);
  apply(2, 360, 2047, 20);
}

void exe11()
{
  apply(1, 0, 2047, 20);
  apply(2, 0, 2047, 20);
}

void exe12()
{
  for (int i = 0; i < 700; i++)
  {
    exe_led(11, 120, 255, 0, 800 - i);
    exe_led(12, 180, 255, 90, 800 - i);
    exe_led(21, 120, 255, 0, 800 - i);
    exe_led(22, 180, 255, 270, 800 - i);
  }
}

void exe13()
{
  for (int i = 0; i < 1800; i += 4)
  {
    exe_led(11, (120 + i * 256 / 1800) % 256, 255, i % 360, 100);
    exe_led(12, (180 + i * 256 / 1800) % 256, 255, i % 360, 100);
    exe_led(21, (120 + i * 256 / 1800) % 256, 255, 360 - i % 360, 100);
    exe_led(22, (180 + i * 256 / 1800) % 256, 255, i % 360, 100);
    apply(1, 180 + 180 * sin(i / 300), 1500, 50);
    apply(2, 180 + 180 * sin(i / 300), 1500, 50);
    apply(3, 90 + 90 * sin(i / 300), 1500, 50);
  }
}

void apply(int id, int target, int speed, int acceleration)
{
  st.WritePosEx(id + 10, (int)(target * 4000 / 360) + 50, speed, acceleration);
  st.WritePosEx(id + 20, (int)((360 - target) * 4000 / 360) + 50, speed, acceleration);
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

int calc_diff(double cossim, int led_width)
{
  double blur = calc_blur(led_width);
  if ((cossim - 1) * blur + 1 > 0)
  {
    return (int)(((cossim - 1) * blur + 1) * led_brightness);
  }
  else
  {
    return 0;
  }
}

void exe_led(int id, int h, int s, int target_center, int target_width)
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
      led1.set_color_hsv(j, h, s, calc_diff(cossim, target_width));
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
      led1.set_color_hsv(j, h, s, calc_diff(cossim, target_width));
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
      led2.set_color_hsv(j, h, s, calc_diff(cossim, target_width));
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
      led2.set_color_hsv(j, h, s, calc_diff(cossim, target_width));
    }
    led2.show();
    break;

  default:
    break;
  }
}