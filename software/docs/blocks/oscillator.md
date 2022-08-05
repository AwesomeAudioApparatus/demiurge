# Oscillator
The Oscillator is a block that generates waveforms at a controllable
frequency and amplitude. The start of the waveform is also controllable
with the *trigger*, but should be used with caution as it introduces
spike frequencies.

[Source Code](../../AwesomeAudioApparatus/Demiurge/Src/oscillator.c)

### Inputs
| Name | Description |
|------|-------------|
| frequency | The frequency to generate. For non-SINE, it is the inverse of the full cycle period |
| amplitude | The amplitude of the output signal. |
| trigger | Re-trigger the wave generation to time zero. |


### Example

