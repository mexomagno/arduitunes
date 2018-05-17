# Arduitunes

A poliphonic chiptune-like music player for the Arduino Uno!

## Samples:

[Pokemon Red, Blue and Yellow Trainer battle music](https://www.youtube.com/watch?v=Nx4-dWHNMjs)

[Sweet Child of Mine](https://www.youtube.com/watch?v=4CrpFCbZ7Xw) by Jean Henrique, using **Arduitunes** and an electric arc speaker. Pretty cool.

## Usage

1. Clone/download the project
1. Load the arduitunes.ino file to your board. I personally use [PlatformIO](http://platformio.org/) that allows me to develop in my [current favorite IDE](https://www.jetbrains.com/clion/). **Important**: You must have already installed the Arduino IDE v1.5.8 or newer.
1. Set up your circuit. See my [proposed setup](#proposed-setup), but please, read the [**Warning**](#warning) section before building it.
1. Load the program to your board
1. Listen! Play with the tempo if you feel like it

#### <a id="proposed-setup"> Proposed Setup </a>

Here I propose a quick and dirty circuit that does the job of safely mixing the outputs. I added some more convenience components such as a volume knob.

![proposed setup sketch](/resources/proposed_sketch.png)

This sketch's parts are:
* An Arduino UNO board
* Two potentiometers (tempo, volume)
* Two LED diodes (tempo, sound indicator)
* 7 300Ω resistors. 5 for the passive mixer, 2 for the LEDs.
* 1 1kΩ resistor
* 1 10kΩ resistor
* 1 audio mono jack

## Changing the preset song

Presets are described as a set of constant strings, each one representing an individual voice. If you succeeded on loading and playing the program (sketch) you should've heard the Pokemon red/blue/yellow battle theme song.

The song data is contained on the code inside the following commented tags:

```cpp
/**** PRESET MELODIES ****/
/** Preset 1 **/
const char voice1_melody[] PROGMEM = {" ... "};
...
const char voiceN_melody[] PROGMEM = {" ... "};
...
/**** END PRESET MELODIES ****/
```

To change the song, just comment out the current song voices and uncomment another set. 

Please that only one song at a time can be active.

Included tunes are:
* Super Mario Brothers overworld theme
* Pokemon red/blue/yellow battle theme
* Tiny bit of Attack on titan's first opening song
* Tiny bit of Fullmetal Alchemist's first opening song

## Making your own tunes

Songs are declared as [RTTTL](https://en.wikipedia.org/wiki/Ring_Tone_Transfer_Language)-like strings for each voice. I created an [ugly but functional webapp](http://www.dcc.uchile.cl/~acastro/arduitunes) which is a somehow easy way to create tunes. I actually used it to create most of the preset tunes.

When done, click **Generate**, copy and paste the resulting text to the sketch, and load it to your board. Remember to define only one tune at a time when loading the code.

The web app's source code is available in this repo too, if you feel like you want to make it better.

> ## <a id="warning">Warning</a>
> 
> Arduino boards output voltages that are commonly logic 3.3V or 5V. This is **TOO MUCH VOLTAGE** for typical audio applications, so your output circuit should handle this to avoid blowing up your speakers, headphones, eardrums, windows... You're just a google search-away of knowing how to avoid this. With my [proposed circuit](#proposed-setup) though, you should be safe. 
> 
> In the proposed circuit, the more voices are singing at the same time, the higher the total peak to peak voltage at the mixed output. If you need it, evaluate implementing a compressor circuit before the actual audio output.
>
> I cannot and won't be responsible for any damage you cause to your arduino, audio devices, yourself or someone else. If you don't know what you're doing I strongly advice you do your research first.


## Features
* True polyphonic harmony
* Mostly accurate note frequencies (equal temperament)
* Note range from G2 to D#8
* Repetitions support **||:  :||**
* Adjustable tempo
* Only a few simple passive components required (plus the arduino)


## What never got to be done
* Support for triplets (and more complex beats)
* More instruments (other than square waves)
* MIDI support
* OOP paradigm
* Decent webapp development
  * Online tune preview
* PCB design to plug and play
* [ADSR envelope](https://www.wikiaudio.org/adsr-envelope/) support


---------------------
## How it works

Everything that oscillates at a relatively constant rate is potentially a musical note: A metal rod when you hit it, a tight string when you strum it, a hollow tube when you blow on it, and even a digital output pin when you toggle it. 

As you may know, every known musical note has a defined frequency in the very common **A 440** equal temperament tuning standard. If you connect a digital pin to something that can produce sound when you toggle it from 0 to 1 in a controlled fashion, then you can reproduce precise notes. The real challenge though, comes when you want to produce various notes at the same time, on a device that is physically constructed to do just one thing at a time: to execute one instruction at a time.

The approach followed in this project is inspired on [this article](https://www.jeremyblum.com/2010/09/05/driving-5-speakers-simultaneously-with-an-arduino/) from Jeremy Blum's blog. If you get this, then you already understand the complicated part of Arduitunes.


## Why I cherish Arduitunes

It was my first Arduino project. When I first saw an arduino blink an LED, I immediately fell in love with the platform, and I, as a music lover, could not get out of my head the idea of making this thing produce some sound.

So, I ended up developing this, which ended up involving some research on the actual Atmega chip guts, that was both scary and thrilling. The sincere joy this process brought to my soul was a kind surprise for me, I didn't know I liked this kind of stuff so much. 

Arduitunes is what taught me the level of satisfaction that messing around with electronics could provide, which was a previously unknown world to me.

## Want to contribute?

Merge requests, comments, suggestions and feedback in general will always be appreciated!