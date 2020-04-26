# midiVisualizer
Command line app for directly visualizing and manipulating midi events/messages in a file.

## File Reading

Midi Visualizer will read any standard compliant .mid file: 

![alt-text](https://github.com/jaredheddinger/midiVisualizer/blob/master/Resources/SampleScore.png)
```
$ ./midiVisualizer 
File: d.mid
Reading: d.mid
Action:
```

### Search 

### Sequential Print 
The score above, printed sequentially: 

```
================================================================================
0    
Track 1:
TimeSignature: 4/4 (24 clocks per metronome click, 8 notes in quarter note)
KeySignature: C major
Controller: Reset Controllers, Value = 0
ProgramChange: Program Piano
Controller: Track Volume, Value = 100
Controller: Pan, Value = 64
Controller: Effect 1 Depth, Value = 0
Controller: Effect 3 Depth, Value = 0
MIDIPort
NoteOn: C5 velocity: 80
EndOfTrack
455 
Track 1:
NoteOn: C5 velocity: 0
480 
Track 1:
NoteOn: C5 velocity: 80
935 
Track 1:
NoteOn: C5 velocity: 0
960 
Track 1:
NoteOn: C5 velocity: 80
1415
Track 1:
NoteOn: C5 velocity: 0
1440
Track 1:
NoteOn: D5 velocity: 80
1895
Track 1:
NoteOn: D5 velocity: 0
```

### Master Print 

## File Writing 

### Output 
