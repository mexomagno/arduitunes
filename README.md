# Arduitunes

Poliphonic chiptune-like music player. An arduino, some electronic elements, a speaker and you're done! Play the presets or make your own.

#### Some examples


## You will need

* **An arduino board**
  Currently supported boards:
  + Arduino UNO
  + _... feel free to implement support for more boards_
* **Arduino IDE 1.5.8 or newer**
* **1 potentiometer** for varying the tempo
* **1 LED** as a tempo indicator
* **A way of mixing the audio outputs**

## Proposed Setup

Here I propose a quick and dirty circuit that does the job of safely mixing the outputs. I added some more convenience components such as a volume knob.

CIRCUIT

## Usage

1. Clone or download the project
1. Set things ready to be able to load the arduitunes.ino file to your board. I personally use [PlatformIO](http://platformio.org/) that allows me to develop in my [current favorite IDE](https://www.jetbrains.com/clion/).
1. Set things up in your output circuit. Please be careful, as you can blow up your speaker or headphones, as well as damage your hearing if you do this carelessly. Refer to the **Warning** section for details.
1. Load the program to your board
1. Enjoy ;) Play with the tempo if you feel like it

#### Listen to another preset song

Presets are defined as constant strings, as a set of voices. If you succeeded on loading and playing the program (sketch) you should've heard the Pokemon red/blue/yellow battle theme song! The song data is contained between the tags

```cpp
/**** PRESET MELODIES ****/
/** Preset 1 **/
const char voice1_melody[] PROGMEM = {" ... "};
...
const char voiceN_melody[] PROGMEM = {" ... "};
...
/**** END PRESET MELODIES ****/
```

To change the song, just comment out the current song voices and uncomment another set. There are several presets included:
* Super Mario Brothers overworld theme
* Pokemon red/blue/yellow battle theme
* Tiny bit of Attack on titan's first opening song
* Tiny bit of Fullmetal Alchemist's first opening song

#### Making your own tunes

Songs are declared as [RTTTL](https://en.wikipedia.org/wiki/Ring_Tone_Transfer_Language)-like strings for each voice. In [this website](http://www.dcc.uchile.cl/~acastro/arduitunes) I provide an crude, ugly but functional web app with a somehow straight forward way to create tunes in a graphical way. I actually used it to create the preset tunes!

When finished, click **Generate**, copy and paste the resulting text to the sketch, and load it to your board. Be sure to comment out any other song voices.

The web app source is also available in this repo, so improvements are very welcome!

## Warning
* Arduino boards output voltages that are commonly logic 3.3V or 5V. This is **TOO MUCH VOLTAGE** for typical audio applications, so your output circuit should handle this to avoid blowing up your speakers, headphones, eardrums, windows... You're just a google search-away of knowing how to avoid this. 
* If you use my proposed circuit, the more voices are singing at the same time, the higher the total peak to peak voltage at the mixed output. If you need it, evaluate implementing a compressor circuit before the actual audio output

I cannot and won't be responsible for any damage you cause to your audio devices, yourself or someone else.

## Features


## TODO (maybe...)
* Support for triplets
* More instruments (other than square waves)
* MIDI input
* Refactor
  + Use c++ classes to modularize stuff
* Prettify web app
* Add play/preview song in web app editor
* Propose a PCB design for audio output
* Add ADSR envelope support

---------------------

This project exists just for fun and was born as a way of supporting my learning process, as it was one of my first arduino creations.

Comments, suggestions and pull requests are welcome!