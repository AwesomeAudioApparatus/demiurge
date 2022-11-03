#include "demiurge.h"
#include "AwesomeAudioApparatus.Demiurge_conf.h"

void MX_User_Init(void)
{
    demiurge_samplerate = SAMPLERATE;
    demiurge_input_mode(1, TYPE_INPUT1);
    demiurge_input_mode(2, TYPE_INPUT2);
    demiurge_input_mode(3, TYPE_INPUT3);
    demiurge_input_mode(4, TYPE_INPUT4);

    demiurge_output_mode(1, TYPE_OUTPUT1);
    demiurge_output_mode(2, TYPE_OUTPUT2);

    demiurge_driver_init();         // Initialize the hardware driver
    demiurge_init();                // Initialize the Demiurge platform
    setup();                        // Set up the application
    demiurge_start();               // Start the audio processing
}

void MX_User_Process(void)
{
    loop();                         // Loop application slow code.
}

