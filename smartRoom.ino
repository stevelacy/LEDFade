// get /led/int red/int green/ int blue
// arduino.local/arduino/led/255/0/0

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN     3
#define LEDNUM  183

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDNUM, PIN, NEO_GRB + NEO_KHZ800);
YunServer server;

int full = 0;
int red = 0;
int green = 0;
int blue = 0;

void setup() {
  Serial.begin(9600);
  Bridge.begin();
  strip.begin();

  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  YunClient request = server.accept();

  if (request) {
    process(request);
    request.stop();
  }

  delay(50); // Poll requests
}

void process(YunClient request) {
  String command = request.readStringUntil('/');

  // /arduino/led/
  if (command == "led") {
    ledCommand(request);
  }

}

void ledCommand(YunClient request) {

  // Read rgb numbers
  red = request.parseInt();
  if (request.read() == '/') {
    green = request.parseInt();
    if (request.read() == '/') {
      blue = request.parseInt();
    }
  }


  full = 0;
  fadeOut();
  setStrip(red, green, blue);

  // Return request
  request.print(F("red: "));
  request.print(red);
  request.print(F(" green: "));
  request.print(green);
  request.print(F(" blue: "));
  request.print(blue);
}


// Set LED strip colors
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
    delay(10);

    if (i == 255) {
      full = 1;
    }
  }
}

void fadeOut () {
  for (int i = 255; i >=0; --i) {
    strip.setBrightness(i);
    strip.show();
    delay(5);
  }
}
