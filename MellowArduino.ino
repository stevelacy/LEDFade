// get /led/int red/int green/ int blue
// arduino.local/arduino/led/255/0/0

#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN     3
// #define LEDNUM  183
#define LEDNUM  72

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDNUM, PIN, NEO_GRB + NEO_KHZ800);
YunServer server;
int enableRainbowCycle = 0;
int finished = 0;

int red = 0;
int green = 0;
int blue = 0;

void setup() {
	Serial.begin(9600);
	Bridge.begin();
	strip.begin();
	strip.setBrightness(90); // adjust brightness here
	strip.show(); // Initialize all pixels to 'off'
	server.listenOnLocalhost();
	server.begin();
}

void loop() {
	YunClient request = server.accept();
	if (enableRainbowCycle == 1) {
		rainbowCycle(20);
	}
	if (request) {
		process(request);
		request.stop();
	}
	delay(30);
}

void process(YunClient request) {
	String command = request.readStringUntil('/');

	// /arduino/led/
	if (command == "led") {
		ledCommand(request);
	}
	if (command == "rainbow") {
		enableRainbowCycle = enableRainbowCycle == 1 ? 0 : 1;
		request.print(F("enableRainbowCycle"));
		request.print(enableRainbowCycle);
	}
}

void ledCommand(YunClient request) {
	enableRainbowCycle = 0;
	red = request.parseInt();
	if (request.read() == '/') {
		green = request.parseInt();
		if (request.read() == '/') {
			blue = request.parseInt();
		}
	}

	finished = 0;
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

/*
	 void rainbow(uint8_t wait) {
	 uint16_t i, j;

	 for(j=0; j<256; j++) {
	 for(i=0; i<strip.numPixels(); i++) {
	 strip.setPixelColor(i, Wheel((i+j) & 255));
	 }
	 strip.show();
	 delay(wait);
	 }
	 }
 */

// set one color on the whole strip
void setStrip (int r, int g, int b) {
	if (finished == 1) {
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
			finished = 1;
		}
	}
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
	uint16_t i, j;
	strip.setBrightness(80); // adjust brightness here

	for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
		for(i=0; i< strip.numPixels(); i++) {
			strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
		}
		strip.show();
		delay(wait);
	}
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
	if(WheelPos < 85) {
		return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	} else if(WheelPos < 170) {
		WheelPos -= 85;
		return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	} else {
		WheelPos -= 170;
		return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
}

void fadeOut () {
	for (int i = 255; i >=0; --i) {
		strip.setBrightness(i);
		strip.show();
		delay(5);
	}
}
