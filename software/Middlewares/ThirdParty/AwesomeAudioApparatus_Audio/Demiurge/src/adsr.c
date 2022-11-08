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
#include "adsr.h"

#include <math.h>

#include "clipping.h"
#include "signal.h"

float slope_log10_6magnitudes(float voltage)
{
    // Logarithmic response, so that;
    // 0V = 10 millisecond, 5V = 1 second, +10V = 100 second
    // TODO:
    float seconds = powf(10.0, (voltage / 3.333333333f) - 2);
    return seconds;
}

void adsr_init(adsr_t *handle, float min, float max)
{
    handle->me.read_fn = adsr_read;
    handle->attack_slope_fn = slope_log10_6magnitudes;
    handle->decay_slope_fn = slope_log10_6magnitudes;
    handle->release_slope_fn = slope_log10_6magnitudes;
    handle->me.data = handle;
#ifdef DEMIURGE_POST_FUNCTION
    handle->me.post_fn = clip_none;
#endif
    handle->min = min;
    handle->max = max;
    handle->range = max - min;
    handle->stateMachine = 0;
    handle->doneAt = 0;
    handle->startedAt = 0;
    threshold_init(&handle->trigThreshold, 0.0f, 1.0f);
    threshold_init(&handle->gateThreshold, 0.0f, 1.0f);
}

void adsr_configure_gate(adsr_t *handle, signal_t *control)
{
    handle->gate = control;
}

void adsr_configure_attack(adsr_t *handle, signal_t *control)
{
    handle->attack = control;
}

void adsr_configure_decay(adsr_t *handle, signal_t *control)
{
    handle->decay = control;
}

void adsr_configure_sustain(adsr_t *handle, signal_t *control)
{
    handle->sustain = control;
}

void adsr_configure_release(adsr_t *handle, signal_t *control)
{
    handle->release = control;
}

void adsr_configure_trig(adsr_t *handle, signal_t *control)
{
    handle->trig = control;
}

// TODO: without floating point, this code becomes VERY different, so let's postpone that/
float adsr_read(signal_t *handle, uint64_t time)
{
    adsr_t *adsr = (adsr_t *) handle->data;
    if (time > handle->last_calc)
    {
        handle->last_calc = time;
        float output;

        bool trigIn = threshold_compute(&adsr->trigThreshold, adsr->trig->read_fn(adsr->trig, time));
        bool gateIn = threshold_compute(&adsr->gateThreshold, adsr->gate->read_fn(adsr->gate, time));
#ifdef DEMIURGE_DEV
        handle->extra1 = trigIn;
        handle->extra2 = gateIn;
#endif
        if (!adsr->currentTrig && trigIn)
        {
            // RISE
            adsr->stateMachine = 1;
            adsr->startedAt = time;
        }
        adsr->currentTrig = trigIn;

        if (gateIn)
        {
            // Ongoing
            switch (adsr->stateMachine)
            {
                case 1:  // Attack
                {
                    float attackIn = adsr->attack->read_fn(adsr->attack, time);
                    float slope_time = adsr->attack_slope_fn(attackIn);                 // attack time in seconds.
                    float k = (adsr->range / slope_time) / 1000000;              // voltage change per microsecond
                    float m = adsr->min;
                    float duration_so_far = time - adsr->startedAt;           // time since trig in microseconds
                    output = duration_so_far * k + m;
                    if (output >= adsr->max)
                    {
                        output = adsr->max;
                        adsr->startedAt = time;
                        adsr->stateMachine++;
                    }
                    break;
                }
                case 2:  // Decay
                {
                    float sustainIn = adsr->sustain->read_fn(adsr->sustain, time);
                    float decayIn = adsr->decay->read_fn(adsr->decay, time);
                    float slope_time = adsr->decay_slope_fn(decayIn);
                    float k = ((adsr->max - sustainIn) / slope_time) / 1000000; // voltage change per microsecond
                    uint64_t duration_so_far = time - adsr->startedAt;    // in microseconds
                    output = adsr->max - duration_so_far * k;
                    if (output <= sustainIn)
                    {
                        output = sustainIn;
                        adsr->startedAt = time;
                        adsr->stateMachine++;
                    }
                    break;
                }
                case 3:  // Sustain
                {
                    output = adsr->sustain->read_fn(adsr->sustain, time);
                    adsr->startedAt = time;     // this will be frozen at the time the 'release' starts.
                    break;
                }
                default:
                    output = -adsr->min;
            }
        } else
        {
            float releaseIn = adsr->release->read_fn(adsr->release, time);
            float sustainIn = adsr->sustain->read_fn(adsr->sustain, time);
            float k = ((sustainIn - adsr->min) / adsr->release_slope_fn(releaseIn)) /
                      1000000; // voltage change per microsecond
            uint64_t duration_so_far = time - adsr->startedAt;    // in microseconds
            output = sustainIn - duration_so_far * k;
            if (output < adsr->min)
            {
                output = adsr->min;
            }
            if (output > adsr->max)
            {
                output = adsr->max;
            }
            if (gateIn)
            {
                // Start new cycle
                adsr->stateMachine = 0;
            }
        }

#ifdef DEMIURGE_POST_FUNCTION
        output = handle->post_fn(output);
#endif
        handle->cached = output;
#ifdef DEMIURGE_DEV
        handle->extra4 = adsr->stateMachine;
#endif
        return output;
    }
    return handle->cached;
}

