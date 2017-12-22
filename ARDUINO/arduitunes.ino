/* This program uses timer 2 to reproduce simultaneous tones, and timer 1 to play songs in tempo.
 * Each melody is specified as a set of characters that must follow a RTTTL-like protocol.
 *
 * Circuit setup:
 *  - Pins 2, 3, 4, 5 and 6 passively mixed and connected toa speaker, or each one connected to a buzzer. Don't forget
 *  resistors to constrain voltages to the 3.5mm jack standard (around 1.1V PP)
 *  - 2 leds on pins 11 and 12, each with current limiting resistors
 *  - Potentiometer in analog input A0
 *
 *  Optional:
 *  - Volume potentiometer between mixer and audio output
 */

#include <Arduino.h>

/* Tempo, recommended: between 30 and 400*/
word tempo=200;

char parseNota(String);
void update();


/* Melody declaration
 *
 * Beware:
 * - No error tolerance
 * - Notation:
 *    Each tone is written as ABC, where:
 *      + A in {1, 2, 4, 8, 16, 32} represents the denominator of a fraction of a whole note (semibreve)
 *      + B in {C, CS, D, DS, E, F, FS, G, GS, A, AS, B, S} represents a musical note in american notation. The S suffix
 *      indicates a sharp. The single S represents a silence
 *      + C in {1, ..., 8} represents the note's octave. As a reference A4 (A tone, 4th octave) corresponds to a 440 Hz tone
 *      This part is omitted for silences
 *    Repetitions are supported by using the ||: and :|| strings. They must open and close correctly. Nesting is not supported.
 *    Each voice must end with an X
 *
 *    Example: You can make it play a C major arpeggio two times with something like "||: 4C4 4E4 4G4 4C5 :|| X"
 *
 *    These rules are the same for each voice

/*######################### PRESET MELODIES #########################*/

/**************** SUPER MARIO*****************/

//Ideal tempo: 200

const char voz1[] PROGMEM={"8E5 8E5 8S 8E5 8S 8C5 8E5 8S 8G5 8S 4S 8G4 4S 8S ||: 8C5 4S 8G4 4S 8E4 4S 8A4 8S 8B4 8S 8AS4 8A4 8S 8G4 8E5 8S 8G5 8A5 8S 8F5 8G5 8S 8E5 8S 8C5 8D5 8B4 4S :|| "
                          "||: 4S 8G5 8FS5 8F5 8DS5 8S 8E5 8S 8GS4 8A4 8C5 8C5 8A4 8C5 8D5 4S 8G5 8FS5 8F5 8DS5 8S 8E5 8S 8C6 8S 8C6 8C6 8S 2S 8G5 8FS5 8F5 8DS5 8S 8E5 8S 8GS4 8A4 8C5 8C5 8A4 8C5 8D5 4S 8DS5 4S 8D5 4S 8C5 4S 8S 2S :|| "
                          "8C5 8C5 8S 8C5 8S 8C5 8D5 8S 8E5 8C5 8S 8A4 8G4 8S 4S 8C5 8C5 8S 8C5 8S 8C5 8D5 8E5 2S 2S 8C5 8C5 8S 8C5 8S 8C5 8D5 8S 8E5 8C5 8S 8A4 8G4 8S 4S 8E5 8E5 8S 8E5 8S 8C5 8E5 8S 8G5 8S 4S 8G4 8S 4S "
                          "||: 8C5 4S 8G4 4S 8E4 4S 8A4 8S 8B4 8S 8AS4 8A4 8S 8G4 8E5 8S 8G5 8A5 8S 8F5 8G5 8S 8E5 8S 8C5 8D5 8B4 4S :|| "
                          "||: 8E5 8C5 8S 8G4 4S 8GS4 8S 8A4 8F5 8S 8F5 8A4 8S 4S 8B4 8A5 8S 8A5 8A5 8G5 8S 8F5 8E5 8C5 8S 8A4 8G4 8S 4S 8E5 8C5 8S 8G4 4S 8GS4 8S 8A4 8F5 8S 8F5 8A4 8S 4S 8B4 8F5 8S 8F5 8F5 8E5 8S 8D5 8C5 8S 4S 2S :|| "
                          "8C5 8C5 8S 8C5 8S 8C5 8D5 8S 8E5 8C5 8S 8A4 8G4 8S 4S 8C5 8C5 8S 8C5 8S 8C5 8D5 8E5 2S 2S 8C5 8C5 8S 8C5 8S 8C5 8D5 8S 8E5 8C5 8S 8A4 8G4 8S 4S 8E5 8E5 8S 8E5 8S 8C5 8E5 8S 8G5 8S 4S 8G4 8S 4S "
                          "8E5 8C5 8S 8G4 4S 8GS4 8S 8A4 8F5 8S 8F5 8A4 8S 4S 8B4 8A5 8S 8A5 8A5 8G5 8S 8F5 8E5 8C5 8S 8A4 8G4 8S 4S 8E5 8C5 8S 8G4 4S 8GS4 8S 8A4 8F5 8S 8F5 8A4 8S 4S 8B4 8F5 8S 8F5 8F5 8E5 8S 8D5 8C5 8S 4S 2S "
                          "8C5 4S 8G4 4S 8E4 8S 8A4 8B4 8A4 4GS4 4AS4 4GS4 1G4 X"};
const char voz2[] PROGMEM={"8FS4 8FS4 8S 8FS4 8S 8FS4 8FS4 8S 8D5 8S 4S 8B3 4S 8S ||: 8G4 4S 8E4 4S 8C4 4S 8F4 8S 8G4 8S 8FS4 8F4 8S 8E4 8C5 8S 8E5 8F5 8S 8D5 8E5 8S 8C5 8S 8A4 8G4 8G4 4S :|| "
                          "||: 4S 8E5 8DS5 8D5 8B4 8S 8C5 8S 8E4 8F4 8G4 8S 8C4 8E4 8F4 4S 8E5 8DS5 8D5 8B4 8S 8C5 8S 8G5 8S 8G5 8G5 8S 2S 8E5 8DS5 8D5 8B4 8S 8C5 8S 8E4 8F4 8G4 8S 8C4 8E4 8F4 4S 8GS4 4S 8F4 4S 8E4 4S 8S 2S :|| "
                          "8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8S 8G4 8E4 8S 8E4 8C4 8S 4S 8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8G4 2S 2S 8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8S 8G4 8E4 8S 8E4 8C4 8S 4S 8FS4 8FS4 8S 8FS4 8S 8FS4 8FS4 8S 8D5 4S 8S 8D4 8S 4S "
                          "||: 8G4 4S 8E4 4S 8C4 4S 8F4 8S 8G4 8S 8FS4 8F4 8S 8E4 8C5 8S 8E5 8F5 8S 8D5 8E5 8S 8D5 8S 8A4 8G4 8D4 4S :|| "
                          "||: 8C5 8G4 8S 8E4 4S 8E4 8S 8F4 8C5 8S 8C5 8F4 8S 4S 8G4 8F5 8S 8F5 8F5 8E5 8S 8D5 8C5 8F4 8S 8F4 8E4 8S 4S 8C5 8G4 8S 8E4 4S 8E4 8S 8F4 8C5 8S 8C5 8F4 8S 4S 8G4 8D5 8S 8D5 8D5 8C5 8S 8B4 8G4 8E4 8S 8E4 8C4 8S 4S :|| "
                          "8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8S 8G4 8E4 8S 8E4 8C4 8S 4S 8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8G4 2S 2S 8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8S 8G4 8E4 8S 8E4 8C4 8S 4S 8FS4 8FS4 8S 8FS4 8S 8FS4 8FS4 8S 8D5 4S 8S 8D4 8S 4S "
                          "8C5 8G4 8S 8E4 4S 8E4 8S 8F4 8C5 8S 8C5 8F4 8S 4S 8G4 8F5 8S 8F5 8F5 8E5 8S 8D5 8C5 8F4 8S 8F4 8E4 8S 4S 8C5 8G4 8S 8E4 4S 8E4 8S 8F4 8C5 8S 8C5 8F4 8S 4S 8G4 8D5 8S 8D5 8D5 8C5 8S 8B4 8G4 8E4 8S 8E4 8C4 8S 4S "
                          "8E4 4S 8C4 4S 8G3 8S 8F4 4F4 4F4 2F4 8E4 8D4 2E4 4E4 X"};
const char voz3[] PROGMEM={"8D3 8D3 8S 8D3 8S 8D3 8D3 8S 8B4 8S 4S 8G3 4S 8S ||: 8E4 4S 8C4 4S 8G3 4S 8C4 8S 8D4 8S 8CS4 8C4 8S 8C4 8G4 8S 8B4 8C5 8S 8A4 8B4 8S 8A4 8S 8E4 8F4 8D4 4S :|| "
                          "||: 8C3 4S 8G3 4S 8C4 8S 8F3 4S 8C4 8C4 8C4 8F3 8S 8C3 4S 8E3 4S 8G3 8C4 8S 8F5 8S 8F5 8F5 8S 8G3 8S 8C3 4S 8G3 4S 8C4 8S 8F3 4S 8C4 8C4 8C4 8F3 8S 8C3 8S 8GS3 4S 8AS3 4S 8C4 4S 8G3 8G3 8S 8C3 8S :|| "
                          "8GS2 4S 8DS3 4S 8GS3 8S 8G3 4S 8C3 4S 8G2 8S 8GS2 4S 8DS3 4S 8GS3 8S 8G3 4S 8C3 4S 8G2 8S 8GS2 4S 8DS3 4S 8GS3 8S 8G3 4S 8C3 4S 8G2 8S 8D3 8D3 8S 8D3 8S 8D3 8D3 8S 8B4 4S 8S 8G3 8S 4S "
                          "||: 8E4 4S 8C4 4S 8G3 4S 8C4 8S 8D4 8S 8CS4 8C4 8S 8C4 8G4 8S 8B4 8C5 8S 8A4 8B4 8S 8A4 8S 8E4 8F4 8D4 4S :|| "
                          "||: 8C3 4S 8FS3 8G3 8S 8C4 8S 8F3 8S 8F3 8S 8C4 8C4 8F3 8S 8D3 4S 8F3 8G3 8S 8A3 8S 8G3 8S 8G3 8S 8C4 8C4 8G3 8S 8C3 4S 8FS3 8G3 8S 8C4 8S 8F3 8S 8F3 8S 8C4 8C4 8F3 8S 8G3 4S 8G3 8G3 8A3 8S 8B3 8C4 8S 8G3 8S 8C3 8S 4S :|| "
                          "8GS2 4S 8DS3 4S 8GS3 8S 8G3 4S 8C3 4S 8G2 8S 8GS2 4S 8DS3 4S 8GS3 8S 8G3 4S 8C3 4S 8G2 8S 8GS2 4S 8DS3 4S 8GS3 8S 8G3 4S 8C3 4S 8G2 8S 8D3 8D3 8S 8D3 8S 8D3 8D3 8S 8B4 4S 8S 8G3 8S 4S "
                          "8C3 4S 8FS3 8G3 8S 8C4 8S 8F3 8S 8F3 8S 8C4 8C4 8F3 8S 8D3 4S 8F3 8G3 8S 8A3 8S 8G3 8S 8G3 8S 8C4 8C4 8G3 8S 8C3 4S 8FS3 8G3 8S 8C4 8S 8F3 8S 8F3 8S 8C4 8C4 8F3 8S 8G3 4S 8G3 8G3 8A3 8S 8B3 8C4 8S 8G3 8S 8C3 8S 4S "
                          "8G3 4S 8E3 4S 8C3 8S 8F3 4F3 4CS3 2CS3 1C3 X"};
const char voz4[] PROGMEM={"X"};
const char voz5[] PROGMEM={"X"};


/******************** POKEMON BATTLE MUSIC*************************/
//ideal tempo: 180
/*
const char voz1[] PROGMEM={
"16A5 16GS5 16G5 16FS5 16A5 16F5 16FS5 16F5 16A5 16E5 16F5 16E5 16A5 16DS5 16E5 16DS5 16A5 16D5 16DS5 16D5 16A5 16CS5 16D5 16CS5 16A5 16C5 16CS5 16C5 16A5 16B4 16C5 16B4 8B5 8S 4S 2S 1S 8B5 8S 4S "
"2S 2S 4S 8A5 8S 8B4 4S 8CS5 4S 8D5 8S 8B4 8CS5 8S 8D5 4S 8A5 8AS5 8B5 4S 8CS6 4S 8D6 8S 8B5 8CS6 8S 8D6 4S 8A5 8S 4B4 8B4 2FS4 8S 4S 4B4 4FS4 4B4 1C5 "
"1S 4B4 8B4 2FS4 8S 4S 4B4 4FS4 4B4 1A4 1S 1G4 2D5 2G4 1A4 1S 1G4 2E5 2FS5 1E5 4G5 8A5 8G5 8FS5 8E5 8D5 8E5 1FS5 1FS5 1G5 4G5 8A5 8G5 8G5 8FS5 8E5 8FS5 1GS5 1GS5 1A5 "
"2CS6 2E6 4D6 4A5 8C6 8B5 1B5 4B5 2B5 4D6 4A5 8AS5 8F6 2F6 4F6 1G6 1E6 2E6 4E6 8E6 8B4 4C5 8C5 8A4 2A4 4S 4C5 4A4 4C5 4AS4 8AS4 2F5 4S 8S 4AS4 4F5 4D5 4C5 8C5 8A4 "
"2A4 4S 4A4 8E5 8D5 8C5 8E5 8D5 4AS4 8F5 2F5 4G5 8G5 4F5 8F5 4D5 1F5 1E5 1D5 1E5 1F6 1E6 1G6 1F6 ||: 8E5 4S 8E5 4S 8E5 8S 8E5 4S 8E5 4S 8E5 8S :|| 4A4 4B4 "
"8G4 8A4 2A4 8B4 8CS5 8E5 8D5 8CS5 8B4 1AS4 8S 8AS4 8C5 8F5 8E5 8D5 8C5 8AS4 1B4 8S 8B4 8CS5 8G5 8FS5 8E5 8D5 8B4 1C5 2E5 2G5 "
"4B4 8B4 2FS4 8S 4S 4B4 4FS4 4B4 1C5 "
"1S 4B4 8B4 2FS4 8S 4S 4B4 4FS4 4B4 1A4 1S 1G4 2D5 2G4 1A4 1S 1G4 2E5 2FS5 1E5 4G5 8A5 8G5 8FS5 8E5 8D5 8E5 1FS5 1FS5 1G5 4G5 8A5 8G5 8G5 8FS5 8E5 8FS5 1GS5 1GS5 1A5 "
"2CS6 2E6 4D6 4A5 8C6 8B5 1B5 4B5 2B5 4D6 4A5 8AS5 8F6 2F6 4F6 1G6 1E6 2E6 4E6 8E6 8B4 4C5 8C5 8A4 2A4 4S 4C5 4A4 4C5 4AS4 8AS4 2F5 4S 8S 4AS4 4F5 4D5 4C5 8C5 8A4 "
"2A4 4S 4A4 8E5 8D5 8C5 8E5 8D5 4AS4 8F5 2F5 4G5 8G5 4F5 8F5 4D5 1F5 1E5 1D5 1E5 1F6 1E6 1G6 1F6 ||: 8E5 4S 8E5 4S 8E5 8S 8E5 4S 8E5 4S 8E5 8S :|| 4A4 4B4 "
"8G4 8A4 2A4 8B4 8CS5 8E5 8D5 8CS5 8B4 1AS4 8S 8AS4 8C5 8F5 8E5 8D5 8C5 8AS4 1B4 8S 8B4 8CS5 8G5 8FS5 8E5 8D5 8B4 1C5 2E5 2G5 1D5 X"};
const char voz2[] PROGMEM={
"2S 16F4 16E4 16F4 16E4 16DS4 16D4 16DS4 16D4 16CS4 16D4 16CS4 16C4 16B3 16C4 16B3 16AS3 16B3 16AS3 16A3 16AS3 16A3 16GS3 16A3 16GS3 ||: 8D4 4S 8E4 4S 8F4 8S 8D4 8E4 8S 8F4 4S 8C4 8S 8D4 "
"4S 8E4 4S 8F4 8S 8D4 8E4 8S 8F4 4S 8C4 8CS4 :|| 8D4 4E4 8CS4 4D4 4B3 4FS3 8E4 4D4 8CS4 8D4 8E4 2F4 16C4 16CS4 16D4 16DS4 16E4 16DS4 16D4 16C4 16C4 16CS4 16D4 16DS4 16E4 16F4 16FS4 "
"16G4 16GS4 16G4 16FS4 16F4 16E4 16DS4 16D4 16CS4 8D4 4E4 8CS4 4D4 4B3 4FS3 8E4 4D4 8CS4 8B3 8CS4 2D4 2CS4 2B3 2CS4 8D4 8CS4 8B3 8A3 8G3 8S 8D4 8CS4 8B3 8A3 8G3 8S 8D4 8CS4 8B3 8CS4 "
"4E4 16G3 16A3 16B3 16CS4 4D4 16FS3 16G3 16A3 16B3 4CS4 16G3 16A3 16B3 16CS4 4D4 16FS3 16G3 16A3 16B3 8D4 8CS4 8B3 8A3 8G3 8S 8D4 8CS4 8B3 8A3 8B3 8G3 8D4 8E4 8FS4 8G4 8A4 8B4 8A4 8G4 "
"8A4 8S 8A4 8B4 8A4 8G4 8FS4 8G4 8A4 8E4 8G4 8FS4 16B3 16C4 16CS4 16D4 16DS4 16E4 16F4 16FS4 16G4 16GS4 16A4 16AS4 16B4 16C5 16CS5 16D5 16DS5 16D5 16CS5 16C5 16B4 16AS4 16A4 16GS4 16G4 16FS4 16F4 16E4 "
"16DS4 16D4 16CS4 16C4 16C4 16CS4 16D4 16DS4 16E4 16F4 16FS4 16G4 16GS4 16A4 16AS4 16B4 16C5 16CS5 16D5 16DS5 16E5 16DS5 16D5 16CS5 16C5 16B4 16AS4 16A4 16GS4 16G4 16FS4 16F4 16E4 16DS4 16D4 16CS4 16CS4 16D4 16DS4 16E4 "
"16F4 16FS4 16G4 16GS4 16A4 16AS4 16B4 16C5 16CS5 16D5 16DS5 16E5 16F5 16E5 16DS5 16D5 16CS5 16C5 16B4 16AS4 16A4 16GS4 16G4 16FS4 16F4 16E4 16DS4 16D4 16D4 16DS4 16E4 16F4 16FS4 16G4 16GS4 16A4 16AS4 16B4 16C5 16CS5 "
"16D5 16DS5 16E5 16F5 16FS5 16F5 16E5 16DS5 16D5 16CS5 16C5 16B4 16AS4 16B4 16C5 16CS5 16D5 16DS5 16E5 16F5 4A4 4D5 8A4 16D4 16E4 16F4 16G4 16A4 16AS4 8B4 8C5 8A4 8C5 8G4 8B4 8F4 8G4 8A4 8C5 8D5 "
"8S 4A4 4C5 8D5 16F4 16G4 16A4 16AS4 16C5 16D5 8E5 8F5 ||: 8D5 8F5 8D5 8F5 :|| 8D5 8F5 ||: 8E5 8F5 8E5 8F5 8E5 8F5 8E5 8F5 :|| ||: 8C4 8A3 8E4 8C4 8A3 8E4 :|| 8C4 8A3 "
"8C4 8E4 ||: 8D4 8AS3 8F4 8D4 8AS3 8F4 :|| 8D4 8AS3 8D4 8F4 ||: 8C4 8A3 8E4 8C4 8A3 8E4 :|| 8C4 8A3 8C4 8E4 ||: 8D4 8AS3 8F4 8D4 8AS3 8F4 :|| 8D4 8AS3 8D4 8F4 1D4 1C4 "
"1AS3 2G3 4G3 16D4 16E4 16G4 16C5 2D5 2F5 2AS4 2C5 2D5 2E5 2F5 2G5 8AS4 4S 8AS4 4S 8AS4 8S 8A4 4S 8A4 4S 8A4 8S 8C5 4S 8C5 4S 8C5 8S 8AS4 4S 8AS4 4S 8A4 8S 8C4 "
"4D4 8B3 4CS4 8S 16CS4 16D4 16E4 16S 16D4 16S 16B3 16S 16CS4 16S 2S 16D4 16DS4 16E4 16F4 16F4 16E4 16DS4 16D4 16CS4 16S 4S 16CS4 16D4 16E4 16S 16F4 16S 16E4 16S 16D4 16S 8D4 8E4 8F4 8G4 "
"16DS4 16E4 16F4 16FS4 16FS4 16F4 16E4 16DS4 16D4 16S 2S 8FS4 4E4 8DS4 8E4 8FS4 8GS4 2E4 2FS4 2G4 4C5 16FS4 16G4 16GS4 16A4 "
"8D4 4E4 8CS4 4D4 4B3 4FS3 8E4 4D4 8CS4 8D4 8E4 2F4 16C4 16CS4 16D4 16DS4 16E4 16DS4 16D4 16C4 16C4 16CS4 16D4 16DS4 16E4 16F4 16FS4 "
"16G4 16GS4 16G4 16FS4 16F4 16E4 16DS4 16D4 16CS4 8D4 4E4 8CS4 4D4 4B3 4FS3 8E4 4D4 8CS4 8B3 8CS4 2D4 2CS4 2B3 2CS4 8D4 8CS4 8B3 8A3 8G3 8S 8D4 8CS4 8B3 8A3 8G3 8S 8D4 8CS4 8B3 8CS4 "
"4E4 16G3 16A3 16B3 16CS4 4D4 16FS3 16G3 16A3 16B3 4CS4 16G3 16A3 16B3 16CS4 4D4 16FS3 16G3 16A3 16B3 8D4 8CS4 8B3 8A3 8G3 8S 8D4 8CS4 8B3 8A3 8B3 8G3 8D4 8E4 8FS4 8G4 8A4 8B4 8A4 8G4 "
"8A4 8S 8A4 8B4 8A4 8G4 8FS4 8G4 8A4 8E4 8G4 8FS4 16B3 16C4 16CS4 16D4 16DS4 16E4 16F4 16FS4 16G4 16GS4 16A4 16AS4 16B4 16C5 16CS5 16D5 16DS5 16D5 16CS5 16C5 16B4 16AS4 16A4 16GS4 16G4 16FS4 16F4 16E4 "
"16DS4 16D4 16CS4 16C4 16C4 16CS4 16D4 16DS4 16E4 16F4 16FS4 16G4 16GS4 16A4 16AS4 16B4 16C5 16CS5 16D5 16DS5 16E5 16DS5 16D5 16CS5 16C5 16B4 16AS4 16A4 16GS4 16G4 16FS4 16F4 16E4 16DS4 16D4 16CS4 16CS4 16D4 16DS4 16E4 "
"16F4 16FS4 16G4 16GS4 16A4 16AS4 16B4 16C5 16CS5 16D5 16DS5 16E5 16F5 16E5 16DS5 16D5 16CS5 16C5 16B4 16AS4 16A4 16GS4 16G4 16FS4 16F4 16E4 16DS4 16D4 16D4 16DS4 16E4 16F4 16FS4 16G4 16GS4 16A4 16AS4 16B4 16C5 16CS5 "
"16D5 16DS5 16E5 16F5 16FS5 16F5 16E5 16DS5 16D5 16CS5 16C5 16B4 16AS4 16B4 16C5 16CS5 16D5 16DS5 16E5 16F5 4A4 4D5 8A4 16D4 16E4 16F4 16G4 16A4 16AS4 8B4 8C5 8A4 8C5 8G4 8B4 8F4 8G4 8A4 8C5 8D5 "
"8S 4A4 4C5 8D5 16F4 16G4 16A4 16AS4 16C5 16D5 8E5 8F5 ||: 8D5 8F5 8D5 8F5 :|| 8D5 8F5 ||: 8E5 8F5 8E5 8F5 8E5 8F5 8E5 8F5 :|| ||: 8C4 8A3 8E4 8C4 8A3 8E4 :|| 8C4 8A3 "
"8C4 8E4 ||: 8D4 8AS3 8F4 8D4 8AS3 8F4 :|| 8D4 8AS3 8D4 8F4 ||: 8C4 8A3 8E4 8C4 8A3 8E4 :|| 8C4 8A3 8C4 8E4 ||: 8D4 8AS3 8F4 8D4 8AS3 8F4 :|| 8D4 8AS3 8D4 8F4 1D4 1C4 "
"1AS3 2G3 4G3 16D4 16E4 16G4 16C5 2D5 2F5 2AS4 2C5 2D5 2E5 2F5 2G5 8AS4 4S 8AS4 4S 8AS4 8S 8A4 4S 8A4 4S 8A4 8S 8C5 4S 8C5 4S 8C5 8S 8AS4 4S 8AS4 4S 8A4 8S 8C4 "
"4D4 8B3 4CS4 8S 16CS4 16D4 16E4 16S 16D4 16S 16B3 16S 16CS4 16S 2S 16D4 16DS4 16E4 16F4 16F4 16E4 16DS4 16D4 16CS4 16S 4S 16CS4 16D4 16E4 16S 16F4 16S 16E4 16S 16D4 16S 8D4 8E4 8F4 8G4 "
"16DS4 16E4 16F4 16FS4 16FS4 16F4 16E4 16DS4 16D4 16S 2S 8FS4 4E4 8DS4 8E4 8FS4 8GS4 2E4 2FS4 2G4 4C5 16FS4 16G4 16GS4 16A4 1B4 X"};
const char voz3[] PROGMEM={
"16B3 16AS3 16A3 16GS3 16A3 16GS3 16G3 16GS3 16G3 16FS3 16F3 16FS3 16F3 16E3 16F3 16E3 16DS3 16E3 16DS3 16D3 16CS3 16D3 16CS3 16C3 16B2 16C3 16B2 16AS2 16A2 16GS2 16A2 16AS2 ||: 8B2 8B2 8D3 8E3 8B2 8F3 8E3 "
"8D3 8B2 8B2 8D3 8E3 8B2 8D3 8AS2 8C3 :|| ||: 8B2 8B2 8D3 8E3 8B2 8F3 8E3 8D3 8B2 8B2 8D3 8E3 8B2 8D3 8AS2 8C3 :|| ||: 8B2 8FS3 8B2 8FS3 8B2 8FS3 8B2 8FS3 :|| 8B2 8G3 "
"8C3 8G3 8C3 8G3 8C3 8G3 8C3 8G3 8A3 8G3 8FS3 8E3 8D3 8C3 ||: 8B2 8FS3 8B2 8FS3 8B2 8FS3 :|| 8B2 8FS3 8B2 8F3 ||: 8A2 8E3 8A2 8E3 :|| 8A2 8E3 8D3 8CS3 8D3 8CS3 8A2 8GS2 "
"||: 8G2 8D3 8G2 8D3 8G2 8D3 :|| 8G2 8D3 8G2 8GS2 ||: 8A2 8E3 8A2 8E3 8A2 8E3 8A2 8E3 :|| ||: 8G2 8D3 8G2 8D3 8G2 8D3 :|| 8G2 8D3 8G2 8GS2 ||: 8A2 8E3 8A2 8E3 8A2 "
"8E3 :|| 8A2 8E3 8D3 8CS3 8B2 8FS3 8B2 8FS3 8B2 8FS3 8B2 8FS3 8B2 8FS3 8D3 8E3 8FS3 8E3 8D3 8FS3 8C3 8G3 8C3 8G3 8C3 8G3 8C3 8G3 8C3 8G3 8DS3 8F3 8G3 8F3 8DS3 8G3 8CS3 8GS3 "
"8CS3 8GS3 8CS3 8GS3 8CS3 8GS3 8CS3 8GS3 8E3 8FS3 8GS3 8FS3 8E3 8GS3 8D3 8A3 8D3 8A3 8D3 8A3 8D3 8A3 8D3 8A3 8F3 8G3 8A3 8G3 8F3 8E3 4D3 4A2 8C3 8G3 8B2 8G3 8B2 8G3 8B2 8G3 "
"||: 8B2 8G3 8B2 8G3 :|| 4D3 4A2 8AS2 8F3 ||: 8AS2 8F3 8AS2 8F3 8AS2 8F3 :|| 8AS2 8F3 ||: 8B2 8E3 8B2 8E3 8B2 8E3 8B2 8E3 :|| ||: 8A2 8E3 8A2 8E3 8A2 8E3 8A2 8E3 :|| "
"||: 8AS2 8F3 8AS2 8F3 8AS2 8F3 8AS2 8F3 :|| ||: 8A2 8E3 8A2 8E3 8A2 8E3 8A2 8E3 :|| ||: 8AS2 8F3 8AS2 8F3 8AS2 8F3 8AS2 8F3 :|| ||: 4AS2 4F3 4AS2 4F3 4AS2 4E3 4AS2 4E3 4AS2 "
"4D3 4AS2 4D3 4AS2 4E3 4AS2 4E3 :|| ||: 8A2 8E3 8E3 8A2 8E3 8E3 8A2 8E3 8E3 8A2 8E3 8E3 8A2 8E3 8E3 8B2 :|| ||: 8A2 8E3 8A2 8E3 8A2 8E3 8A2 8E3 :|| ||: 8AS2 8F3 8AS2 "
"8F3 8AS2 8F3 8AS2 8F3 :|| ||: 8B2 8FS3 8B2 8FS3 8B2 8FS3 8B2 8FS3 :|| ||: 8C3 8G3 8C3 8G3 :|| 8C3 8G3 8FS3 8E3 8FS3 8E3 8D3 8C3 "
"||: 8B2 8FS3 8B2 8FS3 8B2 8FS3 8B2 8FS3 :|| 8B2 8G3 "
"8C3 8G3 8C3 8G3 8C3 8G3 8C3 8G3 8A3 8G3 8FS3 8E3 8D3 8C3 ||: 8B2 8FS3 8B2 8FS3 8B2 8FS3 :|| 8B2 8FS3 8B2 8F3 ||: 8A2 8E3 8A2 8E3 :|| 8A2 8E3 8D3 8CS3 8D3 8CS3 8A2 8GS2 "
"||: 8G2 8D3 8G2 8D3 8G2 8D3 :|| 8G2 8D3 8G2 8GS2 ||: 8A2 8E3 8A2 8E3 8A2 8E3 8A2 8E3 :|| ||: 8G2 8D3 8G2 8D3 8G2 8D3 :|| 8G2 8D3 8G2 8GS2 ||: 8A2 8E3 8A2 8E3 8A2 "
"8E3 :|| 8A2 8E3 8D3 8CS3 8B2 8FS3 8B2 8FS3 8B2 8FS3 8B2 8FS3 8B2 8FS3 8D3 8E3 8FS3 8E3 8D3 8FS3 8C3 8G3 8C3 8G3 8C3 8G3 8C3 8G3 8C3 8G3 8DS3 8F3 8G3 8F3 8DS3 8G3 8CS3 8GS3 "
"8CS3 8GS3 8CS3 8GS3 8CS3 8GS3 8CS3 8GS3 8E3 8FS3 8GS3 8FS3 8E3 8GS3 8D3 8A3 8D3 8A3 8D3 8A3 8D3 8A3 8D3 8A3 8F3 8G3 8A3 8G3 8F3 8E3 4D3 4A2 8C3 8G3 8B2 8G3 8B2 8G3 8B2 8G3 "
"||: 8B2 8G3 8B2 8G3 :|| 4D3 4A2 8AS2 8F3 ||: 8AS2 8F3 8AS2 8F3 8AS2 8F3 :|| 8AS2 8F3 ||: 8B2 8E3 8B2 8E3 8B2 8E3 8B2 8E3 :|| ||: 8A2 8E3 8A2 8E3 8A2 8E3 8A2 8E3 :|| "
"||: 8AS2 8F3 8AS2 8F3 8AS2 8F3 8AS2 8F3 :|| ||: 8A2 8E3 8A2 8E3 8A2 8E3 8A2 8E3 :|| ||: 8AS2 8F3 8AS2 8F3 8AS2 8F3 8AS2 8F3 :|| ||: 4AS2 4F3 4AS2 4F3 4AS2 4E3 4AS2 4E3 4AS2 "
"4D3 4AS2 4D3 4AS2 4E3 4AS2 4E3 :|| ||: 8A2 8E3 8E3 8A2 8E3 8E3 8A2 8E3 8E3 8A2 8E3 8E3 8A2 8E3 8E3 8B2 :|| ||: 8A2 8E3 8A2 8E3 8A2 8E3 8A2 8E3 :|| ||: 8AS2 8F3 8AS2 "
"8F3 8AS2 8F3 8AS2 8F3 :|| ||: 8B2 8FS3 8B2 8FS3 8B2 8FS3 8B2 8FS3 :|| ||: 8C3 8G3 8C3 8G3 :|| 8C3 8G3 8FS3 8E3 8FS3 8E3 8D3 8C3 1B2 X"};
const char voz4[] PROGMEM={
"X"};
const char voz5[] PROGMEM={
"X"};
*/

/************************* Attack on titan intro 1 ***************************/
/*
const char voz1[] PROGMEM={
"8D5 8D5 8F5 4E5 8C5 8C5 4D5 8D5 8F5 4E5 4C5 8S 4A5 8F5 8G5 8S 8E5 8S 8F5 8S 8D5 8S 8E5 8S 4C5 8S 4A5 8F5 8G5 8S 8E5 8S 8F5 8S 8E5 8S 8D5 8S 4C5 8S "
"4C6 8GS5 8AS5 8S 8G5 8S 8GS5 8S 8F5 8S 8G5 8S 4DS5 8S 4C6 8GS5 8AS5 8S 8G5 8S 8GS5 8S 8G5 8S 8F5 8S 4DS5 8S X"};
const char voz2[] PROGMEM={
"8D4 8D4 8F4 4E4 8C4 8C4 4D4 8D4 8F4 4E4 4C4 8S ||: 8D3 8D4 8D3 8D4 :|| 8D3 8D4 8D3 8D4 8D3 8C4 8C3 8C4 ||: 8AS2 8AS3 8AS2 8AS3 :|| 8AS2 8AS3 8AS2 8AS3 8AS2 8C4 8C3 8C4 "
"||: 8F3 8F4 8F3 8F4 :|| 8F3 8F4 8F3 8F4 8F3 8DS4 8DS3 8DS4 ||: 8CS3 8CS4 8CS3 8CS4 :|| 8CS3 8CS4 8CS3 8CS4 8CS3 8DS4 8DS3 8DS4 X"};
const char voz3[] PROGMEM={
"1S 2S 4S 16A4 16AS4 16B4 16CS5 4D5 8D5 8E5 8S 8C5 8S 8D5 8S 8A4 8S 8C5 8S 4G4 8S 4F5 8D5 8E5 8S 8C5 8S 8D5 8S 8C5 8S 8A4 8S 4G4 8S 4GS5 8F5 8G5 8S 8DS5 "
"8S 8F5 8S 8C5 8S 8DS5 8S 4AS4 8S 4GS5 8F5 8G5 8S 8DS5 8S 8F5 8S 8DS5 8S 8C5 8S 4AS4 8S X"};
const char voz4[] PROGMEM={
"X"};
const char voz5[] PROGMEM={
"X"};
*/

/************************* Fullmetal alchemist intro 1 ***************************/


/*
const char voz1[] PROGMEM={
        "8E5 4B5 8E5 4E5 4D5 8E5 8B5 8E5 8E5 16D5 16E5 8E5 4FS5 16FS5 16FS5 8FS5 8G5 8FS5 8G5 8FS5 8B4 4B4 1S X"};
const char voz2[] PROGMEM={
        "8S 8C3 8S 8S 8C3 8S 8S 8C3 8S 8D3 8S 8S 8D3 8S 8S 8D3 8S 8E3 8S 8S 8E3 8S 8S 8E3 8S 8E3 8S 8S 8E3 8S 8S 8E3 8S X"};
const char voz3[] PROGMEM={
        "8S 8G3 8S 8S 8G3 8S 8S 8G3 8S 8A3 4S 8A3 4S 8A3 8S 8B3 4S 8B3 4S 8B3 8S 8B3 4S 8B3 4S 8B3 8S X"};
const char voz4[] PROGMEM={
        "X"};
const char voz5[] PROGMEM={
        "X"};
*/

/*############################## END PRESET MELODYS ##############################*/


const char* const voces[] PROGMEM={voz1,voz2,voz3,voz4,voz5};

// Active buzzers
#define N_BUZZERS 3

// Voices pins
char buzz[N_BUZZERS];

// Status led pins
char rythmLed=11;
char tempoLed=12;

// Led tempo counter
char tempoLedCounter=0;

// Musical notes frequencies (Hz)
const uint16_t NOTAS[] PROGMEM={31,/*C1*/33,35,37,39,41,44,46,49,52,55,58, //12
                               62,/*C2*/65,69,73,78,82,87,93,98,104,110,117,  //24
                               123,/*C3*/131,139,147,156,165,175,185,196,208,220,233, //36
                               247,/*C4*/262,277,294,311,330,349,370,392,415,440,466,  //48
                               494,/*C5*/523,554,587,622,659,698,740,784,831,880,932,  //60
                               988,/*C6*/1047,1109,1175,1245,1319,1397,1480,1568,1661,1760,1865,  //72
                               1976,/*C7*/2093,2217,2349,2489,2637,2794,2960,3136,3322,3520,3729,  //84
                               3951,/*C8*/4186,4435,4699,4978,0};  //90


int delaytempo=7500/(constrain((int)(tempo),30,400));
// 7500 comes from (60*1000/tempo)/8. The 8 is derived by considering a demisemiquaver as the fundamental time unit (1/8 of a quarter note)

// Used to parse each voice's note sequence
String nota[]={"S", "S", "S", "S", "S"};

// Each voice current note duration
unsigned char duracion[]={0,0,0,0,0};

// Used to bring each note from the PROGMEM. The size is derived from the max string size used by our protocol (ex.: 32CS5)
// Don't forget the trailing \0 char
char buffer[6];

// Voice headers, to track where the last reading was standing at
int head[]={0,0,0,0,0};

// Pointers for repetitions
int rep[]={-1, -1, -1, -1, -1};

// Each voice note's period (ms) (period = 1/frequency)
int NOTAS_P[N_BUZZERS];
// Maximum common divider. Voice notes periods must be a multiple of this
// * Less: More precise notes, higher processing frequency, risks timer callback not finishing at time
// * More: Less precise notes, no risks for timer callback
const char mcd=20; // Jeremy Blum proposes: 64.

// Periods initializer
void initNotas_p(){
  for (char i=0;i<N_BUZZERS;i++){
    int progint2int=pgm_read_word_near(NOTAS + parseNota(nota[i]));
    int periodo=(int)(500000.0/progint2int);  // Actual note period
    NOTAS_P[i]=periodo + ((periodo%mcd)<=(mcd/2) ? -(periodo%mcd) : (mcd-(periodo%mcd))); // mcd-adjusted period
  }
}
// Voices tick counters (mutable)
// using chars instead of ints break silences, don't know why
unsigned int count[N_BUZZERS];

// Counts per note (max is 252, so unsigned char is enough)
// 'counts' are the total period divided by the time the timer counter takes to reset.
// As we want the timer to take 'mcd' us (microseconds), 'counts' is period/mcd.
// These values are constant throughout execution
unsigned char counts[N_BUZZERS];


// 'counts' initializer
void initCounts(){
  for (char i=0;i<N_BUZZERS;i++){
    counts[i]=NOTAS_P[i]/mcd;
    count[i]=0;
  }
}
/**
 * Parses a string representing a tone and returns the tone position in the notes list.
 * Perfect inputs assumed
 *
 * s has two possible shapes:
 *  1. XY
 *  2. XSY
 *
 *
 * @param s: input string
 * @return: int: index to notes list
 */
char parseNota(String s){
  if (s[0]=='S') {
    PORTB &= ~(_BV(rythmLed-8));
    return 89;
  }
  // Remembering ASCII code: A=65, ..., G=71
  PORTB |= _BV(rythmLed-8);
  char pos;
  if (s[1]!='S'){ // Natural note, no sharp
    switch (s[0]){
      case 65: //A
        pos=9;
        break;
      case 66: //B
        pos=11;
        break;
      case 67: //C
        pos=0;
        break;
      case 68: //D
        pos=2;
        break;
      case 69: //E
        pos=4;
        break;
      case 70: //F
        pos=5;
        break;
      case 71: //G
        pos=7;
        break;
    }
  }
  else{
    switch (s[0]){ // Sharp note
      case 65: //A#
        pos=10;
        break;
      // B# doesn't exist
      case 67: //C#
        pos=1;
        break;
      case 68: //D#
        pos=3;
        break;
      // E# doesn't exist
      case 70: //F#
        pos=6;
        break;
      case 71: //G#
        pos=8;
        break;
    }
  }
  // Magic equation that returns the exact note index
  return (((s[(s[1]=='S') ? 2 : 1]-1) - '0')*12)+1+pos;
}

void initTimer1(){
  // Disable counter overflow interrupts
  TIMSK1  &= ~(1<<TOIE1);
  // Activate Compare-Match-A interrupts
  TIMSK1 |= (1<<OCIE1A);
  // Clear TCCR2A and TCCR2B registers
  TCCR1A = 0;
  TCCR1B = 0;
  // Set CTC mode (Clear Tomer on Compare match): Automatically resets the timer on OCR2A overflow
  TCCR1B |= (1<<WGM12);
  // Set prescaler to 64
  TCCR1B |= ((1<<CS11) | (1<<CS10));
  // Set value for the timer counter to match.
  // Timer 1 counter overflows on 65535. Each click takes 4us.
  // We want to take 'delaytempo' ms.
  // So, we want to count until X such as 0.004ms * X = delaytempo --> X = delaytempo / 0.004 == delaytempo*250
  OCR1A=delaytempo*250;
}

void initTimer2(){
  // Disable counter overflow interrupts
  TIMSK2  &= ~(1<<TOIE2);
  // Enable Compare Match A interrupts
  TIMSK2 |= (1<<OCIE2A);
  // Clear TCCR2A and TCCR2B registers
  TCCR2A = 0;
  TCCR2B = 0;
  // Set CTC mode
  TCCR2A |= (1<<WGM21);
  // Set prescaler to 8
  TCCR2B |= (1<<CS21);
  // Set value to match for interrupt triggering
  // Timer 2 counter overflows at 255. We want to reach mcd*2-1
  OCR2A=mcd*2-1;
}

// Timer 1 interrupt handler (ISR: Interrupt Service Routine)
ISR(TIMER1_COMPA_vect){
  // tempo led
  PORTB = (tempoLedCounter==7 ? PORTB | _BV(tempoLed -8) : PORTB & ~(_BV(tempoLed-8)));
  tempoLedCounter=((tempoLedCounter+1)%8);
  // update notes
  update();
}

// Timer 2 interrupt handler
ISR(TIMER2_COMPA_vect){
  // For each voice, increase count and check if the output has to be toggled
  for (char i=0; i<N_BUZZERS; i++){
    count[i]++;
    if (count[i] == counts[i]){
      PORTD ^= (_BV(buzz[i]));  // much faster than digitalWrite
      count[i]=0;
    }
  }
}

/**
 * Get next note for this voice and put it in the buffer
 * Here we check for repetitions too
 *
 * @param voz : Voice to read
 */
void getNext(char voz){
  if  (head[voz]==-1){
    buffer[0]='X';
    buffer[1]='\0';
  }
  else{
    // Get pointer to voice string on progmem
    PGM_P prog_str=(const char*)pgm_read_word(&(voces[voz]));
    char j=0;
    boolean fin=false;
    boolean endrep=false,beginrep=false;

    // Read strings until space or 'X'
    while (((buffer[j]=(char) pgm_read_byte(&prog_str[head[voz]+j]))!= ' ')){
      if (buffer[j]=='|'){ // Repetition found
        // Move header on next voice
        head[voz]+=4; // Jump 4 characters, as repetition symbol has 4: '||: '
        // Store repetition to come back later
        rep[voz]=head[voz];
        beginrep=true;
        // Read next note
        break;
      }
      if (buffer[j]==':'){ // Repetition end
        if (rep[voz]!=-1){ // If a repetition was started earlier
          head[voz]=rep[voz];
          rep[voz]=-1;
        }
        else{  //  Already repeated. Ignore repetition end
          head[voz]+=4;
        }
        endrep=true;
        break;
      }
      if (buffer[j]=='X'){
        fin=true;
        break;
      }
      j++;
    }
    if (fin){
      head[voz]=-1;
      buffer[1]='\0';
    }
    else
    if (beginrep||endrep){  // repetition symbol. Read next note
        getNext(voz);
      }
    else{
        head[voz]+=(j+1);
        buffer[j]='\0';
    }
  }
}
boolean melodyOver(){
  char c=0;
  for (int i=0;i<N_BUZZERS;i++){
    c+=head[i];
  }
  return (c==(N_BUZZERS*-1))? true : false;
}

void endProgram(){
  cli();
  TIMSK1 &= ~(1<<OCIE1A);
  TIMSK2 &= ~(1<<OCIE2A);
  sei();
  digitalWrite(rythmLed,HIGH);
  digitalWrite(tempoLed,HIGH);
}

/**
 * Sets the note on the buffer, for the given voice.
 * Notice the note comes in a AB(S)C format that has to be splitted in A and B(S)C. A can have 1 or 2 digits
 * @param voz
 */
void seteaNota(char voz){
  if (buffer[0]=='X'){  // If no notes left, put silences forever until every voice ends.
    nota[voz]="S";
    duracion[voz]=32;
    if (melodyOver()){
      endProgram();
    }
  }
  else{
    // Set duration. Notice that: 32-> 1 time, 16-> 2 times, 8->4, 4->8, 2->16, 1->32
    String numbers="26";  //  cases 16 and 32
    int d;
    boolean dosdig=false; //  if duration had 2 digits
    if (numbers.indexOf(buffer[1])!=-1){
      d=32/((buffer[0]-'0')*10 + (buffer[1]-'0'));
      dosdig=true;
    }
    else
      d=32/(buffer[0]-'0');
    duracion[voz]=d;

    // store note
    char j=dosdig ? 2 : 1; // stands in the note position
    nota[voz]=buffer;
    nota[voz]=nota[voz].substring(j);
  }
}
boolean loadVoices(){
  // Read each voice duration. If counter is 0, read next note, else, do nothing
  boolean changed=false;
  for (char i=0; i<N_BUZZERS; i++){
    if (duracion[i]<=1){
      changed=true;
      // Puts the corresponding note on the buffer
      getNext(i);
      seteaNota(i);
    }
    else duracion[i]--;
  }
  return changed;
}

void update(){
  if (loadVoices()){
    initNotas_p();
    initCounts();
  }
}

void initBuzzers(){
  for (char i=0; i<N_BUZZERS; i++){
    buzz[i]=i+2;
    pinMode(i+2,OUTPUT);
  }
}

void initLeds(){
  pinMode(rythmLed,OUTPUT);
  pinMode(tempoLed,OUTPUT);
  digitalWrite(tempoLed,HIGH);
}

void setup(){
  // Set voice pins
  initBuzzers();
  // set indicator led pins
  initLeds();
  // Load first note for each voice. Calculate mcd-adjusted note period for each note
  update();
  // Setup timers
  cli();
  initTimer1();
  initTimer2();
  sei();

  pinMode(0,INPUT);
}

void loop(){
  // Loop is almost free for anything else. Notice that Timer0 is still free, so Timer0 functions should kind of work
  tempo=map(analogRead(0),0,1023,30,500);
  delaytempo=7500/(int)(tempo);
  OCR1A=delaytempo*250;
  delay(250);
}


/* Review:
-Día 1: Al parecer el asunto del mcd no es trivial: si alguna combinación de notas hace
que las interrupciones ocurran muy seguido, sencillamente no funciona. Deducción sacada a partir de
la experiencia de que para algunas tuplas de notas funciona y ambas suenan al unísono, para otras
hay sonido pero con notas extrañas y para otras sencillamente no hay sonido. Habría que de alguna
manera modificar los valores de las frecuencias para los casos donde las interrupciones ocurren
muy seguido. Habría que modificar la frecuencia de manera de que exista un mcd relativamente
significativo. Por ejemplo, mmodificarlas todas para que siempre sea 5.
-Día 2: Se generan períodos aproximados de manera de cumplir con un mcd global. Con esto se
elimina el problema anterior y se tienen notas bastante buenas. Se cambiará a timer 2 para no
desperdiciar la capacidad del timer 1 en términos de bits del counter.
Ahora se usa timer 2, y el modo de introducir notas es en texto. Se parsean con una función especial.
El reproductor funciona y puede tocar diversos acordes. La meta ahora es determinar un método de introducción
de la música que sea cómodo y considere duraciones de cada nota para todas las voces. Los silencios deberían
poder implementarse como frecuencias nulas. Silencios implementados
  Idea:
    Melodías como un string, para cada voz (cada Buzzer)
    Protocolo: AB(S)C o AS si es silencio.
          A=Duración (1=redonda, 2= blanca, 4=negra, 8=corchea, 16=semicorchea, 32=fusa. Se admitirá hasta fusa, luego, "loop()" debe actualizarse a esta velocidad, que es delaytempo/8)
          B=Nota (A, B, C, D, E, F, G)
          C=Octava de la nota. de 0 a 8, el La 440 es A4
    Separador: Espacio.
    Ejemplo: "8C5 8S 8G4 8G4 8A4 8S 8G4 4S 8S 8B4 8S 8C4"
    Eso para una sola voz.

    Cada voz carga su nota y debe durar lo que dice la información.
    Cargar la nota significa:
      -Actualizar las notas del vector de notas. Notar que el vector de notas tiene strings.
      -Establecer duración de nota. Para esto, crear un contador para cada nota, que
        parte en el valor que le corresponde y decrece hasta llegar a 0, en cuyo caso
        se procede a leer la siguiente nota para esa voz, desde el string destinado a eso.
        Como idea para la lectura, ir "consumiendo" de alguna manera el string.
    Tras cargar la nota, actualizar los "counts" y los períodos "NOTAS_P" para cada nota.
      Para eso existe función update();
    El delay de cada ciclo depende del tempo que se especifique.

-Día 3: implementado toque de melodías. Problema: se perdió polifonía, reproduce tan solo la última voz.
  Observación: Está actualizando TODAS LAS NOTAS con la última voz. Error de subindicación???
  NO. Se sobreescribe la referencia al char*, en seteaNota().
  Fix: Usar String en vez de Char*. Los String son mutables y cambian su tamaño a gusto.

  Problema: notas se repiten 5 veces al usar corcheas...
  Fixed. Problema con los contadores de duración.

-Día 4: El sistema funciona completamente. Único detalle desagradable: el delay produce pequeños
  silencios no deseados que a la larga estorban en la melodía. Posible arreglo: Usar otro timer.
  Este arreglo haría que el timer provoque una interrupción cada "delaytempo" segundos.
  "delaytempo" depende de "tempo". Asumiendo que "tempo" va de 30 a 300, delaytempo va de 250ms a 25ms
  Estos son los posibles tiempos que puede quererse para los interrupts. Si se usa un prescaler de 64,
  se ve que se necesitan 6250 clicks y para 250ms 62500. Convendría usar el timer 1 que cubre estos
  valores por ser de 16 bits.

  -Se implementa con timer 1 en vez de delays en el loop. Los silencios siguen de todas maneras.
  Explicación: Si bien el timer 1 y el 2 son independientes entre sí, cada vez que ocurre una interrupción
  por el timer 1, el timer 2 pierde la cpu por las instrucciones que debe ejecutar el procesador por la
  interrupción del timer 1. Luego, la idea sería ejecutar cosas sólo cuando las notas cambien.
  Estrategia actual de detección de cambios de notas: interrupciones cada fusa. Cada nota tiene una duración
  definida por la cantidad de fusas que caben en su tempo.
  El problema es que ahora, aunque NINGUNA nota cambie, se está evaluando la duración de cada nota y ejecutando varias instrucciones.
  La interrupción debería saber cuándo ejecutar realmente update. La idea es que sólo se ejecute cuando alguna nota
  debe cambiar.
  Comprobado: funciona la continuidad del sonido aunque haya que evaluar un if.
  SOLUCIONADO

  Problema: ruidos extraños al terminar reproducción, y a veces en medio de la reproducción.
    Detalles: -Ocurre sólo cuando la melodía se hace larga. Si es corta, de a tres suenan perfecto.
              -Si agrego otra voz, crece la cantidad de datos y vuelve el ruido.

  IDENTIFICADO: El problema se produce por poca memoria RAM. Es posible ocupar la memoria flash PROGMEM, donde
    se almacena el programa. Esta memoria es mucho más grande que la memoria RAM. Un gran contra de usar esta
    memoria es que solo se almacenan valores constantes, y la implementación actual necesita hacer variar el
    texto de las voces. Para implementarel programa usando memoria flash, se debe redefinir los algoritmos
    de parseo del texto de las voces, dado que además hay limitados tipos de datos para la memoria flash, y
    String no es uno de ellos, aunque char si lo es. Luego, se pierden valiosos métodos para trabajar strings.
--Guardando versión actual 3.0 en respaldo de versiones--

    Se implementa versión con almacenamiento en PROGMEM y se solucionan los problemas descritos


Proyecto terminado. Única limitante hasta ahora es el largo posible de los strings. A partir de cierto largo, Java
lanza StackOverflow.

Ideas para el futuro:
    DONE (sin opamp) -Con un opamp hacer un sumador y reproducir los sonidos en una única salida de audio, ya sea a un
    parlante o a audífonos.
    DONE -Implementar parseo de repeticiones, para no tener que copiar pegar todo un bloque. Ejemplo: "||:4C4 4D4 4E4 4F4:||4C4 X".
    -Implementar tresillos.
    DONE -Crear software de inserción amigable de notas. El resultado del software es el resultado en texto que hay que copiar pegar para tener melodía.

--Guardando versión 4.0 en respaldo de versiones--
    -Se implementó repeticiones. Nomenclatura explicada al inicio.
    -Se testeó el jack de audio (chassis) y funciona bien mezclando directamente, pero podría quizás mejorarse
      la calidad del audio usando un sumador activo.
      EDIT: estaba mal hecho el mezclador. Se implementó un sumador pasivo con resistencias y suena bien.
    -Se varía el tempo con un potenciómetro.
--Guardando versión 5.0 en respaldo de versiones--

    .*/
