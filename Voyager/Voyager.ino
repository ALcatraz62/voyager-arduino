#include <FastLED.h>

//Voyager Controller
// This code handles the special effects for the Star Trek Voyager Scale Model.
#include "VoyagerConfig.h"
#include "SfButton.h"


String _serialCmd;
unsigned long _lastUpdate;

int _navCount;
int  _warpCount;

byte _navState;
byte _headlightState;
byte _interiorState;
byte _deflectorState;
byte _warpState;

SfButton _navBtn(NAV_TOGGLE_BTN);
SfButton _headlightBtn(HEADLIGHT_BTN);
SfButton _interiorBtn(INTERIOR_BTN);
SfButton _deflectorBtn(DEFLECTOR_BTN);
SfButton _warpBtn(WARP_BTN);

CRGB _warpEngines[WARP_LED_CNT];
void printState(int state)
{
 switch(state)
 {
  case OFF: Serial.print("OFF");break;
  case ON: Serial.print("ON");break;
  case BLINK_ON: Serial.print("BLINK_ON");break;
  case BLINK_OFF: Serial.print("BLINK OFF"); break;
  case WARP_IDLE: Serial.print("WARP IDLE"); break;
  case WARP_ENGAGE: Serial.print("WARP ENGAGE"); break;
  case WARP_FLASH: Serial.print("WARP FLASH"); break;
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
  Serial.print("WARP: ");printState(_warpState);Serial.println();
}

void setup() {
  Serial.begin(9600);
  _navState = BLINK_ON;
  _headlightState = OFF;
  _interiorState = OFF;
  _deflectorState = OFF;
  _warpState = OFF;
  _navBtn.setup();
  _headlightBtn.setup();
  _interiorBtn.setup();
  _deflectorBtn.setup();
  _warpBtn.setup();

  pinMode(NAV_LIGHTS_PIN,OUTPUT);
  pinMode(NAV_BEACON_PIN, OUTPUT);
  pinMode(HEADLIGHT_PIN, OUTPUT);
  pinMode(INTERIOR_LIGHTS_PIN, OUTPUT);
  pinMode(DEFLECTOR_PIN, OUTPUT);
  FastLED.addLeds<NEOPIXEL,WARP_ENGINES_PIN>(_warpEngines,WARP_LED_CNT);
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
    else if(_serialCmd.startsWith("WARP"))
    {
      _warpBtn.press();
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
  _warpBtn.update(currTime);
  
  updateNavState(delta);
  updateHeadlightState(delta);
  updateInteriorState(delta);
  updateDeflectorState(delta);
  updateWarpState(delta);
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

void updateWarpState(unsigned long delta)
{
  int dim = 0;
  switch(_warpState)
  {
    case WARP_IDLE:
      for(int i = 0; i < WARP_LED_CNT; i++)
      {
        if(i < COLLECTOR_INDEX)
        {
          _warpEngines[i] = IDLE_CHILLER_COLOR;
        }
        else
        {
          _warpEngines[i] = IDLE_COLLECTOR_COLOR;
        }
      }
      if(_warpBtn.isPressed())
      {
        _warpCount = 0;
        _warpState = WARP_ENGAGE;
      }
      break;
    case WARP_ENGAGE:
      _warpCount += delta;
      dim = map(_warpCount,0, ENGAGE_TIME,0x30,0xE8);
      Serial.println(dim);
      for(int i = 0; i < WARP_LED_CNT; i++)
      {
        if( i < COLLECTOR_INDEX)
          _warpEngines[i].setRGB(0x00,0x00,dim);
        else
          _warpEngines[i].setRGB(dim, 0x00, 0x00);
      }
      if(_warpCount >= ENGAGE_TIME)
      {
        _warpState = WARP_FLASH;
        _warpCount = 0;
      }
      break;
    case WARP_FLASH:
      for(int i = 0; i < WARP_LED_CNT; i++)
      {
        _warpEngines[i] = CRGB::White;
      }
      _warpCount += delta;
      if(_warpCount >= ENGAGE_FLASH_TIME)
      {
        _warpState = ON;
        _warpCount = 0;
      }
      break;
    case ON:
      for(int i = 0; i < WARP_LED_CNT; i++)
      {
        if(i < COLLECTOR_INDEX)
        {
          _warpEngines[i] = ON_CHILLER_COLOR;
        }
        else
        {
          _warpEngines[i] = ON_COLLECTOR_COLOR;
        }
      }
      if(_warpBtn.isPressed())
      {
        _warpState = OFF;
      }
      break;
    case OFF:
      for(int i = 0; i < WARP_LED_CNT; i++)
      {
        _warpEngines[i] = CRGB::Black;
      }
      if(_warpBtn.isPressed())
      {
        _warpState = WARP_IDLE;
      }
      break;
  }
  FastLED.show();
}
