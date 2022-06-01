#include <Adafruit_NeoPixel.h>

// Digital pins for LED strips
#define PIN0 53
#define PIN1 49

Adafruit_NeoPixel strip0 = Adafruit_NeoPixel(100, PIN0, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(100, PIN1, NEO_GRB + NEO_KHZ800);

void setup() {
  strip0.begin();
  strip1.begin();
  strip0.show();
  strip1.show();
  strip0.setBrightness(25);
  strip1.setBrightness(25);
}

void loop() {
  theaterChaseRainbow(100);
}

// https://github.com/adafruit/Adafruit_NeoPixel/blob/0f6c881b919f0e1fba387fbb22100b8d70f5751b/examples/strandtest/strandtest.ino#L123
// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times... // Wouldnt this be 29 times?
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip0.clear();         //   Set all pixels in RAM to 0 (off)
      strip1.clear();
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip0.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / strip0.numPixels();
        uint32_t color = strip0.gamma32(strip0.ColorHSV(hue)); // hue -> RGB
        strip0.setPixelColor(c, color); // Set pixel 'c' to value 'color'
        strip1.setPixelColor(c, color);
      }
      strip0.show();                // Update strip with new contents
      strip1.show();
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
