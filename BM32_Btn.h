#ifndef __BM32_BTN_H__
#define __BM32_BTN_H__
#include "Arduino.h"

#define BTN_ON_CNT   	10 
#define BTN_OFF_CNT 	50 
#define BTN_RELEASE_CNT 10 
#define BTN_NBR_OF       8
 
#define PRESS_BUFF_LEN  4
typedef enum
{
	PRESSED_UNDEFINED = 0,
	PRESSED_NOT,
	PRESSED_ON,
	PRESSED_OFF
} btn_status_et;

typedef struct
{
	uint32_t 	bm32;
	//char 		tag;
    byte 	state;
    byte 	cntr;
	btn_status_et pressed;
} bm32_st;	

class Bm32Btn{
public:
    Bm32Btn();
    void 			Init(void);
	uint8_t 		AddBm(uint32_t key_bm32);
    void 			Process(uint32_t pressed_bm32);
    btn_status_et	Read(uint8_t bindx);
private:
	uint8_t free_indx;
	bm32_st  bm[BTN_NBR_OF];
};
#endif
