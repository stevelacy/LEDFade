#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN     3
#define LEDNUM  183
#define DELAY   50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDNUM, PIN, NEO_GRB + NEO_KHZ800);

const int buttonPin = 2;
int full = 0;

void setup() {
  strip.begin();
  pinMode(buttonPin, INPUT);
}

void loop() {
  setStrip(50, 100, 200);
  delay(50);
}

void setStrip (int r, int g, int b) {
  if (full == 1) {
    return;
  }

  for (int i = 0; i <=255; i++) {
    strip.setBrightness(i);

    for (int l=0; l < strip.numPixels(); l++) {
      strip.setPixelColor(l, strip.Color(r, g, b));
    }

    strip.show();

    if (i == 255) {
      full = 1;
    }
  }

}
