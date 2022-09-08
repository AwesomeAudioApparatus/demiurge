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

#include "demiurge-hardware.h"
extern void vco_prepare();
extern void vco_setup();
extern void vco_loop();

int main(void)
{
    vco_prepare();                  // Prepare the board for the functionality

    demiurge_driver_init();         // Initialize the hardware driver
    demiurge_init();                // Initialize the Demiurge platform
    vco_setup();                    // Set up the functionality
    demiurge_start();               // Start the audio processing
//    int start = 10000000;
//    int count = 10000000;
//    int step = 0;
//    while(1)
//    {
//        vco_loop();                 // Give time for LEDs and Buttons
//
//        /// Testing that interrupts are not interfering
//        if( count-- < 0 ){
//            printf("Hello: %d\n", step++);
//            count = start;
//        }
//    }
}
