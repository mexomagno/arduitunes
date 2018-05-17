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

char noteStringToFrequency(String note_string);
void doTick();


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
 */

/**** PRESET MELODIES ****/

/** SUPER MARIO (ideal tempo: 200) **/

const char voice1_melody[] PROGMEM = {
        "8E5 8E5 8S 8E5 8S 8C5 8E5 8S 8G5 8S 4S 8G4 4S 8S ||: 8C5 4S 8G4 4S 8E4 4S 8A4 8S 8B4 8S 8AS4 8A4 8S 8G4 8E5 8S 8G5 8A5 8S 8F5 8G5 8S 8E5 8S 8C5 8D5 8B4 4S :|| "
                "||: 4S 8G5 8FS5 8F5 8DS5 8S 8E5 8S 8GS4 8A4 8C5 8C5 8A4 8C5 8D5 4S 8G5 8FS5 8F5 8DS5 8S 8E5 8S 8C6 8S 8C6 8C6 8S 2S 8G5 8FS5 8F5 8DS5 8S 8E5 8S 8GS4 8A4 8C5 8C5 8A4 8C5 8D5 4S 8DS5 4S 8D5 4S 8C5 4S 8S 2S :|| "
                "8C5 8C5 8S 8C5 8S 8C5 8D5 8S 8E5 8C5 8S 8A4 8G4 8S 4S 8C5 8C5 8S 8C5 8S 8C5 8D5 8E5 2S 2S 8C5 8C5 8S 8C5 8S 8C5 8D5 8S 8E5 8C5 8S 8A4 8G4 8S 4S 8E5 8E5 8S 8E5 8S 8C5 8E5 8S 8G5 8S 4S 8G4 8S 4S "
                "||: 8C5 4S 8G4 4S 8E4 4S 8A4 8S 8B4 8S 8AS4 8A4 8S 8G4 8E5 8S 8G5 8A5 8S 8F5 8G5 8S 8E5 8S 8C5 8D5 8B4 4S :|| "
                "||: 8E5 8C5 8S 8G4 4S 8GS4 8S 8A4 8F5 8S 8F5 8A4 8S 4S 8B4 8A5 8S 8A5 8A5 8G5 8S 8F5 8E5 8C5 8S 8A4 8G4 8S 4S 8E5 8C5 8S 8G4 4S 8GS4 8S 8A4 8F5 8S 8F5 8A4 8S 4S 8B4 8F5 8S 8F5 8F5 8E5 8S 8D5 8C5 8S 4S 2S :|| "
                "8C5 8C5 8S 8C5 8S 8C5 8D5 8S 8E5 8C5 8S 8A4 8G4 8S 4S 8C5 8C5 8S 8C5 8S 8C5 8D5 8E5 2S 2S 8C5 8C5 8S 8C5 8S 8C5 8D5 8S 8E5 8C5 8S 8A4 8G4 8S 4S 8E5 8E5 8S 8E5 8S 8C5 8E5 8S 8G5 8S 4S 8G4 8S 4S "
                "8E5 8C5 8S 8G4 4S 8GS4 8S 8A4 8F5 8S 8F5 8A4 8S 4S 8B4 8A5 8S 8A5 8A5 8G5 8S 8F5 8E5 8C5 8S 8A4 8G4 8S 4S 8E5 8C5 8S 8G4 4S 8GS4 8S 8A4 8F5 8S 8F5 8A4 8S 4S 8B4 8F5 8S 8F5 8F5 8E5 8S 8D5 8C5 8S 4S 2S "
                "8C5 4S 8G4 4S 8E4 8S 8A4 8B4 8A4 4GS4 4AS4 4GS4 1G4 X"};
const char voice2_melody[] PROGMEM = {
        "8FS4 8FS4 8S 8FS4 8S 8FS4 8FS4 8S 8D5 8S 4S 8B3 4S 8S ||: 8G4 4S 8E4 4S 8C4 4S 8F4 8S 8G4 8S 8FS4 8F4 8S 8E4 8C5 8S 8E5 8F5 8S 8D5 8E5 8S 8C5 8S 8A4 8G4 8G4 4S :|| "
                "||: 4S 8E5 8DS5 8D5 8B4 8S 8C5 8S 8E4 8F4 8G4 8S 8C4 8E4 8F4 4S 8E5 8DS5 8D5 8B4 8S 8C5 8S 8G5 8S 8G5 8G5 8S 2S 8E5 8DS5 8D5 8B4 8S 8C5 8S 8E4 8F4 8G4 8S 8C4 8E4 8F4 4S 8GS4 4S 8F4 4S 8E4 4S 8S 2S :|| "
                "8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8S 8G4 8E4 8S 8E4 8C4 8S 4S 8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8G4 2S 2S 8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8S 8G4 8E4 8S 8E4 8C4 8S 4S 8FS4 8FS4 8S 8FS4 8S 8FS4 8FS4 8S 8D5 4S 8S 8D4 8S 4S "
                "||: 8G4 4S 8E4 4S 8C4 4S 8F4 8S 8G4 8S 8FS4 8F4 8S 8E4 8C5 8S 8E5 8F5 8S 8D5 8E5 8S 8D5 8S 8A4 8G4 8D4 4S :|| "
                "||: 8C5 8G4 8S 8E4 4S 8E4 8S 8F4 8C5 8S 8C5 8F4 8S 4S 8G4 8F5 8S 8F5 8F5 8E5 8S 8D5 8C5 8F4 8S 8F4 8E4 8S 4S 8C5 8G4 8S 8E4 4S 8E4 8S 8F4 8C5 8S 8C5 8F4 8S 4S 8G4 8D5 8S 8D5 8D5 8C5 8S 8B4 8G4 8E4 8S 8E4 8C4 8S 4S :|| "
                "8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8S 8G4 8E4 8S 8E4 8C4 8S 4S 8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8G4 2S 2S 8GS4 8GS4 8S 8GS4 8S 8GS4 8AS4 8S 8G4 8E4 8S 8E4 8C4 8S 4S 8FS4 8FS4 8S 8FS4 8S 8FS4 8FS4 8S 8D5 4S 8S 8D4 8S 4S "
                "8C5 8G4 8S 8E4 4S 8E4 8S 8F4 8C5 8S 8C5 8F4 8S 4S 8G4 8F5 8S 8F5 8F5 8E5 8S 8D5 8C5 8F4 8S 8F4 8E4 8S 4S 8C5 8G4 8S 8E4 4S 8E4 8S 8F4 8C5 8S 8C5 8F4 8S 4S 8G4 8D5 8S 8D5 8D5 8C5 8S 8B4 8G4 8E4 8S 8E4 8C4 8S 4S "
                "8E4 4S 8C4 4S 8G3 8S 8F4 4F4 4F4 2F4 8E4 8D4 2E4 4E4 X"};
const char voice3_melody[] PROGMEM = {
        "8D3 8D3 8S 8D3 8S 8D3 8D3 8S 8B4 8S 4S 8G3 4S 8S ||: 8E4 4S 8C4 4S 8G3 4S 8C4 8S 8D4 8S 8CS4 8C4 8S 8C4 8G4 8S 8B4 8C5 8S 8A4 8B4 8S 8A4 8S 8E4 8F4 8D4 4S :|| "
                "||: 8C3 4S 8G3 4S 8C4 8S 8F3 4S 8C4 8C4 8C4 8F3 8S 8C3 4S 8E3 4S 8G3 8C4 8S 8F5 8S 8F5 8F5 8S 8G3 8S 8C3 4S 8G3 4S 8C4 8S 8F3 4S 8C4 8C4 8C4 8F3 8S 8C3 8S 8GS3 4S 8AS3 4S 8C4 4S 8G3 8G3 8S 8C3 8S :|| "
                "8GS2 4S 8DS3 4S 8GS3 8S 8G3 4S 8C3 4S 8G2 8S 8GS2 4S 8DS3 4S 8GS3 8S 8G3 4S 8C3 4S 8G2 8S 8GS2 4S 8DS3 4S 8GS3 8S 8G3 4S 8C3 4S 8G2 8S 8D3 8D3 8S 8D3 8S 8D3 8D3 8S 8B4 4S 8S 8G3 8S 4S "
                "||: 8E4 4S 8C4 4S 8G3 4S 8C4 8S 8D4 8S 8CS4 8C4 8S 8C4 8G4 8S 8B4 8C5 8S 8A4 8B4 8S 8A4 8S 8E4 8F4 8D4 4S :|| "
                "||: 8C3 4S 8FS3 8G3 8S 8C4 8S 8F3 8S 8F3 8S 8C4 8C4 8F3 8S 8D3 4S 8F3 8G3 8S 8A3 8S 8G3 8S 8G3 8S 8C4 8C4 8G3 8S 8C3 4S 8FS3 8G3 8S 8C4 8S 8F3 8S 8F3 8S 8C4 8C4 8F3 8S 8G3 4S 8G3 8G3 8A3 8S 8B3 8C4 8S 8G3 8S 8C3 8S 4S :|| "
                "8GS2 4S 8DS3 4S 8GS3 8S 8G3 4S 8C3 4S 8G2 8S 8GS2 4S 8DS3 4S 8GS3 8S 8G3 4S 8C3 4S 8G2 8S 8GS2 4S 8DS3 4S 8GS3 8S 8G3 4S 8C3 4S 8G2 8S 8D3 8D3 8S 8D3 8S 8D3 8D3 8S 8B4 4S 8S 8G3 8S 4S "
                "8C3 4S 8FS3 8G3 8S 8C4 8S 8F3 8S 8F3 8S 8C4 8C4 8F3 8S 8D3 4S 8F3 8G3 8S 8A3 8S 8G3 8S 8G3 8S 8C4 8C4 8G3 8S 8C3 4S 8FS3 8G3 8S 8C4 8S 8F3 8S 8F3 8S 8C4 8C4 8F3 8S 8G3 4S 8G3 8G3 8A3 8S 8B3 8C4 8S 8G3 8S 8C3 8S 4S "
                "8G3 4S 8E3 4S 8C3 8S 8F3 4F3 4CS3 2CS3 1C3 X"};
const char voice4_melody[] PROGMEM = {"X"};
const char voice5_melody[] PROGMEM = {"X"};


/** POKEMON BATTLE MUSIC (ideal tempo: 180) **/
/*
const char voice1_melody[] PROGMEM={
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
const char voice2_melody[] PROGMEM={
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
const char voice3_melody[] PROGMEM={
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
const char voice4_melody[] PROGMEM={
"X"};
const char voice5_melody[] PROGMEM={
"X"};
*/

/** Attack on titan intro 1 **/
/*
const char voice1_melody[] PROGMEM={
"8D5 8D5 8F5 4E5 8C5 8C5 4D5 8D5 8F5 4E5 4C5 8S 4A5 8F5 8G5 8S 8E5 8S 8F5 8S 8D5 8S 8E5 8S 4C5 8S 4A5 8F5 8G5 8S 8E5 8S 8F5 8S 8E5 8S 8D5 8S 4C5 8S "
"4C6 8GS5 8AS5 8S 8G5 8S 8GS5 8S 8F5 8S 8G5 8S 4DS5 8S 4C6 8GS5 8AS5 8S 8G5 8S 8GS5 8S 8G5 8S 8F5 8S 4DS5 8S X"};
const char voice2_melody[] PROGMEM={
"8D4 8D4 8F4 4E4 8C4 8C4 4D4 8D4 8F4 4E4 4C4 8S ||: 8D3 8D4 8D3 8D4 :|| 8D3 8D4 8D3 8D4 8D3 8C4 8C3 8C4 ||: 8AS2 8AS3 8AS2 8AS3 :|| 8AS2 8AS3 8AS2 8AS3 8AS2 8C4 8C3 8C4 "
"||: 8F3 8F4 8F3 8F4 :|| 8F3 8F4 8F3 8F4 8F3 8DS4 8DS3 8DS4 ||: 8CS3 8CS4 8CS3 8CS4 :|| 8CS3 8CS4 8CS3 8CS4 8CS3 8DS4 8DS3 8DS4 X"};
const char voice3_melody[] PROGMEM={
"1S 2S 4S 16A4 16AS4 16B4 16CS5 4D5 8D5 8E5 8S 8C5 8S 8D5 8S 8A4 8S 8C5 8S 4G4 8S 4F5 8D5 8E5 8S 8C5 8S 8D5 8S 8C5 8S 8A4 8S 4G4 8S 4GS5 8F5 8G5 8S 8DS5 "
"8S 8F5 8S 8C5 8S 8DS5 8S 4AS4 8S 4GS5 8F5 8G5 8S 8DS5 8S 8F5 8S 8DS5 8S 8C5 8S 4AS4 8S X"};
const char voice4_melody[] PROGMEM={
"X"};
const char voice5_melody[] PROGMEM={
"X"};
*/

/** Fullmetal alchemist intro 1 **/

/*
const char voice1_melody[] PROGMEM={
        "8E5 4B5 8E5 4E5 4D5 8E5 8B5 8E5 8E5 16D5 16E5 8E5 4FS5 16FS5 16FS5 8FS5 8G5 8FS5 8G5 8FS5 8B4 4B4 1S X"};
const char voice2_melody[] PROGMEM={
        "8S 8C3 8S 8S 8C3 8S 8S 8C3 8S 8D3 8S 8S 8D3 8S 8S 8D3 8S 8E3 8S 8S 8E3 8S 8S 8E3 8S 8E3 8S 8S 8E3 8S 8S 8E3 8S X"};
const char voice3_melody[] PROGMEM={
        "8S 8G3 8S 8S 8G3 8S 8S 8G3 8S 8A3 4S 8A3 4S 8A3 8S 8B3 4S 8B3 4S 8B3 8S 8B3 4S 8B3 4S 8B3 8S X"};
const char voice4_melody[] PROGMEM={
        "X"};
const char voice5_melody[] PROGMEM={
        "X"};
*/

/**** END PRESET MELODIES ****/


const char *const all_melodies[] PROGMEM = {voice1_melody, voice2_melody, voice3_melody, voice4_melody, voice5_melody};

// Musical notes frequencies (Hz)
const uint16_t NOTE_FREQUENCIES[] PROGMEM = {
        31,/*C1*/33, 35, 37, 39, 41, 44, 46, 49, 52, 55, 58, //12
        62,/*C2*/65, 69, 73, 78, 82, 87, 93, 98, 104, 110, 117,  //24
        123,/*C3*/131, 139, 147, 156, 165, 175, 185, 196, 208, 220, 233, //36
        247,/*C4*/262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466,  //48
        494,/*C5*/523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932,  //60
        988,/*C6*/1047, 1109, 1175, 1245, 1319, 1397, 1480, 1568, 1661, 1760, 1865,  //72
        1976,/*C7*/2093, 2217, 2349, 2489, 2637, 2794, 2960, 3136, 3322, 3520, 3729,  //84
        3951,/*C8*/4186, 4435, 4699, 4978, 0};  //90


// Maximum common divider. Voice notes periods must be a multiple of this
// * Less: More precise notes, higher processing frequency, risks timer callback not finishing at time
// * More: Less precise notes (untuned), no risks for timer callback
const char mcd = 20; // Jeremy Blum proposes: 64.
// Tempo, recommended: between 30 and 400
word tempo = 200;  // Initial value, will be immediately changed by analog input
// Time period for a 32th-note, derived from the tempo
int tempo_32th_tick_period = 7500 / (constrain((int) (tempo), 30, 400));
// 7500 comes from (60*1000/tempo)/8. The 8 is derived by considering a demisemiquaver as the fundamental time unit (1/8 of a quarter note)

// Number of voices to activate
#define ACTIVE_VOICES_COUNT 3

// VOICES NOTE STATE TRACKERS

// Voices pins
char voices_output_pins[ACTIVE_VOICES_COUNT];
// Used to parse each voice's note sequence
String current_note_strings[] = {"S", "S", "S", "S", "S"};
// Each voice current note duration
unsigned char current_note_durations[] = {0, 0, 0, 0, 0};
// Voice headers, to track where the last reading was standing at
int current_notestring_indexes[] = {0, 0, 0, 0, 0};
// Pointers for repetitions
int repetition_indexes[] = {-1, -1, -1, -1, -1};
// Each voice note's period (microseconds, us) (period = 1/frequency)
// 'Half period' is the time we must take to toggle the voice's output in order to do it in the desired frequency.
int current_note_half_periods_us[ACTIVE_VOICES_COUNT];
// Voices tick counters (mutable). Tracks each voice's current note ticks. Needed to know when to change the note.
// Using chars instead of ints break silences, don't know why yet
unsigned int current_note_tick_counts[ACTIVE_VOICES_COUNT];
// Counts per note (max is 252, so unsigned char is enough)
// 'current_note_tick_limits' symbolize the total period divided by the time the timer counter takes to reset.
// As we want the timer to take 'mcd' us (microseconds), 'current_note_tick_limits' is period/mcd.
// If a voice reaches this value, it's current note life ended and we must load the next note.
unsigned char current_note_tick_limits[ACTIVE_VOICES_COUNT];

// Used to bring each note from the PROGMEM. The size is derived from the max string size used by our protocol (ex.: 32CS5)
// Don't forget the trailing \0 char
char progmem_buffer[6];

// LED STATE TRACKERS
// Status led pins
char rhythm_led_pin = 11;
char tempo_led_pin = 12;
// Led tempo counter
char tempo_led_counter = 0;

/**
 * Initializes time periods for each voice for their current notes.
 *
 * - For each active note:
 *    + Get period from note frequency
 *    + Adjust period to be a multiple of mcd
 *    + Update period in global list
 */
void computeNotePeriods() {
    for (char i = 0; i < ACTIVE_VOICES_COUNT; i++) {
        // get current note frequency
        int progint2int = pgm_read_word_near(NOTE_FREQUENCIES + noteStringToFrequency(current_note_strings[i]));
        // derive period from frequency.
        /* Don't exactly remember where the 500000 comes from. I think that from the fact that:
         * 1/f[Hz] = P [s]
         *         = 1000P [ms]
         *         = 1000000P [us]
         *  half_period = P/2
         *  half_period[us] = 500000P
        */
        int half_period = (int) (500000.0 / progint2int);  // Real half period of this tone
        current_note_half_periods_us[i] = half_period + ((half_period % mcd) <= (mcd / 2) ?
                                           - (half_period % mcd) :
                                           (mcd - (half_period % mcd))); // mcd-adjusted period
    }
}

/**
 * Initializes the tick counters for each note, used to track each note duration.
 */
void initDurationTickCounters() {
    for (char i = 0; i < ACTIVE_VOICES_COUNT; i++) {
        current_note_tick_limits[i] = current_note_half_periods_us[i] / mcd;
        current_note_tick_counts[i] = 0;
    }
}

/**
 * Parses a string representing a tone and returns the tone position in the notes list.
 * Perfect inputs assumed
 *
 * note_string has two possible shapes:
 *  1. XY
 *  2. XSY
 *
 * @param note_string: input string
 * @return: int: index to notes list
 */
char noteStringToFrequency(String note_string) {
    if (note_string[0] == 'S') {
        PORTB &= ~(_BV(rhythm_led_pin - 8));
        return 89;
    }
    // Remembering ASCII code: A=65, ..., G=71
    PORTB |= _BV(rhythm_led_pin - 8);
    char pos;
    if (note_string[1] != 'S') { // Natural note, no sharp
        switch (note_string[0]) {
            case 65: //A
                pos = 9;
                break;
            case 66: //B
                pos = 11;
                break;
            case 67: //C
                pos = 0;
                break;
            case 68: //D
                pos = 2;
                break;
            case 69: //E
                pos = 4;
                break;
            case 70: //F
                pos = 5;
                break;
            case 71: //G
                pos = 7;
                break;
            default:  // should never happen
                pos = 9;
        }
    } else {
        switch (note_string[0]) { // Sharp note
            case 65: //A#
                pos = 10;
                break;
                // B# doesn't exist in equal temperament
            case 67: //C#
                pos = 1;
                break;
            case 68: //D#
                pos = 3;
                break;
                // E# doesn't exist in equal temperament
            case 70: //F#
                pos = 6;
                break;
            case 71: //G#
                pos = 8;
                break;
            default:  // should never happen
                pos = 10;
        }
    }
    // Magic equation that returns the exact note index
    return (((note_string[(note_string[1] == 'S') ? 2 : 1] - 1) - '0') * 12) + 1 + pos;
}

void initTimer1() {
    // Disable counter overflow interrupts
    TIMSK1 &= ~(1 << TOIE1);
    // Activate Compare-Match-A interrupts
    TIMSK1 |= (1 << OCIE1A);
    // Clear TCCR2A and TCCR2B registers
    TCCR1A = 0;
    TCCR1B = 0;
    // Set CTC mode (Clear Tomer on Compare match): Automatically resets the timer on OCR2A overflow
    TCCR1B |= (1 << WGM12);
    // Set prescaler to 64
    TCCR1B |= ((1 << CS11) | (1 << CS10));
    // Set value for the timer counter to match.
    // Timer 1 counter overflows on 65535. Each click takes 4us.
    // We want to take 'tempo_32th_tick_period' ms.
    // So, we want to current_note_tick_counts until X such as 0.004ms * X = tempo_32th_tick_period --> X = tempo_32th_tick_period / 0.004 == tempo_32th_tick_period*250
    OCR1A = tempo_32th_tick_period * 250;
}

void initTimer2() {
    // Disable counter overflow interrupts
    TIMSK2 &= ~(1 << TOIE2);
    // Enable Compare Match A interrupts
    TIMSK2 |= (1 << OCIE2A);
    // Clear TCCR2A and TCCR2B registers
    TCCR2A = 0;
    TCCR2B = 0;
    // Set CTC mode
    TCCR2A |= (1 << WGM21);
    // Set prescaler to 8
    TCCR2B |= (1 << CS21);
    // Set value to match for interrupt triggering
    // Timer 2 counter overflows at 255. We want to reach mcd*2-1
    OCR2A = mcd * 2 - 1;
}

// Timer 1 interrupt handler (ISR: Interrupt Service Routine)
ISR(TIMER1_COMPA_vect) {
    // tempo led
    PORTB = (tempo_led_counter == 7 ? PORTB | _BV(tempo_led_pin - 8) : PORTB & ~(_BV(tempo_led_pin - 8)));
    tempo_led_counter = ((tempo_led_counter + 1) % 8);
    // update notes
    doTick();
}

// Timer 2 interrupt handler
ISR(TIMER2_COMPA_vect) {
    // For each voice, increase current_note_tick_counts and check if the output has to be toggled
    for (char i = 0; i < ACTIVE_VOICES_COUNT; i++) {
        current_note_tick_counts[i]++;
        if (current_note_tick_counts[i] == current_note_tick_limits[i]) {
            PORTD ^= (_BV(voices_output_pins[i]));  // much faster than digitalWrite
            current_note_tick_counts[i] = 0;
        }
    }
}

/**
 * Get next note for this voice and put it in the buffer
 * Here we check for repetitions too
 *
 * @param voice_index : Voice for which to read it's next note string from buffer
 */
void loadNextVoiceNotestringToBuffer(char voice_index) {
    // if this voice is already finished
    if (current_notestring_indexes[voice_index] == -1) {
        progmem_buffer[0] = 'X';
        progmem_buffer[1] = '\0';
    } else {
        // Get pointer to voice string on progmem
        PGM_P prog_str = (const char *) pgm_read_word(&(all_melodies[voice_index]));
        char j = 0;
        boolean is_voice_end = false;
        boolean is_repetition_start = false, is_repetition_end = false;

        // Read strings until space or 'X'
        while (((progmem_buffer[j] = (char) pgm_read_byte(&prog_str[current_notestring_indexes[voice_index] + j])) != ' ')) {
            if (progmem_buffer[j] == '|') { // Repetition start found
                // Move header on next voice
                current_notestring_indexes[voice_index] += 4; // Jump 4 characters, as repetition symbol has 4: '||: '
                // Store repetition to come back later
                repetition_indexes[voice_index] = current_notestring_indexes[voice_index];
                is_repetition_start = true;
                // Read next note
                break;
            }
            if (progmem_buffer[j] == ':') { // Repetition end
                if (repetition_indexes[voice_index] != -1) { // If a repetition was started earlier
                    current_notestring_indexes[voice_index] = repetition_indexes[voice_index];
                    repetition_indexes[voice_index] = -1;
                } else {  //  Already repeated. Ignore repetition end
                    current_notestring_indexes[voice_index] += 4;
                }
                is_repetition_end = true;
                break;
            }
            if (progmem_buffer[j] == 'X') {
                is_voice_end = true;
                break;
            }
            j++;
        }
        if (is_voice_end) {
            current_notestring_indexes[voice_index] = -1;
            progmem_buffer[1] = '\0';
        } else if (is_repetition_start || is_repetition_end) {  // repetition symbol. Read next note
            loadNextVoiceNotestringToBuffer(voice_index);
        } else {
            current_notestring_indexes[voice_index] += (j + 1);
            progmem_buffer[j] = '\0';
        }
    }
}

boolean isMelodyOver() {
    char c = 0;
    for (int i = 0; i < ACTIVE_VOICES_COUNT; i++) {
        c += current_notestring_indexes[i];
    }
    return (c == (ACTIVE_VOICES_COUNT * -1)) ? true : false;
}

void endProgram() {
    cli();
    TIMSK1 &= ~(1 << OCIE1A);
    TIMSK2 &= ~(1 << OCIE2A);
    sei();
    digitalWrite(rhythm_led_pin, HIGH);
    digitalWrite(tempo_led_pin, HIGH);
}

/**
 * For the given voice, update it's state variables according to it's current note string.
 * Notice the note comes in a AB(S)C format that has to be splitted in A and B(S)C. A can have 1 or 2 digits
 * @param voice_index: Index to the voice to update
 *
 */
void updateVoiceStateFromBuffer(char voice_index) {
    if (progmem_buffer[0] == 'X') {  // If no notes left, put silences forever until every voice ends.
        current_note_strings[voice_index] = "S";
        current_note_durations[voice_index] = 32;
        if (isMelodyOver()) {
            endProgram();
        }
    } else {
        // Set duration. Notice that: 32-> 1 time, 16-> 2 times, 8->4, 4->8, 2->16, 1->32
        String numbers = "26";  //  cases 16 and 32
        int d;
        boolean has_two_digits = false; //  if duration had 2 digits
        if (numbers.indexOf(progmem_buffer[1]) != -1) {
            d = 32 / ((progmem_buffer[0] - '0') * 10 + (progmem_buffer[1] - '0'));
            has_two_digits = true;
        } else
            d = 32 / (progmem_buffer[0] - '0');
        current_note_durations[voice_index] = d;

        // store note
        char j = has_two_digits ? 2 : 1; // stands in the note position
        current_note_strings[voice_index] = progmem_buffer;
        current_note_strings[voice_index] = current_note_strings[voice_index].substring(j);
    }
}

/**
 * Update each voice's duration state. If it's duration expired, load next note.
 *
 * @return if any voice changed it's note
 *  true if one or more voices changed their current notes.
 *  false otherwise
 */
boolean updateVoices() {
    boolean changes_occurred = false;
    for (char i = 0; i < ACTIVE_VOICES_COUNT; i++) {
        if (current_note_durations[i] <= 1) {
            changes_occurred = true;
            // Puts the corresponding note on the progmem_buffer
            loadNextVoiceNotestringToBuffer(i);
            updateVoiceStateFromBuffer(i);
        } else current_note_durations[i]--;
    }
    return changes_occurred;
}

/**
 * Perform a duration tick
 *
 * Each tick may involve updating the current notes being played
 */
void doTick() {
    if (updateVoices()) {
        computeNotePeriods();
        initDurationTickCounters();
    }
}

void setup() {
    // Set voice pins
    for (char i = 0; i < ACTIVE_VOICES_COUNT; i++) {
        voices_output_pins[i] = i + 2;
        pinMode(i + 2, OUTPUT);
    }
    // set indicator led pins
    pinMode(rhythm_led_pin, OUTPUT);
    pinMode(tempo_led_pin, OUTPUT);
    digitalWrite(tempo_led_pin, HIGH);
    // set tempo pot input pin
    pinMode(0, INPUT);

    // Load first note for each voice. Calculate mcd-adjusted note period for each note
    doTick();

    // Setup timers
    cli();
    initTimer1();
    initTimer2();
    sei();
}

/** loop() is almost free for anything else.
 * Notice that Timer0 is still free, so Timer0 functions should kind of work, if the ISR's are quick enough
 */
void loop() {
    tempo = map(analogRead(0), 0, 1023, 30, 500);
    tempo_32th_tick_period = 7500 / (int) (tempo);
    OCR1A = tempo_32th_tick_period * 250;
    delay(250);
}
