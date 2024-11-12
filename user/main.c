/**
  **************************************************************************
  * @file     main.c
  * @brief    main program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  *
  **************************************************************************
  */

#include "at32f415_board.h"
#include "at32f415_clock.h"
#include "at32f415_gpio.h"

/** @addtogroup AT32F415_periph_template
  * @{
  */

/** @addtogroup 415_LED_toggle LED_toggle
  * @{
  */

#define DELAY                            100
#define FAST                             1
#define SLOW                             4
#define BUFF_SIZE						 50

uint32_t PhaseA = 0;
uint32_t PhaseB = 0;
uint32_t PhaseC = 0;
crm_clocks_freq_type crm_clocks_freq_struct = {0};
//uint32_t srcBuffer[BUFF_SIZE] = {500, 1000, 1600, 2000, 2500,  3000, 3500, 4000, 4500, 5000, 5500, 6000, 6500, 7000, 7500, 8000, 8500, 9999};

//OSNOVNOY MASSIV DAANYX SINUSA NE YDALYAT!!!
uint8_t srcBufferA[BUFF_SIZE] = {125, 137, 150, 162, 174, 184, 194, 203,
		210, 216, 220, 223, 224, 224, 222, 218, 213, 207, 199, 189, 179,
		168, 156, 144, 131, 118, 105, 93, 81, 70, 60, 50, 42, 36, 31, 27,
		25, 25, 26, 29, 33, 39, 46, 55, 65, 75, 87, 99, 112, 124
};

uint8_t srcBufferB[BUFF_SIZE] = {38, 32, 28, 25, 25, 25, 28, 31, 37, 44, 52,
		61, 72, 83, 95, 107, 120, 133, 146, 158, 170, 181, 191, 200, 208, 214,
		219, 222, 224, 224, 223, 220, 215, 209, 201, 193, 183, 172, 160, 148, 135,
		122, 110, 97, 85, 74, 63, 53, 45, 38
};

uint8_t srcBufferC[BUFF_SIZE] = {211, 204, 196, 186, 175, 164, 152, 139, 127,
		114, 101, 89, 77, 66, 56, 48, 40, 34, 29, 26, 25, 25, 27, 30, 35, 41,
		49, 58, 68, 79, 91, 103, 116, 129, 142, 154, 166, 177, 188, 197, 205,
		212, 218, 221, 224, 224, 224, 221, 217, 211
};

//uint8_t srcBuffer[BUFF_SIZE] = {25, 100, 210};
tmr_output_config_type tmr_output_struct;
dma_init_type dma_init_struct = {0};

void StatusLedInit(void) {
	gpio_init_type gpio_init_struct;
	/* enable gpio port clock */
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE); // - ��� ��� �������, �
	/* set default parameter */
	gpio_default_para_init(&gpio_init_struct);
	/* clkout gpio init */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_0; // - ��� ��� ��
	gpio_init_struct.gpio_pull = GPIO_PULL_DOWN;
	gpio_init(GPIOA, &gpio_init_struct); // - ��� ��� �������, �
}

void PWMPinsInit(void) {
/*	PWM PINS ARE:
 *  PA8 for phase A
 *  PB15 for phase B
 *  PB14 for phase C*/

//	PA8
    gpio_init_type gpio_init_struct;
	/* enable gpio port clock */
	crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE); //
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
	/* set default parameter */
	gpio_default_para_init(&gpio_init_struct);
	/* clkout gpio init */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_8; //
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOA, &gpio_init_struct); //

//	PB15
	/* enable gpio port clock */
	gpio_default_para_init(&gpio_init_struct);
	/* clkout gpio init */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_15; //
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOB, &gpio_init_struct); //

//	PB14
	/* enable gpio port clock */
	gpio_default_para_init(&gpio_init_struct);
	/* clkout gpio init */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_14; //
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOB, &gpio_init_struct); //
}

void TMR1Init() {
	  /* get system clock */
	  crm_clocks_freq_get(&crm_clocks_freq_struct);
	  /* enable tmr1 clock */
	  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);


	  /* tmr1 configuration */
	  /* time base configuration */
	  // div is 144000000/ n
	  /* systemclock/ div / tmr value = f (hz) */
	  // tmr_base_init(TMR1, tmr value, (crm_clocks_freq_struct.ahb_freq / n) - 1);
//	  OSNOVNAYA NSTROIKA TAIMERA NE YDALYAT
	  tmr_base_init(TMR1, 400, 200);
//	  tmr_base_init(TMR1, 248, (crm_clocks_freq_struct.ahb_freq / 10000000) - 1);
	  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);


	  /* overflow interrupt enable */
	  tmr_interrupt_enable(TMR1, TMR_OVF_INT, TRUE);
	  tmr_interrupt_enable(TMR1, TMR_C1_INT, TRUE);
	  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, srcBufferA[0]);
	  /* tmr1 overflow interrupt nvic init */
	  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
	  nvic_irq_enable(TMR1_OVF_TMR10_IRQn, 0, 0);
	  nvic_irq_enable(TMR1_CH_IRQn, 0, 0);


	  /* channel 1 configuration in output mode */
	  tmr_output_default_para_init(&tmr_output_struct);
	  tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
	  tmr_output_struct.oc_output_state = TRUE;
	  tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
	  tmr_output_struct.oc_idle_state = FALSE;


	  /* channel 1 */
	  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
	  tmr_output_channel_buffer_enable(TMR1, TMR_SELECT_CHANNEL_1, TRUE);

	  tmr_period_buffer_enable(TMR1, TRUE);
	  tmr_counter_enable(TMR1, TRUE);

	  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, srcBufferA[0]);






	  /* tmr1 configuration */
	  /* time base configuration */
	  // div is 144000000/ n
	  /* systemclock/ div / tmr value = f (hz) */
	  // tmr_base_init(TMR1, tmr value, (crm_clocks_freq_struct.ahb_freq / n) - 1);
//	  OSNOVNAYA NSTROIKA TAIMERA NE YDALYAT



	  /* overflow interrupt enable */
	  tmr_interrupt_enable(TMR1, TMR_OVF_INT, TRUE);
	  tmr_interrupt_enable(TMR1, TMR_C2_INT, TRUE);
	  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, srcBufferA[0]);
	  /* tmr1 overflow interrupt nvic init */
	  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
	  nvic_irq_enable(TMR1_OVF_TMR10_IRQn, 0, 0);
	  nvic_irq_enable(TMR1_CH_IRQn, 0, 0);


	  /* channel 1 configuration in output mode */
	  tmr_output_default_para_init(&tmr_output_struct);
	  tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
	  tmr_output_struct.oc_output_state = TRUE;
	  tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
	  tmr_output_struct.oc_idle_state = FALSE;


	  /* channel 1 */
	  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_2, &tmr_output_struct);
	  tmr_output_channel_buffer_enable(TMR1, TMR_SELECT_CHANNEL_2, TRUE);

	  tmr_period_buffer_enable(TMR1, TRUE);
	  tmr_counter_enable(TMR1, TRUE);

	  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_2, srcBufferB[0]);








	  /* tmr1 configuration */
	  /* time base configuration */
	  // div is 144000000/ n
	  /* systemclock/ div / tmr value = f (hz) */
	  // tmr_base_init(TMR1, tmr value, (crm_clocks_freq_struct.ahb_freq / n) - 1);
//	  OSNOVNAYA NSTROIKA TAIMERA NE YDALYAT


	  /* overflow interrupt enable */
	  tmr_interrupt_enable(TMR1, TMR_OVF_INT, TRUE);
	  tmr_interrupt_enable(TMR1, TMR_C4_INT, TRUE);
	  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_4, srcBufferC[0]);
	  /* tmr1 overflow interrupt nvic init */
	  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
	  nvic_irq_enable(TMR1_OVF_TMR10_IRQn, 0, 0);
	  nvic_irq_enable(TMR1_CH_IRQn, 0, 0);


	  /* channel 1 configuration in output mode */
	  tmr_output_default_para_init(&tmr_output_struct);
	  tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
	  tmr_output_struct.oc_output_state = TRUE;
	  tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
	  tmr_output_struct.oc_idle_state = FALSE;


	  /* channel 1 */
	  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_4, &tmr_output_struct);
	  tmr_output_channel_buffer_enable(TMR1, TMR_SELECT_CHANNEL_4, TRUE);

	  tmr_period_buffer_enable(TMR1, TRUE);
	  tmr_counter_enable(TMR1, TRUE);

	  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_4, srcBufferC[0]);
}

void DMAInit() {
	  /* dma config for tmr1 overflow dma request */
	  /* dma1 channel5 configuration */
	crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);


//	CH1 of TMR1 and CH2 of DMA1 for phase A
	tmr_dma_request_enable(TMR1, TMR_C1_DMA_REQUEST, TRUE);

	dma_reset(DMA1_CHANNEL2);
	dma_init_struct.buffer_size = BUFF_SIZE;
	dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
	dma_init_struct.memory_base_addr = (uint32_t)srcBufferA;
//	dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_WORD;
	dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
	dma_init_struct.memory_inc_enable = TRUE;
	dma_init_struct.peripheral_base_addr = (uint32_t)&TMR1->c1dt;
	dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
	dma_init_struct.peripheral_inc_enable = FALSE;
	dma_init_struct.priority = DMA_PRIORITY_VERY_HIGH;
	dma_init_struct.loop_mode_enable = TRUE;


//	dma_flexible_config(DMA1, FLEX_CHANNEL2, DMA_FLEXIBLE_TMR1_OVERFLOW);

	dma_init(DMA1_CHANNEL2, &dma_init_struct);

    // enable transfer full data interrupt
    dma_interrupt_enable(DMA1_CHANNEL2, DMA_FDT_INT, TRUE);
//END OF SETTiNGS FOR CH1 of TMR1 and CH2 of DMA1 for phase A


//	CH2 of TMR1 and CH3 of DMA1 for phase B
    // dma channel1 and 2 interrupt nvic init
    nvic_irq_enable(DMA1_Channel2_IRQn, 1, 0);

	tmr_dma_request_enable(TMR1, TMR_C2_DMA_REQUEST, TRUE);

	dma_reset(DMA1_CHANNEL3);
	dma_init_struct.buffer_size = BUFF_SIZE;
	dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
	dma_init_struct.memory_base_addr = (uint32_t)srcBufferB;
//	dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_WORD;
	dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
	dma_init_struct.memory_inc_enable = TRUE;
	dma_init_struct.peripheral_base_addr = (uint32_t)&TMR1->c2dt;
	dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
	dma_init_struct.peripheral_inc_enable = FALSE;
	dma_init_struct.priority = DMA_PRIORITY_VERY_HIGH;
	dma_init_struct.loop_mode_enable = TRUE;


//	dma_flexible_config(DMA1, FLEX_CHANNEL2, DMA_FLEXIBLE_TMR1_OVERFLOW);

	dma_init(DMA1_CHANNEL3, &dma_init_struct);

    // enable transfer full data interrupt
    dma_interrupt_enable(DMA1_CHANNEL3, DMA_FDT_INT, TRUE);


    // dma channel1 and 2 interrupt nvic init
    nvic_irq_enable(DMA1_Channel3_IRQn, 1, 0);


//END OF SETTiNGS FOR CH2 of TMR1 and CH3 of DMA1 for phase B

//CH4 of TMR1 and CH4 of DMA1 for phase B
        // dma channel1 and 2 interrupt nvic init

    	tmr_dma_request_enable(TMR1, TMR_C4_DMA_REQUEST, TRUE);

    	dma_reset(DMA1_CHANNEL4);
    	dma_init_struct.buffer_size = BUFF_SIZE;
    	dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
    	dma_init_struct.memory_base_addr = (uint32_t)srcBufferC;
    //	dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_WORD;
    	dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
    	dma_init_struct.memory_inc_enable = TRUE;
    	dma_init_struct.peripheral_base_addr = (uint32_t)&TMR1->c4dt;
    	dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
    	dma_init_struct.peripheral_inc_enable = FALSE;
    	dma_init_struct.priority = DMA_PRIORITY_VERY_HIGH;
    	dma_init_struct.loop_mode_enable = TRUE;


    //	dma_flexible_config(DMA1, FLEX_CHANNEL2, DMA_FLEXIBLE_TMR1_OVERFLOW);

    	dma_init(DMA1_CHANNEL4, &dma_init_struct);

        // enable transfer full data interrupt
        dma_interrupt_enable(DMA1_CHANNEL4, DMA_FDT_INT, TRUE);


        // dma channel1 and 2 interrupt nvic init
        nvic_irq_enable(DMA1_Channel4_IRQn, 1, 0);


//END OF SETTiNGS FOR CH4 of TMR1 and CH4 of DMA1 for phase C

    dma_channel_enable(DMA1_CHANNEL2, TRUE);
    dma_channel_enable(DMA1_CHANNEL3, TRUE);
    dma_channel_enable(DMA1_CHANNEL4, TRUE);
}

void DMA1_Channel2_IRQHandler() {
	if(dma_interrupt_flag_get(DMA1_FDT2_FLAG) != RESET)
	    {

	        dma_flag_clear(DMA1_FDT2_FLAG);

	    }
}

void DMA1_Channel3_IRQHandler() {
	if(dma_interrupt_flag_get(DMA1_FDT3_FLAG) != RESET)
	    {

	        dma_flag_clear(DMA1_FDT3_FLAG);

	    }
}

void DMA1_Channel4_IRQHandler() {
	if(dma_interrupt_flag_get(DMA1_FDT4_FLAG) != RESET)
	    {

	        dma_flag_clear(DMA1_FDT4_FLAG);

	    }
}

void DMA1_Channel5_IRQHandler() {
	if(dma_interrupt_flag_get(DMA1_FDT5_FLAG) != RESET)
	    {
	        dma_flag_clear(DMA1_FDT5_FLAG);

	    }
}

void TMR1_OVF_TMR10_IRQHandler(void)
{
  if(tmr_interrupt_flag_get(TMR1, TMR_OVF_FLAG) != RESET)
  {
//    /* add user code... */
//    chOutputCompareValue += 500;
//
//    if (chOutputCompareValue >= 9999) {
//    	chOutputCompareValue = 199;
//    }
//    tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, chOutputCompareValue);
    tmr_flag_clear(TMR1, TMR_OVF_FLAG);

//    gpio_bits_reset(GPIOA, GPIO_PINS_0);
//    statusLedFlag = 0;
    gpio_bits_reset(GPIOA, GPIO_PINS_0);

    GPIOA->scr = GPIO_PINS_8;
    GPIOB->scr = GPIO_PINS_14;
    GPIOB->scr = GPIO_PINS_15;

//    gpio_bits_set(GPIOA, GPIO_PINS_8);
//    gpio_bits_set(GPIOB, GPIO_PINS_14);
//    gpio_bits_set(GPIOB, GPIO_PINS_15);
    tmr_flag_clear(TMR1, TMR_OVF_FLAG);
  }
}

void TMR1_CH_IRQHandler(void) {

	if(tmr_interrupt_flag_get(TMR1, TMR_C1_FLAG) != RESET) {
//		uint32_t channelValue = 0;
//		channelValue = tmr_channel_value_get(TMR1, TMR_SELECT_CHANNEL_1);

		gpio_bits_set(GPIOA, GPIO_PINS_0);
		GPIOA->clr = GPIO_PINS_8;

//		gpio_bits_reset(GPIOA, GPIO_PINS_8);
		PhaseA = tmr_channel_value_get(TMR1, TMR_SELECT_CHANNEL_1);
		tmr_flag_clear(TMR1, TMR_C1_FLAG);

	  }
	else if(tmr_interrupt_flag_get(TMR1, TMR_C2_FLAG) != RESET) {
//		uint32_t channelValue = 0;
//		channelValue = tmr_channel_value_get(TMR1, TMR_SELECT_CHANNEL_2);

//		gpio_bits_reset(GPIOB, GPIO_PINS_15);
		GPIOB->clr = GPIO_PINS_15;

		PhaseB = tmr_channel_value_get(TMR1, TMR_SELECT_CHANNEL_2);
		tmr_flag_clear(TMR1, TMR_C2_FLAG);
	  }
	else if(tmr_interrupt_flag_get(TMR1, TMR_C4_FLAG) != RESET) {
//		uint32_t channelValue = 0;
//		channelValue = tmr_channel_value_get(TMR1, TMR_SELECT_CHANNEL_4);

//		gpio_bits_reset(GPIOB, GPIO_PINS_14);
	    GPIOB->clr = GPIO_PINS_14;
		PhaseC = tmr_channel_value_get(TMR1, TMR_SELECT_CHANNEL_4);
		tmr_flag_clear(TMR1, TMR_C4_FLAG);

	  }
}

void TMR_Compare_Init() {
//	TMR2 CH1
	/* get system clock */
		  crm_clocks_freq_get(&crm_clocks_freq_struct);
		  /* enable tmr1 clock */
		  crm_periph_clock_enable(CRM_TMR2_PERIPH_CLOCK, TRUE);


		  /* tmr1 configuration */
		  /* time base configuration */
		  // div is 144000000/ n
		  /* systemclock/ div / tmr value = f (hz) */
		  // tmr_base_init(TMR1, tmr value, (crm_clocks_freq_struct.ahb_freq / n) - 1);
	//	  OSNOVNAYA NSTROIKA TAIMERA NE YDALYAT
		  tmr_base_init(TMR2, 400, 200);
	//	  tmr_base_init(TMR1, 248, (crm_clocks_freq_struct.ahb_freq / 10000000) - 1);
		  tmr_cnt_dir_set(TMR2, TMR_COUNT_UP);


		  /* overflow interrupt enable */
		  tmr_interrupt_enable(TMR2, TMR_OVF_INT, TRUE);
		  tmr_interrupt_enable(TMR2, TMR_C1_INT, TRUE);
		  tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, srcBufferA[0]);
		  /* tmr1 overflow interrupt nvic init */
		  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
		  nvic_irq_enable(TMR2_GLOBAL_IRQn, 1, 0);


		  /* channel 1 configuration in output mode */
		  tmr_output_default_para_init(&tmr_output_struct);
		  tmr_output_struct.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
		  tmr_output_struct.oc_output_state = TRUE;
		  tmr_output_struct.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
		  tmr_output_struct.oc_idle_state = FALSE;


		  /* channel 1 */
		  tmr_output_channel_config(TMR2, TMR_SELECT_CHANNEL_1, &tmr_output_struct);
		  tmr_output_channel_buffer_enable(TMR2, TMR_SELECT_CHANNEL_1, TRUE);

		  tmr_period_buffer_enable(TMR2, TRUE);
		  tmr_counter_enable(TMR2, TRUE);

		  tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, srcBufferA[0]);

}

void TMR2_GLOBAL_IRQHandler() {
	if(tmr_interrupt_flag_get(TMR2, TMR_C1_FLAG) != RESET) {
		tmr_flag_clear(TMR2, TMR_C1_FLAG);
	}
	else if(tmr_interrupt_flag_get(TMR2, TMR_OVF_FLAG) != RESET) {
	//    /* add user code... */
		// ADJUST PINS WITH ALTIUMDESIGN
		PhaseA > PhaseB ? (void)(GPIOA->scr = GPIO_PINS_4) : (void)(GPIOA->clr = GPIO_PINS_4);
		PhaseB > PhaseC ? (void)(GPIOA->scr = GPIO_PINS_5) : (void)(GPIOA->clr = GPIO_PINS_5);
		PhaseC > PhaseA ? (void)(GPIOA->scr = GPIO_PINS_6) : (void)(GPIOA->clr = GPIO_PINS_6);

		tmr_flag_clear(TMR2, TMR_OVF_FLAG);
	}
}

void Compare_Pins_Init() {
	/* set default parameter */
//	PA4
    gpio_init_type gpio_init_struct;
		gpio_default_para_init(&gpio_init_struct);
		/* clkout gpio init */
		gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
		gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
		gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
		gpio_init_struct.gpio_pins = GPIO_PINS_4; //
		gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
		gpio_init(GPIOA, &gpio_init_struct); //

	//	PA5
		/* enable gpio port clock */
		gpio_default_para_init(&gpio_init_struct);
		/* clkout gpio init */
		gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
		gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
		gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
		gpio_init_struct.gpio_pins = GPIO_PINS_5; //
		gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
		gpio_init(GPIOA, &gpio_init_struct); //

	//	PA6
		/* enable gpio port clock */
		gpio_default_para_init(&gpio_init_struct);
		/* clkout gpio init */
		gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
		gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
		gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
		gpio_init_struct.gpio_pins = GPIO_PINS_6; //
		gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
		gpio_init(GPIOA, &gpio_init_struct); //
}

void DMA_Compare_Init() {
	 /* dma config for tmr1 overflow dma request */
		  /* dma1 channel5 configuration */
	//	CH1 of TMR1 and CH2 of DMA1 for phase A
		tmr_dma_request_enable(TMR1, TMR_C1_DMA_REQUEST, TRUE);

		dma_reset(DMA1_CHANNEL5);
		dma_init_struct.buffer_size = BUFF_SIZE;
		dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
		dma_init_struct.memory_base_addr = (uint32_t)srcBufferA;
	//	dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_WORD;
		dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_BYTE;
		dma_init_struct.memory_inc_enable = TRUE;
		dma_init_struct.peripheral_base_addr = (uint32_t)&TMR2->c1dt;
		dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
		dma_init_struct.peripheral_inc_enable = FALSE;
		dma_init_struct.priority = DMA_PRIORITY_VERY_HIGH;
		dma_init_struct.loop_mode_enable = TRUE;


	//	dma_flexible_config(DMA1, FLEX_CHANNEL2, DMA_FLEXIBLE_TMR1_OVERFLOW);

		dma_init(DMA1_CHANNEL5, &dma_init_struct);

	    // enable transfer full data interrupt
	    dma_interrupt_enable(DMA1_CHANNEL5, DMA_FDT_INT, TRUE);
	//END OF SETTiNGS FOR CH1 of TMR1 and CH2 of DMA1 for phase A


	//	CH2 of TMR1 and CH3 of DMA1 for phase B
	    // dma channel1 and 2 interrupt nvic init
	    nvic_irq_enable(DMA1_Channel5_IRQn, 1, 0);

		tmr_dma_request_enable(TMR2, TMR_C1_DMA_REQUEST, TRUE);
}

/**
  * @brief  main function.
  * @param  none
  * @retval none
  */
int main(void)
{
//	after reset HICK is default
//  system_clock_config();

  at32_board_init();
  PWMPinsInit();
  StatusLedInit();
  TMR1Init();
  DMAInit();

  Compare_Pins_Init();
  TMR_Compare_Init();
  DMA_Compare_Init();

//  debug_periph_mode_set(DEBUG_TMR1_PAUSE, TRUE);
//  debug_periph_mode_set(DEBUG_TMR2_PAUSE, TRUE);


  while(1)
  {
//	  CODE HERE
//	  END OF CODE
  }
}

/**
  * @}
  */

/**
  * @}
  */
