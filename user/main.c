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

uint8_t g_speed = FAST;
crm_clocks_freq_type crm_clocks_freq_struct = {0};
uint8_t statusLedFlag = 0;
uint32_t chOutputCompareValue = 199;

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
	/* set default parameter */
	gpio_default_para_init(&gpio_init_struct);
	/* clkout gpio init */
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_8; //
	gpio_init_struct.gpio_pull = GPIO_PULL_UP;
	gpio_init(GPIOA, &gpio_init_struct); //

//	PB15
	/* enable gpio port clock */
	gpio_init_struct.gpio_pins = GPIO_PINS_15; //
	gpio_init(GPIOB, &gpio_init_struct); //

//	PB14
	gpio_init_struct.gpio_pins = GPIO_PINS_14; //
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
	  tmr_base_init(TMR1, 9999, (crm_clocks_freq_struct.ahb_freq / 10000) - 1);
	  tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);

	  /* overflow interrupt enable */
	  tmr_interrupt_enable(TMR1, TMR_OVF_INT, TRUE);
	  tmr_interrupt_enable(TMR1, TMR_C1_INT, TRUE);
	  tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, chOutputCompareValue);
	  /* tmr1 overflow interrupt nvic init */
	  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
	  nvic_irq_enable(TMR1_OVF_TMR10_IRQn, 0, 0);
	  nvic_irq_enable(TMR1_CH_IRQn, 0, 0);

	  /* enable tmr1 */
	  tmr_counter_enable(TMR1, TRUE);
}

void TMR1_OVF_TMR10_IRQHandler(void)
{
  if(tmr_interrupt_flag_get(TMR1, TMR_OVF_FLAG) != RESET)
  {
    /* add user code... */
    chOutputCompareValue += 100;

    if (chOutputCompareValue >= 9999) {
    	chOutputCompareValue = 999;
    }
    tmr_channel_value_set(TMR1, TMR_SELECT_CHANNEL_1, chOutputCompareValue);
    tmr_flag_clear(TMR1, TMR_OVF_FLAG);
    gpio_bits_reset(GPIOA, GPIO_PINS_0);
    statusLedFlag = 0;
    tmr_flag_clear(TMR1, TMR_OVF_FLAG);
  }
}

void TMR1_CH_IRQHandler(void) {
	gpio_bits_set(GPIOA, GPIO_PINS_0);
	statusLedFlag = 1;
	tmr_flag_clear(TMR1, TMR_C1_FLAG);
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
  TMR1Init();



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
