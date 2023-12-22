#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define arm1_PIN PA6
#define arm2_PIN PA5
#define numpixels 24
#define led_count 12
#define light_blur 60

Adafruit_NeoPixel pixels1(numpixels, arm1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2(numpixels, arm2_PIN, NEO_GRB + NEO_KHZ800);

int calc_led_angle(int led_id);
byte calc_delta_angle(int target_center, int target_halfwidth, int led_angle);
int calc_brightness(int start_angle, int end_angle, int led_id);

void setup()
{
  pixels1.begin();
  pixels2.begin();
  delay(1000);
}

void loop()
{
  // for (int i = 0; i < numpixels; i++)
  // {
  //   pixels1.setPixelColor(i, pixels1.Color(250, 250, 250));
  //   pixels1.show();
  //   pixels2.setPixelColor(i, pixels2.Color(250, 250, 250));
  //   pixels2.show();
  // }
  // for (int j = 0; j < 360; j++)
  // {
  //   for (int i = 0; i < led_count; i++)
  //   {
  //     arm1_pixels.setPixelColor(i, arm1_pixels.Color(calc_brightness(j, j, i), calc_brightness(j, j, i), calc_brightness(j, j, i)));
  //     arm1_pixels.show();
  //     arm1_pixels.setPixelColor(i + 12, arm1_pixels.Color(calc_brightness(j, j, i), calc_brightness(j, j, i), calc_brightness(j, j, i)));
  //     arm1_pixels.show();
  //   }
  // }
}

void calc_led_vector(int led_id, double* led_vector[2])
{
  int degree = 15 + led_id * (360 / led_count);
  *led_vector[0] = sin(degree);
  *led_vector[1] = cos(degree);
}

byte calc_delta_angle(int target_center, int target_halfwidth, int led_angle)
{
  if (target_center - target_halfwidth <= led_angle && led_angle <= target_center + target_halfwidth)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int calc_brightness(int target_center, int target_width, int led_id)
{
  int led_angle = calc_led_angle(led_id);
  bool delta_angle = calc_delta_angle(start_angle, end_angle, led_angle);
  if (delta_angle < 0)
  {
    return 255;
  }
  else if (255 * (1 - delta_angle / light_blur) >= 0)
  {
    return (int)(64 * (1 - delta_angle / light_blur));
  }
  else
  {
    return 0;
  }
}