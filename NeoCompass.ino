#include <LSM303.h>
#include "MyLSM303.h"
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

// NeoPixel 12-LED ring on PIN 12
#define LEDS 12
#define PIN 12
#define PIXEL_SHIFT 130
#define MAX_BRIGHTNESS 128
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
    compass.m_min = (LSM303::vector<int32_t>){  -593,   -777,   -834 };
    compass.m_max = (LSM303::vector<int32_t>){  +670,   +432,   +227 };
}

int zval = 0;

// the loop routine runs over and over again forever:
void loop() {
    compass.read();

    double a_magnitude = sqrt(LSM303::vector_dot(&compass.a, &compass.a));
    LSM303::vector<float> a = { compass.a.x/a_magnitude, compass.a.y/a_magnitude, compass.a.z/a_magnitude };

    double m_magnitude = sqrt(LSM303::vector_dot(&compass.m, &compass.m));
    LSM303::vector<float> m = { compass.m.x/m_magnitude, compass.m.y/m_magnitude, compass.m.z/m_magnitude };

    float yaw = compass.yaw();
    float heading = compass.heading();
    float pitch = compass.pitch();
    float roll = compass.roll();

    // This is the angle of the PCB relative to the horizon: 0=face-up, 90=vertical, 180=face-down
    // "pitch" is relative to the X-axis, this is not.
    float zangle = atan2(sqrt(sq(a.x)+sq(a.y)), a.z) * 180 / M_PI;
    Serial.print("xybyz: "); Serial.println(zangle);

    float topangle = atan2(a.y, a.x) * 180 / M_PI;

    Serial.print("yaw: "); Serial.print(round(yaw));
    Serial.print(", top: "); Serial.print(round(topangle));
    Serial.print(", pitch: "); Serial.print(round(pitch));
    Serial.print(", roll: "); Serial.println(round(roll));

    Serial.print("(ax,ay,az) = (");
    Serial.print(a.x); Serial.print(", ");
    Serial.print(a.y); Serial.print(", ");
    Serial.print(a.z); Serial.println(")");
    Serial.print("(mx,my,mz) = (");
    Serial.print(m.x); Serial.print(", ");
    Serial.print(m.y); Serial.print(", ");
    Serial.print(m.z); Serial.println(")");

    yaw   = 360 - yaw + PIXEL_SHIFT;
    topangle = 380 - topangle + PIXEL_SHIFT;
    int toppixel = ((int) (topangle / 30)) % 12;
    int pixel = ((int) (yaw / 30)) % 12;

    strip.clear();

    if(pixel != toppixel) {
	strip.setPixelColor(pixel, doGamma(MAX_BRIGHTNESS), 0, 0);
	strip.setPixelColor(toppixel, 0, 0, doGamma(MAX_BRIGHTNESS));
    } else {
	strip.setPixelColor(toppixel, doGamma(MAX_BRIGHTNESS), 0, doGamma(MAX_BRIGHTNESS));
    }

    /*
    int zperint = (int) (abs(pitch) / 90 * 70);

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
    delay(500);
}
