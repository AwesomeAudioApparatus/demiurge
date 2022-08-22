# Demiurge

Demiurge is a combination of Hardware and Software for Digital Sound Processing in the Eurorack Modular Synthesizer environment.

The "Demiurge 1" board is a 4 HP Eurorack compatible board, the functionality defined in software and custom front panel can either 
be made by hand, ordered from local mechanical shops or ordered from PCB manufacturers (KiCad template for front panel exists).

The "Demiurge Sound Processing Engine" is a system to more easily create Eurorack module functionality. A simple VCO module may look like this;

```C
#include "demiurge.h"
#include "demiurge-board.h"

static control_pair_t pair1;  // frequency
static control_pair_t pair2;  // amplitude

static control_pair_t pair3;  // frequency
static control_pair_t pair4;  // amplitude

static audio_outport_t out1;
static audio_outport_t out2;
static oscillator_t oscillator1;
static oscillator_t oscillator2;

/* Simple 2 VCO. SAW on output 1 and SQUARE on output 2. */

void vco_prepare() {
    demiurge_samplerate = 48000;      // 48000 samples/second
    demiurge_set_inport_cv(1);
    demiurge_set_inport_cv(2);
    demiurge_set_inport_cv(3);
    demiurge_set_inport_cv(4);

    demiurge_set_outport_audio(1);
    demiurge_set_outport_audio(2);
}

void vco_setup() {
    // Initialize the hardware configuration
    control_pair_init(&pair1, 1);     // FREQUENCY = CV+Pot at the top
    control_pair_init(&pair2, 2);     // AMPLITUDE = CV+Pot at the second position from the top
    control_pair_init(&pair3, 3);     // FREQUENCY = CV+Pot at the third position from top
    control_pair_init(&pair4, 4);     // AMPLITUDE = CV+Pot at the bottom
    audio_outport_init(&out1, 1);     // Audio Out on left output channel
    audio_outport_init(&out2, 2);     // Audio Out on right output channel

    // Initialize the oscillators
    oscillator_init(&oscillator1);
    oscillator_init(&oscillator2);

    // Set up the oscillators to SINE wave form

    oscillator_configure_mode(&oscillator1, SAW);
    oscillator_configure_mode(&oscillator2, SQUARE);

    oscillator_configure_frequency(&oscillator1, &pair1.me);
    oscillator_configure_frequency(&oscillator2, &pair3.me);

    oscillator_configure_amplitude(&oscillator1, &pair2.me);
    oscillator_configure_amplitude(&oscillator2, &pair4.me);

    // Connect Oscillator to outputs
    audio_outport_configure_input(&out1, &oscillator1.me);
    audio_outport_configure_input(&out2, &oscillator2.me);
}

void vco_loop() {
   // handle buttons and LEDs
}

int main(void)
{
    vco_prepare();                  // Prepare the board for the functionality

    demiurge_driver_init();         // Initialize the hardware driver
    demiurge_init();                // Initialize the Demiurge platform
    vco_setup();                    // Set up the functionality
    demiurge_start();               // Start the audio processing
    while(1)
    {
        vco_loop();                 // Give time for LEDs and Buttons
    }
}

```
