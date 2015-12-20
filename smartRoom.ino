// get /led/int red/int green/ int blue
// arduino.local/arduino/led/255/0/0

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN     3
#define LEDNUM  177

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDNUM, PIN, NEO_GRB + NEO_KHZ800);
YunServer server;

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

  delay(200); // Poll requests
}

void process(YunClient request) {
  String command = request.readStringUntil('/');

  // http://<ip>/arduino/led/<red>/<green>/<blue>
  // /arduino/led/
  if (command == "led") {
    ledCommand(request);
  }

}

void ledCommand(YunClient request) {
  int r = 0;
  int g = 0;
  int b = 0;

  // Read rgb numbers
  r = request.parseInt();
  if (request.read() == '/') {
    g = request.parseInt();
    if (request.read() == '/') {
      b = request.parseInt();
    }
  }

  setStrip(r, g, b);

  // Return request
  request.print(F("red: "));
  request.print(r);
  request.print(F(" green: "));
  request.print(g);
  request.print(F(" blue: "));
  request.print(b);
  return;
}
void setStrip (int r, int g, int b) {
  for (int l=0; l < strip.numPixels(); l++) {
    strip.setPixelColor(l, strip.Color(r, g, b));
    strip.show();
  }
}
