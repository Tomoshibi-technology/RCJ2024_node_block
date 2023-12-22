#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define arm1_PIN PA6
#define numpixels 24
#define led_count 12
#define led_brightness 255
#define blur 3

HardwareSerial Serial1(PA10, PA9);

Adafruit_NeoPixel pixels1(numpixels, arm1_PIN, NEO_GRB + NEO_KHZ800);

void calc_led_vector(int led_id, double led_vector[]);
void calc_target_vector(int target_center, double target_vector[]);
void calc_cossim(double led_vector[], double target_vector[], double *cossim);
int calc_diff(double cossim);

void setup()
{
  pixels1.begin();
  Serial1.begin(115200);
  delay(1000);
}

void loop()
{
  double led_vector[] = {0, 0};
  double target_vector[] = {0, 0};
  double cossim;
  for (int i = 0; i < 360; i++)
  {
    for (int j = 12; j < 24; j++)
    {
      calc_led_vector(j, led_vector);
      calc_target_vector(i, target_vector);
      calc_cossim(led_vector, target_vector, &cossim);
      pixels1.setPixelColor(j, pixels1.Color(calc_diff(cossim), calc_diff(cossim), calc_diff(cossim)));
    }
    pixels1.show();
    delay(10);
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

int calc_diff(double cossim)
{
  if((cossim - 1) * blur + 1 > 0)
  {
    return (int)(((cossim - 1) * blur + 1) * led_brightness);
  }
  else
  {
    return 0;
  }
}