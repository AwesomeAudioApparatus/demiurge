# Blocks
All blocks has a single ***Output*** signal, to which an ***Input*** on
another block can be connected. The rare cases of multiple ***Output***
blocks, such as [Pan](#Pan), accomplishes this by having sub-blocks for
each ***Output*** that downstream blocks can be connected to.

## ADSR - Attack, Decay, Sustain, Release

[Source Code](../AwesomeAudioApparatus/Demiurge/Src/adsr.c)

### Inputs
| Name | Description |
|------|-------------|
|  |  |
| attack | Sets the attack time of the ADSR function | 
| decay | Sets the decay time of the ADSR function |
| sustain | Sets the sustain level of the ADSR function |     
| release | Sets the release time of the ADSR function |
| gate | Gate signal for the ADSR function |
| trig | Re-trig signal for the ADSR function |

## Audio Input
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/audio_inport.c)

### Inputs
| Name | Description |
|------|-------------|
| *none* |  |

## Audio Output
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/audio_outport.c)

### Inputs
| Name | Description |
|------|-------------|
| input | The audio signal to be set to the physical output port as an audio signal. |

## Averager
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/averager.c)

### Inputs
| Name | Description |
|------|-------------|
| input | The signal to be averaged. |
| averaging_control | The amount of averging to be applied to the signal |

## Calculator
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/calculator.c)

### Inputs      
| Name | Description |
|------|-------------|
| input |  The signal to apply the [***Function***](functions.md) on. |
           

## CFX RBJ Filter
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/cfxrbjfilter.c)

### Inputs
| Name | Description |
|------|-------------|
| input | Signal to be filtered |
| frequency | Frequency to be filtered at |
| Q | Q factor which sets slope and ringing |
| gain | Gain/attenuation in the filter |
| type | <ul><li>0 - low pass</li><li>1 - high pass</li><li>2 - bandpass (csg)</li><li>3 - bandpass (czpg) </li><li>4 - notch</li><li>5 - allpass</li><li>6 - peaking</li><li>7 - low shelf</li><li>8 - high shelf</li></ul> |

## Control Pair
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/control_pair.c)

### Inputs
| Name | Description |
|------|-------------|
| *none* |  |

## CV Input
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/cv_inport.c)

### Inputs
| Name | Description |
|------|-------------|
| *none* |  |

## CV Output
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/cv_outport.c)

### Inputs
| Name | Description |
|------|-------------|
| input | The signal to be set to the physical output prot as a CV signal |

## Fixed Signal
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/fixed_signal.c)

### Inputs
| Name | Description |
|------|-------------|
| *none* |  |

## Gate Input
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/gate_inport.c)

### Inputs
| Name | Description |
|------|-------------|
| *none* |  |

## Gate Output
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/gate_outport.c)

### Inputs
| Name | Description |
|------|-------------|
| input | The signal to be set to the physical output prot as a Gate signal. |

## Inverter
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/inverter.c)

### Inputs
| Name | Description |
|------|-------------|
| input | The signal to be inverted. |
| scale | Scaling of the signal |
| offset | Offset around which the signal should be inverted. |

## LED
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/led.c)

### Inputs
| Name | Description |
|------|-------------|
| input | The RGB value of the LED to be set. Only used if r, g and b are not configured. |
| r | The RED value to be set to the physical LED. |
| g | The GREEN value to be set to the physical LED. |
| b | The BLUE value to be set to the physical LED. |

## Lg
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/lg.c)

### Inputs
| Name | Description |
|------|-------------|
| input | The signal that wil have *ln* (the natural logarithm) applied to it |

## Lg2
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/lg2.c)

### Inputs
| Name | Description |
|------|-------------|
| input | The signal that wil have *log2* applied to it |

## Lg10
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/lg10.c)

### Inputs
| Name | Description |
|------|-------------|
| input | The signal that wil have *log10* applied to it |

## Mixer
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/mixer.c)

### Inputs
| Name | Description |
|------|-------------|
| inputs[] | An array of signals to be mixed to the ***Output*** |
| volumes[] | An array of volume controllers, one for each ***Input*** |

## Offset
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/offset.c)

### Inputs
| Name | Description |
|------|-------------|
| input | The signal to be offsetted |
| offset_control | The amount of offset to be applied to the ***Input*** |

## Oscillator
The Oscillator is a block that generates waveforms at a controllable 
frequency and amplitude. The start of the waveform is also controllable
with the *trigger*, but should be used with caution as it introduces 
spike frequencies.

[Source Code](../AwesomeAudioApparatus/Demiurge/Src/oscillator.c)

### Inputs
| Name | Description |
|------|-------------|
| frequency | The frequency to generate. For non-SINE, it is the inverse of the full cycle period |
| amplitude | The amplitude of the output signal. |
| trigger | Re-trigger the wave generation to time zero. |

## Pan
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/pan.c)

### Inputs
| Name | Description |
|------|-------------|
| input  | The signal to be split in a *left* and *right* channel. |
| control  | The balance between *left* (0V) and *right* (10V) channel |

### Sub-blocks
| Name | Description |
|------|-------------|
| left_channel |  |
| right_channel |  |

## Passthru
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/passthru.c)

### Inputs
| Name | Description |
|------|-------------|
| input | The signal to be pass to the output. |

## Potentiometer
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/potentiometer.c)

### Inputs
| Name | Description |
|------|-------------|
| *none* |  |

## Push Button
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/pushbutton.c)

### Inputs
| Name | Description |
|------|-------------|
| *none* |  |

## Scale
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/scale.c)

### Inputs
| Name | Description |
|------|-------------|
| input | The signal to be scaled.  |
| control | The amplication/attenuation control. 0V=muted, 10V=~20dB gain

## State Variable Filter
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/svf.c)

### Inputs
| Name | Description |
|------|-------------|
| input | Signal to be filtered |
| frequency | Frequency to be filtered at |
| Q | Q factor which sets slope and ringing |
| gain | Gain/attenuation in the filter |

### Sub-blocks
| Name | Description |
|------|-------------|
| lowpass_out | Low pass filter output signal |
| highpass_out | High pass filter output signal |
| bandpass_out | Bandpass filter output signal |
| notch_out | Notch filter output signal |

## Volume
[Source Code](../AwesomeAudioApparatus/Demiurge/Src/volume.c)

### Inputs
| Name | Description |
|------|-------------|
| input  | Signal to be volume controlled.  |
| control  | The amplication/attenuation control. output=input*(0.05 * control + 0.5)  |
