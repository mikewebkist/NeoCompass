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
float Pi = 3.14159;
float heading;
float accelMagnitude, magMagnitude;
float xyval;
int pixel;
float xaccel, yaccel, zaccel, zmag, ymag, xmag, zpercent, topLEDs;

// the loop routine runs over and over again forever:
void loop() {
    accel.getEvent(&event);

    xaccel = event.acceleration.x;
    yaccel = event.acceleration.y;
    zaccel = event.acceleration.z;

    accelMagnitude = sqrt(sq(xaccel) + sq(yaccel) + sq(zaccel));

    mag.getEvent(&event);
    xmag = event.magnetic.x;
    ymag = event.magnetic.y;
    zmag = event.magnetic.z;

    magMagnitude = sqrt(sq(xmag) + sq(ymag) + sq(zmag));

    zpercent = (zaccel / accelMagnitude + 1.0) / 2.0;
    // zpercent = (zmag / magMagnitude + 1.0) / 2.0;

    xyval = sqrt(sq(zaccel) + sq(yaccel));
    heading   = (atan2(xaccel, yaccel) * 180) / Pi;
    Serial.print("accel: ");
    Serial.print(xaccel); Serial.print(", ");
    Serial.print(yaccel); Serial.print(", ");
    Serial.print(zaccel); Serial.print(") %: ");
    Serial.println(accelMagnitude); 

    // Value when pointing north: 15.28285714, -25.27, -70.72857143
    Serial.print("mag: ");
    Serial.print(xmag); Serial.print(", ");
    Serial.print(ymag); Serial.print(", ");
    Serial.print(zmag); Serial.print(") %: ");
    Serial.println(magMagnitude); 

    // Normalize to 0-360
    heading   = heading + 360 + PIXEL_SHIFT;
    if (heading >= 360) { heading = heading - ((int) (heading / 360)) * 360; }
    pixel = ((int) (heading / 30)) % 12;

    strip.clear();

    int zperint = (int) (zpercent * 70);
    for(int i=0; i<(zperint / 10); i++) {
	strip.setPixelColor((pixel + i) % 12, doGamma(64), 0, 0);
	strip.setPixelColor((pixel - i) % 12, doGamma(64), 0, 0);
    }

    if(zperint % 10) {
	float fraction = (zperint % 10) / 10.0 * 64.0;
	strip.setPixelColor((pixel + zperint / 10) % 12, doGamma((int) fraction), 0, 0);
	strip.setPixelColor((pixel - zperint / 10) % 12, doGamma((int) fraction), 0, 0);
    }

    strip.show();
    delay(100);
}

