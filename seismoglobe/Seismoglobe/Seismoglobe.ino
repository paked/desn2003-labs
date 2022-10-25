// Use if you want to force the software SPI subsystem to be used for some reason (generally, you don't)
// #define FASTLED_FORCE_SOFTWARE_SPI
// Use if you want to force non-accelerated pin access (hint: you really don't, it breaks lots of things)
// #define FASTLED_FORCE_SOFTWARE_SPI
// #define FASTLED_FORCE_SOFTWARE_PINS
#include <FastLED.h>

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,     0,  0,  255,   // blue
85,   255,  0,  255,    // purple
170,   255, 140, 0,     // orange
255,   255, 255, 0,     // yellow
}; //full white

CRGBPalette16 myPal = heatmap_gp;

extern const uint8_t ledsToStationsLength;
extern const uint8_t ledsToStations[];
extern const uint8_t PROGMEM stationsToData[][122];

// How many leds are in the strip?
#define NUM_LEDS 150

#define DATA_PIN 5
#define BRIGHTNESS 128

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

int focus = 99;

// This function sets up the ledsand tells the controller about them
void setup() {
  Serial.begin(9600);
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
 	delay(2000);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );

  Serial.println("focus: " + String(focus));
}

int worldtime = 0;

void loop() {
//  if (Serial.available() > 0) {
//    String str = Serial.readString();
//
//    focus += 1;
//
//    Serial.println("focus: " + String(focus));
//
//    delay(100);
//
//    // we don't care what the string is! mwahahah
//  }
//
//  for (int i = 0; i < NUM_LEDS; i++) {
//    if (i == focus) {
//      leds[i] = CRGB::White;
//    } else {
//      leds[i] = CRGB::Black;
//    }
//  }

//  FastLED.show();
  
//  for (int i = 0; i < ledsToStationsLength; i++) {
//    uint8_t val = ledsToStations[i];
//    if (val == 255) {
//      continue;
//    }
//
//    leds[i] = CRGB::White;
//  }
//
//  FastLED.show();

//  delay(1000);

//  return;
  
  for (int led = 0; led < ledsToStationsLength; led++) {
    // station
    uint8_t station = ledsToStations[led];

    if (station >= 100) {
      continue;
    }

    uint8_t val = pgm_read_byte(&stationsToData[station][worldtime % 120]);

//    Serial.println("s: " + String((int) station) + " v: " + String((int) val));

    leds[led] = ColorFromPalette( myPal, val, 64, LINEARBLEND);
  }

  FastLED.show();
//  worldtime += 1;

  int potVal = analogRead(A0);

  worldtime = map(potVal, 0, 1024, 0, 120);

  Serial.println("d" + String(potVal) + " " + String(worldtime));

//  delay(100);k
//
//  Serial.println("done!" + String(worldtime % 120));
}
