#define UPDATES_PER_SECOND 100

CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

void helperSetup() {
  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}

void paletteRoutine() {
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
    
  FillLEDsFromPaletteColors( startIndex);
    
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
    uint8_t brightness = 32;
    
    for( int i = 0; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

void runningPixel(CRGB color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = color;
    FastLED.show();
    delay(50);
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(10);
  }
}

void pixelSpectre() {
  for (int r = 0; r < 10; r++) {
    for (int g = 0; g < 10; g++) {
      for (int b = 0; b < 10; b++) {
        leds[0].r = r*25;
        leds[0].g = g*25;
        leds[0].b = b*25;
        FastLED.show();
        delay(15);
      }
    }
  }
}
