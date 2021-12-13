#ifndef SFBUTTON_H
#define SFBUTTON_H
/* Spiral Forge
 *  9/28/2019
 *  Andy Linton
 *  This is a small class to handle reading button and debouncing it. 
 *  setup is called in the main setup() funct
 *  update() will read the inputs and update the button state and return true if pressed
 *  isPressed() will return true if the state is pressed
 */

#include "Arduino.h"

#define BTN_OFF 1 
#define BTN_ON 0 
#define BTN_PRESSED 2
#define BTN_HELD 3 
#define DEBOUNCE_TIME 75 

class SfButton
{
  public:
    SfButton(int btnPin);
    void setup();
    bool update();
    bool update(unsigned long currTime);
    bool isPressed();
    bool isHeld();
    bool isOn();

  private:
    int _btnPin;
    unsigned int _btnState;
    unsigned long _pressTime;
};
#endif 
