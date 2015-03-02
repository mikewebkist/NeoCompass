#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

// NeoPixel 12-LED ring on PIN 12
#define LEDS 12
#define PIN 12
#define PIXEL_SHIFT 40
uint32_t topColor, bottomColor;
const float Pi = 3.14159;

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

sensors_event_t event; 

// the loop routine runs over and over again forever:
void loop() {
    accel.getEvent(&event);

    float xaccel = event.acceleration.x;
    float yaccel = event.acceleration.y;
    float zaccel = event.acceleration.z;

    float accelMagnitude = sqrt(sq(xaccel) + sq(yaccel) + sq(zaccel));

    mag.getEvent(&event);
    float xmag = event.magnetic.x;
    float ymag = event.magnetic.y;
    float zmag = event.magnetic.z;

    float magMagnitude = sqrt(sq(xmag) + sq(ymag) + sq(zmag));

    float zpercent = (zaccel / accelMagnitude + 1.0) / 2.0;

    float xyval = sqrt(sq(zaccel) + sq(yaccel));
    float heading   = (atan2(xaccel, yaccel) * 180) / Pi;

    /* */ 
    Serial.print("xaccel: "); Serial.print(xaccel); 
    Serial.print(" yaccel: "); Serial.print(yaccel); 
    Serial.print(" zaccel: "); Serial.print(zaccel); 
    Serial.print(" zaccel%: "); Serial.println(accelMagnitude); 
    /**/
    Serial.print("xmag  : "); Serial.print(xmag); 
    Serial.print(" ymag  : "); Serial.print(ymag); 
    Serial.print(" zmag  : "); Serial.print(zmag); 
    Serial.print(" zmag%  : "); Serial.println(magMagnitude); 
    // 15.28285714	-25.27	-70.72857143

    // Normalize to 0-360
    heading   = heading + 360 + PIXEL_SHIFT;
    if(heading >= 360.0) { heading = heading - 360.0; }
    int pixel = ((int) (heading / 30)) % 12;

    strip.clear();

    byte values[12];

    for(int j=0; j<LEDS; j++) {
	values[j] = 255;
	for(int k=1; k<LEDS; k++) {
	    float fade = 5 * k;
	    values[j - k] = values[j - k] - fade;
	}
	for(int k=0; k<LEDS; k++) {
	    strip.setPixelColor(k, doGamma(values[k]), 0, 0);
	}
	delay(50);
    }
    strip.show();
}

