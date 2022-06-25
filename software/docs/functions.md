## Functions
Functions are used primarily in the [Calculator block](blocks.md#Calculator).

### Example
```C
static calculator_t calc;           // Declaration of a Mixer block

float myslope(float input) {
    // nonsense function...
    if( input > 1.0f )
        return input * -1.3f + input / 5;
    return return input * 0.7f + input / 4;
}
  :
  :
  calculator_configure_function(&calc, &myslope );  // calc log10() on input signal
  :
```

