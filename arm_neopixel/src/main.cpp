#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define PIN PA6
#define NUMPIXELS 24

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  pixels.begin();
}

void loop()
{
  for (int i = 0; i < 24; i++)
  {
    if (i % 3 == 0)
    {
      pixels.setPixelColor(i, pixels.Color(250, 250, 250));
    }
    else if (i % 3 == 1)
    {
      pixels.setPixelColor(i, pixels.Color(250, 250, 250));
    }
    else if (i % 3 == 2)
    {
      pixels.setPixelColor(i, pixels.Color(250, 250, 250));
    }
    pixels.show();
    delay(200);
  }
  pixels.clear();
  pixels.show();
  delay(500);
}