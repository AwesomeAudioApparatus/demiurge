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

#include "stm32f4xx_hal.h"
#include "demiurge.h"
#include "logger.h"
#include "main.h"

#define TAG "SOUND"

static uint32_t gpio_a;
static uint32_t gpio_b;
static uint32_t gpio_c;

// Figure out later whether the hardware should be initialized in main.c or just moved here.
extern DAC_HandleTypeDef hdac;

extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc2;

extern TIM_HandleTypeDef htim7;  // Timer for realtime clock.

uint32_t demiurge_samplerate;
uint64_t demiurge_current_time;
uint32_t micros_per_tick;

#ifdef DEMIURGE_TICK_TIMING
static uint32_t tick_update = 0;
static volatile uint64_t tick_start = 0;
static volatile uint64_t tick_duration = 0;
static volatile uint64_t tick_interval = 0;
#endif

static volatile signal_t *sinks[DEMIURGE_MAX_SINKS];
float inputs[8] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
float outputs[2] = {0.0f, 0.0f};
float leds[4] = {0.0f, 0.0f};


// Overrun increments means that the tick() took longer than the sample time.
// If you see this happening, either decrease the sample rate or optimize the tick()
// evaluation to take less time.
static uint32_t overrun = -3;  // 3 overruns happen during startup, and that is ok. Let's compensate for that.

void demiurge_registerSink(signal_t *processor) {
   logI(TAG, "Registering Sink: %p", (void *) processor);
   configASSERT(processor != NULL)
   for (int i = 0; i < DEMIURGE_MAX_SINKS; i++) {
      if (sinks[i] == NULL) {
         sinks[i] = processor;
         logI(TAG, "Registering Sink: %d", i);
         break;
      }
   }
}

void demiurge_unregisterSink(signal_t *processor) {
   logI(TAG, "Unregistering Sink: %p", (void *) processor);
   configASSERT(processor != NULL)
   for (int i = 0; i < DEMIURGE_MAX_SINKS; i++) {
      if (sinks[i] == processor) {
         sinks[i] = NULL;
         logI(TAG, "Unregistering Sink: %d, %p", i, (void *) processor);
         break;
      }
   }
}

bool demiurge_gpio(int pin) {
   uint32_t gpios = 0;
   if (pin >= 0 && pin < 16)
      gpios = gpio_a;
   else if (pin >= 16 && pin < 32)
      gpios = gpio_b;
   else if (pin >= 32 && pin < 48)
      gpios = gpio_c;
   return (gpios >> pin & 1) != 0;
}

void demiurge_set_output(int number, float value) {
   configASSERT(number > 0 && number <= 2)
   outputs[number - 1] = value;
}

void demiurge_set_led(int number, float value) {
   configASSERT(number > 0 && number <= 4)
   leds[number - 1] = value;
}

void demiurge_print_overview(const char *tag, signal_t *signal) {
#ifdef DEMIURGE_TICK_TIMING
   logI("TICK", "interval=%lld, duration=%lld, start=%lld, overrun=%d",
            tick_interval, tick_duration, tick_start, overrun);
#endif  //DEMIURGE_TICK_TIMING

   configASSERT(6 > 0 && 6 <= 8)
   configASSERT(8 > 0 && 8 <= 8)
   configASSERT(7 > 0 && 7 <= 8)
   configASSERT(1 > 0 && 1 <= 8)
   configASSERT(2 > 0 && 2 <= 8)
   configASSERT(5 > 0 && 5 <= 8)
   configASSERT(3 > 0 && 3 <= 8)
   configASSERT(4 > 0 && 4 <= 8)
   logI(tag, "Input: %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f",
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
   logI(tag, "Output: %2.1f, %2.1f",
        outputs[0],
        outputs[1]
   );
#ifdef DEMIURGE_DEV
   logI(tag, "Extras: [%lld] : %2.1f - %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f, %2.1f",
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

static inline void readGpio() {
   gpio_a = GPIOA->IDR;
   gpio_b = GPIOB->IDR;
   gpio_c = GPIOC->IDR;
}

static inline void update_dac() {
   uint16_t ch1 = (uint16_t) ((10.0f - outputs[0]) * 204.7f);
   uint16_t ch2 = (uint16_t) ((10.0f - outputs[1]) * 204.7f);
   hdac.Instance->DHR12R1 = ch1;
   hdac.Instance->DHR12R2 = ch2;
}

static inline void update_leds() {
   // TODO:
}

static inline void read_adc() {
   inputs[0] = ((float) hadc1.Instance->DR) / 204.8f;
   inputs[1] = ((float) hadc2.Instance->DR) / 204.8f;
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
   readGpio();
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

void demiurge_start() {
   octave_init();
   micros_per_tick = 1000000 / demiurge_samplerate;
   logI(TAG, "Starting Demiurge. Tick rate: %d microseconds\n", micros_per_tick);

   for (int i = 0; i < DEMIURGE_MAX_SINKS; i++)
      sinks[i] = NULL;

   HAL_ADC_Start(&hadc1);
   HAL_ADC_Start(&hadc2);
   HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
   HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
   HAL_TIM_Base_Start_IT(&htim7);    // start TIMER7
   logI(TAG, "Demiurge Started.");
}

void demiurge_stop(uint64_t rate) {
   for (int i = 0; i < DEMIURGE_MAX_SINKS; i++)
      sinks[i] = NULL;
   HAL_ADC_Stop(&hadc1);
   HAL_ADC_Stop(&hadc2);
   HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
   HAL_DAC_Stop(&hdac, DAC_CHANNEL_2);
   HAL_TIM_Base_Stop(&htim7);    // start TIMER7
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
   HAL_GPIO_WritePin(TP2_GPIO_Port, TP2_Pin, GPIO_PIN_SET);
   __HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);
   demiurge_tick();
   HAL_GPIO_WritePin(TP2_GPIO_Port, TP2_Pin, GPIO_PIN_RESET);
}
#undef TAG
