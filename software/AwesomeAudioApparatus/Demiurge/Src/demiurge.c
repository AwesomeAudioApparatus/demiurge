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

#include "demiurge.h"
#include "demiurge-spi.h"
#include "rtthread.h"
#include <rtdevice.h>

#define TAG "CORE"

uint32_t demiurge_samplerate;
uint64_t demiurge_current_time;
uint32_t micros_per_tick;

#ifdef DEMIURGE_TICK_TIMING
static uint32_t tick_update = 0;
static volatile uint64_t tick_start = 0;
static volatile uint64_t tick_duration = 0;
static volatile uint64_t tick_interval = 0;
#endif

static demiurge_driver_info_t *driver_info;

static volatile signal_t *sinks[DEMIURGE_MAX_SINKS];

void demiurge_registerSink(signal_t *processor) {
   configASSERT(processor != NULL)
   for (uint32_t i = 0; i < DEMIURGE_MAX_SINKS; i++) {
      if (sinks[i] == NULL) {
         sinks[i] = processor;
         printf("Registering Sink: %u, %p\n", i, (void *) processor);
         break;
      }
   }
}

void demiurge_unregisterSink(signal_t *processor) {
   configASSERT(processor != NULL)
   for (int i = 0; i < DEMIURGE_MAX_SINKS; i++) {
      if (sinks[i] == processor) {
         sinks[i] = NULL;
         printf("Unregistering Sink: %u, %p\n", i, (void *) processor);
         break;
      }
   }
}

void demiurge_set_output(int number, float value) {
   configASSERT(number > 0 && number <= 2)
   outputs[number - 1] = value;
}

void demiurge_set_led(int number, float value) {
   configASSERT(number > 0 && number <= 4)
   leds[number - 1] = value;
}

void set_gate_to_input(int position)
{
    direction_gate(position, false);
}

void set_gate_to_output(int position)
{
    direction_gate(position, true);
}

void demiurge_print_overview(const char *tag, signal_t *signal) {
#ifdef DEMIURGE_TICK_TIMING
   printf("%s interval=%lld, duration=%lld, start=%lld, overrun=%d\n",
            tag, tick_interval, tick_duration, tick_start, overrun);
#endif  //DEMIURGE_TICK_TIMING

   configASSERT(6 > 0 && 6 <= 8)
   configASSERT(8 > 0 && 8 <= 8)
   configASSERT(7 > 0 && 7 <= 8)
   configASSERT(1 > 0 && 1 <= 8)
   configASSERT(2 > 0 && 2 <= 8)
   configASSERT(5 > 0 && 5 <= 8)
   configASSERT(3 > 0 && 3 <= 8)
   configASSERT(4 > 0 && 4 <= 8)
   printf("%s Input: %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f",
        tag,
        inputs[0],
        inputs[1],
        inputs[2],
        inputs[3],
        inputs[4],
        inputs[5],
        inputs[6],
        inputs[7]
   );
   configASSERT(1 > 0 && 1 <= DEMIURGE_MAX_SINKS)
   configASSERT(2 > 0 && 2 <= DEMIURGE_MAX_SINKS)
   printf("%s Output: %2.1f, %2.1f", tag, outputs[0], outputs[1]);
#ifdef DEMIURGE_DEV
   printf("%s Extras: [%lld] : %2.1f - %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f",
        tag,
        signal->last_calc,
        signal->cached,
        signal->extra1,
        signal->extra2,
        signal->extra3,
        signal->extra4,
        signal->extra5,
        signal->extra6,
        signal->extra7,
        signal->extra8
   );
#endif // DEMIURGE_DEV
}

void demiurge_tick() {
   demiurge_current_time+= micros_per_tick;
#ifdef DEMIURGE_TICK_TIMING
   tick_interval = timer_counter - tick_start;
   tick_start = timer_counter;
#endif
   // We are setting the outputs at the start of a cycle, to ensure that the interval is identical from cycle to cycle.
   update_dac();
   update_leds();
   update_gates();
   read_gates();
   read_adc();

   for (int i = 0; i < DEMIURGE_MAX_SINKS; i++) {
      signal_t *sink = sinks[i];
      if (sink != NULL) {
         sink->read_fn(sink, demiurge_current_time);  // ignore return value
      }
   }
#ifdef DEMIURGE_TICK_TIMING
   if (tick_update > 200000) {
      tick_duration = timer_counter - tick_start;
      tick_update = 0;
   }
   tick_update++;
#endif
}

void demiurge_init() {

    octave_init();
    micros_per_tick = 1000000 / demiurge_samplerate;
    driver_info = demiurge_driver_info();
    printf("Sample rate: %ld microseconds\n", micros_per_tick);

    printf("Initializing Demiurge.\n" );
    printf("Driver\n  Name: %s\n", driver_info->name);
    printf("  Inputs: %d\n", driver_info->inputs);
    printf("  Outputs: %d\n", driver_info->outputs);
    printf("  LEDs: %d\n", driver_info->leds);
    printf("  Buttons: %d\n", driver_info->buttons);
    printf("  Potentiometers: %d\n", driver_info->potentionmeters);
    printf("  Display: %s\n", driver_info->display? "yes" : "no");
    printf("  Navigation: %s\n", driver_info->button_navigation ? "yes" : "no");
    printf("  USB: %s\n", driver_info->usb ? "yes" : "no");
    printf("  SDcard: %s\n", driver_info->sdcard ? "yes" : "no");
    printf("  Flash: %d MB\n", driver_info->flash);

    for (int i = 0; i < DEMIURGE_MAX_SINKS; i++)
       sinks[i] = NULL;

}

void demiurge_start() {
    demiurge_driver_start();
    printf("Demiurge Started.\n");
}

void demiurge_stop(uint64_t rate) {
    for (int i = 0; i < DEMIURGE_MAX_SINKS; i++)
        sinks[i] = NULL;
    demiurge_driver_stop();
    printf("Demiurge Stopped.\n");
}

#undef TAG
