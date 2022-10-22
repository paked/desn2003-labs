// Use if you want to force the software SPI subsystem to be used for some reason (generally, you don't)
// #define FASTLED_FORCE_SOFTWARE_SPI
// Use if you want to force non-accelerated pin access (hint: you really don't, it breaks lots of things)
// #define FASTLED_FORCE_SOFTWARE_SPI
// #define FASTLED_FORCE_SOFTWARE_PINS
#include <FastLED.h>

DEFINE_GRADIENT_PALETTE( colourGradient ) {
  0,     18, 22, 255,   
40,   200, 18, 255, 
50,   255, 0, 0 };


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
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
}
int worldtime = 0;

void loop() {
  CRGBPalette16 myPal = colourGradient;


  for (int led = 0; led < 26; led++) {
    // station
    uint8_t station = ledsToStations[led];

    if (station >= 100) {
      continue;
    }

    uint8_t val = pgm_read_byte(&stationsToData[station][worldtime % 122]);

    Serial.println(val);

    leds[led].r = val;
        leds[led].b = val;
            leds[led].g = val;
  }

  FastLED.show();
  worldtime += 1;

  delay(100);

  Serial.println("done!" + String(worldtime % 122));
}
