# Mental Model
The mental model for the Demiurge Sound Processing Engine (DSPE) is tha of
blocks of computations that are wired together in the directed acyclical
graph (DAG) where the wires represents audio, CV or gate voltage levels.

The simple example can be seen in the [README file](../README.md), but a more
explicit example is seen below.

![Visual of Block model](blocks-vca.svg)

And to code that up, we need to declare each of the ***Inputs***, ***Outputs***,
***ControlPairs*** and the ***Mixer***, as follows

```C
#include "demiurge.h"

static audio_inport_t in1;      // Declaration of Audio Input Ports
static audio_inport_t in2;

static control_pair_t pair1;    // Declaration of CV+Potentiometer Input pairs
static control_pair_t pair2;

static audio_outport_t out1;    // Declaration of Audio Output Ports
static audio_outport_t out2;

static mixer_t mixer;           // Declaration of a Mixer block

/*
 * A two port Mixer, with CV control
 */
void setup() {
// Initialize the hardware configuration
control_pair_init(&pair1, 1);       // CV+Pot at the top of Demiurge
control_pair_init(&pair2, 2);       // CV+Pot at the second position from the top of Demiurge
audio_inport_init(&in1, 3);         // Audio In on third input from the top
audio_inport_init(&in2, 4);         // Audio In on fourth input from the top
audio_outport_init(&out1, 1);       // Audio Out on left output channel
audio_outport_init(&out2, 2);       // Audio Out on right output channel

// Initialize Mixer with 2 channels.
mixer_init(&mixer, 2);

// Connect in1 on mixer channel 1, with pair1 as the volume control
mixer_configure_input(&mixer, 1, &in1.me, &pair1.me);

// Connect in2 on mixer channel 2, with pair2 as the volume control
mixer_configure_input(&mixer, 2, &in2.me, &pair2.me);

// Connect mixer output to out1
audio_outport_configure_input(&out1, &mixer.me);

// Connect mixer output to out2
audio_outport_configure_input(&out2, &mixer.me);
}

void loop() {
}
```

The `setup()` and `loop()` functions are also custom, just to create a
familiarity from the Arduino environment. In reality, this is defined in
our `main.c` file, after starting the DSPE and part of the creatioin of
a Demiurge project for the STM32CubeMX and optionally the STM32CubeIDE.

```C
   demiurge_start();
   setup();
   while (1) {
      loop();
```
