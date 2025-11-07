#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define LED_PIN 1

#define LED_COUNT 16

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);

int speed = 100;
int shortSpeed = 50;
int pattern = 0;
int wipeSpeed = 100;
int wipeCounter = 0;
int directionHolder = 0;
int powerSaveSwitch0 = 0;
const int left = 1;
const int right = 2;
const int scrollFast = 200;
const int scrollSlow = 300;
uint32_t red = strip.Color(0,255,0);
uint32_t green = strip.Color(255,0,0);
uint32_t blue = strip.Color(0,0,200);
uint32_t white = strip.Color(0,0,0,255);
uint32_t amber = strip.Color(60,255,0);

//SELECT COLORS HERE
uint32_t colorA = blue;
uint32_t colorB = blue;

void setup()
{
  strip.begin();
  strip.show();
  strip.setBrightness(50);
  Serial.begin(9600);
}

void loop()
{
  if(directionHolder == 0)
  {
    emergency();
  }
  else if(directionHolder == 1)
  {
    leftScroll();
  }
  else if(directionHolder == 2)
  {
    rightScroll();
  }
  else if(directionHolder == 4)
  {
    patrol();
  }
}

void emergency()
{
  interRed();
  interBlue();
  interRed();
  interBlue();
  interRed();
  interBlue();
  interRed();
  interBlue();
  sideRed();
  sideBlue();
  sideRed();
  sideBlue();
  sideRed();
  sideBlue();
  sideRed();
  sideBlue();
}
void leftScroll()
{
  for(int i=0; i<8; i++)
  {
    strip.setPixelColor(i, amber);
    strip.show();
    delay(scrollFast);
  }
  strip.clear();
  strip.show();
  delay(scrollSlow);
  for(int i=0; i<8; i++)
  {
    strip.setPixelColor(i, amber);
  }
  strip.show();
  delay(scrollSlow);
  strip.clear();
  strip.show();
  delay(scrollSlow);
}

void rightScroll()
{
  for(int i=7; i>=0; i--)
  {
    strip.setPixelColor(i, amber);
    strip.show();
    delay(scrollFast);
  }
  strip.clear();
  strip.show();
  delay(scrollSlow);
  for(int i=0; i<8; i++)
  {
    strip.setPixelColor(i, amber);
  }
  strip.show();
  delay(scrollSlow);
  strip.clear();
  strip.show();
  delay(scrollSlow);
}

void interRed()
{
  for(int i=0; i<3; i++)
  {
    strip.setPixelColor(0, colorA);
    strip.setPixelColor(2, colorA);
    strip.setPixelColor(4, colorA);
    strip.setPixelColor(6, colorA);
    strip.setPixelColor(9, colorA);
    strip.setPixelColor(11, colorA);
    strip.setPixelColor(13, colorA);
    strip.setPixelColor(15, colorA);
    strip.show();
    delay(speed);
    strip.clear();
    strip.show();
    delay(shortSpeed);
  }
}

void interBlue()
{
  for(int i=0; i<3; i++)
  {
    strip.setPixelColor(1, colorB);
    strip.setPixelColor(3, colorB);
    strip.setPixelColor(5, colorB);
    strip.setPixelColor(7, colorB);
    strip.setPixelColor(8, colorB);
    strip.setPixelColor(10, colorB);
    strip.setPixelColor(12, colorB);
    strip.setPixelColor(14, colorB);
    strip.show();
    delay(speed);
    strip.clear();
    strip.show();
    delay(shortSpeed);
  }
}

void sideRed()
{
  for(int i=0; i<3; i++)
  {
    strip.setPixelColor(0, colorA);
    strip.setPixelColor(1, colorA);
    strip.setPixelColor(2, colorA);
    strip.setPixelColor(3, colorA);
    strip.setPixelColor(12, colorA);
    strip.setPixelColor(13, colorA);
    strip.setPixelColor(14, colorA);
    strip.setPixelColor(15, colorA);
    strip.show();
    delay(speed);
    strip.clear();
    strip.show();
    delay(shortSpeed);
  }
}

void sideBlue()
{
  for(int i = 0; i < 3; i++)
  {
    strip.setPixelColor(4, colorB);
    strip.setPixelColor(5, colorB);
    strip.setPixelColor(6, colorB);
    strip.setPixelColor(7, colorB);
    strip.setPixelColor(8, colorB);
    strip.setPixelColor(9, colorB);
    strip.setPixelColor(10, colorB);
    strip.setPixelColor(11, colorB);
    strip.show();
    delay(speed);
    strip.clear();
    strip.show();
    delay(shortSpeed);
  }
}

void patrol()
{
  if(powerSaveSwitch0 == 0)
  {
    strip.setPixelColor(0, colorA);
    strip.setPixelColor(1, colorA);
    strip.setPixelColor(15, colorB);
    strip.setPixelColor(14, colorB);
    strip.show();
    powerSaveSwitch0 = 1;
  }
  else
  {

  }
}