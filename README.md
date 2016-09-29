# About

The Sketches repository contains a collection of my sketches for the Arduino, Teensy
and ESP8266 platform.

Each directory contains its corresponding README file which describes the
specific requirements etc.

In order not to conflict with other license models, third party libraries are not
included into these projects but links to them (for downloading) are be provided.
For some third party libraries patch instructions are given, if applicable.

# Sub-projects

## Libraries

A collection of my own libraries. They have to be copied/symlinked to the
Arduino libraries directory.

## Memory/FlashReader

Arduino based device which can read EPROMs and EEPROMs in 24 .. 32 pin DIP cases.

## Transverter

Control software to steer two rigs, one for transmitting in the 28 MHz band 
and one for receiving in the 70 MHz band. Both rigs are synchronized via the 
CAT protocol. 

The folder contains a separate README file.

## TreibBoje

Some experimental software running on a swimming beacon (buoy). It uses a
cheap 433 MHz transmitter and send its data via Morse signs (CW).

In an earlier implementation the rc-switch code was used and the received 
packets where decoded on a Raspberry but the achieved distance for error-free 
decoding was too low.
