#include <Adafruit_NeoPixel.h>
#include <Adafruit_SPIFlash.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif
#include <AntiDelay.h>

AntiDelay speedDelay(100);
AntiDelay shortDelay(50);
AntiDelay wipeSpeedDelay(15);
AntiDelay scrollFastDelay(200);
AntiDelay scrollSlowDelay(300);

#define LED_PIN 1

#define LED_COUNT 16

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);

//buttons
const int elBtn = 8;
const int elModBtn = 7;
const int leftBtn = 6;
const int hlBtn = 0;
const int hlModBtn = 0;
const int rightBtn = 5;

//states
int elBtnState = 0;
bool elState = false;

//timers
const int speed = 100;
const int shortSpeed = 50;
const int wipeSpeed = 15;
const int scrollFast = 200;
const int scrollSlow = 300;

//counters
int wipeCounter = 0;

int pattern = 0;
int directionHolder = 9;
int powerSaveSwitch0 = 0;
int breatheHolder = 0;
int breatheDirection = 0;
int brightness = 50;
bool gotInterrupt = false;

//colors
uint32_t red = strip.Color(0,255,0);
uint32_t halfRed = strip.Color(0,125,0);
uint32_t green = strip.Color(255,0,0);
uint32_t blue = strip.Color(0,0,255);
uint32_t white = strip.Color(0,0,0,255);
uint32_t amber = strip.Color(70,255,0);
uint32_t orange = strip.Color(25,255,0);
uint32_t purple = strip.Color(0,255,255);
uint32_t off = strip.Color(0,0,0);

//SELECT COLORS HERE
uint32_t colorA = red;
uint32_t colorB = blue;

//LIGHT GROUPS
//dev
int groupL[] = {8, 9, 10, 11, 12, 13, 14, 15, -1};
int groupR[] = {0, 1, 2, 3, 4, 5, 6, 7, -1};
//end dev

void setup()
{
  strip.begin();
  strip.show();
  strip.setBrightness(brightness);
  Serial.begin(9600);
  pinMode(elBtn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(elBtn), elBtnISR, FALLING);
  pinMode(elModBtn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(elModBtn), elModBtnISR, FALLING);
  pinMode(leftBtn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(leftBtn), leftBtnISR, FALLING);
  pinMode(rightBtn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(leftBtn), rightBtnISR, FALLING);

}

void loop()
{
  gotInterrupt = false;
    if(elState)
    {
      if(elBtnState == 0)
      {
        strip.clear();
        strip.show();
        elStandard();
      }
      else if(elBtnState == 1)
      {
        strip.clear();
        strip.show();
        full(shortSpeed, 255);
      }
    }
    else
    {
      strip.clear();
      strip.show();
    }
}

void elStandard()
{
  strip.setBrightness(brightness);
  interA();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  interB();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  interA();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  interB();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  interA();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  interB();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  interA();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  interB();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  wipeWhite();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  sideA();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  sideB();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  sideA();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  sideB();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  sideA();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  sideB();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  sideA();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  sideB();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  wipeWhite();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  full(speed, brightness);
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  full(speed, brightness);
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  full(speed, brightness);
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  full(speed, brightness);
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
  wipeWhite();
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

void interA()
{
  for(int i=0; i<3; i++)
  {
    strip.setPixelColor(0, colorA);
    strip.setPixelColor(2, colorA);
    strip.setPixelColor(4, colorA);
    strip.setPixelColor(6, colorA);
    strip.setPixelColor(9, colorB);
    strip.setPixelColor(11, colorB);
    strip.setPixelColor(13, colorB);
    strip.setPixelColor(15, colorB);
    strip.show();
    if(gotInterrupt)
    {
      
      strip.clear();
      strip.show();
      break;
    }
    delay(speed);
    strip.clear();
    strip.show();
    if(gotInterrupt)
    {
      
      strip.clear();
      strip.show();
      break;
    }
    delay(shortSpeed);
  }
}

void interB()
{
  for(int i=0; i<3; i++)
  {
    strip.setPixelColor(1, colorB);
    strip.setPixelColor(3, colorB);
    strip.setPixelColor(5, colorB);
    strip.setPixelColor(7, colorB);
    strip.setPixelColor(8, colorA);
    strip.setPixelColor(10, colorA);
    strip.setPixelColor(12, colorA);
    strip.setPixelColor(14, colorA);
    strip.show();
    if(gotInterrupt)
    {
      
      strip.clear();
      strip.show();
      break;
    }
    delay(speed);
    strip.clear();
    strip.show();
    if(gotInterrupt)
    {
      
      strip.clear();
      strip.show();
      break;
    }
    delay(shortSpeed);
  }
}

void sideA()
{
  for(int i=0; i<3; i++)
  {
    strip.setPixelColor(0, colorA);
    strip.setPixelColor(1, colorA);
    strip.setPixelColor(2, colorA);
    strip.setPixelColor(3, colorA);
    strip.setPixelColor(12, colorB);
    strip.setPixelColor(13, colorB);
    strip.setPixelColor(14, colorB);
    strip.setPixelColor(15, colorB);
    strip.show();
    if(gotInterrupt)
    {
      
      strip.clear();
      strip.show();
      break;
    }
    delay(speed);
    strip.clear();
    strip.show();
    if(gotInterrupt)
    {
      
      strip.clear();
      strip.show();
      break;
    }
    delay(shortSpeed);
  }
}

void sideB()
{
  for(int i = 0; i < 3; i++)
  {
    strip.setPixelColor(4, colorB);
    strip.setPixelColor(5, colorB);
    strip.setPixelColor(6, colorB);
    strip.setPixelColor(7, colorB);
    strip.setPixelColor(8, colorA);
    strip.setPixelColor(9, colorA);
    strip.setPixelColor(10, colorA);
    strip.setPixelColor(11, colorA);
    strip.show();
    if(gotInterrupt)
    {
      
      strip.clear();
      strip.show();
      break;
    }
    delay(speed);
    strip.clear();
    strip.show();
    if(gotInterrupt)
    {
      
      strip.clear();
      strip.show();
      break;
    }
    delay(shortSpeed);
  }
}

void full(int fullSpeed, int bright)
{
  strip.setBrightness(bright);
  for(int i=0; i<3; i++)
  {
    for(int a=0; a<8; a++)
    {
      strip.setPixelColor(a, colorA);
    }
    for(int b=8; b<16; b++)
    {
      strip.setPixelColor(b, colorB);
    }
    strip.show();
    if(gotInterrupt)
    {
      
      strip.clear();
      strip.show();
      break;
    }
    delay(fullSpeed);
    strip.clear();
    strip.show();
    if(gotInterrupt)
    {
      
      strip.clear();
      strip.show();
      break;
    }
    delay(shortSpeed);
  }
  for(int i=0; i<3; i++)
  {
    for(int a=0; a<8; a++)
    {
      strip.setPixelColor(a, colorB);
    }
    for(int b=8; b<16; b++)
    {
      strip.setPixelColor(b, colorA);
    }
    strip.show();
    if(gotInterrupt)
    {
      
      strip.clear();
      strip.show();
      break;
    }
    delay(fullSpeed);
    strip.clear();
    strip.show();
    if(gotInterrupt)
    {
      
      strip.clear();
      strip.show();
      break;
    }
    delay(shortSpeed);
  }
}

void wipe()
{
  for(; wipeCounter<8; wipeCounter++)
  {
    if(wipeCounter % 2 == 0)
    {
      strip.setPixelColor(wipeCounter, red);
      strip.setPixelColor((wipeCounter+8), red);
    }
    else
    {
      strip.setPixelColor(wipeCounter, blue);
      strip.setPixelColor((wipeCounter+8), blue);
    }
    strip.show();
    delay(wipeSpeed);
    strip.clear();
  }
  wipeCounter = 7;
  for(; wipeCounter>=0; wipeCounter--)
  {
    if(wipeCounter % 2 == 0)
    {
      strip.setPixelColor(wipeCounter, red);
      strip.setPixelColor((wipeCounter+8), red);
    }
    else
    {
      strip.setPixelColor(wipeCounter, blue);
      strip.setPixelColor((wipeCounter+8), blue);
    }
    strip.show();
    delay(wipeSpeed);
    strip.clear();
  }
  wipeCounter = 0;
}

void wipeWhite()
{
    for(; wipeCounter<8; wipeCounter++)
  {
    if(wipeCounter % 2 == 0)
    {
      strip.setPixelColor(wipeCounter, white);
      strip.setPixelColor((wipeCounter+8), white);
    }
    else
    {
      strip.setPixelColor(wipeCounter, white);
      strip.setPixelColor((wipeCounter+8), white);
    }
    strip.show();
    if(gotInterrupt)
    {
      
      strip.clear();
      strip.show();
      break;
    }
    delay(wipeSpeed);
    strip.clear();
  }
  wipeCounter = 7;
  for(; wipeCounter>=0; wipeCounter--)
  {
    if(wipeCounter % 2 == 0)
    {
      strip.setPixelColor(wipeCounter, white);
      strip.setPixelColor((wipeCounter+8), white);
    }
    else
    {
      strip.setPixelColor(wipeCounter, white);
      strip.setPixelColor((wipeCounter+8), white);
    }
    strip.show();
    if(gotInterrupt)
    {
      
      strip.clear();
      strip.show();
      break;
    }
    delay(wipeSpeed);
    strip.clear();
  }
  wipeCounter = 0;
  strip.show();
}

void patrol()
{
  if(powerSaveSwitch0 == 0)
  {
    strip.setBrightness(10);
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

void headlight()
{
  for(int i=0; i<16; i++)
  {
    strip.setPixelColor(i, white);
  }
}

void headlightPatrol()
{
  if(powerSaveSwitch0 == 0)
  {
    strip.setPixelColor(0, colorA);
    strip.setPixelColor(1, colorA);
    strip.setPixelColor(2, white);
    strip.setPixelColor(3, white);
    strip.setPixelColor(4, white);
    strip.setPixelColor(5, white);
    strip.setPixelColor(6, white);
    strip.setPixelColor(7, white);
    strip.setPixelColor(8, white);
    strip.setPixelColor(9, white);
    strip.setPixelColor(10, white);
    strip.setPixelColor(11, white);
    strip.setPixelColor(12, white);
    strip.setPixelColor(13, white);
    strip.setPixelColor(14, colorB);
    strip.setPixelColor(15, colorB);
    strip.show();
    powerSaveSwitch0 = 1;
  }
  else
  {

  }
}

void blinkerRearLeft()
{
  strip.clear();
  strip.setPixelColor(0, halfRed);
  strip.setPixelColor(1, halfRed);
  strip.setPixelColor(2, halfRed);
  strip.setPixelColor(3, halfRed);
  strip.setPixelColor(4, amber);
  strip.setPixelColor(5, amber);
  strip.setPixelColor(6, amber);
  strip.setPixelColor(7, amber);
  strip.show();
  delay(200);
  strip.setPixelColor(4, off);
  strip.setPixelColor(5, off);
  strip.setPixelColor(6, off);
  strip.setPixelColor(7, off);
  strip.show();
  delay(200);
}

void blinkerRearRight()
{
  strip.clear();
  strip.setPixelColor(4, halfRed);
  strip.setPixelColor(5, halfRed);
  strip.setPixelColor(6, halfRed);
  strip.setPixelColor(7, halfRed);
  strip.setPixelColor(0, amber);
  strip.setPixelColor(1, amber);
  strip.setPixelColor(2, amber);
  strip.setPixelColor(3, amber);
  strip.show();
  delay(200);
  strip.setPixelColor(0, off);
  strip.setPixelColor(1, off);
  strip.setPixelColor(2, off);
  strip.setPixelColor(3, off);
  strip.show();
  delay(200);
}

void tailLight()
{
  for(int i=0; i<8; i++)
  {
    strip.setPixelColor(i, red);
  }
  strip.show();
}

void breathe()
{
  if(breatheDirection == 0 && breatheHolder < brightness)
  {
    strip.setBrightness(breatheHolder);
    strip.show();
    delay(10);
    breatheHolder += 1;
  }
  else if(breatheDirection == 0 && breatheHolder >= (brightness - 1))
  {
    breatheDirection = 1;
  }
  else if(breatheDirection == 1 && breatheHolder >= 1)
  {
    strip.setBrightness(breatheHolder);
    strip.show();
    delay(10);
    breatheHolder -= 1;
  }
  else
  {
    breatheDirection = 0;
  }
}

void elBtnISR()
{
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if(interruptTime - lastInterruptTime > 200)
  {
    if(elState)
    {
      elState = false;
      gotInterrupt = true;
    }
    else
    {
      elState = true;
      gotInterrupt = true;
    }
  }
lastInterruptTime = interruptTime;
}

void elModBtnISR()
{
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if(interruptTime - lastInterruptTime > 200)
  {
    if(elBtnState == 0)
    {
      elBtnState = 1;
      gotInterrupt = true;
    }
    else
    {
      elBtnState = 0;
      gotInterrupt = true;
    }
  }
  lastInterruptTime = interruptTime;
}

void leftBtnISR()
{

}

void rightBtnISR()
{

}