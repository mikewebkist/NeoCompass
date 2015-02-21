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
//Adafruit_LSM303_Accel_Unified sensor = Adafruit_LSM303_Accel_Unified(12345);
Adafruit_LSM303_Mag_Unified sensor = Adafruit_LSM303_Mag_Unified(12345);

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  strip.begin();
  strip.clear();
  strip.show();  

  topColor = strip.Color(1 << 3, 0, 0);
  bottomColor = strip.Color(0, 0, 1 << 3);

  Serial.begin(9600);

  if(!sensor.begin())
  {
    /* There was a problem detecting the compass ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }
sensor.enableAutoRange(true);

}

int zval = 0;
sensors_event_t event; 
float Pi = 3.14159;
float heading;
float headingz;
float xyval;
int pixel;
float x, y, z, gmag, zpercent, topLEDs;

// the loop routine runs over and over again forever:
void loop() {
  sensor.getEvent(&event);
  x = event.acceleration.x;
  y = event.acceleration.y;
  z = event.acceleration.z;
  //x = event.magnetic.x;
  //y = event.magnetic.y;
  //z = event.magnetic.z;
  gmag = sqrt(sq(x) + sq(y) + sq(z));
  zpercent = (z / gmag + 1.0) / 2.0;
  
  xyval = sqrt(sq(z) + sq(y));
  heading   = (atan2(x, y) * 180) / Pi;
  headingz = (atan2(z, xyval) * 180) / Pi;
  
  Serial.print("x: "); Serial.print(x); 
  Serial.print(" y: "); Serial.print(y); 
  Serial.print(" z: "); Serial.print(z); 
  Serial.print(" z%: "); Serial.println(zpercent); 

  // Normalize to 0-360
  heading   = heading + 360 + PIXEL_SHIFT;
  if(heading >= 360.0) { heading = heading - 360.0; }
  pixel = ((int) (heading / 30)) % 12;

  strip.clear();

  // The north- & south-pointing pixels.
  strip.setPixelColor(pixel, topColor);
  // strip.setPixelColor((pixel + 6) % 12, bottomColor);
         if(zpercent < 0.1) {
    topLEDs = 0;
  } else if(zpercent < 0.2) {
    topLEDs = 1;
  } else if(zpercent < 0.3) {
    topLEDs = 2;
  } else if(zpercent < 0.4) {
    topLEDs = 3;
  } else if(zpercent < 0.5) {
    topLEDs = 3;
  } else if(zpercent < 0.6) {
    topLEDs = 3;
  } else if(zpercent < 0.7) {
    topLEDs = 4;
  } else if(zpercent < 0.8) {
    topLEDs = 5;
  } else if(zpercent < 0.9) {
    topLEDs = 6;
  } else {
    topLEDs = 7;
  }
    
  // topLEDs = (int) (zpercent * 6);
  for(int i=0; i<topLEDs; i++) {
    // A bit to the west...
    strip.setPixelColor((pixel + i) % 12, topColor);
    // strip.setPixelColor((pixel + 6 + i) % 12, 0, 0, bottomColor);
    // A bit to the east...
    strip.setPixelColor((pixel + 12 - i) % 12, topColor);
    // strip.setPixelColor((pixel + 6 - i) % 12, bottomColor);
  }
  for(int i=topLEDs; i<7; i++) {
    // A bit to the west...
    strip.setPixelColor((pixel + i) % 12, bottomColor);
    // A bit to the east...
    strip.setPixelColor((pixel + 12 - i) % 12, bottomColor);
  }
  
  strip.show();
  delay(100);
}

