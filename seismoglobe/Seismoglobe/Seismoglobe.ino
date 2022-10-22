// Use if you want to force the software SPI subsystem to be used for some reason (generally, you don't)
// #define FASTLED_FORCE_SOFTWARE_SPI
// Use if you want to force non-accelerated pin access (hint: you really don't, it breaks lots of things)
// #define FASTLED_FORCE_SOFTWARE_SPI
// #define FASTLED_FORCE_SOFTWARE_PINS
#include <FastLED.h>

DEFINE_GRADIENT_PALETTE( heatmap_gp ) {
  0,     0,  0,  255,   //blue
64,   255,  0,  255,   //purple
128,   255, 140, 0, // orange
192,   255, 255, 0, // yellow
255,   255,0,  0,   //red
}; //full white

CRGBPalette16 myPal = heatmap_gp;

extern const uint8_t ledsToStations[];
extern const uint8_t PROGMEM stationsToData[][122];

// How many leds are in the strip?
#define NUM_LEDS 100

#define DATA_PIN 3

// This is an array of leds.  One item for each led in your strip.
CRGB leds[NUM_LEDS];

// This function sets up the ledsand tells the controller about them
void setup() {
  Serial.begin(9600);
	// sanity check delay - allows reprogramming if accidently blowing power w/leds
 	delay(2000);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}
int worldtime = 0;

void loop() {
  for (int led = 0; led < 26; led++) {
    // station
    uint8_t station = ledsToStations[led];

    if (station >= 100) {
      continue;
    }

    uint8_t val = pgm_read_byte(&stationsToData[station][worldtime % 120]);

    leds[led] = ColorFromPalette( myPal, val, 64, LINEARBLEND);
  }

  FastLED.show();
  worldtime += 1;

  delay(100);

  Serial.println("done!" + String(worldtime % 120));
}
