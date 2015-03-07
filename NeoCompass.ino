#include <LSM303.h>
#include "MyLSM303.h"
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

// NeoPixel 12-LED ring on PIN 12
#define LEDS 12
#define PIN 12
#define PIXEL_SHIFT 220
uint32_t topColor, bottomColor;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, NEO_GRB + NEO_KHZ800);
MyLSM303 compass;

// the setup routine runs once when you press reset:
void setup() {                
    // initialize the digital pin as an output.
    strip.begin();
    strip.clear();
    strip.show();  

    topColor = strip.Color(1 << 3, 0, 0);
    bottomColor = strip.Color(0, 0, 1 << 3);

    Serial.begin(9600);

    compass.init();
    compass.enableDefault();
    compass.m_min = (LSM303::vector<int16_t>){  -593,   -777,   -834};
    compass.m_max = (LSM303::vector<int16_t>){  +670,   +432,   +227};
}

int zval = 0;
const float Pi = 3.14159;

// the loop routine runs over and over again forever:
void loop() {
    compass.read();

    float heading = compass.heading();
    double val = compass.vertical_magnitude();
    double maxA = sqrt(sq(compass.a.x) + sq(compass.a.y) + sq(compass.a.z));
    Serial.print("val: "); Serial.println(val);
    // Serial.print(", z: "); Serial.println(compass.a.z);
    Serial.print("(ax,ay,az) = (");
    Serial.print(compass.a.x); Serial.print(", ");
    Serial.print(compass.a.y); Serial.print(", ");
    Serial.print(compass.a.z); Serial.println(")");

    /*
    // zpercent = (zaccel / accelMagnitude + 1.0) / 2.0;
    float zpercent = (zmag / magMagnitude + 1.0) / 2.0;

    // Normalize to 0-360
    // float heading   = (atan2(xaccel, yaccel) * 180) / Pi;
    float heading   = (atan2(xmag, ymag) * 180) / Pi;
    if (heading >= 360) { heading = heading - ((int) (heading / 360)) * 360; }
    */
    heading   = 360 - heading + PIXEL_SHIFT;
    int pixel = ((int) (heading / 30)) % 12;

    strip.clear();

    strip.setPixelColor(pixel, doGamma(64), 0, 0);
    /*
    int zperint = (int) (zpercent * 70);
    for(int i=0; i<(zperint / 10); i++) {
	strip.setPixelColor((pixel + i) % 12, doGamma(64), 0, 0);
	strip.setPixelColor((pixel - i + 12) % 12, doGamma(64), 0, 0);
    }

    for(int i=(zperint / 10); i<7; i++) {
	strip.setPixelColor((pixel + i) % 12, 0, 0, doGamma(64));
	strip.setPixelColor((pixel - i + 12) % 12, 0, 0, doGamma(64));
    }

    if(zperint % 10) {
	float fraction = (zperint % 10) / 10.0 * 64.0;
	strip.setPixelColor((pixel + zperint / 10) % 12, doGamma((int) fraction), 0, doGamma((int) (64.0 - fraction)));
	strip.setPixelColor((pixel - zperint / 10 + 12) % 12, doGamma((int) fraction), 0, doGamma((int) (64.0 - fraction)));
    }

    */
    strip.show();
    delay(300);
}
