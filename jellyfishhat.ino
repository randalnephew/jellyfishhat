#include <FastLED.h>

//#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 19

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN

#define DATA_PIN07 7
#define DATA_PIN08 8
#define DATA_PIN09 9
#define DATA_PIN10 10
#define DATA_PIN11 11
#define DATA_PIN12 12

#define CLOCK_PIN 13

#define NUM_PATTERNS 8
typedef void (* GenericFP)();
GenericFP patterns[NUM_PATTERNS] = {&p_all_color_go_around, 
                                    &dots_chase,
                                    &p_color_temp,
                                    &dots_chase_every_other_inverted, 
                                    &p_one_color_go_around,
                                    &p_strobe_slide,
                                    &p_random_on_off_all_one_color,
                                    &p_random_all_different_color};

#define NUM_LINES 4
// Define the array of leds
int DATA_PINS[NUM_LINES] = {9, 10, 11, 12};

CRGB leds[NUM_LINES+2][NUM_LEDS];

// Defines for p_color_temp
#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky
// How many seconds to show each temperature before switching
#define DISPLAYTIME 20
// How many seconds to show black between switches
#define BLACKTIME   3

void setup() { 
       FastLED.addLeds<WS2812B, DATA_PIN12, GRB>(leds[0], NUM_LEDS);
       FastLED.addLeds<WS2812B, DATA_PIN11, GRB>(leds[1], NUM_LEDS);
       FastLED.addLeds<WS2812B, DATA_PIN10, GRB>(leds[2], NUM_LEDS);
       FastLED.addLeds<WS2812B, DATA_PIN09, GRB>(leds[3], NUM_LEDS);      
       FastLED.addLeds<WS2812B, DATA_PIN08, GRB>(leds[4], NUM_LEDS);
       FastLED.addLeds<WS2812B, DATA_PIN07, GRB>(leds[5], NUM_LEDS); 
}

void loop() {
  six_test();
  six_test_rainbow();
//  patterns[NUM_PATTERNS-1]();
//  for(int x=0; x<NUM_PATTERNS;x++)
//    patterns[x]();
}

void dots_chase_every_other_inverted(){
  int repeat = 10;
  CRGB target;
  target.red=random8();
  target.green=random8();
  target.blue=random8();
  for(int x=0; x<repeat;x++){
    for(int dot=0; dot < NUM_LEDS; dot++) {
      leds[0][dot] = target;
      leds[1][NUM_LEDS - dot - 1] = target;
      leds[2][dot] = target;
      leds[3][NUM_LEDS - dot - 1] = target;
      FastLED.show();
      leds[0][dot] = CRGB::Black;
      leds[1][NUM_LEDS - dot - 1] = CRGB::Black;
      leds[2][dot] = CRGB::Black;
      leds[3][NUM_LEDS - dot - 1] = CRGB::Black;
      delay(30); 
    }
  }
}  


CRGB get_random_color() {
  CRGB target;
  target.red=random8();
  target.blue=random8();
  target.green=random8();
  return target;
}

void dots_chase() {
  int repeat = 10;
  CRGB target = get_random_color();
  for(int x=0; x<repeat;x++){
    for(int dot=0; dot < NUM_LEDS; dot++) {
      leds[0][dot] = target;
      leds[1][dot] = target;
      leds[2][dot] = target;
      leds[3][dot] = target;
      FastLED.show();
      leds[0][dot] = CRGB::Black;
      leds[1][dot] = CRGB::Black;
      leds[2][dot] = CRGB::Black;
      leds[3][dot] = CRGB::Black;
      delay(30); 
    }
  }
}

void _base_go_around(CRGB active, CRGB not_active){
  int repeat = random8(10,20);
  for(int z=0; z<repeat;z++){
    for(int y=0; y < 4; y++) {
      CRGB color_one = not_active;
      CRGB color_two = not_active;
      CRGB color_three = not_active;
      CRGB color_four = not_active;
      if (y % 4 == 0)
        color_one = active;
      else if (y % 4 == 1)
        color_two = active;
      else if (y % 4 == 2)
        color_three = active;
      else if (y % 4 == 3 )
        color_four = active;
     
      for(int x=0; x < NUM_LEDS; x++){
        leds[0][x] = color_one;
        leds[1][x] = color_two;
        leds[2][x] = color_three;
        leds[3][x] = color_four;
      }
      FastLED.show();
      delay(100);
    }
  }
}

void p_one_color_go_around() {
  _base_go_around(get_random_color(), CRGB::Black);  
}

void p_all_color_go_around() {
  _base_go_around(get_random_color(), get_random_color());
}

void p_color_temp() {
  int cycles=1000;
  for (int x=0; x < cycles; x++){
    // draw a generic, no-name rainbow
    static uint8_t starthue = 0;
    for(int z=0;z<NUM_LINES;z++)
      fill_rainbow(leds[z], NUM_LEDS, --starthue, 20);
    // Choose which 'color temperature' profile to enable.
    uint8_t secs = (millis() / 1000) % (DISPLAYTIME * 2);
    if( secs < DISPLAYTIME) {
      FastLED.setTemperature( TEMPERATURE_1 ); // first temperature
      for(int z=0;z<NUM_LINES;z++)
        leds[z][0] = TEMPERATURE_1; // show indicator pixel
    } else {
      FastLED.setTemperature( TEMPERATURE_2 ); // second temperature
      for(int z=0;z<NUM_LINES;z++)
        leds[z][0] = TEMPERATURE_2; // show indicator pixel
    }
    FastLED.show();
    FastLED.delay(8);
  }
  
}

void p_strobe_slide() {
  int cycles = random8(10, 20);
  CRGB target = get_random_color();
  for( int x=0;x<cycles;x++){ 
    for( int y = 0; y < NUM_LEDS; y++){
      for(int z=0;z<NUM_LINES;z++)
        leds[z][y] = target;
      FastLED.show();
      FastLED.delay(30);
      for(int z=0;z<NUM_LINES;z++)
        leds[z][y] = CRGB::Black;  
      FastLED.show();
      FastLED.delay(30);
    }
  }
}

void p_random_on_off_all_one_color() {
  int cycles = random8(10,20);
  CRGB target = get_random_color();
  for (int x=0;x<cycles;x++){
    for (int y=0; y<NUM_LINES; y++){
      if (random8() % 2 == 0){
        for(int z=0;z<NUM_LEDS;z++)
          leds[y][z] = target;
      }
      else{
        for(int z=0;z<NUM_LEDS;z++)
          leds[y][z] = CRGB::Black;
      }
        
    }
    FastLED.show();
    FastLED.delay(100);
  }
}

void p_random_all_different_color() {
  int cycles = random8(10,20);
  for (int x=0;x<cycles;x++){
    for (int y=0; y<NUM_LINES; y++){
      CRGB target = get_random_color();
      for(int z=0;z<NUM_LEDS;z++)
        leds[y][z] = target;
    }
    FastLED.show();
    FastLED.delay(100);
  }
}



void six_test(){
  int cycles = random8(10, 20);
  CRGB target = get_random_color();
  for( int x=0;x<cycles;x++){ 
    for( int y = 0; y < NUM_LEDS; y++){
      for(int z=0;z<NUM_LINES+2;z++)
        leds[z][y] = target;
      FastLED.show();
      FastLED.delay(30);
      for(int z=0;z<NUM_LINES+2;z++)
        leds[z][y] = CRGB::Black;  
      FastLED.show();
      FastLED.delay(30);
    }
  }
}

void six_test_rainbow(){
  int cycles=1000;
  for (int x=0; x < cycles; x++){
    // draw a generic, no-name rainbow
    static uint8_t starthue = 0;
    for(int z=0;z<NUM_LINES+2;z++)
      fill_rainbow(leds[z], NUM_LEDS, --starthue, 20);
    // Choose which 'color temperature' profile to enable.
    uint8_t secs = (millis() / 1000) % (DISPLAYTIME * 2);
    if( secs < DISPLAYTIME) {
      FastLED.setTemperature( TEMPERATURE_1 ); // first temperature
      for(int z=0;z<NUM_LINES+2;z++)
        leds[z][0] = TEMPERATURE_1; // show indicator pixel
    } else {
      FastLED.setTemperature( TEMPERATURE_2 ); // second temperature
      for(int z=0;z<NUM_LINES+2;z++)
        leds[z][0] = TEMPERATURE_2; // show indicator pixel
    }
    FastLED.show();
    FastLED.delay(8);
  }
  
}

