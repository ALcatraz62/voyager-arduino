//Voyager Controller
// This code handles the special effects for the Star Trek Voyager Scale Model.
#include "VoyagerConfig.h"
#include "SfButton.h"

unsigned long _lastUpdate;
byte _navState;
int _navCount;


SfButton _navBtn(NAV_TOGGLE_BTN);

void setup() {
  _navState = OFF;
  
  _navBtn.setup();
  pinMode(NAV_LIGHTS_PIN,OUTPUT);
  pinMode(NAV_BEACON_PIN, OUTPUT);
}

void loop()
{
  unsigned long currTime = millis();
  unsigned long delta = currTime - _lastUpdate;

  updateNavState(delta);
  _lastUpdate = currTime;
}

void updateNavState(unsigned long delta)
{
  switch(_navState)
  {
    case OFF:
    digitalWrite(NAV_LIGHTS_PIN, LOW);
    digitalWrite(NAV_BEACON_PIN,LOW); 
    break;
    case ON:
    digitalWrite(NAV_LIGHTS_PIN, HIGH);
    digitalWrite(NAV_BEACON_PIN, HIGH);
    break;
    case BLINK_ON:
    digitalWrite(NAV_LIGHTS_PIN,HIGH);
    digitalWrite(NAV_BEACON_PIN,HIGH);
    _navCount += delta;
    if(_navCount >= BLINK_ON_TIME)
    {
      _navCount = 0;
      _navState = BLINK_OFF;
    }
    break;
    case BLINK_OFF:
    digitalWrite(NAV_LIGHTS_PIN,HIGH);
    digitalWrite(NAV_BEACON_PIN,LOW);
    _navCount += delta;
    if(_navCount >= BLINK_OFF_TIME)
    {
      _navCount = 0;
      _navState = BLINK_ON;
    }
    break;
  }
}
