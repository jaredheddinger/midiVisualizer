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

### Sequential Print 
The 'p' command will print out the entire file's contents - organized first by tick location then track. 

Controllers will show under tick 0 and track 1. 

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

### Complete Print 

The 'o' command will print out the entire file's contents in global event order. 

```
Track 1
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
NoteOn: C5 velocity: 0
NoteOn: C5 velocity: 80
NoteOn: C5 velocity: 0
NoteOn: C5 velocity: 80
NoteOn: C5 velocity: 0
NoteOn: D5 velocity: 80
NoteOn: D5 velocity: 0
EndOfTrack

```

### Search and Manipulation

The 's' command allows the user to search for any and all midi events of a certain type. For example, searching for all "NoteOn" events will print them out as below: 

```
Action: s
Search Name: NoteOn
Results: 
[1] NoteOn: C5 velocity: 80
[2] NoteOn: C5 velocity: 0
[3] NoteOn: C5 velocity: 80
[4] NoteOn: C5 velocity: 0
[5] NoteOn: C5 velocity: 80
[6] NoteOn: C5 velocity: 0
[7] NoteOn: D5 velocity: 80
[8] NoteOn: D5 velocity: 0
Would you like to do anything with these?: 
```

An 'n' will return the user to the previous options. 

### Edit 

**TODO**

The 'e' command will let the user edit the specfied event, after a search. 

## File Writing 

### Output 

The file loaded can be rewritten out to a new file using the 'a' command. 
