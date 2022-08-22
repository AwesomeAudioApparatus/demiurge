# Mental Model
The mental model for the Demiurge Sound Processing Engine (DSPE) is that of
blocks of computations that are wired together in the directed acyclical
graph (DAG) where the wires represents audio, CV or gate/trig voltage levels.

The simple example can be seen in the [README file](../README.md), but a more
explicit example is seen below.

![Visual of Block model](blocks-vca.svg)

And to code that up, we need to declare each of the ***Inputs***, ***Outputs***,
***ControlPairs*** and the ***Mixer***, as follows

```C
#include "demiurge.h"
#include "demiurge-board.h"

static audio_inport_t input1;      // Declaration of Audio Input Ports
static audio_inport_t input2;

static control_pair_t pair1;    // Declaration of CV+Potentiometer Input pairs
static control_pair_t pair2;

static audio_outport_t out1;    // Declaration of Audio Output Ports
static audio_outport_t out2;

static scale_t scale1;
static scale_t scale2;

/*
 * Dual VCA.
 */
void vca_prepare() {
    demiurge_samplerate = 50000;     // 30000 samples/second
    demiurge_set_inport_audio(1);    // set the first jack as Audio input.
    demiurge_set_inport_cv(2);       // set the second jack as Control Voltage input
    demiurge_set_inport_audio(3);    // set the third jack as Audio input.
    demiurge_set_inport_cv(4);       // set the fourth jack as Control Voltage input
    
    demiurge_set_potentiometer(2, 0.0f, 10.0f);  // Potentiometers to a 0-10 range
    demiurge_set_potentiometer(4, 0.0f, 10.0f);
    
    demiurge_set_outport_audio(1);   // Set the outputs to Audio output.
    demiurge_set_outport_audio(2);
}

void vca_setup() {

    control_pair_init(&pair1, 2);       // CV+Pot at the second position from the top of Demiurge
    control_pair_init(&pair2, 4);       // CV+Pot at the fourth position from the top of Demiurge
    audio_inport_init(&input1, 1);         // Audio In on first input from the top
    audio_inport_init(&input2, 3);         // Audio In on third input from the top
    audio_outport_init(&out1, 1);       // Audio Out on left output channel
    audio_outport_init(&out2, 2);       // Audio Out on right output channel
    
    scale_init(&scale1);
    scale_configure(&scale1, &input1.me, &pair1.me);  //
    scale1.scale = 0.1f;
    
    scale_init(&scale2);
    scale_configure(&scale2, &input2.me, &pair2.me);
    scale2.scale = 0.1f;
    
    // Connect mixer output to out1
    audio_outport_configure_input(&out1, &scale1.me);
    
    // Connect mixer output to out2
    audio_outport_configure_input(&out2, &scale2.me);
}

void vca_loop() {
}
```

The `setup()` and `loop()` functions are also custom, just to create a
familiarity from the Arduino environment. In reality, this is defined in
our `main.c` file, after starting the DSPE and part of the creatioin of
a Demiurge project for the STM32CubeMX and optionally the STM32CubeIDE.

```C
    vca_prepare();                  // Prepare the board for the functionality
    demiurge_driver_init();         // Initialize the hardware driver
    demiurge_init();                // Initialize the Demiurge platform
    vca_setup();                    // Set up the functionality
    demiurge_start();               // Start the audio processing
    while(1)
    {
        vca_loop();                 // Give time for LEDs and Buttons
    } 
```
