/*
  MIDIStompbox
  
  The MIDIStompbox is an Arduino 32U4 sketch that connects simple switches
  to a computer via USB, appearing to the computer as a MIDI device. The resulting
  device can be used as a controller, an instrument, or any other use case
  where having a MIDI input is valuable.

  Interface functions are based on the example code in the MIDIUSB library.
  
*/

#include <MIDIUSB.h>
#include <MIDIUSB_Defs.h>
#include <frequencyToNote.h>
#include <pitchToFrequency.h>
#include <pitchToNote.h>

// Change this if more switches are needed.
const int SWITCH_PINS[] = {2, 3, 4, 5, 6, 7};

// NOTE: Switches are normally open, and tied from the specified pin to ground.
//       Internal pullup resistors are used so digitalRead(pin) == LOW means the
//       switch is closed.
const int SWITCH_OPEN   = HIGH;
const int SWITCH_CLOSED = LOW;

const int MIDI_BASE_NOTE = 48; // Middle C


// ----------------

// The Arduino Pro Micro has no LED_BUILTIN but does provide programmatic access to the rx led.
// Adjust as needed for other Arduino variants.
const int STATUS_LED = 17;

int numSwitches; // Calculated later.

// This is arbitrary. Choose a lower number to be able to send keys faster or a higher number to debounce noisy switches.
long sendDelay = 100; 

void setup() {
  numSwitches = sizeof SWITCH_PINS/sizeof SWITCH_PINS[0];
  
  for (int i = 0; i < numSwitches; i++) {
    int switchPin = SWITCH_PINS[i];
    pinMode(switchPin, INPUT_PULLUP);
  }
  
  pinMode(STATUS_LED, OUTPUT);
}

void loop() {
  for (int i = 0; i < numSwitches; i++) {
    int switchPin = SWITCH_PINS[i];
    if (switchClosed(i)) {
      sendMIDINote(i);
      break; // Only send one note.
    }
  }
}

void sendMIDINote(int i) {
  int note = MIDI_BASE_NOTE + i;
  digitalWrite(STATUS_LED, HIGH);
//  Serial.print("Sending NOTE_ON: ");
//  Serial.println(note);
  noteOn(0, note, 64);   // Channel 0, specified note, normal velocity
  MidiUSB.flush();
  delay(sendDelay);

  digitalWrite(STATUS_LED, LOW);
//  Serial.print("Sending NOTE_OFF: ");
//  Serial.println(note);
  noteOff(0, note, 64);
  MidiUSB.flush();
  delay(sendDelay);

}

boolean switchClosed(int switchNum) {  
  if (switchNum >= numSwitches) {
    //Serial.println("Detected attempt to check a nonexistent switch: " + switchNum);
    return false;
  }
  if (digitalRead(SWITCH_PINS[switchNum]) == SWITCH_CLOSED) {
    return true;
  }
  return false;
}

// -------------------------------------------------------------------
// First parameter is the event type (0x09 = note on, 0x08 = note off).
// Second parameter is note-on/note-off, combined with the channel.
// Channel can be anything between 0-15. Typically reported to the user as 1-16.
// Third parameter is the note number (48 = middle C).
// Fourth parameter is the velocity (64 = normal, 127 = fastest).

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}


// First parameter is the event type (0x0B = control change).
// Second parameter is the event type, combined with the channel.
// Third parameter is the control number number (0-119).
// Fourth parameter is the control value (0-127).

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
