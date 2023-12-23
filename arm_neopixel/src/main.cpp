#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "./fled/fled.h"

#define ledpin1 PA5
#define ledpin2 PA6
#define numpixels 24
#define led_count 12
#define led_brightness 255

HardwareSerial Serial1(PA10, PA9);

Adafruit_NeoPixel raw_led1(numpixels, ledpin1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel raw_led2(numpixels, ledpin2, NEO_GRB + NEO_KHZ800);
FLED led1(&raw_led1, numpixels);
FLED led2(&raw_led2, numpixels);

void calc_led_vector(int led_id, double led_vector[]);
void calc_target_vector(int target_center, double target_vector[]);
void calc_cossim(double led_vector[], double target_vector[], double *cossim);
double calc_blur(int width);
int calc_diff(double cossim, double led_width);
void exe_led(int id, int h, int s, int target_center, int target_width);

void setup()
{
  led1.init();
  led2.init();
  Serial1.begin(115200);
  delay(1000);
}

void loop()
{
  for (int i = 0; i < 360; i++)
  {
    exe_led(11, int(i*255/360), 255, i, 3);
    exe_led(12, int(i*255/360), 255, i, 3);
    exe_led(21, int(i*255/360), 255, i, 3);
    exe_led(22, int(i*255/360), 255, i, 3);
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

// double calc_blur(int width)
// {
//   double rad = width * 2 * PI / 360;
//   return 1 / (1 - cos(rad / 4));
// }

// int calc_diff(double cossim, int led_width)
// {
//   double blur = calc_blur(led_width);
//   if ((cossim - 1) * blur + 1 > 0)
//   {
//     return (int)(((cossim - 1) * blur + 1) * led_brightness);
//   }
//   else
//   {
//     return 0;
//   }
// }

double calc_blur(double width)
{
  double rad = width * 2 * PI / 360;
  return 1 / (1 - cos(rad / 4));
}

int calc_diff(double cossim, double blur)
{
  // double blur = calc_blur(led_width);
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

  default:
    break;
  }
}