#include "BM32_Btn.h"

Bm32Btn::Bm32Btn(){
	free_indx = 0;

}

void Bm32Btn::Init(void)
{
	for(uint8_t indx = 0; indx < BTN_NBR_OF; indx++)
	{
		bm[indx].bm32 = 0;
		bm[indx].state = 0;
		bm[indx].cntr = 0;
		bm[indx].pressed = PRESSED_UNDEFINED;
	}
	free_indx = 0;
};

uint8_t Bm32Btn::AddBm(uint32_t key_bm32)
{
	uint8_t btn_indx = 255;
	if (free_indx <  BTN_NBR_OF)
	{
		btn_indx = free_indx++;
		bm[btn_indx].bm32 = key_bm32; 
	}
	return btn_indx;
}


void Bm32Btn::Process(uint32_t pressed_bm32)
{
	for(uint8_t indx = 0; indx < BTN_NBR_OF; indx++)
	{
		if(bm[indx].bm32 != 0)
		{
			switch(bm[indx].state)
			{
				case 0:  // idle state
					if ((pressed_bm32 & bm[indx].bm32) != 0)
					{
						bm[indx].state = 1;
						bm[indx].cntr = 0;
						bm[indx].pressed = PRESSED_NOT;
					}
					break;
				case 1:  //  pressed, debounch  
					if ((pressed_bm32 & bm[indx].bm32) != 0)
					{
						if (bm[indx].cntr++ >= BTN_ON_CNT) 
						bm[indx].state = 2;
						bm[indx].pressed = PRESSED_ON;
					}
					else bm[indx].state = 0;
					break;
				case 2:  // ON state  
					if ((pressed_bm32 & bm[indx].bm32) != 0)
					{
						if (bm[indx].cntr++ >= BTN_OFF_CNT) 
						{
							bm[indx].state = 3;
							bm[indx].pressed = PRESSED_OFF;
						}
					}
					else 
					{
						bm[indx].state = 10;
						bm[indx].cntr = 0;
					}
					break;
				case 3:  // OFF state 
					if ((pressed_bm32 & bm[indx].bm32) != 0)
					{
						// just wait
					}
					else
					{
						bm[indx].state = 10;
						bm[indx].cntr = 0;
					}
					break;
				case 10:  // released
					if ((pressed_bm32 & bm[indx].bm32) == 0)
					{
						if (bm[indx].cntr++ >= BTN_RELEASE_CNT) 
						bm[indx].state = 20;
					}
					break;
				case 20:  // released, wait for value to be read	
					if (bm[indx].pressed == PRESSED_UNDEFINED) bm[indx].state = 0;
					break;
			}
		}
			
	}
}

btn_status_et Bm32Btn::Read(uint8_t bindx)
{
	btn_status_et btn_status = PRESSED_UNDEFINED;
	if (bindx < BTN_NBR_OF)
	{
		if (bm[bindx].state == 20)
		{
			btn_status = bm[bindx].pressed;
			bm[bindx].pressed = PRESSED_UNDEFINED;
		}
	}
	return btn_status;
}
