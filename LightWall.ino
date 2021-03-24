#include <FastLED.h>


#define LED_PIN     2
#define WIDTH       8
#define HEIGHT      6
#define NUM_LEDS    WIDTH*HEIGHT
#define BRIGHTNESS  32
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
CRGB activeColor = CRGB::Green;

//Declarations
void setPixel(int8_t x, int8_t y, CRGB color);
void unsetPixel(int8_t x, int8_t y);
void setSinglePixel(int8_t x, int8_t y, CRGB color);
int getIndex(uint8_t x, uint8_t y);
void clearDisplay();
//BLE
void BLESetup();
void BLERoutine();
void helperSetup();

void paletteRoutine();


void setup() {
//  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );

  Serial.begin(115200);

  BLESetup();
  clearDisplay();
  helperSetup();
}


void loop()
{
  BLERoutine();
}
