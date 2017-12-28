Learning process diary
======================

## Day 1
Looks like the `mcd` concept is not trivial: if some note combination makes interrupts occur too often, it simply doesn't work. This was empirically tested: For some note tuples it works and they sound good together. For others, there's sound but with weird notes. For some, there's simply no sound at all. We should somehow modify frenquency values for the cases that produce too frequent interrupts. Whe should change the frequencies such as there's a relatively big `mcd`. For example, change them all so `mcd=5`.

## Day 2
Note periods (period = 1/frequency) are approximated in such way that there's a global `mcd` for every note. With this, the latter problem is eliminated, without untuning the notes too much. We'll use timer 2 instead of 1, as 2 has a sufficiently big counter.

Now timer 2 is being used, and melodies are specified through [RTTTL-like](https://en.wikipedia.org/wiki/Ring_Tone_Transfer_Language) strings, locally parsed in real time.

Everything works and chords can be played. The goal now is to develop a way to comfortably input music that takes into account every note for each voice. 

Implemented silences.

#### Idea: 
Melodies as a string for each voice.
Protocol: `AB(S)C` or `AS` if it's a silence.

  * **A**: Duration, as a fraction of a whole note
    - 1 = Whole note
    - 2 = Half note
    - ...
    - 32 = 32th note
  * **B** = Tone in american notation (A, B, C, D, E, F, G)
  * **C** = Octave from 0 to 8. 4 is where A=440Hz is located

Separator: Space character ' '
Example: `8C5 8S 8G4 8G4 8A4 8S 8G4 4S 8S 8B4 8S 8C4`
This would be one voice

Each voice should load it's current note and must last what the duration indicates.
Load a note means:
* Update note vector notes. Notice that the vector contains strings
* Define note duration. For this, create a counter for each note that is consumed until 0 is reached, time in which the next note must be loaded. This implies "consuming" the voice string notes somehow.

After loading the note, update each note "count" and period. This is done in `update()`.
Each cycle's delay depends on the specified **tempo**.

## Day 3
Melody reproduction implemented. Problem: Poliphony lost!! :( It only plays the last voice.

> **Problem**
> 
> It's updating **every note** with the last voice. Index error?
> NO: We're overriding the char* reference in the note setting function.
> Fix: Use String instead of char*. Strings are mutable and they change size at will.
> 

> **Problem**
> 
> Notes are repeated 5 times when using 8th notes...
> Fix: There was a problem with the duration counters.


## Day 4

The program is fully working. There's only one problem: The delay produces little undesired silences that in the long run disturb the original melody. Possible fix: Using another timer. 

This fix would trigger an interrupt each `delaytempo` seconds. This variable depends of the song tempo. Assuming that it goes from 30 to 300, `delaytempo` goes from 250ms to 25ms. These are the probable times to be used in the melody reading interrupts. If we use a prescaler of 64, we would need 6250 clicks, and for 250ms 62500. Timer 1 fulfills this requirement as it can loop after 65536 clicks.

Melody reading is now implemented using timer 1 instead of built in delays in  `loop()`. Silences work ok.

Explanation: Although Timer 1 and 2 are independent from each other, each time an ISR is executed, the cpu stops executing anything else, including another ISR routine. So we should only execute stuff when things need to change.

Current note change strategy: Generate interrupts each 32th note (consider as a minimum unit of time).

A minor problem derived from this is that now there are changes triggered even if no note have to change. The interrupt should know when to update.

**Verified**: Sound continuity works even when evaluating an if statement.
**SOLVED**.

> **Problem**
> 
>  Weird noises when finishing the reproduction, and some times during reproduction.
>  * Only when the melody is long
>  * If another voice is added, the total data increases and the noise is back

**Problem identified**: The problem is produced due to low RAM. 
Solution: Use other memory: PROGMEM (flash memory), where the program is stored. This memory is much bigger than the RAM. The drawback is that we can only store constant values, and the current implementation needs to change the voices text. To implement this solution, we have to redesign the melody parsing algorithm, and stop using `string` data type as it is not supported by PROGMEM. With this we lose lot's of code.

**SOLVED**

Project finished. The only big problem left is that for now is the max allowed string length. When big enough, java throws StackOverflow.

## Ideas for the future:

* **[DONE]** Mix the output voices instead of playing each voice in a separated speaker.
* **[DONE]** Implement repetitions (Something like `||:  :||` )
* **[DONE]** Create a friendly melody generator.
* **[DONE]** Let the user vary the tempo using external input
* Implement triplets