/*
  Copyright 2019-2022, Awesome Audio Apparatus.

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

#include "oscillator.h"

#include <math.h>
#include <float.h>

#include "clipping.h"
#include "demiurge.h"
#include "demi_asserts.h"
#include "octave_per_volt.h"

// static bool sine_wave_initialized = false;
// static float *sine_wave;

void oscillator_init(oscillator_t *handle)
{
// Lookup of sine wave function from a table is slower than simply execute sinf() on CH32V307
//   if (!sine_wave_initialized) {
//      sine_wave = (float *) calloc(SINEWAVE_SAMPLES, sizeof(float));
//      for (int i = 0; i < SINEWAVE_SAMPLES; i++) {
//         double radians = TWO_PI * ((double) i) / ((double) SINEWAVE_SAMPLES);
//         sine_wave[i] = arm_sin_f32(radians);
//         sine_wave[i] = sinf(radians);
//      }
//      sine_wave_initialized = true;
//   }
    handle->me.read_fn = oscillator_saw;
    handle->me.data = handle;
#ifdef DEMIURGE_POST_FUNCTION
    handle->me.post_fn = clip_none;
#endif
    handle->frequency = NULL;
    handle->amplitude = NULL;
    handle->trigger = NULL;
    handle->lastTrig = 0;
}

void oscillator_configure(oscillator_t *handle, signal_t *freqCtrl, signal_t *amplitudeCtrl, signal_t *trigCtrl)
{
    oscillator_configure_frequency(handle, freqCtrl);
    oscillator_configure_amplitude(handle, amplitudeCtrl);
    oscillator_configure_trig(handle, trigCtrl);
}

void oscillator_configure_mode(oscillator_t *handle, oscillator_mode mode)
{
    switch (mode)
    {
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
        case SAW_DIGITIZED:
            handle->me.read_fn = oscillator_saw_digitized;
            break;
        case SINE_DIGITIZED:
            handle->me.read_fn = oscillator_sine_digitized;
            break;
        case SQUARE_DIGITIZED:
            handle->me.read_fn = oscillator_square_digitized;
            break;
        case TRIANGLE_DIGITIZED:
            handle->me.read_fn = oscillator_triangle_digitized;
            break;
        default:
            break;
    }
}

void oscillator_configure_frequency(oscillator_t *handle, signal_t *control)
{
    configASSERT(control != NULL)
    handle->frequency = control;
}

void oscillator_configure_amplitude(oscillator_t *handle, signal_t *control)
{
    configASSERT(control != NULL)
    handle->amplitude = control;
}

void oscillator_configure_trig(oscillator_t *handle, signal_t *control)
{
    configASSERT(control != NULL)
    handle->trigger = control;
}

static inline float angular_pos(oscillator_t *osc, uint64_t time_in_us)
{
    signal_t *trigControl = osc->trigger;
    if (trigControl)
    {
        float voltage = trigControl->read_fn(trigControl, time_in_us);
        if (osc->lastTrig)
        {
            // at high, check for FALLING
            if (voltage < 2.0f)
            {
                osc->lastTrig = false;
            }
        } else
        {
            if (voltage > 2.2f)
            {
                osc->lastTrig = true;
                osc->angular_pos = 0.0f;
            }
        }
    }
    signal_t *freqControl = osc->frequency;
    float freq;
    if (freqControl)
    {
        float voltage = freqControl->read_fn(freqControl, time_in_us);
        freq = octave_frequency_of(voltage);
#ifdef DEMIURGE_DEV
        handle->extra4 = voltage;
    } else {
       freq = 440;
    }
    handle->extra5 = freq;
#else
    } else
    {
        freq = 440;
    }
#endif
    float delta = freq / (float) demiurge_samplerate;
    float x = osc->angular_pos + delta;
    if (x >= TWO_PI)
        x = 0.0f;
    osc->angular_pos = x;
    return x;
}

// Returns 0-1
static inline float amplitude(oscillator_t *osc, uint64_t time_in_us)
{
    signal_t *amplitudeControl = osc->amplitude;
    float amplitude = 2.0f;
    if (amplitudeControl)
    {
        float voltage = amplitudeControl->read_fn(amplitudeControl, time_in_us);
        amplitude = (voltage + 10) / 20.0f;  // results in 0-1
        // should perhaps be the following, so slow saw tooth can be inverted? Or maybe it should be selectable by programmer.
        //  amplitude = voltage / 10.0f;
#ifdef DEMIURGE_DEV
        handle->extra6 = voltage;
    }
    handle->extra7 = freq;
#else
    }
#endif
    return amplitude;
}

float oscillator_saw_digitized(signal_t *handle, uint64_t time_in_us)
{
    return digitize(oscillator_saw(handle, time_in_us));
}

float oscillator_saw(signal_t *handle, uint64_t time_in_us)
{
    if (time_in_us > handle->last_calc)
    {
        handle->last_calc = time_in_us;
        oscillator_t *osc = (oscillator_t *) handle->data;
        float x = angular_pos(osc, time_in_us);  // 0 - 2PI
        float out =
            x * 3.183098861f * amplitude(osc, time_in_us) - 10.0f; // multiply to a 0-20V range, subtract to -10V to 10V
#ifdef DEMIURGE_POST_FUNCTION
        out = handle->post_fn(out);
#endif
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

float oscillator_triangle_digitized(signal_t *handle, uint64_t time_in_us)
{
    return digitize(oscillator_triangle(handle, time_in_us));
}

float oscillator_triangle(signal_t *handle, uint64_t time_in_us)
{
    if (time_in_us > handle->last_calc)
    {
        handle->last_calc = time_in_us;
        oscillator_t *osc = (oscillator_t *) handle->data;
        float x = angular_pos(osc, time_in_us);             // 0 to 2PI
        float out;
        if (x < M_PI)
        {
            out = x * (10 / M_PI) - 5.0f;
        } else
        {
            out = -x * (10 / M_PI) + 15.0f;
        }
        out = out * amplitude(osc, time_in_us);
#ifdef DEMIURGE_POST_FUNCTION
        out = handle->post_fn(out);
#endif
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

float oscillator_sine_digitized(signal_t *handle, uint64_t time_in_us)
{
    return digitize(oscillator_sine(handle, time_in_us));
}

float oscillator_sine(signal_t *handle, uint64_t time_in_us)
{
    if (time_in_us > handle->last_calc)
    {
        handle->last_calc = time_in_us;
        oscillator_t *osc = (oscillator_t *) handle->data;
        float x = angular_pos(osc, time_in_us);
//      float out = arm_sin_f32(TWO_PI * x) * 10 * amplitude(osc, time_in_us);
        float out = sinf(x) * 10 * amplitude(osc, time_in_us);
        // Below is SLOWER on ch32v307
//      int idx = (int) (x * SINEWAVE_SAMPLES);
//      float out;
//      if (idx >= SINEWAVE_SAMPLES)
//         out = amplitude(osc, time_in_us);
//      else
//         out = sine_wave[idx] * amplitude(osc, time_in_us);
#ifdef DEMIURGE_POST_FUNCTION
        out = handle->post_fn(out);
#endif
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

float oscillator_square_digitized(signal_t *handle, uint64_t time_in_us)
{
    return digitize(oscillator_square(handle, time_in_us));
}

float oscillator_square(signal_t *handle, uint64_t time_in_us)
{
    if (time_in_us > handle->last_calc)
    {
        handle->last_calc = time_in_us;
        oscillator_t *osc = (oscillator_t *) handle->data;
        float x = angular_pos(osc, time_in_us);
        float out;
        if (x > M_PI)
            out = 5.0f;
        else
            out = -5.0f;
        out = out * amplitude(osc, time_in_us);
#ifdef DEMIURGE_POST_FUNCTION
        out = handle->post_fn(out);
#endif
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

