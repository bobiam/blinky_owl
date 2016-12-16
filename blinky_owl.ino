#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 4

//Where's the button - momentary tactile switch for pattern advancement
const byte interruptPin = 2;

//for catching the serial in - used for remote control
byte inByte = 0;

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  
#define DATA_PIN 3

//this is the pin that takes the battery down
#define BATTERY_DISABLE_PIN 9

void flashlight();
void flashlight100();
void flashlight75();
void flashlight50();
void flashlight25();
void flashlight10();
void breath();
void randy();
void red_rand();
void green_rand();
void blue_rand();
void earth_rand();
void water_rand();
void fire_rand();
void air_rand();
void bw_rand();
void off();
void rainbow_cylon();
void palette_fader();
void haha();
void FillLEDsFromPaletteColors( uint8_t colorIndex);
void ChangePalettePeriodically();

//initialize palettes
CRGBPalette16 currentPalette( CRGB::Black);
CRGBPalette16 targetPalette( OceanColors_p );

//super helpful FastLED array size function.  WHY IS THIS NOT VANILLA ARDUINO?!
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Define the array of leds
CRGB leds[NUM_LEDS];

//our list of patterns
typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {  breath, off, flashlight100, off, flashlight50, off, flashlight10, off, rainbow_cylon, off, red_rand, off, palette_fader, off, green_rand, off, blue_rand, off, earth_rand, off, air_rand, off, fire_rand, off, water_rand, off, randy, off, haha };
volatile uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current

void setup() { 
  //set up our initial pins and serial comm
  Serial.begin(9600);
	  
  //define and configure button/interrupt for pattern advancement
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), nextPattern, CHANGE);
  
	LEDS.addLeds<NEOPIXEL,DATA_PIN>(leds,NUM_LEDS);
	LEDS.setBrightness(64);

  //how big's your battery?  Limit power.
  FastLED.setMaxPowerInVoltsAndMilliamps(5,400); 
 
  leds[0] = CRGB::Red;
  delay(2000);
  FastLED.show();
  delay(200);
  LEDS.setBrightness(255);
  Serial.println("Welcome to The Blinky Owl.  Send an 'n' to trigger the next pattern");
}

void loop() { 
  if (Serial.available() > 0) {
    inByte = Serial.read();
    Serial.println(inByte);
    if(inByte == 110)
      nextPattern();
  }  
  gPatterns[gCurrentPatternNumber]();

  if(random(0,50000) == 42){
    Serial.println("The owl has found the answer.  Laugh with me");
    haha();
  }
}


void nextPattern()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) 
  {
    // add one to the current pattern number, and wrap around at the end
    gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns); 
  }
  last_interrupt_time = interrupt_time;  
}

int hue = 0;
int divisor = 30;
void breath()
{
  Serial.println("Starting breath");
  float breath = (exp(sin(millis()/5000.0*PI)) - 0.36787944)*108.0;
  breath = map(breath, 0, 255, 20, 255);
  FastLED.setBrightness(breath);
  fill_rainbow(leds, NUM_LEDS, (hue++/divisor));
  Serial.println(hue);
  if(hue == (255 * divisor)) {
    hue = 0;
  }
  FastLED.show();  
}

void setall(CRGB c)
{
  for(int i=0;i<NUM_LEDS;i++){
    leds[i] = c;
  }
  FastLED.show();
}

void haha()
{
  Serial.println("Starting haha");
  CRGB c,b;

    b.setRGB(0,0,0);
    c.setRGB(random(0,256),random(0,256),random(0,256));
    for(int i=0;i<4;i++){
      setall(c);
      delay(100);
      setall(b);
      delay(100);
    }
    delay(100);
    setall(c);
    delay(100);
    setall(b);
    delay(100);
    setall(c);
    delay(300);
    setall(b);
    delay(700);
}
void randy()
{
  Serial.println("Starting randy");
  CRGB r;
  leds[random(0,NUM_LEDS)] = r.setRGB(random(0,256),random(0,256),random(0,256));
  FastLED.show();
  FastLED.delay(random(0,255));
}


void flashlight100(){
  FastLED.setBrightness(255);
  flashlight();
}

void flashlight75(){
  FastLED.setBrightness(190);
  flashlight();
}

void flashlight50(){
  FastLED.setBrightness(128);
  flashlight();
}

void flashlight25(){
  FastLED.setBrightness(64);
  flashlight();
}

void flashlight10(){
  FastLED.setBrightness(25);
  flashlight();
}

void flashlight()
{
  Serial.println("Starting flashlight");
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::White;
    // Show the leds
    FastLED.show(); 
  }
}

void off()
{
  Serial.println("Turning Off");
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
    // Show the leds
    FastLED.show(); 
  }
}


void green_rand()
{
  CRGB colors[24];
  for(int i=0;i<23;i++)
  {
    colors[i].setRGB(0,i*8,0); 
  }
  palette_rand(colors, 24, 200);
}

void red_rand()
{
  CRGB colors[24];
  for(int i=0;i<23;i++)
  {
    colors[i].setRGB(i*8,0,0); 
  }
  palette_rand(colors, 24, 200);
}

void blue_rand()
{
  CRGB colors[24];
  for(int i=0;i<23;i++)
  {
    colors[i].setRGB(0,0,i*8); 
  }
  palette_rand(colors, 24, 200);
}

void fire_rand()
{
  CRGB colors[] = {CRGB::Tomato, CRGB::Red, CRGB::OrangeRed, CRGB::Brown, CRGB::FireBrick, CRGB::Maroon};
  palette_rand(colors, 6, 200);
}

void water_rand()
{
  CRGB colors[] = {CRGB::Blue, CRGB::DarkBlue, CRGB::DarkTurquoise, CRGB::Aqua, CRGB::SeaGreen, CRGB::Aquamarine};
  palette_rand(colors, 6, 200);
}

void earth_rand()
{
  CRGB colors[] = {CRGB::Chartreuse, CRGB::Green, CRGB::DarkOliveGreen, CRGB::DarkGreen, CRGB::LimeGreen, CRGB::ForestGreen};
  palette_rand(colors, 6, 200);
}

void air_rand()
{
  CRGB colors[] = {CRGB::Azure, CRGB::Cyan, CRGB::DeepSkyBlue, CRGB::LightSkyBlue, CRGB::DodgerBlue, CRGB::Turquoise};
  palette_rand(colors, 6, 200);
}

void bw_rand()
{
  CRGB colors[] = {CRGB::Black, CRGB::White};
  palette_rand(colors, 2, 200);
}

void palette_rand(CRGB colors[], int colorCount,int wait)
{
    leds[random(0,NUM_LEDS)] = colors[random(0,colorCount)];
    FastLED.show();
    fadeall();
    FastLED.delay(wait); 
}


void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void rainbow_cylon() { 
  FastLED.setBrightness(255);
  static uint8_t hue = 0;
  // First slide the led in one direction
  for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show(); 
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }

  // Now go in the other direction.  
  for(int i = (NUM_LEDS)-1; i >= 0; i--) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show();
    // now that we've shown the leds, reset the i'th led to black
    // leds[i] = CRGB::Black;
    fadeall();
    // Wait a little bit before we loop around and do it again
    delay(10);
  }
}

static uint8_t startIndex = 0;
void palette_fader(){
  ChangePalettePeriodically();

  // Crossfade current palette slowly toward the target palette
  //
  // Each time that nblendPaletteTowardPalette is called, small changes
  // are made to currentPalette to bring it closer to matching targetPalette.
  // You can control how many changes are made in each call:
  //   - the default of 24 is a good balance
  //   - meaningful values are 1-48.  1=veeeeeeeery slow, 48=quickest
  //   - "0" means do not change the currentPalette at all; freeze
  
  uint8_t maxChanges = 1; 
  nblendPaletteTowardPalette( currentPalette, targetPalette, maxChanges);


  
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);
  FastLED.show();
}


void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex + sin8(i*16), brightness);
    colorIndex += 3;
  }
}


void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    /*
    CRGB p = CHSV( HUE_PURPLE, 255, 255);
    CRGB g = CHSV( HUE_GREEN, 255, 255);
    CRGB b = CRGB::Black;
    CRGB w = CRGB::White;
    */
    if( secondHand == 0)   { targetPalette = OceanColors_p; }
    if( secondHand == 10)  { targetPalette = ForestColors_p; }
    if( secondHand == 20)  { targetPalette = OceanColors_p; }
    if( secondHand == 30)  { targetPalette = ForestColors_p; }    
    if( secondHand == 40)  { targetPalette = OceanColors_p; }
    if( secondHand == 50)  { targetPalette = ForestColors_p; }
    /*
    if( secondHand == 50)  { targetPalette = PartyColors_p; }
    if( secondHand == 10)  { targetPalette = CRGBPalette16( g,g,b,b, p,p,b,b, g,g,b,b, p,p,b,b); }
    if( secondHand == 20)  { targetPalette = CRGBPalette16( b,b,b,w, b,b,b,w, b,b,b,w, b,b,b,w); }
    if( secondHand == 30)  { targetPalette = LavaColors_p; }
    */

  }
}
