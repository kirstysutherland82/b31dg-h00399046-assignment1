#ifndef HEADER_H
#define HEADER_H

// define gpio pins
#define DATA 4
#define SYNC 7
#define PB1 2
#define PB2 3

// define unscaled timing parameters (and c which remains unscaled)
#define PARAM_TSYNC 50
#define PARAM_A 800
#define PARAM_B 600
#define C 11
#define PARAM_D 4000

//comment out line 18 to toggle on/off debug mode
//#define DEBUG_MODE

#ifdef DEBUG_MODE
  #define SCALE 1000
#else
  #define SCALE 1
#endif

//scale timings based on if debug mode is toggled
#define TSYNC (PARAM_TSYNC * SCALE)
#define A (PARAM_A * SCALE)
#define B (PARAM_B * SCALE)
#define D (PARAM_D * SCALE)


#endif