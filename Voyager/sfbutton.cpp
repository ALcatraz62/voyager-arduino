/* Spiral Forge
 *  9/28/2019
 *  Andy Linton
 *  This is a small class to handle reading button and debouncing it. 
 *  setup is called in the main setup() funct
 *  update() will read the inputs and update the button state and return true if pressed
 *  isPressed() will return true if the state is pressed
 */
#include "sfbutton.h"

SfButton::SfButton(int btnPin)
{
  _btnPin = btnPin;
  setup();
}

void SfButton::setup()
{
  pinMode(_btnPin, INPUT_PULLUP);
  _btnState = BTN_OFF;
  _softPress = false;
  _pressTime = millis();
}
bool SfButton::update()
{
  unsigned long currTime = millis();
  update(currTime);
}

void SfButton::press()
{
  _softPress = true;
}

bool SfButton::update(unsigned long currTime)
{
  int currState = digitalRead(_btnPin);
  int delta = currTime -_pressTime;
  if(_softPress)
  {
    currState = BTN_ON;
    _btnState = BTN_PRESSED;
    delta = DEBOUNCE_TIME;
    _softPress = false;
  }
  if(currState == BTN_ON)
  {
    if(_btnState == BTN_OFF)
    {
      _pressTime = currTime;
      _btnState = BTN_PRESSED;
    }
    else if(_btnState == BTN_PRESSED && delta >= DEBOUNCE_TIME)
    {
      _btnState = BTN_ON;
    }
    else if(_btnState == BTN_ON)
    {
      _btnState = BTN_HELD;
    }
    
  }
  else
  {
    _btnState = BTN_OFF;
    _pressTime = currTime;  
  }
  return isOn();
}

bool SfButton::isPressed()
{
  return _btnState == BTN_ON;
}

bool SfButton::isHeld()
{
  return _btnState == BTN_HELD;
}

bool SfButton::isOn()
{
  return _btnState == BTN_ON || _btnState == BTN_HELD;
}
