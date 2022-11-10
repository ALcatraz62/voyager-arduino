/* Spiral Forge
 *  9/28/2019
 *  Andy Linton
 *  This is a small class to handle reading button and debouncing it. 
 *  setup is called in the main setup() funct
 *  update() will read the inputs and update the button state and return true if pressed
 *  isPressed() will return true if the state is pressed
 */
#include "softButton.h"

SoftButton::SoftButton()
{
  setup();
}

void SoftButton::setup()
{
  _softPress = false;
  _pressTime = millis();
}


void SoftButton::press()
{
  _softPress = true;
}


bool SoftButton::update(uint8_t currState,uint32_t currTime)
{
  uint32_t delta = currTime -_pressTime;
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

bool SoftButton::isPressed()
{
  return _btnState == BTN_ON;
}

bool SoftButton::isHeld()
{
  return _btnState == BTN_HELD;
}

bool SoftButton::isOn()
{
  return _btnState == BTN_ON || _btnState == BTN_HELD;
}
