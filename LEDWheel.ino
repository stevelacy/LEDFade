#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN     3
#define LEDNUM  183
#define DELAY   50

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LEDNUM, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  pixels.begin();
}

void loop() {
  for (int j=0; j < 256; j++) {
    for (int q=0; q < 3; q++) {
      for (int i=0; i < pixels.numPixels(); i = i+3) {
        pixels.setPixelColor(i+q, Wheel( (i+j) % 255));
      }
      pixels.show();
      delay(DELAY);
    }
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;

  if (WheelPos < 85) {
   return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }

  if (WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }

  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
