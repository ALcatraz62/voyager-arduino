#ifndef _SOUNDFX_H_
#define _SOUNDFX_H_

#include "Arduino.h"

#define LINE_BUFFER_SIZE 50 

class SoundFx
{
  private:
    char line_buffer[LINE_BUFFER_SIZE];
    Stream *stream; //serial stream
    int8_t reset_pin;

  public:
    SoundFx(Stream *serial,int8_t resetPin);
    bool reset(void);
    uint8_t volUp(void);
    bool playTrack(uint8_t id);
    bool stop(void);
    int readLine(void);
    
};
#endif
