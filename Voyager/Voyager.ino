#include <FastLED.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "SoundFx.h"
#include "Adafruit_MPR121.h"

//Voyager Controller
// This code handles the special effects for the Star Trek Voyager Scale Model.
#include "VoyagerConfig.h"
#include "VoyagerCommands.h"
#include "softButton.h"

#ifndef _BV
#define _BV(bit) (1 << (bit)) 
#endif

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();
// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;

SoftwareSerial ss = SoftwareSerial(SND_TX,SND_RX);
SoundFx sfx = SoundFx(&ss, SND_RST);

uint32_t _lastUpdate;

uint16_t _navCount;
uint16_t _warpCount;
uint16_t _fwdCount;
uint16_t _aftCount;

uint8_t _navState;
uint8_t _headlightState;
uint8_t _interiorState;
uint8_t _deflectorState;
uint8_t _impulseState;
uint8_t _warpState;
uint8_t _fwdPhotonState;
uint8_t _aftPhotonState;
uint8_t _phaserState;

int8_t _phaserSide;
int8_t _phaserStart;
int8_t _phaserRevStart;
int8_t _phaserFireIndex;
uint16_t _blastCnt =0;

SoftButton _navBtn;
SoftButton _headlightBtn;
SoftButton _interiorBtn;
SoftButton _deflectorBtn;
SoftButton _impulseBtn;
SoftButton _warpBtn;
SoftButton _fwdPhotonBtn;
SoftButton _aftPhotonBtn;
SoftButton _photonToggleBtn;
SoftButton _phaserPortBtn;
SoftButton _phaserStarBtn;

CRGB _warpEngines[WARP_LED_CNT];
CRGB _fwdPhotons[2];
CRGB _aftPhotons[2];
CRGB _phasers[PHASER_LED_CNT];

void printState(uint8_t state)
{
 switch(state)
 {
  case OFF: Serial.print(F("OFF"));break;
  case ON: Serial.print(F("ON"));break;
  case BLINK_ON: Serial.print(F("BLINK_ON"));break;
  case BLINK_OFF: Serial.print(F("BLINK OFF")); break;
  case WARP_IDLE: Serial.print(F("WARP IDLE")); break;
  case WARP_ENGAGE: Serial.print(F("WARP ENGAGE")); break;
  case WARP_FLASH: Serial.print(F("WARP FLASH")); break;
  case PHOTON_IDLE_1: Serial.print(F("PHOTON IDLE 1")); break;
  case PHOTON_IDLE_2: Serial.print(F("PHOTON IDLE 2")); break;
  case FIRING_1: Serial.print(F("FIRING 1")); break;
  case FIRING_2: Serial.print(F("FIRING 2")); break;
  default: Serial.print(state);break;
  }
}

void printStatus()
{
  Serial.println(F("Voyager Status"));
  Serial.print(F("NAV: "));printState(_navState);Serial.println();
  Serial.print(F("HEADLIGHT: "));printState(_headlightState);Serial.println();
  Serial.print(F("INTERIOR: "));printState(_interiorState);Serial.println();
  Serial.print(F("DEFLECTOR: "));printState(_deflectorState);Serial.println();
  Serial.print(F("IMPULSE: "));printState(_impulseState);Serial.println();
  Serial.print(F("WARP: "));printState(_warpState);Serial.println();
  Serial.print(F("FWD PHOTONS: "));printState(_fwdPhotonState);Serial.println();
  Serial.print(F("AFT PHOTONS: "));printState(_aftPhotonState);Serial.println();
  Serial.print(F("PHASER: "));printState(_phaserState);Serial.println();
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  ss.begin(9600);
  _navState = OFF;
  _headlightState = OFF;
  _interiorState = OFF;
  _impulseState = OFF;
  _deflectorState = OFF;
  _warpState = OFF;
  _fwdPhotonState = OFF;
  _aftPhotonState = OFF;
  _phaserState = OFF;
  
  _navBtn.setup();
  _headlightBtn.setup();
  _interiorBtn.setup();
  _deflectorBtn.setup();
  _impulseBtn.setup();
  _warpBtn.setup();
  _fwdPhotonBtn.setup();
  _aftPhotonBtn.setup();
  _photonToggleBtn.setup();
  _phaserPortBtn.setup();
  
  pinMode(NAV_LIGHTS_PIN,OUTPUT);
  pinMode(NAV_BEACON_PIN, OUTPUT);
  pinMode(HEADLIGHT_PIN, OUTPUT);
  pinMode(REAR_LOUNGE_PIN,OUTPUT);
  pinMode(IMPLUSE_ENGINES_PIN,OUTPUT);
  pinMode(INTERIOR_LIGHTS_PIN, OUTPUT);
  pinMode(DEFLECTOR_PIN, OUTPUT);
  FastLED.addLeds<NEOPIXEL,WARP_ENGINES_PIN>(_warpEngines,WARP_LED_CNT);
  FastLED.addLeds<NEOPIXEL,AFT_PHOTON>(_aftPhotons,2);
  FastLED.addLeds<NEOPIXEL,FWD_PHOTON>(_fwdPhotons,2);
  FastLED.addLeds<NEOPIXEL,PHASER_PIN>(_phasers,PHASER_LED_CNT);
  
  if (!cap.begin(0x5A))
    Serial.println(F("MPR121 (Touch Sensor) not found, check wiring?"));
  else
    Serial.println(F("MPR121 (Touch Sensor) found!"));

  if(!sfx.reset())
    Serial.println(F("Sound Board not found"));  
  else
  {
    Serial.println(F("Sound Board found!"));
    setVolume();
  }
  
  printStatus();
}

void setVolume()
{
  uint16_t vol;
  while(vol < SND_VOL)
  {
    vol = sfx.volUp();
  }
  Serial.print(F("Volume: "));
  Serial.println(vol);
}

void handleCapTouch(uint32_t currTime)
{
    // Get the currently touched pads
  currtouched = cap.touched();
  //Serial.println(currtouched);
  uint8_t currState = BTN_OFF;
  for (uint8_t i=0; i<12; i++) {
    
    // it if *is* touched and *wasnt* touched before, alert!
    if (currtouched & _BV(i))
    {
      
      currState = BTN_ON;
    }
    // if it *was* touched and now *isnt*, alert!
    else
    { 
      currState = BTN_OFF;
    }
      switch(i)
      {
        case 0:_interiorBtn.update(currState, currTime); break;
        case 1:_navBtn.update(currState, currTime); break;
        case 2:_headlightBtn.update(currState, currTime); break;
        case 3:_deflectorBtn.update(currState, currTime); break;
        case 4:_impulseBtn.update(currState, currTime); break;
        case 5:_warpBtn.update(currState, currTime); break;
        case 6:_photonToggleBtn.update(currState, currTime); break;
        case 7:_fwdPhotonBtn.update(currState, currTime); break;
        case 8:_aftPhotonBtn.update(currState, currTime); break;
        case 9:_phaserPortBtn.update(currState, currTime); break;
        case 10:_phaserStarBtn.update(currState, currTime); break;
        case 11: break;
      }  
  }
  // reset our state
  lasttouched = currtouched;
}

void handleSerial()
{
  String serialCmd;
  if(Serial.available())
  {
    serialCmd = Serial.readString();
    
    if(serialCmd.startsWith(NAV_CMD))
    {
      _navBtn.press();
    }
    else if(serialCmd.startsWith(HEADLIGHT_CMD))
    {
     _headlightBtn.press(); 
    }
    else if(serialCmd.startsWith(DEFLECTOR_CMD))
    {
      _deflectorBtn.press();
    }
    else if(serialCmd.startsWith(INTERIOR_CMD))
    {
      _interiorBtn.press();
    }
    else if(serialCmd.startsWith(WARP_CMD))
    {
      _warpBtn.press();
    }
    else if(serialCmd.startsWith(PHOTON_CMD))
    {
      _photonToggleBtn.press();
    }
    else if(serialCmd.startsWith(FOWARD_FIRE_CMD))
    {
      _fwdPhotonBtn.press();
    }
    else if(serialCmd.startsWith(AFT_FIRE_CMD))
    {
      _aftPhotonBtn.press();
    }
    else if(serialCmd.startsWith(IMPULSE_CMD))
    {
      _impulseBtn.press();
    }
    else if(serialCmd.startsWith(PHASER_PORT_CMD))
    {
      _phaserPortBtn.press();
    }
    else if(serialCmd.startsWith(PHASER_STAR_CMD))
    {
      _phaserStarBtn.press();
    }
    else if(serialCmd.startsWith(STATUS_TEXT_CMD))
    {
      printStatus();
    }
  }
}

void loop()
{
  uint32_t currTime = millis();
  uint32_t delta = currTime - _lastUpdate;

  //Serial button pressing
  handleCapTouch(currTime);
  handleSerial();
  
  updateNavState(delta);
  updateHeadlightState(delta);
  updateInteriorState(delta);
  updateDeflectorState(delta);
  updateImpulseState(delta);
  updateWarpState(delta);
  updatePhaserState(delta);
  _fwdPhotonState = updatePhotonState(_fwdPhotonState, &_fwdCount, _fwdPhotons, _photonToggleBtn.isPressed(), _fwdPhotonBtn.isPressed(), delta);
  _aftPhotonState = updatePhotonState(_aftPhotonState, &_aftCount, _aftPhotons, _photonToggleBtn.isPressed(), _aftPhotonBtn.isPressed(), delta);

  _lastUpdate = currTime;
   FastLED.show();
}

void updateDeflectorState(uint32_t delta)
{
  switch(_deflectorState)
  {
    case OFF:
      digitalWrite(DEFLECTOR_PIN, LOW);
      if(_deflectorBtn.isPressed())
      {
        sfx.playTrack((uint8_t)DEFLECTOR_ON_FX);
        _deflectorState = ON;
      }
      break;
    case ON:
       
      digitalWrite(DEFLECTOR_PIN, HIGH);
      if(_deflectorBtn.isPressed())
      {
        sfx.playTrack((uint8_t)DEFLECTOR_OFF_FX);
        _deflectorState = OFF;
      }
      break;
  }
}

void updateInteriorState(uint32_t delta)
{
  switch(_interiorState)
  {
    case OFF:
      digitalWrite(INTERIOR_LIGHTS_PIN, LOW);
      digitalWrite(REAR_LOUNGE_PIN,LOW);
      if(_interiorBtn.isPressed())
      {
         sfx.playTrack((uint8_t)INTERIOR_ON_FX);
        _interiorState = ON;
      }
      break;
    case ON:
     analogWrite(INTERIOR_LIGHTS_PIN,INTERIOR_BRIGHTNESS);
     digitalWrite(REAR_LOUNGE_PIN, HIGH);
     if(_interiorBtn.isPressed())
     {        
        sfx.playTrack((uint8_t)INTERIOR_OFF_FX);
        _interiorState = OFF; 
     }
  }
}

void updateImpulseState(uint32_t delta)
{
  switch(_impulseState)
  {
    case OFF:
      digitalWrite(IMPLUSE_ENGINES_PIN, LOW);
      if(_impulseBtn.isPressed())
      {
        sfx.playTrack((uint8_t)IMPULSE_ON_FX);
        _impulseState = ON;
      }
      break;
    case ON:
     digitalWrite(IMPLUSE_ENGINES_PIN,HIGH);
     if(_impulseBtn.isPressed())
     {
        sfx.playTrack((uint8_t)IMPULSE_OFF_FX);
        _impulseState = OFF; 
     }
  }
}

void updateHeadlightState(uint32_t delta)
{
  switch(_headlightState)
  {
    case OFF:
      digitalWrite(HEADLIGHT_PIN, LOW);
      if(_headlightBtn.isPressed())
      {
         sfx.playTrack((uint8_t)HEADLIGHT_ON_FX);
        _headlightState = ON;
      }
      break;
    case ON:
     digitalWrite(HEADLIGHT_PIN,HIGH);
     if(_headlightBtn.isPressed())
     {
        sfx.playTrack((uint8_t)HEADLIGHT_OFF_FX);
        _headlightState = OFF; 
     }
  }
}

/*
 * NAV Lights are always on, but beacon blinks at a rate
 * Nav Button cycles thru 3 modes OFF->BLINK->STEADY ON
 */
void updateNavState(uint32_t delta)
{
  switch(_navState)
  {
    case OFF:
      digitalWrite(NAV_LIGHTS_PIN, LOW);
      digitalWrite(NAV_BEACON_PIN,LOW); 
      if(_navBtn.isPressed())
      {
        sfx.playTrack((uint8_t)NAV_BLINK_FX);
        _navCount = 0;
        _navState = BLINK_ON;
      }
      
      break;
    case ON:
      digitalWrite(NAV_LIGHTS_PIN, HIGH);
      digitalWrite(NAV_BEACON_PIN, HIGH);
      if(_navBtn.isPressed())
      {
        sfx.playTrack((uint8_t)NAV_OFF_FX);
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
         sfx.playTrack((uint8_t)NAV_ON_FX);
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
        sfx.playTrack((uint8_t)NAV_ON_FX);
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

void updateWarpState(uint32_t delta)
{
  uint8_t dim = 0;
   uint16_t maxIndex = 0;
  switch(_warpState)
  {
    case WARP_POWERUP:
      _warpCount += delta;
       maxIndex = map(_warpCount,0,POWERUP_TIME,0,WARP_LED_CNT);
      for( uint16_t i=0; i< maxIndex; i++)
      {
        if(i < COLLECTOR_INDEX)
          _warpEngines[i] = IDLE_CHILLER_COLOR;
        else
          _warpEngines[i] = IDLE_COLLECTOR_COLOR;
      }
      if(_warpCount > POWERUP_TIME)
      {
        _warpState = WARP_IDLE;
        _warpCount = 0;
      }
      break;
    case WARP_IDLE:
      for(uint16_t i = 0; i < WARP_LED_CNT; i++)
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
        sfx.playTrack((uint8_t)WARP_ENGAGE_FX);
        _warpCount = 0;
        _warpState = WARP_ENGAGE;
      }
      break;
    case WARP_ENGAGE:
      _warpCount += delta;
      dim = map(_warpCount,0, ENGAGE_TIME,0x30,0xE8);
      for(uint16_t i = 0; i < WARP_LED_CNT; i++)
      {
        if( i < COLLECTOR_INDEX)
          _warpEngines[i].setRGB(0x00,0x00,dim);
        else
          _warpEngines[i].setRGB(dim, 0x00, 0x00);
      }
      if(_warpCount >= ENGAGE_TIME)
      {
        _warpState = WARP_FLASH;
        sfx.playTrack((uint8_t)WARP_FLASH_FX);
        _warpCount = 0;
      }
      break;
    case WARP_FLASH:
      for(uint16_t i = 0; i < WARP_LED_CNT; i++)
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
      for(uint16_t i = 0; i < WARP_LED_CNT; i++)
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
        sfx.playTrack((uint8_t)WARP_OFF_FX);
        _warpState = OFF;
      }
      break;
    case OFF:
      for(uint16_t i = 0; i < WARP_LED_CNT; i++)
      {
        _warpEngines[i] = CRGB::Black;
      }
      if(_warpBtn.isPressed())
      {
        sfx.playTrack((uint8_t)WARP_ON_FX);
        _warpState = WARP_POWERUP;
      }
      break;
  }
}

byte updatePhotonState(uint8_t currState,uint16_t* animeCnt, CRGB leds[],bool btnPressed, bool firePressed, uint32_t delta)
{
  switch(currState)
  {
   case OFF:
      leds[0] = CRGB::Black;
      leds[1] = CRGB::Black;
      *animeCnt = 0;
      if(btnPressed)
      {
        sfx.playTrack((uint8_t)PHOTON_ON_FX);
        currState = PHOTON_IDLE_1;
      }else if(firePressed)
      {
        sfx.playTrack((uint8_t)ERROR_FX);
      }
      break;
   case PHOTON_IDLE_1:
      leds[0] = PHOTON_IDLE_COLOR;
      leds[1] = PHOTON_IDLE_COLOR;
      *animeCnt = 0;
      if(firePressed)
      {
        sfx.playTrack((uint8_t)PHOTON_FIRE_FX);
        currState = FIRING_1;
      }
      if(btnPressed)
      {
        sfx.playTrack((uint8_t)PHOTON_OFF_FX);
        currState = OFF;
      }
      break;
   case FIRING_1:
      if(fireAnime(*animeCnt,leds[0]))
        currState = PHOTON_IDLE_2;
      *animeCnt = *animeCnt + delta;
      break;
   case PHOTON_IDLE_2:
      leds[0] = PHOTON_IDLE_COLOR;
      leds[1] = PHOTON_IDLE_COLOR;
      *animeCnt = 0;
      if(firePressed)
      {
        sfx.playTrack((uint8_t)PHOTON_FIRE2_FX);
        currState = FIRING_2;
      }
      if(btnPressed)
      {
        sfx.playTrack((uint8_t)PHOTON_OFF_FX);
        currState = OFF;
      }    
      break;
    case FIRING_2:
      if(fireAnime(*animeCnt, leds[1]))
        currState = PHOTON_IDLE_1;
      *animeCnt = *animeCnt + delta;
      break;
  }
  return currState; 
}

bool fireAnime(uint16_t animeCnt, CRGB &led)
{
  if(animeCnt <= 60) { led = CRGB::White; return false;}
  else if(animeCnt <= 220) { led = PHOTON_FIRE_COLOR; return false;}
  else if(animeCnt <= 900) {  led = CRGB::Black; return false;} 
  else
    return true;
}

void updatePhaserState(uint32_t delta)
{
 switch(_phaserState)
 {
  case OFF: clearPhaser(0,PHASER_LED_CNT,1); 
          if(_phaserPortBtn.isPressed())
          {
            sfx.playTrack((uint8_t)PHASER_ON_FX);
            setPhaserBlastLoc(PHASER_PORT);
            _blastCnt = 0;
            _phaserState = PHASER_MOVING;
          }
          else if(_phaserStarBtn.isPressed())
          {
            sfx.playTrack((uint8_t)PHASER_ON_FX);
            setPhaserBlastLoc(PHASER_STAR);
            _blastCnt = 0;
            _phaserState = PHASER_MOVING;
          }
          break;
  case PHASER_MOVING:
    _blastCnt = _blastCnt + delta;
    if(phaserTrails(_blastCnt))
    {
      _blastCnt = 0;
      _phaserState = PHASER_FIRE;
    }
    break;
  case PHASER_FIRE:
    _blastCnt = _blastCnt + delta;
    if(phaserBlastAnime(_blastCnt,_phaserFireIndex,PHASER_FIRE_SIZE))
    {
      _blastCnt = 0;
      _phaserState = OFF;          
    }
    break;
  case ON:
    colorPhaser(PHASER_PORT_START,PHASER_LEN,1,CRGB::Red);
    colorPhaser(PHASER_STAR_START,PHASER_LEN,1,CRGB::Blue);
    if(_phaserPortBtn.isPressed())
      _phaserState = OFF;
    break;
 }
}

void setPhaserBlastLoc(int8_t side)
{
  int padding = PHASER_LEN *.2;
 
  if(side==PHASER_STAR)
   _phaserStart = PHASER_STAR_START;
  else
    _phaserStart = PHASER_PORT_START;
  _phaserRevStart = _phaserStart + PHASER_LEN-1;
  _phaserFireIndex = random(_phaserStart+padding, _phaserStart+PHASER_LEN-padding);
  _phaserSide = side;
}

bool phaserTrails(uint32_t delta)
{
  if(delta <= PHASER_MOVE_TIME)
  {
    int8_t trailLen = _phaserFireIndex - _phaserStart;
    phaserMoveAnime(_phaserStart,trailLen, true, delta);
    phaserMoveAnime(_phaserRevStart,PHASER_LEN-trailLen, false, delta);
    return false;
  }
  else
  {
    clearPhaser(_phaserStart,PHASER_LEN,1);
    return true;
  }
}

void phaserMoveAnime(int8_t startPt,int8_t trailLen, bool forward, uint32_t moveCnt)
{
  int8_t backIndex = 0;
  int8_t currPt = startPt;
  int8_t dir = 1;
  if(!forward)
    dir = -1;

  currPt = map(moveCnt, 0, PHASER_MOVE_TIME,startPt,startPt+(trailLen*dir));
  _phasers[currPt] = PHASER_MOVE_COLOR;
  backIndex = currPt - dir;
  while( (forward && (backIndex >=startPt)) || (!forward && (backIndex <= startPt)))
  {
    _phasers[backIndex].fadeToBlackBy(20);
    backIndex = backIndex - dir;
  }
}

//returns true when animation sequence is complete
bool phaserBlastAnime(uint32_t blastCnt, int8_t startIndex, int8_t len)
{
  if(blastCnt <= 100)
  {
    int8_t amt = map(blastCnt,0,100,0,255);
    for(int8_t i=0; i<len;i++)
      _phasers[startIndex+i] = CRGB(amt,amt,amt);
    return false;
  } 
  else if(blastCnt < PHASER_BLAST_TIME)
  {
    int8_t state = map(blastCnt,0,PHASER_BLAST_TIME,0,30);
    for(int8_t i=0; i<len; i++)
    {
      if(state%2)
        _phasers[startIndex+i]= PHASER_FIRE_COLOR;
       else
        _phasers[startIndex+i].fadeLightBy(200);
    }
    return false;
  }
  else
  {
    return true;
  }
}

void colorPhaser(int8_t startIndex, int8_t len, int8_t dir, const CRGB &color)
{
  for(int8_t i=0; i < len; i++)
    _phasers[startIndex+(i*dir)] = color;
}

void clearPhaser(int8_t startIndex, int8_t len, int8_t dir)
{
  for(int8_t i=0; i < len; i++)
    _phasers[startIndex+(i*dir)] = CRGB(0,0,0);
}
