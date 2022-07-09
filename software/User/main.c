
#include "ch32v30x.h"
#include <rtthread.h>
#include <rthw.h>
#include "demiurge.h"
#include "demiurge-spi.h"

extern void vco_setup();
extern void vco_loop();

extern float adc_scales[];
extern float adc_offsets[];

int main(void)
{
	demiurge_samplerate = 48000;
	adc_scales[4] = -10.0f/4095.0f;
	adc_offsets[4] = 8.0f;
	adc_scales[6] = -10.0f/4095.0f;
	adc_offsets[6] = 8.0f;
    demiurge_driver_init();
	demiurge_init();

	vco_setup();
	demiurge_start();
	while(1)
	{
		vco_loop();
	}
}


