# Beeptimer
I disliked all the "alarm" like sound my phone made after a timer. This is therefore an attempt to create a simple C program to use in terminal to have a more soothing sound when the timer is up.

Thanks to open source sounds!

## Requirements
Need [https://www.libsdl.org/](SDL library) to make.

## Installation
Run make

```
$ make
```

## Usage

```
Usage: $ ./Beeptimer [options] (seconds)
The seconds will turn into seconds if provided, but is not in itself necessary as long as any option provided has included numeric.
--help - Shows this information.
-h H   - stands for [h]ours and numeric (H) after is required.
       - Will turn H into H*3600 seconds added before beep.
-m M   - stands for [m]inutes and numeric (M) after is required.
       - Will turn M into M*60 seconds added before beep.
-s S   - stands for [s]econds and numeric (S) after is required.
       - Will turn S into S seconds added before beep.
Any option above can be simultanously provided, and thus be added up.
=== EXAMPLEs ===
'./Beeptimer -m 1 2' is equivalent to
'./Beeptimer -s 2 -m 1' is equivalent to
'./Beeptimer 62'
```
