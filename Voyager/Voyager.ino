//Voyager Controller
// This code handles the special effects for the Star Trek Voyager Scale Model.
#include "VoyagerConfig.h"
#include "SfButton.h"


String _serialCmd;
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

void printState(int state)
{
 switch(state)
 {
  case OFF: Serial.print("OFF");break;
  case ON: Serial.print("ON");break;
  case BLINK_ON: Serial.print("BLINK_ON");break;
  case BLINK_OFF: Serial.print("BLINK OFF"); break;
  default: Serial.print(state);break;
  }
}

void printStatus()
{
  Serial.println("Voyager Status");
  Serial.print("NAV: ");printState(_navState);Serial.println();
  Serial.print("HEADLIGHT: ");printState(_headlightState);Serial.println();
  Serial.print("INTERIOR: ");printState(_interiorState);Serial.println();
  Serial.print("DEFLECTOR: ");printState(_deflectorState);Serial.println();
}

void setup() {
  Serial.begin(9600);
  _navState = BLINK_ON;
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

  printStatus();
}

void handleSerial()
{
  if(Serial.available())
  {
    _serialCmd = Serial.readString();
    
    if(_serialCmd.startsWith("NAV"))
    {
      _navBtn.press();
    }
    else if(_serialCmd.startsWith("HEAD"))
    {
     _headlightBtn.press(); 
    }
    else if(_serialCmd.startsWith("DEFLECT"))
    {
      _deflectorBtn.press();
    }
    else if(_serialCmd.startsWith("INTER"))
    {
      _interiorBtn.press();
    }
    else if(_serialCmd.startsWith("STATUS"))
    {
      printStatus();
    }
  }
}

void loop()
{
  unsigned long currTime = millis();
  unsigned long delta = currTime - _lastUpdate;

  //Serial button pressing
  handleSerial();
  
  _navBtn.update(currTime);
  _headlightBtn.update(currTime);
  _interiorBtn.update(currTime);
  _deflectorBtn.update(currTime);
  
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
        _navState = ON;
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
        _navState = ON;
      }
      else if(_navCount >= BLINK_OFF_TIME)
      {
        _navCount = 0;
        _navState = BLINK_ON;
      }
      break;
  }
}
