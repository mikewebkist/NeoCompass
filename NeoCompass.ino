#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

// NeoPixel 12-LED ring on PIN 12
#define LEDS 12
#define PIN 12
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LEDS, PIN, NEO_GRB + NEO_KHZ800);

// Compass+Accellerometer
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);


// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  strip.begin();
  strip.clear();
  strip.show();  
  Serial.begin(9600);

  if(!mag.begin())
  {
    /* There was a problem detecting the compass ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

}

int red = 0;
int green = 0;
int blue = 0;
int delayval = 0;
int zval = 0;
sensors_event_t event; 
float Pi = 3.14159;
float heading;
int pixel;

// the loop routine runs over and over again forever:
void loop() {
  green = random(0, 16) * 4;
  blue = random(0, 16) * 4;
  red = random(0, 16) * 4;
  delayval = random(1, 100) * 5;
  mag.getEvent(&event);

  heading = (atan2(event.magnetic.y,event.magnetic.x) * 180) / Pi;

  // Normalize to 0-360
  heading = heading + (heading < 0 ? 360 : 0);

  pixel = ((int) ((heading / 30) + 8)) % 12;
  Serial.print("heading: "); 
  Serial.print((int) heading); 
  Serial.print(" [");
  Serial.print(pixel); 
  Serial.println("]");
  delay(100);


  strip.clear();

  strip.setPixelColor(pixel, 1 << 7, 0, 0);
  for(int i=0; i<4; i++) {
    strip.setPixelColor((pixel + 8 + i) % 12, 1 << (i + 2), 0, 0);
    strip.setPixelColor((pixel + (4 - i)) % 12, 1 << (i + 2  ), 0, 0);
    // strip.setPixelColor((pixel + 8 + i) % 12, 1 << i, 0, 0);
  }
  // strip.setPixelColor(pixel, red, green, blue);
  strip.show();
  delay(delayval);
}








