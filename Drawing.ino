#define ACTUAL_WIDTH WIDTH
#define ACTUAL_X (WIDTH - x - 1)
#define ACTUAL_Y (HEIGHT - y - 1)

void setPixel(int8_t x, int8_t y, CRGB color) {
  if (x < 0 || x > WIDTH - 1 || y < 0 || y > HEIGHT - 1) return;
  int pixel = getIndex(x, y);
  leds[pixel] = color;
  FastLED.show();
}

void unsetPixel(int8_t x, int8_t y) {
  if (x < 0 || x > WIDTH - 1 || y < 0 || y > HEIGHT - 1) return;
  int index = getIndex(x, y);
  leds[index] = CRGB::Black;
  FastLED.show();
}

void setSinglePixel(int8_t x, int8_t y, CRGB color) {
  clearDisplay();
  setPixel(x, y, color);
}

int getIndex(uint8_t x, uint8_t y) {
  if (ACTUAL_Y % 2 == 0) { 
    return (ACTUAL_Y * ACTUAL_WIDTH + ACTUAL_X);
  } else {
    return (ACTUAL_Y * ACTUAL_WIDTH + ACTUAL_WIDTH - ACTUAL_X - 1);
  }
}

void clearDisplay() {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
  }

  FastLED.show();
}
