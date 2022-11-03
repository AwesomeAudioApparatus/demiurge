#include "demiurge.h"

// VCO for development

static control_pair_t pair1;  // frequency
static control_pair_t pair2;  // amplitude

static control_pair_t pair3;  // frequency
static control_pair_t pair4;  // amplitude

static audio_outport_t out1;
static audio_outport_t out2;
static oscillator_t oscillator1;
static oscillator_t oscillator2;

void setup()
{
    control_pair_init(&pair1, 1);     // FREQUENCY = CV+Pot at the top
    control_pair_init(&pair2, 2);     // AMPLITUDE = CV+Pot at the second position from the top
    control_pair_init(&pair3, 3);     // FREQUENCY = CV+Pot at the third position from top
    control_pair_init(&pair4, 4);     // AMPLITUDE = CV+Pot at the bottom
    audio_outport_init(&out1, 1);     // Audio Out on left output channel
    audio_outport_init(&out2, 2);     // Audio Out on right output channel

    oscillator_init(&oscillator1);
    oscillator_init(&oscillator2);

    oscillator_configure_mode(&oscillator1, SAW);
    oscillator_configure_mode(&oscillator2, SQUARE);

    oscillator_configure_frequency(&oscillator1, &pair1.me);
    oscillator_configure_frequency(&oscillator2, &pair3.me);

    oscillator_configure_amplitude(&oscillator1, &pair2.me);
    oscillator_configure_amplitude(&oscillator2, &pair4.me);

    audio_outport_configure_input(&out1, &oscillator1.me);
    audio_outport_configure_input(&out2, &oscillator2.me);
}

void loop()
{
}

