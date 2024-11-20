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


uint16_t PhaseA = 0;
uint16_t PhaseB = 0;
uint16_t PhaseC = 0;
crm_clocks_freq_type crm_clocks_freq_struct = {0};
//uint32_t srcBuffer[BUFF_SIZE] = {500, 1000, 1600, 2000, 2500,  3000, 3500, 4000, 4500, 5000, 5500, 6000, 6500, 7000, 7500, 8000, 8500, 9999};

//OSNOVNOY MASSIV DAANYX SINUSA NE YDALYAT!!!
//uint8_t ledBuff[BUFF_SIZE] = {125, 137, 150, 162, 174, 184, 194, 203,
//		210, 216, 220, 223, 224, 224, 222, 218, 213, 207, 199, 189, 179,
//		168, 156, 144, 131, 118, 105, 93, 81, 70, 60, 50, 42, 36, 31, 27,
//		25, 25, 26, 29, 33, 39, 46, 55, 65, 75, 87, 99, 112, 124
//};

//uint16_t srcBufferA[BUFF_SIZE] = {200, 220, 241, 260, 279, 296, 311, 325, 337, 346, 353, 358, 360, 359, 356,
//		 350, 342, 331, 318, 304, 287, 269, 250, 231, 210, 190, 169, 150, 131, 113,
//		  96,  82,  69,  58,  50,  44,  41,  40,  42,  47,  54,  63,  75,  89, 104,
//		 121, 140, 159, 180, 200
//};
//
//uint16_t srcBufferB[BUFF_SIZE] = {61,  52,  46,  42,  40,  41,  45,  51,  60,  71,  84,  99, 116, 134, 153,
//		 173, 193, 214, 234, 254, 272, 290, 306, 321, 333, 343, 351, 357, 360, 360,
//		 357, 352, 345, 335, 323, 309, 293, 276, 257, 237, 217, 197, 176, 156, 137,
//		 119, 102,  86,  73,  61
//};
//
//uint16_t srcBufferC[BUFF_SIZE] = {339, 327, 314, 298, 281, 263, 244, 224, 203, 183, 163, 143, 124, 107,  91,
//		  77,  65,  55,  48,  43,  40,  40,  43,  49,  57,  67,  79,  94, 110, 128,
//		 146, 166, 186, 207, 227, 247, 266, 284, 301, 316, 329, 340, 349, 355, 359,
//		 360, 358, 354, 348, 339
//};
//  Array of 50 points frequency 400 Hz / 20kHz
uint16_t srcBufferA[BUFF_SIZE] = {314, 346, 378, 408, 437, 464, 489, 510, 529, 544, 555, 562, 565, 564, 559,
		 550, 537, 520, 500, 477, 451, 423, 393, 362, 330, 298, 266, 235, 205, 177,
		 151, 128, 108,  91,  78,  69,  64,  63,  66,  73,  84,  99, 118, 139, 164,
		 191, 220, 250, 282, 314
};

uint16_t srcBufferB[BUFF_SIZE] = {97,  82,  72,  65,  63,  65,  71,  80,  94, 111, 132, 155, 182, 210, 240,
		 271, 303, 335, 367, 398, 428, 455, 481, 503, 523, 539, 551, 560, 564, 565,
		 561, 553, 541, 526, 507, 485, 460, 432, 403, 372, 341, 309, 277, 245, 215,
		 186, 160, 136, 114,  97
};

uint16_t srcBufferC[BUFF_SIZE] = {531, 514, 492, 468, 442, 413, 383, 351, 319, 287, 256, 225, 196, 168, 143,
		 121, 102,  87,  75,  67,  63,  64,  68,  77,  89, 105, 125, 147, 173, 200,
		 230, 261, 293, 325, 357, 388, 418, 446, 473, 496, 517, 534, 548, 557, 563,
		 565, 563, 556, 546, 531
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
	gpio_bits_reset(GPIOA, GPIO_PINS_0);
}

void DMA1_Channel1_IRQHandler(void) {
	if(dma_interrupt_flag_get(DMA1_FDT1_FLAG) != RESET)
		    {

		        dma_flag_clear(DMA1_FDT1_FLAG);

		    }
}


void TMR4_GLOBAL_IRQHandler() {
	if(tmr_interrupt_flag_get(TMR4, TMR_C1_FLAG) != RESET) {
		gpio_bits_set(GPIOA, GPIO_PINS_0);
		tmr_flag_clear(TMR4, TMR_C1_FLAG);
	}
	else if(tmr_interrupt_flag_get(TMR4, TMR_OVF_FLAG) != RESET) {
		//    /* add user code... */
	    	gpio_bits_reset(GPIOA, GPIO_PINS_0);
			tmr_flag_clear(TMR4, TMR_OVF_FLAG);
		}
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
	gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
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

static void tmr1_config(void)
{
  gpio_init_type gpio_initstructure;
  tmr_output_config_type tmr_oc_init_structure;
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  gpio_default_para_init(&gpio_initstructure);
  gpio_initstructure.gpio_mode = GPIO_MODE_MUX;
  gpio_initstructure.gpio_pins = GPIO_PINS_8;
  gpio_initstructure.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_initstructure.gpio_pull = GPIO_PULL_NONE;
  gpio_initstructure.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init(GPIOA, &gpio_initstructure);

  gpio_initstructure.gpio_pins = GPIO_PINS_14;
  gpio_init(GPIOB, &gpio_initstructure);
  gpio_initstructure.gpio_pins = GPIO_PINS_15;
    gpio_init(GPIOB, &gpio_initstructure);


  tmr_base_init(TMR1, 626, 0);
  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);

  tmr_output_default_para_init(&tmr_oc_init_structure);
  tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_oc_init_structure.oc_output_state = TRUE;
  tmr_oc_init_structure.oc_idle_state = FALSE;
  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_1, &tmr_oc_init_structure);
  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, srcBufferA[0]);

  tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
  tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
  tmr_oc_init_structure.oc_output_state = TRUE;
  tmr_oc_init_structure.oc_idle_state = FALSE;
  tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_2, &tmr_oc_init_structure);
  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_2, srcBufferB[0]);

    tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
    tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
    tmr_oc_init_structure.oc_output_state = TRUE;
    tmr_oc_init_structure.oc_idle_state = FALSE;
    tmr_output_channel_config(TMR1, TMR_SELECT_CHANNEL_3, &tmr_oc_init_structure);
    tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_3, srcBufferC[0]);
  /* overflow interrupt enable */
       tmr_interrupt_enable(TMR1, TMR_OVF_INT, TRUE);
       tmr_interrupt_enable(TMR1, TMR_C1_INT, TRUE);
       tmr_interrupt_enable(TMR1, TMR_C2_INT, TRUE);
       tmr_interrupt_enable(TMR1, TMR_C3_INT, TRUE);
        /* tmr1 overflow interrupt nvic init */
       nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
       nvic_irq_enable(TMR1_OVF_TMR10_IRQn, 0, 0);
       nvic_irq_enable(TMR1_CH_IRQn, 0, 0);
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
	dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
	dma_init_struct.memory_inc_enable = TRUE;
	dma_init_struct.peripheral_base_addr = (uint32_t)&TMR1->c1dt;
	dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
	dma_init_struct.peripheral_inc_enable = FALSE;
	dma_init_struct.priority = DMA_PRIORITY_VERY_HIGH;
	dma_init_struct.loop_mode_enable = TRUE;


	//	dma_flexible_config(DMA1, FLEX_CHANNEL2, DMA_FLEXIBLE_TMR1_OVERFLOW);

	dma_init(DMA1_CHANNEL2, &dma_init_struct);

    // // enable transfer full data interrupt
    // dma_interrupt_enable(DMA1_CHANNEL2, DMA_FDT_INT, TRUE);
	// //END OF SETTiNGS FOR CH1 of TMR1 and CH2 of DMA1 for phase A


	// //	CH2 of TMR1 and CH3 of DMA1 for phase B
    // // dma channel1 and 2 interrupt nvic init
    // nvic_irq_enable(DMA1_Channel2_IRQn, 0, 0);

	tmr_dma_request_enable(TMR1, TMR_C2_DMA_REQUEST, TRUE);

	dma_reset(DMA1_CHANNEL3);
	dma_init_struct.buffer_size = BUFF_SIZE;
	dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
	dma_init_struct.memory_base_addr = (uint32_t)srcBufferC;
	//	dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_WORD;
	dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
	dma_init_struct.memory_inc_enable = TRUE;
	dma_init_struct.peripheral_base_addr = (uint32_t)&TMR1->c2dt;
	dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
	dma_init_struct.peripheral_inc_enable = FALSE;
	dma_init_struct.priority = DMA_PRIORITY_VERY_HIGH;
	dma_init_struct.loop_mode_enable = TRUE;


//	dma_flexible_config(DMA1, FLEX_CHANNEL2, DMA_FLEXIBLE_TMR1_OVERFLOW);

	dma_init(DMA1_CHANNEL3, &dma_init_struct);

    // enable transfer full data interrupt
    // dma_interrupt_enable(DMA1_CHANNEL3, DMA_FDT_INT, TRUE);


    // // dma channel1 and 2 interrupt nvic init
    // nvic_irq_enable(DMA1_Channel3_IRQn, 0, 0);


//END OF SETTiNGS FOR CH2 of TMR1 and CH3 of DMA1 for phase B

//CH4 of TMR1 and CH4 of DMA1 for phase B
        // dma channel1 and 2 interrupt nvic init

    	tmr_dma_request_enable(TMR1, TMR_C3_DMA_REQUEST, TRUE);

    	dma_reset(DMA1_CHANNEL6);
    	dma_init_struct.buffer_size = BUFF_SIZE;
    	dma_init_struct.direction = DMA_DIR_MEMORY_TO_PERIPHERAL;
    	dma_init_struct.memory_base_addr = (uint32_t)srcBufferB;
    //	dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_WORD;
    	dma_init_struct.memory_data_width = DMA_MEMORY_DATA_WIDTH_HALFWORD;
    	dma_init_struct.memory_inc_enable = TRUE;
    	dma_init_struct.peripheral_base_addr = (uint32_t)&TMR1->c3dt;
    	dma_init_struct.peripheral_data_width = DMA_PERIPHERAL_DATA_WIDTH_HALFWORD;
    	dma_init_struct.peripheral_inc_enable = FALSE;
    	dma_init_struct.priority = DMA_PRIORITY_VERY_HIGH;
    	dma_init_struct.loop_mode_enable = TRUE;


    //	dma_flexible_config(DMA1, FLEX_CHANNEL2, DMA_FLEXIBLE_TMR1_OVERFLOW);

    	dma_init(DMA1_CHANNEL6, &dma_init_struct);

        // // enable transfer full data interrupt
        // dma_interrupt_enable(DMA1_CHANNEL6, DMA_FDT_INT, TRUE);


        // // dma channel1 and 2 interrupt nvic init
        // nvic_irq_enable(DMA1_Channel6_IRQn, 0, 0);


//END OF SETTiNGS FOR CH4 of TMR1 and CH4 of DMA1 for phase C


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

void DMA1_Channel5_IRQHandler() {
	if(dma_interrupt_flag_get(DMA1_FDT5_FLAG) != RESET)
	    {
	        dma_flag_clear(DMA1_FDT5_FLAG);
	    }
}
void DMA1_Channel6_IRQHandler() {
	if(dma_interrupt_flag_get(DMA1_FDT6_FLAG) != RESET)
	    {

	        dma_flag_clear(DMA1_FDT6_FLAG);

	    }
}


void TMR1_OVF_TMR10_IRQHandler(void)
{
  if(tmr_interrupt_flag_get(TMR1, TMR_OVF_FLAG) != RESET)
  {
//    /* add user code... */

     //GPIOA->scr = GPIO_PINS_8;
     //GPIOB->scr = GPIO_PINS_14;
     //GPIOB->scr = GPIO_PINS_15;
  	tmr_counter_enable(TMR2, TRUE);

    tmr_flag_clear(TMR1, TMR_OVF_FLAG);
  }
}

void TMR1_CH_IRQHandler(void) {

	if(tmr_interrupt_flag_get(TMR1, TMR_C1_FLAG) != RESET) {
//		uint32_t channelValue = 0;


		// GPIOA->clr = GPIO_PINS_8;
		PhaseA = tmr_channel_value_get(TMR1, TMR_SELECT_CHANNEL_1);
		tmr_flag_clear(TMR1, TMR_C1_FLAG);

	  }
 	else if(tmr_interrupt_flag_get(TMR1, TMR_C2_FLAG) != RESET) {
 		PhaseB = tmr_channel_value_get(TMR1, TMR_SELECT_CHANNEL_2);
 		tmr_flag_clear(TMR1, TMR_C2_FLAG); 	  }
 	else if(tmr_interrupt_flag_get(TMR1, TMR_C3_FLAG) != RESET) {
		PhaseC = tmr_channel_value_get(TMR1, TMR_SELECT_CHANNEL_3);
		tmr_flag_clear(TMR1, TMR_C3_FLAG);
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
		  tmr_base_init(TMR2, 626, 0);
	//	  tmr_base_init(TMR1, 248, (crm_clocks_freq_struct.ahb_freq / 10000000) - 1);
		  tmr_cnt_dir_set(TMR2, TMR_COUNT_UP);


		  /* overflow interrupt enable */
		  tmr_interrupt_enable(TMR2, TMR_OVF_INT, TRUE);
	//		  tmr_interrupt_enable(TMR2, TMR_C1_INT, TRUE);
	//		  tmr_channel_value_set(TMR2, TMR_SELECT_CHANNEL_1, 100);
		  /* tmr1 overflow interrupt nvic init */
		  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
		  nvic_irq_enable(TMR2_GLOBAL_IRQn, 0, 0);


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


}

void TMR2_GLOBAL_IRQHandler() {
	if(tmr_interrupt_flag_get(TMR2, TMR_C1_FLAG) != RESET) {
		tmr_flag_clear(TMR2, TMR_C1_FLAG);
	}
	else if(tmr_interrupt_flag_get(TMR2, TMR_OVF_FLAG) != RESET) {
	//    /* add user code... */
		// ADJUST PINS WITH ALTIUMDESIGN
		// PhaseA = tmr_channel_value_get(TMR1, TMR_SELECT_CHANNEL_1);
		// PhaseB = tmr_channel_value_get(TMR1, TMR_SELECT_CHANNEL_2);
		// PhaseC = tmr_channel_value_get(TMR1, TMR_SELECT_CHANNEL_3);

		PhaseA > PhaseB ? (void)(GPIOA->scr = GPIO_PINS_6) : (void)(GPIOA->clr = GPIO_PINS_6);
		PhaseB > PhaseC ? (void)(GPIOA->scr = GPIO_PINS_5) : (void)(GPIOA->clr = GPIO_PINS_5);
		PhaseC > PhaseA ? (void)(GPIOA->scr = GPIO_PINS_4) : (void)(GPIOA->clr = GPIO_PINS_4);

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

		GPIOA->scr = GPIO_PINS_4;
		GPIOA->scr = GPIO_PINS_5;
		GPIOA->scr = GPIO_PINS_6;
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

  StatusLedInit();

//   PWMPinsInit();
  tmr1_config();
  DMAInit();

  Compare_Pins_Init();
  TMR_Compare_Init();


  dma_channel_enable(DMA1_CHANNEL2, TRUE);
  dma_channel_enable(DMA1_CHANNEL3, TRUE);
  dma_channel_enable(DMA1_CHANNEL6, TRUE);

  tmr_channel_enable(TMR1, TMR_SELECT_CHANNEL_1, TRUE);
  tmr_channel_enable(TMR1, TMR_SELECT_CHANNEL_2C, TRUE);
  tmr_channel_enable(TMR1, TMR_SELECT_CHANNEL_3C, TRUE);
  tmr_output_enable(TMR1, TRUE);

  tmr_counter_enable(TMR1, TRUE);
//   debug_periph_mode_set(DEBUG_TMR1_PAUSE, TRUE);
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