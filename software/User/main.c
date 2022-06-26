
#include "ch32v30x.h"
#include <rtthread.h>
#include <rthw.h>
#include "demiurge.h"


void TIM7_IRQHandler(void)   __attribute__((interrupt("WCH-Interrupt-fast")));

int main(void)
{
    rt_kprintf("\r\n Demiurge - Risc-V\r\n");
	rt_kprintf(" SysClk: %dHz\r\n",SystemCoreClock);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE );

//	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	vco_setup();
	demiurge_start();
	while(1)
	{
//	    GPIO_SetBits(GPIOA,GPIO_Pin_0);
		vco_loop();
	}
}

inline void read_adc() {
//   inputs[0] = ((float) hadc1.Instance->DR) / 204.8f;
//   inputs[1] = ((float) hadc2.Instance->DR) / 204.8f;
}

inline void init_adc() {

	GPIO_InitTypeDef GPIO_InitStructure={0};

	// Analog Inputs on Port A
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Analog Inputs on Port B
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);

	// Analog Inputs on Port C
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);

    // What was this for? The DAC example had this for one of the 2 AI used.
	// GPIO_SetBits(GPIOA,GPIO_Pin_4);
}

inline void start_adc() {
//   HAL_ADC_Start(&hadc1);
//   HAL_ADC_Start(&hadc2);
}

inline void stop_adc() {
//   HAL_ADC_Stop(&hadc1);
//   HAL_ADC_Stop(&hadc2);
}

inline void read_gpio() {
   gpio_a = GPIOA->INDR;
   gpio_b = GPIOB->INDR;
   gpio_c = GPIOC->INDR;
}

inline void update_dac() {
   uint16_t ch1 = (uint16_t) ((10.0f - outputs[0]) * 204.7f);
   uint16_t ch2 = (uint16_t) ((10.0f - outputs[1]) * 204.7f);
   DAC_SetDualChannelData(DAC_Align_12b_R, ch1,ch2);
}

inline void init_dac() {
	DAC_InitTypeDef DAC_InitType={0};

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );


	DAC_InitType.DAC_Trigger=DAC_Trigger_None;
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;
    DAC_Init(DAC_Channel_1,&DAC_InitType);
    DAC_Init(DAC_Channel_2,&DAC_InitType);

    DAC_Cmd(DAC_Channel_1, ENABLE);
    DAC_Cmd(DAC_Channel_2, ENABLE);
}

inline void start_dac() {
//   HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
//   HAL_DAC_Start(&hdac, DAC_CHANNEL_2);
}

inline void stop_dac() {
//   HAL_DAC_Stop(&hdac, DAC_CHANNEL_1);
//   HAL_DAC_Stop(&hdac, DAC_CHANNEL_2);
}

inline void init_timer() {

    uint32_t prescale = 0;
    uint32_t period = (84000000 / demiurge_samplerate) - 1;

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure={0};
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = period;
    TIM_TimeBaseStructure.TIM_Prescaler = prescale;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    TIM_SetAutoreload( TIM7,TIM_PSCReloadMode_Immediate );
    TIM_SelectMasterSlaveMode(TIM7, TIM_MasterSlaveMode_Disable);
    TIM_SelectOutputTrigger(TIM7, TIM_TRGOSource_Update);

    NVIC_EnableIRQ(TIM7_IRQn);
    NVIC_SetPriority(TIM7_IRQn,(5<<5) | (0x01<<4));
}

inline void start_timer() {
    TIM_Cmd(TIM7, ENABLE);
}

inline void stop_timer() {
    TIM_Cmd(TIM7, DISABLE);
}

inline void update_leds() {
   // TODO:
}

void TIM7_IRQHandler() {
	demiurge_tick();
}


