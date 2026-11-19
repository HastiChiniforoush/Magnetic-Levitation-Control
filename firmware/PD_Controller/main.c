/**
	@author Thomas Grunenberg
	@author Hasti Chiniforoush
	@version 0.1
	@file main.c
	@brief Software for Lev. Magnet
*/

/**
	@brief The CPU speed in Hz
*/
#define F_CPU 20000000UL

const char MtrNum[] __attribute__((__progmem__)) = "33340";



/******************************************************************************/
/* INCLUDED FILES                                                             */
/******************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "hall.h"
#include "led.h"
#include "uart.h"
#include "magnet.h"



/******************************************************************************/

/******************************************************************************/
/* Global memory                                                              */
/******************************************************************************/


/******************************************************************************/
/* FUNCTIONS                                                                  */
/******************************************************************************/

void long_delay(uint16_t time){

	while(time > 0){
		if (time > 255){
			_delay_ms(255);
			time -= 255;
		} else {
			_delay_ms(1);
			time--;
		}
	}
	
	return;
}
/******************************************************************************/


/**
	@brief Main function
	@return only a dummy to avoid a compiler warning, not used
*/
int main(void){
	
	uint8_t i, aim;
	uint16_t comcount;
	uint8_t Kp = 75;
	uint8_t Kd = 10;//derivative gain scaled by 10
	const uint8_t gainScale = 20;
	uint8_t setPoint;
	uint8_t input;
	int16_t output;//PD controller output
	int16_t error;
	int16_t previousError = 0;//previous error for derivative term
	int16_t P;//proportional term
	int16_t D;//derivative term
	uint8_t basePower = 220;
	
	led_init();
	uart_init();
	magnet_init();
	hall_init();
	comcount = 0;
	
	sei();
	magnet_set(0);
	led_green(1);
	_delay_ms(250);
	led_green(0);
	
	led_red(1);
	for (i = 0; i < 255; i++){
		magnet_set(i);
		_delay_ms(1);
		hall_wait();
		hall_calibrate(i);
	}
	led_red(0);
	magnet_set(255);
	hall_wait();
	hall_calibrate(255);

	magnet_set(0);
	hall_setmagpower(0);

	led_green(1);
	aim = 0x88;
	setPoint = aim;

	for(;;){

		input = hall_get();
		
		error = (int16_t)setPoint - (int16_t)input;

		int16_t abs_error = (error < 0) ? -error : error;
		if (abs_error < 5) {
			led_green(1);
			led_red(0);
		} else if (abs_error > 20) {
			led_green(0);
			led_red(1);
		} else {
			led_green(1);
			led_red(1);
		}
		
		//proportional term
		P = ((int16_t)Kp * error) / gainScale;
		
		//derivative term
		D = ((int16_t)Kd * (error - previousError)) / gainScale;
		
		//compute PD controller output
		output = (int16_t)basePower - P - D;
		
		if (output > 255) output = 255;
		if (output < 0) output = 0;
		
		//storing current error for next derivative calculation
		previousError = error;
		
		magnet_set((uint8_t)output);
		hall_setmagpower((uint8_t)output);
		
		_delay_ms(1);

	}

	return 0;
}
/******************************************************************************/

