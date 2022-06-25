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

#include "clipping.h"
#include "demiurge.h"
#include "demi_asserts.h"
#include "oscillator.h"
#include "octave_per_volt.h"

static bool sine_wave_initialized = false;
static float *sine_wave;

void oscillator_init(oscillator_t *handle) {
   if (!sine_wave_initialized) {
      sine_wave = (float *) calloc(SINEWAVE_SAMPLES, sizeof(float));
      for (int i = 0; i < SINEWAVE_SAMPLES; i++) {
         double radians = M_TWOPI * ((double) i) / ((double) SINEWAVE_SAMPLES);
         sine_wave[i] = arm_sin_f32(radians);
      }
      sine_wave_initialized = true;
   }
   handle->me.read_fn = oscillator_saw;
   handle->me.data = handle;
   handle->me.post_fn = clip_none;
   handle->frequency = NULL;
   handle->amplitude = NULL;
   handle->trigger = NULL;
   handle->lastTrig = 0;
}

void oscillator_configure(oscillator_t *handle, signal_t *freqCtrl, signal_t *amplitudeCtrl, signal_t *trigCtrl) {
   oscillator_configure_frequency(handle, freqCtrl);
   oscillator_configure_amplitude(handle, amplitudeCtrl);
   oscillator_configure_trig(handle, trigCtrl);
}

void oscillator_configure_mode(oscillator_t *handle, oscillator_mode mode) {
   switch (mode) {
      case SAW:
         handle->me.read_fn = oscillator_saw;
         break;
      case SINE:
         handle->me.read_fn = oscillator_sine;
         break;
      case SQUARE:
         handle->me.read_fn = oscillator_square;
         break;
      case TRIANGLE:
         handle->me.read_fn = oscillator_triangle;
         break;
      default:
         break;
   }
}

void oscillator_configure_frequency(oscillator_t *handle, signal_t *control) {
   configASSERT(control != NULL)
   handle->frequency = control;
}

void oscillator_configure_amplitude(oscillator_t *handle, signal_t *control) {
   configASSERT(control != NULL)
   handle->amplitude = control;
}

void oscillator_configure_trig(oscillator_t *handle, signal_t *control) {
   configASSERT(control != NULL)
   handle->trigger = control;
}

static inline float angular_pos(oscillator_t *osc, uint64_t time_in_us) {
   signal_t *trigControl = osc->trigger;
   if (trigControl) {
      float voltage = trigControl->read_fn(trigControl, time_in_us);
      if (osc->lastTrig) {
         // at high, check for FALLING
         if (voltage < 2.0f) {
            osc->lastTrig = false;
         }
      } else {
         if (voltage > 2.2f) {
            osc->lastTrig = true;
            osc->angular_pos = 0.0f;
         }
      }
   }
   signal_t *freqControl = osc->frequency;
   float freq = 440;
   if (freqControl) {
      float voltage = freqControl->read_fn(freqControl, time_in_us);
      freq = octave_frequency_of(voltage);
#ifdef DEMIURGE_DEV
      handle->extra4 = voltage;
   }
   handle->extra5 = freq;
#else
   }
#endif
   float delta = freq / (float) demiurge_samplerate;
   float x = osc->angular_pos + delta;
   if (x >= 1.0f)
      x = 0.0f;
   osc->angular_pos = x;
   return x;
}

// TODO: Should amplitude really be linear?? log2? log10?
static inline float amplitude(oscillator_t *osc, uint64_t time_in_us) {
   signal_t *amplitudeControl = osc->amplitude;
   float amplitude = 1.0f;
   if (amplitudeControl) {
      float voltage = amplitudeControl->read_fn(amplitudeControl, time_in_us);
      // TODO: This is temporary, since current set up doesn't have pot+cv inputs
      amplitude = (10 - voltage) / 10.0f;
      // should probably be
//      amplitude = voltage / 10.0f;
#ifdef DEMIURGE_DEV
      handle->extra6 = voltage;
   }
   handle->extra7 = freq;
#else
   }
#endif
   return amplitude;
}

float oscillator_saw(signal_t *handle, uint64_t time_in_us) {
   if (time_in_us > handle->last_calc) {
      handle->last_calc = time_in_us;
      oscillator_t *osc = (oscillator_t *) handle->data;
      float x = angular_pos(osc, time_in_us);
      float out = (x * 20.0f - 10.0f) * amplitude(osc, time_in_us);
      out = handle->post_fn(out);
#ifdef DEMIURGE_DEV
      handle->extra1 = out;
      handle->extra2 = x;
      handle->extra3 = delta;
#endif
      handle->cached = out;
      return out;
   }
   return handle->cached;
}

float oscillator_triangle(signal_t *handle, uint64_t time_in_us) {
   if (time_in_us > handle->last_calc) {
      handle->last_calc = time_in_us;
      oscillator_t *osc = (oscillator_t *) handle->data;
      float x = angular_pos(osc, time_in_us);
      float out;
      if (x > 0.5f) {
         out = 30 - x * 40.0f;
      } else {
         out = x * 40.0f - 10.0f;
      }
      out = handle->post_fn(out * amplitude(osc, time_in_us));
#ifdef DEMIURGE_DEV
      handle->extra1 = out;
      handle->extra2 = x;
      handle->extra3 = delta;
#endif
      handle->cached = out;
      return out;
   }
   return handle->cached;
}

float oscillator_sine(signal_t *handle, uint64_t time_in_us) {
   if (time_in_us > handle->last_calc) {
      handle->last_calc = time_in_us;
      oscillator_t *osc = (oscillator_t *) handle->data;
      float x = angular_pos(osc, time_in_us);
      float out = arm_sin_f32(M_TWOPI * x) * 10 * amplitude(osc, time_in_us);
      // Optimized
//      int idx = (int) (x * SINEWAVE_SAMPLES);
//      float out;
//      if (idx >= SINEWAVE_SAMPLES)
//         out = osc->scale;
//      else
//         out = sine_wave[idx] * osc->scale;
      out = handle->post_fn(out);
#ifdef DEMIURGE_DEV
      handle->extra1 = out;
      handle->extra2 = x;
      handle->extra3 = delta;
#endif
      handle->cached = out;
      return out;
   }
   return handle->cached;
}

float oscillator_square(signal_t *handle, uint64_t time_in_us) {
   if (time_in_us > handle->last_calc) {
      handle->last_calc = time_in_us;
      oscillator_t *osc = (oscillator_t *) handle->data;
      float x = angular_pos(osc, time_in_us);
      float out;
      if (x > 0.5f)
         out = 10.0f;
      else
         out = -10.0f;
      out = handle->post_fn(out * amplitude(osc, time_in_us));
#ifdef DEMIURGE_DEV
      handle->extra1 = out;
      handle->extra2 = x;
      handle->extra3 = delta;
#endif
      handle->cached = out;
      return out;
   }
   return handle->cached;
}

