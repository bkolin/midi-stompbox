# midi-stompbox
This repo contains the code needed to build an Arduino 32U4-based MIDI controller. This should work out of the box with the Arduino Leonardo and the Arduino Pro Micro and clones.

In its current form, the code suppports up to 6 momentary-contact switches, which will be assigned to chromatic notes starting at MIDI 48 (C).

It is simple to add support for more switches and more involved to add support for dynamic force (key is hit softly vs harder) as well.

This controller can be used to produce notes according to whatever mapping you configure in your DAW (digital audio workstation), or to send MIDI signals to any other MIDI-input-capable software, including OBS. MIDI has the advantage of not being constrained by which application currently has the focus.

MIDIStompbox depends on the MIDIUSB library.
