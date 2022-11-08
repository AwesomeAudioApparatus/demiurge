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

#include "rgb.h"

#include "demiurge.h"

static uint8_t values[20][3] = {
    { 0,0,0 },
    { 0,0,0x80 },
    { 0,0x80,0 },
    { 0,0x80,0x80 },
    { 0x80,0,0 },
    { 0x80,0,0x80 },
    { 0x80,0x80,0 },
    { 0x80,0x80,0x80 }
};

static void default_fn(float value, uint8_t* rgb)
{
    int index = ((int)(value + 0.5)) % 8;
    uint8_t *leds = values[index];
    rgb[0] = leds[0];
    rgb[1] = leds[1];
    rgb[2] = leds[2];
}

void rgb_init(rgb_t *handle, int position)
{
    configASSERT(position > 0 && position <= DEMIURGE_NUM_LEDS)
    handle->me.read_fn = rgb_read;
    handle->me.data = handle;
#ifdef DEMIURGE_POST_FUNCTION
    handle->me.post_fn = clip_none;
#endif
    handle->position = position - 1;
    handle->registered = false;
    handle->input = NULL;
    handle->leds = rgb_leds(position);
    handle->convert_fn = default_fn;
}

void rgb_configure_conversion( rgb_t *handle, void (*convert_fn)(float, uint8_t* rgb) )
{
    configASSERT(convert_fn != NULL)
    handle->convert_fn = convert_fn;
}

void rgb_configure_input(rgb_t *handle, signal_t *input)
{
    handle->input = input;
    if (!handle->registered)
    {
        demiurge_registerSink(&handle->me);
        handle->registered = true;
    }
}

void rgb_release(rgb_t *handle)
{
    if (!handle->registered)
    {
        demiurge_unregisterSink(&handle->me);
        handle->registered = false;
    }
}

float rgb_read(signal_t *handle, uint64_t time)
{
    if (time > handle->last_calc)
    {
        handle->last_calc = time;
        rgb_t *rgb = (rgb_t *) handle->data;
        signal_t *upstream = rgb->input;
        float out = upstream->read_fn(upstream, time);
#ifdef DEMIURGE_POST_FUNCTION
        out = handle->post_fn(out);
#endif
        rgb->convert_fn(out, rgb->leds);
        handle->cached = out;
        return 0.0f;
    }
    return 0.0f;
}
