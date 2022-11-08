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

#include "octave_per_volt.h"

#include <stdint.h>

#ifndef OCTAVE_LOOKUP_SIZE
#define OCTAVE_LOOKUP_SIZE 4096
#endif

static float octave_freq[OCTAVE_LOOKUP_SIZE];
static double octave_step;

static float digitized[3952];
static float semitones[] = {32.70, 34.65, 36.71, 38.89, 41.20, 43.65, 46.25,
                            49.00, 51.91, 55.00, 58.27, 61.74, 65.41, 69.30, 73.42, 77.78, 82.41,
                            87.31, 92.50, 98.00, 103.83, 110.00, 116.54, 123.47, 130.81, 138.59,
                            146.83, 155.56, 164.81, 174.61, 185.00, 196.00, 207.65, 220.00, 233.08,
                            246.94, 261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00,
                            415.30, 440.00, 466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.26,
                            698.46, 739.99, 783.99, 830.61, 880.00, 932.33, 987.77, 1046.50,
                            1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22,
                            1760.00, 1864.66, 1975.53, 2093.00, 2217.46, 2349.32, 2489.02, 2637.02,
                            2793.83, 2959.96, 3135.96, 3322.44, 3520.00, 3729.31, 3951.07, 4186.01
};


/** Octave per Volt uses a lookup table to ensure speed in run time, as
 * the 2^Volt is too slow. The table is populated with a -10V to +10V
 * range, giving 4096 output_levels for ~1/15 Hz to 65kHz, which is way wider range
 * than needed.
 *
 * It is up to the rest of the system to keep the input voltage to this
 * function "block" within the reasonable range for the application.
 */
void octave_init()
{
    octave_step = 20.0f / OCTAVE_LOOKUP_SIZE;
    double base = 440.0f / pow(2.0, 2.75);
    double volt = -10.0f;
    for (int i = 0; i < OCTAVE_LOOKUP_SIZE; i++)
    {
        octave_freq[i] = (float) (base * pow(2.0, volt));
        volt = volt + octave_step;
    }
    int pos = 0;
    float semitone = semitones[pos++];
    for (int i = 0; i < 3952; i++)
    {
        digitized[i] = semitone;
        if (i > (semitones[pos] - (semitones[pos] - semitones[pos - 1]) / 2))
            semitone = semitones[pos++];
    }
}

float octave_frequency_of(float voltage)
{
// Too slow. Doing pre-calculated and lookup instead.
//   float result = 65.40639133f * pow(2, voltage);
    int pos = (voltage + 8.0) / octave_step;
    if (pos >= OCTAVE_LOOKUP_SIZE)
        pos = OCTAVE_LOOKUP_SIZE - 1;
    if (pos < 0)
        pos = 0;
    return octave_freq[pos];
}

float octave_voltage_of(float frequency)
{
    return log2f(frequency / 27.5);
}

/** This function will convert the incoming frequency to the nearest
 * semi-tone
 */
float digitize(float frequency)
{
    // The digitization is pre-calculated into a lookup table.
    // This is quite wasteful with RAM, but since the RAM isn't
    // used for much else, it is a worthwhile optimization.
    if (frequency > 3951)
    {
        if (frequency > 4068)
            return 4186.01;
        else
            return 3951.07;
    }
    return digitized[(int) frequency];
}

