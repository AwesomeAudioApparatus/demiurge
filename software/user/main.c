/*
  Copyright 2019, Awesome Audio Apparatus.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
      limitations under the License.
*/

#include "demiurge-board.h"

extern void vca_prepare();
extern void vca_setup();
extern void vca_loop();

int main(void)
{
    vca_prepare();                  // Prepare the board for the functionality

    demiurge_driver_init();         // Initialize the hardware driver
    demiurge_init();                // Initialize the Demiurge platform

    vca_setup();                    // Set up the functionality
    demiurge_start();               // Start the audio processing
    while(1)
    {
        vca_loop();                 // Give time for LEDs and Buttons
    }
}


