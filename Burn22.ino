#include <Adafruit_NeoPixel.h>

#define STRIP_COUNT 2
#define LEDS_PER_STRIP 100
#define COLUMNS_PER_STRIP 2

// The number of pixels in a column
#define HEIGHT 49

// The number of unused pixels between columns
#define DEAD 2

#define STRIP_0_PIN 53
#define STRIP_1_PIN 49

const int pins[STRIP_COUNT] = {STRIP_0_PIN, STRIP_1_PIN};
Adafruit_NeoPixel strips[STRIP_COUNT];

void setup() {
  // For each strip...
  for (int i = 0; i < STRIP_COUNT; i++) {
    // Create a stip instance using the GRB and 800KHZ comm format
    strips[i] = Adafruit_NeoPixel(LEDS_PER_STRIP, pins[i], NEO_GRB + NEO_KHZ800);
    // Configure the pin for output
    strips[i].begin();
    // Transmit data in RAM to the strip
    strips[i].show();
    // Set brightness to 10%ish (out of 255)
    strips[i].setBrightness(25);
  }
}

void loop() {
  // theaterChaseRainbow(200);
  xyDemo(25);
}

uint32_t getColor(uint8_t r, uint8_t g, uint8_t b) {
  return Adafruit_NeoPixel::Color(r, g, b);
}

void setPixel(int x, int y, uint32_t color) {
  // The strips will start at the bottom, go up HEIGHT pixels,
  // cross at the top with DEAD unused pixels, and come back down.
  
  if (x >= COLUMNS_PER_STRIP * STRIP_COUNT || y >= HEIGHT) {
    return;
  }

  // Integer division to get the strip index.
  // i.e. if COLUMNS_PER_STRIP is 2 and x is 3, 3 / 2  is 1.
  const int stripIndex = x / COLUMNS_PER_STRIP;

  // Modulo to determine if this is the strip going up (starting at index 0)
  // or coming down (starting at index HEIGHT + DEAD - 1).
  // TODO: Only works when COLUMNS_PER_STRIP == 2 (ok for this project).
  const bool isUpStrip = x % COLUMNS_PER_STRIP == 0;

  // If we're on the "up" side of the strip, the 0th pixel is in the middle
  // of the strip, specifically, at HEIGHT - 1. Increasing y to move down,
  // we subtract from there. If we're on the "down" side of the strip, the 0th
  // pixel is on the other side of the DEAD space after all of HEIGHT from the 
  // "up" side, at HEIGHT + DEAD.
  const int pixelIndex = 
    isUpStrip 
      ? HEIGHT - 1 - y
      : HEIGHT + DEAD + y;

  strips[stripIndex].setPixelColor(pixelIndex, color);
}

void prepareFrame() {
  for (int i = 0; i < STRIP_COUNT; i++)  {
    strips[i].clear(); // Set all pixels in RAM to 0 (off)
  }
}

void displayFrame() {
  for (int i = 0; i < STRIP_COUNT; i++)  {
    strips[i].show(); // Update strip with new contents
  }
}

void xyDemo(int wait) {
  for (int stepCount = 0; stepCount < HEIGHT; stepCount++) {
    prepareFrame();
    for (int x = 0; x < COLUMNS_PER_STRIP * STRIP_COUNT; x++) {
      for (int y = 0; y < HEIGHT; y++) {
        if (y % HEIGHT == stepCount) {
          setPixel(x, y, getColor(255, 255, 255));
        } else if (y == 0) {
          setPixel(x, y, getColor(0, 255, 0));
        } else if (y == HEIGHT - 1) {
          setPixel(x, y, getColor(255, 255, 0));
        } else {
          setPixel(x, y, getColor(255 - (y * 5), 0, (y * 5)));
        }
      }
    }
    displayFrame();
    delay(wait);
  }
}

// https://github.com/adafruit/Adafruit_NeoPixel/blob/0f6c881b919f0e1fba387fbb22100b8d70f5751b/examples/strandtest/strandtest.ino#L123
// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a = 0; a < 30; a++) {  // Repeat 30 times...
    for(int b = 0; b < 3; b++) { //  'b' counts from 0 to 2...
      prepareFrame();
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int y = b; y < HEIGHT; y += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the HEIGHT
        int      hue   = firstPixelHue + y * 65536L / HEIGHT;
        uint32_t color = Adafruit_NeoPixel::gamma32(Adafruit_NeoPixel::ColorHSV(hue)); // hue -> RGB
        for (int x = 0; x < COLUMNS_PER_STRIP * STRIP_COUNT; x++)  {
          setPixel(x, y, color);
        }
      }
      
      displayFrame();
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}
