//Version 0.1.3
// full working alpha with 4 Buttons.
/* This is seemingly in a fully working state as it sits. A full build
might be neccessary to go further with the code.

I need to try and figure out a way to make the functions more streamlined
and not as chaotic looking. Maybe grouping could do this, but I'm not sure
that wont just add complexity for the sake of simply making the code
easier to look at.
*/

/*
!!!TODO!!!
figure out flash storage and save elBtnState to that.
*/
#include <Adafruit_NeoPixel.h>
#include <Adafruit_SPIFlash.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif

#define LED_PIN 1

#define LED_COUNT 16

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGBW + NEO_KHZ800);

//buttons
const int elBtn = 13;
const int elModBtn = 12;
const int leftBtn = 11;
const int hlBtn = 7;
const int hlModBtn = 9;
const int rightBtn = 10;

//states
int elBtnState = 0;
bool elState = false;
bool rightBtnState = false;
bool leftBtnState = false;
bool sceneLeftState = false;
bool sceneRightState = false;
bool hlBtnState = false;
int hlModBtnState = 0;

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
  attachInterrupt(digitalPinToInterrupt(rightBtn), rightBtnISR, FALLING);
  pinMode(hlBtn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(hlBtn), hlBtnISR, FALLING);
  pinMode(hlModBtn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(hlModBtn), hlModBtnISR, FALLING);

}

void loop()
{
  gotInterrupt = false;
  if(hlBtnState)
  {
    if(elState)
    {
      if(leftBtnState)
      {
        elStandardSceneLeft();
      }
      else if(rightBtnState)
      {
        elStandardSceneRight();
      }
      else
      {
        if(elBtnState == 0)
        {
          headLightPatrol();
        }
        else if(elBtnState == 1)
        {
          elHeadLight();
        }
        else
        {
          full(shortSpeed, 255);
        }
      }
    }
    else
    {
      if(hlModBtnState == 0)
      {
        headLight();
      }
      else
      {
        headLight();
        breathe();
      }
    }
  }
  else
  {
    if(elState)
    {
      if(leftBtnState)
      {
        elStandardSceneLeft();
      }
      else if (rightBtnState)
      {
        elStandardSceneRight();
      }
      else
      {
        if(elBtnState == 0)
        {
          patrol();
        }
        else if(elBtnState == 1)
        {
          elStandard();
        }
        else
        {
          full(shortSpeed, 255);
        }
      }
    }
    else
    {
      strip.clear();
      strip.show();
    }
  }
}

void interrupt()
{
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
}

void elStandard()
{
  strip.clear();
  strip.show();
  strip.setBrightness(brightness);
  for(int i=0; i<4; i++)
  {
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
  }
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
  for(int i=0; i<4; i++)
  {
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
  }
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
  for(int i=0; i<4; i++)
  {
    full(speed, brightness);
    if(gotInterrupt)
    {
      gotInterrupt = false;
      return;
    }
  }
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
}

void elStandardSceneLeft()
{
  strip.setBrightness(255);
  if(sceneLeftState)
  {
    for(int i=0; i<8; i++)
    {
      strip.setPixelColor(i, white);
    }
    sceneLeftState = false;
  }
  for(int i=0; i<3; i++)
  {
    for(int a=8; a<16; a++)
    {
      strip.setPixelColor(a, colorA);
    }
    strip.show();
    if(gotInterrupt)
    {
      strip.clear();
      strip.show();
      break;
    }
    delay(shortSpeed);
    for(int a=8; a<16; a++)
    {
      strip.setPixelColor(a, off);
    }
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
    for(int a=8; a<16; a++)
    {
      strip.setPixelColor(a, colorB);
    }
    strip.show();
    if(gotInterrupt)
    {
      strip.clear();
      strip.show();
      break;
    }
    delay(shortSpeed);
    for(int a=8; a<16; a++)
    {
      strip.setPixelColor(a, off);
    }
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

void elStandardSceneRight()
{
  strip.setBrightness(255);
  if(sceneRightState)
  {
    for(int i=8; i<16; i++)
    {
      strip.setPixelColor(i, white);
    }
    sceneRightState = false;
  }
  for(int i=0; i<3; i++)
  {
    for(int a=0; a<8; a++)
    {
      strip.setPixelColor(a, colorA);
    }
    strip.show();
    if(gotInterrupt)
    {
      strip.clear();
      strip.show();
      break;
    }
    delay(shortSpeed);
    for(int a=0; a<8; a++)
    {
      strip.setPixelColor(a, off);
    }
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
    strip.show();
    if(gotInterrupt)
    {
      strip.clear();
      strip.show();
      break;
    }
    delay(shortSpeed);
    for(int a=0; a<8; a++)
    {
      strip.setPixelColor(a, off);
    }
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
  strip.setBrightness(10);
  strip.setPixelColor(0, colorA);
  strip.setPixelColor(1, colorA);
  strip.setPixelColor(15, colorB);
  strip.setPixelColor(14, colorB);
  for(int i=2; i<14; i++)
  {
    strip.setPixelColor(i, off);
    if(gotInterrupt)
    {
      gotInterrupt = false;
      return;
    }
  }
  strip.show();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
  }
}

void headLightPatrol()
{
  strip.setBrightness(brightness);
  strip.setPixelColor(0, colorA);
  strip.setPixelColor(1, colorA);
  strip.setPixelColor(14, colorB);
  strip.setPixelColor(15, colorB);
  for(int i=2; i<14; i++)
  {
    strip.setPixelColor(i, white);
    if(gotInterrupt)
    {
      gotInterrupt = false;
      return;
    }
  }
  strip.show();
  if(gotInterrupt)
  {
    gotInterrupt = false;
    return;
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

void headLight()
{
  strip.setBrightness(brightness);
  for(int i=0; i<16; i++)
  {
    strip.setPixelColor(i, white);
  }
  strip.show();
}

void elHeadLight()
{
  strip.setBrightness(brightness);
  strip.setPixelColor(4, white);
  strip.setPixelColor(5, white);
  strip.setPixelColor(6, white);
  strip.setPixelColor(7, white);
  strip.setPixelColor(8, white);
  strip.setPixelColor(9, white);
  strip.setPixelColor(10, white);
  strip.setPixelColor(11, white);
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
    strip.setPixelColor(0, off);
    strip.setPixelColor(1, off);
    strip.setPixelColor(2, off);
    strip.setPixelColor(3, off);
    strip.setPixelColor(12, off);
    strip.setPixelColor(13, off);
    strip.setPixelColor(14, off);
    strip.setPixelColor(15, off);
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
    strip.setPixelColor(0, colorB);
    strip.setPixelColor(1, colorB);
    strip.setPixelColor(2, colorB);
    strip.setPixelColor(3, colorB);
    strip.setPixelColor(12, colorA);
    strip.setPixelColor(13, colorA);
    strip.setPixelColor(14, colorA);
    strip.setPixelColor(15, colorA);
    strip.show();
    if(gotInterrupt)
    {
      strip.clear();
      strip.show();
      break;
    }
    delay(speed);
    strip.setPixelColor(0, off);
    strip.setPixelColor(1, off);
    strip.setPixelColor(2, off);
    strip.setPixelColor(3, off);
    strip.setPixelColor(12, off);
    strip.setPixelColor(13, off);
    strip.setPixelColor(14, off);
    strip.setPixelColor(15, off);
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
  Serial.println("elBtn");
  if(interruptTime - lastInterruptTime > 300)
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
  if(interruptTime - lastInterruptTime > 300)
  {
    if(elBtnState == 0)
    {
      leftBtnState = false;
      rightBtnState = false;
      elBtnState = 1;
      gotInterrupt = true;
    }
    else if(elBtnState == 1)
    {
      leftBtnState = false;
      rightBtnState = false;
      elBtnState = 2;
      gotInterrupt = true;
    }
    else
    {
      leftBtnState = false;
      rightBtnState = false;
      elBtnState = 0;
      gotInterrupt = true;
    }
  }
  lastInterruptTime = interruptTime;
}

void leftBtnISR()
{
  Serial.println("Left Button Pressed");
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if(interruptTime - lastInterruptTime > 300)
  {
    if(leftBtnState)
    {
      leftBtnState = false;
      gotInterrupt = true;
    }
    else
    {
      rightBtnState = false;
      leftBtnState = true;
      gotInterrupt = true;
      sceneLeftState = true;
    }
  }
  lastInterruptTime = interruptTime;
}

void rightBtnISR()
{
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if(interruptTime - lastInterruptTime > 300)
  {
    if(rightBtnState)
    {
      rightBtnState = false;
      gotInterrupt = true;
    }
    else
    {
      leftBtnState = false;
      rightBtnState = true;
      gotInterrupt = true;
      sceneRightState = true;
    }
  }
  lastInterruptTime = interruptTime;
}

void hlBtnISR()
{
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if(interruptTime - lastInterruptTime > 300)
  {
    if(hlBtnState)
    {
      hlBtnState = false;
      gotInterrupt = true;
    }
    else
    {
      hlBtnState = true;
      gotInterrupt = true;
    }
  }
  lastInterruptTime = interruptTime;
}

void hlModBtnISR()
{
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  if(interruptTime - lastInterruptTime > 300)
  {
    if(hlModBtnState == 0)
    {
      hlModBtnState = 1;
      gotInterrupt = true;
    }
    else
    {
      hlModBtnState = 0;
      gotInterrupt = true;
      strip.setBrightness(brightness);
    }
  }
  lastInterruptTime = interruptTime;
}