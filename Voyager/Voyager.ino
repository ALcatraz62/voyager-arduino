//Voyager Controller
// This code handles the special effects for the Star Trek Voyager Scale Model.
#include "VoyagerConfig.h"
#include "SfButton.h"

unsigned long _lastUpdate;

int _navCount;

byte _navState;
byte _headlightState;
byte _interiorState;
byte _deflectorState;

SfButton _navBtn(NAV_TOGGLE_BTN);
SfButton _headlightBtn(HEADLIGHT_BTN);
SfButton _interiorBtn(INTERIOR_BTN);
SfButton _deflectorBtn(DEFLECTOR_BTN);

void setup() {
  _navState = OFF;
  _headlightState = OFF;
  _interiorState = OFF;
  _deflectorState = OFF;
  
  _navBtn.setup();
  _headlightBtn.setup();
  _interiorBtn.setup();
  _deflectorBtn.setup();
  
  pinMode(NAV_LIGHTS_PIN,OUTPUT);
  pinMode(NAV_BEACON_PIN, OUTPUT);
  pinMode(HEADLIGHT_PIN, OUTPUT);
  pinMode(INTERIOR_LIGHTS_PIN, OUTPUT);
  pinMode(DEFLECTOR_PIN, OUTPUT);
}

void loop()
{
  unsigned long currTime = millis();
  unsigned long delta = currTime - _lastUpdate;

  _navBtn.update(currTime);
  _headlightBtn.update(currTime);
  
  updateNavState(delta);
  updateHeadlightState(delta);
  updateInteriorState(delta);
  updateDeflectorState(delta);
  _lastUpdate = currTime;
}

void updateDeflectorState(unsigned long delta)
{
  switch(_deflectorState)
  {
    case OFF:
      digitalWrite(DEFLECTOR_PIN, LOW);
      if(_deflectorBtn.isPressed())
      {
        _deflectorState = ON;
      }
      break;
    case ON:
      digitalWrite(DEFLECTOR_PIN, HIGH);
      if(_deflectorBtn.isPressed())
      {
        _deflectorState = OFF;
      }
      break;
  }
}

void updateInteriorState(unsigned long delta)
{
  switch(_interiorState)
  {
    case OFF:
      digitalWrite(INTERIOR_LIGHTS_PIN, LOW);
      if(_interiorBtn.isPressed())
      {
        _interiorState = ON;
      }
      break;
    case ON:
     digitalWrite(INTERIOR_LIGHTS_PIN,HIGH);
     if(_interiorBtn.isPressed())
     {
        _interiorState = OFF; 
     }
  }
}

void updateHeadlightState(unsigned long delta)
{
  switch(_headlightState)
  {
    case OFF:
      digitalWrite(HEADLIGHT_PIN, LOW);
      if(_headlightBtn.isPressed())
      {
        _headlightState = ON;
      }
      break;
    case ON:
     digitalWrite(HEADLIGHT_PIN,HIGH);
     if(_headlightBtn.isPressed())
     {
        _headlightState = OFF; 
     }
  }
}

/*
 * NAV Lights are always on, but beacon blinks at a rate
 * Nav Button cycles thru 3 modes OFF->BLINK->STEADY ON
 */
void updateNavState(unsigned long delta)
{
  switch(_navState)
  {
    case OFF:
      digitalWrite(NAV_LIGHTS_PIN, LOW);
      digitalWrite(NAV_BEACON_PIN,LOW); 
      if(_navBtn.isPressed())
      {
        _navCount = 0;
        _navState = BLINK_ON;
      }
      break;
    case ON:
      digitalWrite(NAV_LIGHTS_PIN, HIGH);
      digitalWrite(NAV_BEACON_PIN, HIGH);
      if(_navBtn.isPressed())
      {
        _navState = OFF;
      }
      break;
    case BLINK_ON:
      digitalWrite(NAV_LIGHTS_PIN,HIGH);
      digitalWrite(NAV_BEACON_PIN,HIGH);
      _navCount += delta;

      if(_navBtn.isPressed())
      {
        _navState = OFF;
      }
      else if(_navCount >= BLINK_ON_TIME)
      {
        _navCount = 0;
        _navState = BLINK_OFF;
      }
      break;
    case BLINK_OFF:
      digitalWrite(NAV_LIGHTS_PIN,HIGH);
      digitalWrite(NAV_BEACON_PIN,LOW);
      _navCount += delta;

      if(_navBtn.isPressed())
      {
        _navState = OFF;
      }
      else if(_navCount >= BLINK_OFF_TIME)
      {
        _navCount = 0;
        _navState = BLINK_ON;
      }
      break;
  }
}
