#include "soundFx.h"

SoundFx::SoundFx(Stream *serial, int8_t reset):stream(serial), reset_pin(reset)
{
  stream->setTimeout(500);
}

int SoundFx::readLine(void) {
  int x = stream->readBytesUntil('\n', line_buffer, LINE_BUFFER_SIZE);
  line_buffer[x] = 0;

  if (stream->peek() == '\r')
    stream->read();
  return x;
}

boolean SoundFx::reset(void) {
  digitalWrite(reset_pin, LOW);
  pinMode(reset_pin, OUTPUT);
  delay(10);
  pinMode(reset_pin, INPUT);
  delay(1000); // give a bit of time to 'boot up'
  // eat new line
  readLine();
  readLine();
  if (!strstr(line_buffer, "Adafruit FX Sound Board"))
    return false;
  delay(250);
  readLine();
  readLine();
  return true;
}

boolean SoundFx::playTrack(uint8_t n) {
  while (stream->available())
    stream->read();

  stream->print(F("#"));
  stream->println(n);

  readLine(); // eat return
  readLine();

  // check we got "play" back
  if (strstr(line_buffer, "play") == 0) {
    return false;
  }
  // check the # is correct
  int playing = atoi(line_buffer + 5);

  if (n != playing)
    return false;
  return true;
}

uint8_t SoundFx::volUp() {
  while (stream->available())
    stream->read();

  stream->println(F("+"));
  readLine();
  uint8_t v = atoi(line_buffer);
  return v;
}

boolean SoundFx::stop() {
  while (stream->available())
    stream->read();

  stream->print(F("q"));
  readLine();

  if (line_buffer[0] != 'q')
    return false;
  return true;
}
