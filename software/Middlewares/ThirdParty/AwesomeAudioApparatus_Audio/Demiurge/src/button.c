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

#include "button.h"

#include "clipping.h"
#include "demiurge.h"


void button_init(button_t *handle, int position)
{
    configASSERT(position > 0 && position <= DEMIURGE_NUM_BUTTONS)
    handle->me.read_fn = button_read;
    handle->me.data = handle;
#ifdef DEMIURGE_POST_FUNCTION
    handle->me.post_fn = clip_none;
#endif
    handle->input = &buttons[position - 1];
    handle->mode = BUTTON_MODE_PRESS;
    handle->debounce = 500;
    handle->count = 500;
}

void button_configure_for_press(button_t *handle, float off, float on)
{
    handle->mode = BUTTON_MODE_PRESS;
    handle->output_levels = calloc(2, sizeof(float));
    handle->output_levels[0] = off;
    handle->output_levels[1] = on;
    handle->number_of_steps = 2;
    handle->step = 0;
    handle->me.cached = handle->output_levels[0];  // prime the output from the start
}

void button_configure_for_toggle(button_t *handle, float first, float second)
{
    handle->mode = BUTTON_MODE_TOGGLE;
    handle->output_levels = calloc(2, sizeof(float));
    handle->output_levels[0] = first;
    handle->output_levels[1] = second;
    handle->number_of_steps = 2;
    handle->step = 0;
    handle->me.cached = handle->output_levels[0];  // prime the output from the start
}

void button_configure_for_steps(button_t *handle, int number_of_steps, float *stepValues)
{
    handle->mode = BUTTON_MODE_STEPS;
    handle->output_levels = stepValues;
    handle->number_of_steps = number_of_steps;
    handle->step = 0;
    handle->me.cached = handle->output_levels[0];  // prime the output from the start
}

float button_read(signal_t *handle, uint64_t time)
{
    if (time > handle->last_calc)
    {
        handle->last_calc = time;
        button_t *button = (button_t *) handle->data;
        bool state = *button->input;
        if( state )
        {
            if (state ^ button->last_input)   // changed happened
            {
                int debounce = button->count - 1;
                if (debounce == 0)
                {
                    float new_output = 0;
                    switch (button->mode)
                    {
                        case BUTTON_MODE_PRESS:
                            new_output = state ? button->output_levels[1] : button->output_levels[0];
                            break;
                        case BUTTON_MODE_TOGGLE:
                        case BUTTON_MODE_STEPS:
                            if (state)  // increase at press, not at the release.
                            {
                                button->step = (button->step + 1) % button->number_of_steps;
                                new_output = button->output_levels[button->step];
                            }
                            break;
                    }
                    handle->cached = new_output;
                    button->last_input = state;
                } else
                {
                    button->count = debounce;
                }
            }
        }
        else
        {
            button->count = button->debounce;
            button->last_input = state;
        }
    }
    return handle->cached;
}
