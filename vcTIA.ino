#include "definitions.h"

int last_wf = 0;
int last_pitch = 0;

void setup()
{
  pinMode(TIA_LATCH_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  Serial.begin(115200);
  Serial.flush();

  setPitch(2, 10);
  setVolume(2, 2);
  
}


void loop()
{

  int wf_read = analogRead(A0);
  int wf = map(wf_read, 0, 1024, 0, 16);

  int pitch_read = analogRead(A1);
  int pitch = map(pitch_read, 0, 1024, 0, 32);
  
  if (last_wf != wf) {
    last_wf = wf;
    setWaveform(2, wf);
  }

  if (last_pitch != pitch) {
    last_pitch = pitch;
    setPitch(2, pitch);
  }
//  Serial.println("foo");
//  delay(100);
  
//  setVolume(2, 3);   // 0-15
//  setPitch(2, 30);   // 0-31
//  setWaveform(0, 4); // 0-15
//  setWaveform(1, 7); // 0-15
////  delay(1000);

//  setVolume(2, 3);
//  setWaveform(2, 0);
//  for (int wf = 0; wf < 16; wf++) {
//    Serial.print("Waveform ");
//    Serial.print(wf);
//    Serial.print("  pitch ");
//    setWaveform(2, wf);
//    for (int pt = 0; pt < 32; pt++) {
//      Serial.print(pt);
//      Serial.print(".");
//      setPitch(2, pt);
//      delay(100);
//    }
//    Serial.println();
//  }
//  
//  setWaveform(2, 0);
//  setPitch(2, 10);
//  for (int wf = 0; wf < 16; wf++) {
//    setWaveform(2, wf);
//    delay(100);
//  }
//  setWaveform(2, 0);
//  
//  delay(1000000);

  //  updateShiftRegister(0b00000000, 0b11000111);
  //  delay(100000);
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
  delay(10);
  digitalWrite(TIA_LATCH_PIN, LOW);
//  delay(DELAY);

}

// Volume goes from 0b0000 = 0 to 0b1111 = 15
void setVolume(int voice, byte volume)
{
  if (voice == 0 || voice == 2) {
    updateShiftRegister(volume, AVOL0);
    latchTIA();
  }

  if (voice == 1 || voice == 2) {
    updateShiftRegister(volume, AVOL1);
    latchTIA();
  }

}

// divisor divides 30kHz and goes from 1 (30kHz) to 32 (937.5 Hz)
void setPitch(int voice, int divisor)
{
  if (voice == 0 || voice == 2) {
    updateShiftRegister(divisor, AFREQ0);
    latchTIA();
  }

  if (voice == 1 || voice == 2) {
    updateShiftRegister(divisor, AFREQ1);
    latchTIA();
  }
}

// waveform goes from 0b0000 = 0 to 0b1111 = 15
void setWaveform(int voice, int waveform)
{
  if (voice == 0 || voice == 2) {
    updateShiftRegister(waveform, ACTRL0);
    latchTIA();
  }
  delay(1);
  if (voice == 1 || voice == 2) {
    updateShiftRegister(waveform, ACTRL1);
    latchTIA();
  }
}



//void input_and_play_semitone() {
//  static String inData;
//
//  Serial.println("");
//  for (;;) {
//    char received = ' '; // Each character received
//    inData = ""; // Clear recieved buffer
//    Serial.print("DAC value (0-4095): ");
//
//    while (received != '\n') { // When new line character is received (\n = LF, \r = CR)
//      if (Serial.available() > 0) // When character in serial buffer read it
//      {
//        received = Serial.read();
//        Serial.print(received); // Echo each received character, terminal dont need local echo
//        inData += received; // Add received character to inData buffer
//      }
//    }
//    inData.trim(); // Eliminate \n, \r, blank and other not “printable”
//    Serial.println();
//    MCP.setValue(inData.toInt());
//
//  }
//
//}
