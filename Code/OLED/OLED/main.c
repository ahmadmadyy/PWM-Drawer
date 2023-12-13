#include <avr/io.h>
#include <util/delay.h>
#include <u8g2.h>
#include <u8x8_avr.h>
#include <stdlib.h>
#include "ADC.h"
#include "Timer.h"
#include "main.h"

#define SSD1306_ADDR 0x78

u8g2_t u8g2;

// To calculate the duty cycle from the value of the OCRx register
float calc_duty_cycle(void)
{
	#define MAX_VALUE 255/8
	float d = ((OCR0A)/(1.0*(MAX_VALUE))) * 100.0;
	return d;
}

// To draw the square wave
void drawMultiplePWMWaveforms(float dutyCycle, uint8_t numCycles)
{
	u8g2_ClearBuffer(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_5x8_tf);
	u8g2_SetFontRefHeightText(&u8g2);
	u8g2_SetFontPosTop(&u8g2);

	// Calculate the width of each cycle
	float cycleWidth = 128.0 / numCycles;

	for (uint8_t i = 0; i < numCycles; ++i)
	{
		// Define the start of each rising and falling edge
		float startX = i * cycleWidth;
		float endX = (i + 1) * cycleWidth;

		// Draw the vertical line
		u8g2_DrawLine(&u8g2, startX, 30, startX, 60);

		// Draw the first horizontal line
		u8g2_DrawLine(&u8g2, startX + 1, 30, startX + 1 + (dutyCycle * cycleWidth / 100.0), 30);

		// Draw the second vertical line
		u8g2_DrawLine(&u8g2, startX + 1 + (dutyCycle * cycleWidth / 100.0), 30, startX + 1 + (dutyCycle * cycleWidth / 100.0), 60);

		// Draw the second horizontal line
		u8g2_DrawLine(&u8g2, startX + 1 + (dutyCycle * cycleWidth / 100.0), 60, endX, 60);
	}

	u8g2_SendBuffer(&u8g2);
}

int main(void)
{
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte_avr_hw_i2c, u8x8_avr_delay);
	u8g2_SetI2CAddress(&u8g2, SSD1306_ADDR);
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);

	adc_init();
	initPWM();

	while (1)
	{
		uint16_t adc_value = adc_read();
		setPWMDutyCycle(adc_value);
		float dutyCycle = calc_duty_cycle();

		char buffer2[20];
		itoa(dutyCycle, buffer2, 10);

		// Display 5 cycles simultaneously
		drawMultiplePWMWaveforms(dutyCycle, 5);
		//_delay_ms(100);
		
		u8g2_DrawStr(&u8g2, 15, 10, "Time: 0.25 milli-sec");
		u8g2_DrawStr(&u8g2, 0, 0, "F: 3.9 kHz ");
		//u8g2_DrawStr(&u8g2, 10, 0, "3.9 ");
		//u8g2_DrawStr(&u8g2, 17, 0, "kHz");
		u8g2_DrawStr(&u8g2, 55, 0, "Duty Cycle:");
		u8g2_DrawStr(&u8g2, 110, 0, buffer2);
		u8g2_DrawStr(&u8g2, 124, 0, "%");
		u8g2_SendBuffer(&u8g2);
		
		_delay_ms(100);
	}

	return 0;
}


