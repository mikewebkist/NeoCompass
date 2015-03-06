#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

// NeoPixel 12-LED ring on PIN 12
#define LEDS 12
#define PIN 12
#define PIXEL_SHIFT 40
uint32_t topColor, bottomColor;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, NEO_GRB + NEO_KHZ800);

// Compass+Accellerometer
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(456);
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(123);

// the setup routine runs once when you press reset:
void setup() {                
    // initialize the digital pin as an output.
    strip.begin();
    strip.clear();
    strip.show();  

    topColor = strip.Color(1 << 3, 0, 0);
    bottomColor = strip.Color(0, 0, 1 << 3);

    Serial.begin(9600);

    if(!accel.begin())
    {
	/* There was a problem detecting the compass ... check your connections */
	Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
	while(1);
    }
    // accel.enableAutoRange(true);
    if(!mag.begin())
    {
	/* There was a problem detecting the compass ... check your connections */
	Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
	while(1);
    }
    // mag.enableAutoRange(true);

}

int zval = 0;
sensors_event_t event; 
const float Pi = 3.14159;

// the loop routine runs over and over again forever:
void loop() {
    accel.getEvent(&event);

    float xaccel = event.acceleration.x;
    float yaccel = event.acceleration.y;
    float zaccel = event.acceleration.z;

    float accelMagnitude = sqrt(sq(xaccel) + sq(yaccel) + sq(zaccel));

    // Mag Minimums: -69.45  -69.27  -83.57
    // Mag Maximums:  65.55   39.36   25.61

    // Mag Minimums: -56.27  -71.00  -83.27
    // Mag Maximums: 64.27  43.64  47.76


    mag.getEvent(&event);
    float xmag = (event.magnetic.x + 70) / (70 + 70);
    float ymag = (event.magnetic.y + 70) / (70 + 40);
    float zmag = (event.magnetic.z + 85) / (85 + 30);

    float magMagnitude = sqrt(sq(xmag) + sq(ymag) + sq(zmag));

    // zpercent = (zaccel / accelMagnitude + 1.0) / 2.0;
    float zpercent = (zmag / magMagnitude + 1.0) / 2.0;

    float xyval = sqrt(sq(zaccel) + sq(yaccel));
    Serial.print("accel: (");
    Serial.print((int)xaccel); Serial.print(", ");
    Serial.print((int)yaccel); Serial.print(", ");
    Serial.print((int)zaccel); Serial.print(") %: ");
    Serial.println((int)accelMagnitude); 

    // Value when pointing north: 15.28285714, -25.27, -70.72857143
    Serial.print("  mag: (");
    Serial.print(xmag); Serial.print(", ");
    Serial.print(ymag); Serial.print(", ");
    Serial.print(zmag); Serial.print(") %: ");
    Serial.println(magMagnitude); 

    // Normalize to 0-360
    // float heading   = (atan2(xaccel, yaccel) * 180) / Pi;
    float heading   = (atan2(xmag, ymag) * 180) / Pi;
    heading   = heading + 360 + PIXEL_SHIFT;
    if (heading >= 360) { heading = heading - ((int) (heading / 360)) * 360; }
    int pixel = ((int) (heading / 30)) % 12;

    strip.clear();

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

    strip.show();
    delay(300);
}
