
#include "ch32v30x.h"
#include <rtthread.h>
#include <rthw.h>
#include "demiurge.h"



extern void vco_setup();
extern void vco_loop();

int main(void)
{
    demiurge_samplerate = 48000;
	demiurge_init();

	vco_setup();
	demiurge_start();
	while(1)
	{
		vco_loop();
	}
}


