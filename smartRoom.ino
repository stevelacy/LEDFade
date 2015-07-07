#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN     3
#define LEDNUM  3
#define DELAY   50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDNUM, PIN, NEO_GRB + NEO_KHZ800);

const int buttonPin = 2;
int full = 0;

void setup() {
  strip.begin();
  pinMode(buttonPin, INPUT);
}

void loop() {
  //buttonState = digitalRead(buttonPin);

  for (int i = 0; i <= 255; i++) {
    if (full == 1) {
      return;
    }
    for (int l=0; l < strip.numPixels(); l++) {
      strip.setPixelColor(l, strip.Color(i,i,0));
    }
    strip.show();
    delay(50);
    if (i == 255) {
      full = 1;
    }
  }
}
