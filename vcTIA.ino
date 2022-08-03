#include "definitions.h"

int last_wf = -1;
int last_pitch = -1;

void setup()
{
  pinMode(TIA_LATCH_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  pinMode(WAVEFORM_POT, INPUT);
  pinMode(PITCH_POT, INPUT);

  Serial.begin(115200);
  Serial.flush();

  setWaveform(2, 0);
  setPitch(2, 0);
  setVolume(2, 2);

//  demo();
}

void loop()
{

  int wf_read = analogRead(WAVEFORM_POT);
  int wf = map(wf_read, 0, 1024, 0, 16);

  int pitch_read = analogRead(PITCH_POT);
  int pitch = map(pitch_read, 0, 1024, 0, 32);

  if (last_wf != wf) {
    last_wf = wf;
    setWaveform(2, wf);
    Serial.print("Waveform "); Serial.println(wf);
  }

  if (last_pitch != pitch) {
    last_pitch = pitch;
    setPitch(2, pitch);
    Serial.print("Pitch "); Serial.println(30000/(pitch + 1));
  }

}

void demo()
{
  setVolume(2, 2);
  setWaveform(2, 0);
  for (int wf = 0; wf < 16; wf++) {
    Serial.print("Waveform ");
    Serial.print(wf);
    Serial.print("  pitch ");
    setWaveform(2, wf);
    for (int pt = 0; pt < 32; pt++) {
      Serial.print(pt);
      Serial.print(".");
      setPitch(2, pt);
      delay(100);
    }
    Serial.println();
  }
  setWaveform(2, 0);
}

/*
   updateShiftRegister() - This function sets the latchPin to low, then calls the Arduino function 'shiftOut' to shift out contents of variable 'leds' in the shift register before putting the 'latchPin' high again.
*/
void updateShiftRegister(byte data_byte, byte address_byte)
{
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, address_byte);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, data_byte);
  digitalWrite(LATCH_PIN, HIGH);
}

void latchTIA()
{
  digitalWrite(TIA_LATCH_PIN, LOW);
  digitalWrite(TIA_LATCH_PIN, HIGH);
  delay(1);
  digitalWrite(TIA_LATCH_PIN, LOW);
}

// Volume goes from 0b0000 = 0 to 0b1111 = 15
void setVolume(int voice, byte volume)
{
  if (voice == 0 || voice == 2) {
    updateShiftRegister(volume, ADDR_VOL0);
    latchTIA();
  }

  if (voice == 1 || voice == 2) {
    updateShiftRegister(volume, ADDR_VOL1);
    latchTIA();
  }

}

// divisor divides 30kHz and goes from 1 (30kHz) to 32 (937.5 Hz)
void setPitch(int voice, int divisor)
{
  if (voice == 0 || voice == 2) {
    updateShiftRegister(divisor, ADDR_FREQ0);
    latchTIA();
  }

  if (voice == 1 || voice == 2) {
    updateShiftRegister(divisor, ADDR_FREQ1);
    latchTIA();
  }
}

// waveform goes from 0b0000 = 0 to 0b1111 = 15
void setWaveform(int voice, int waveform)
{
  if (voice == 0 || voice == 2) {
    updateShiftRegister(waveform, ADDR_CTRL0);
    latchTIA();
  }

  if (voice == 1 || voice == 2) {
    updateShiftRegister(waveform, ADDR_CTRL1);
    latchTIA();
  }
}
