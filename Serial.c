/*
 * Serial.c
 *
 * Created: 5/8/2021 5:19:32 AM
 *  Author: hp
 */ 
#define F_CPU 8000000UL
#include <util/delay.h>

//#include <math.h>
//#include <stdio.h>
#include "data_types.h"

#include "Serial.h"

#include "Uart.h"

#include "Timers.h"

#include "Sensors.h"

#include "DIO_Interface.h"
#include <stdio.h>
#define TRIGERING_TIME (100) 
#define  ARR_MAX       (6)
	/*
	*Send every 100ms
	*Send data with no blockage
	*begin new conversioning
	
	*/
uint8 x;
static void Serial_excution() // Update function
{
	static uint8 Time_Counter=0;
	
	static uint16 Adc_Value=0;
	//Sending fraction
	static Bool_t fraction_flag=FALSE;
	
	static uint8 real_part=0;
	
	static uint8 frac_part=0; 
    
	double fraction=20.95;
	
    static uint16 conv_float_t_int=0;
	
	uint16 temp1=0;
	
	uint16 temp2=0;
	  if (Time_Counter!=TRIGERING_TIME)
	  {
		  if (fraction_flag==TRUE)
		  {
			  
               //  UART_ASY_TxChar(frac_part);
			  UART_ASY_TxChar(5);
			  fraction_flag=FALSE;
		  }
		  
		  Time_Counter++;
		return;  
	  }
	  else
	  {
		  // error in sending float 
		  
		  /*
		  1)Get float from adc or make it float 
		  2) extract real & fraction numbers using modf from <<math.h>> libaray
		  3)send real first 
		  4)raise fraction_flag
		  5)Send fraction 
		  6)Low down flag  
		  */
		  
          //Adc_Value=Volt_read();
		  //x=((uint16)Adc_Value/1000);
		  
		  //if (x>2)
		  //{
			  //fraction=((uint32)x)+1.1;
		  //}
		  // extracted real & fraction
		  //f=modf(fraction,&i);
		  //f=(f*100.00);
		  //real=(fraction);
		  
		  //frac=fraction-(real);
		  
		  //frac=frac*100;
		  // sending real
		  //fraction=fraction*100.00;
		  
		  //real=(uint8)(fraction);
		  //UART_ASY_TxChar(real);
		  
		  /*
		  x=((uint16)Adc_Value/1000);
		  f=modf(,&i);
		  		  
		  UART_ASY_TxChar(x);
		  */
		  
		  Time_Counter=0;
		  
		  conv_float_t_int=(uint16)(fraction*100);
		  if (conv_float_t_int<5)
		  {
			  fraction=20.95;
		  }
		  else
		  {
			  fraction=fraction-1.5;
		  }
		  temp1=(conv_float_t_int%100);
		  
		  frac_part=(uint8)(temp1);
		  
		  temp2=(((uint16)(conv_float_t_int-temp1)/100));
		  
		  real_part=(uint8)temp2;
		  			  
		  UART_ASY_TxChar(real_part);
		  //gcvt()
		  
		  fraction_flag=TRUE;
		  
		 // fraction=fraction+(1.1);
		  //For testing 
		  //Toggle_Pin(PINC0);	
	  }


	//UART_TxChar(c);
}
void Serial_Init()
{
    UART_init(9600);
    //For testing 
	Pin_Init(PINC0,OUTPUT);
	
	Set_Pin(PINC0,LOW);	
	
	Timer1_CTC_OC1RA_Set_Call_Back(Serial_excution);
	
	ADC_Init(AVCC,CLK_ADC_DIV_64);
	
	_delay_ms(500);
	
	ADC_Converstion(Conversion_on);
	
	Timer1_OCR1A=999;
	
	Timer1_Init(TIMER1_CTC_OCRA_TOP_MODE,TIMER1_PRESCALING_CLK8,OC1RA_DISCONNECTED,OC1RB_DISCONNECTED);
	
	Timer1_CTC_OCR1A_Interrupt_State(TIMER1_CTC_OC1RA_INT_ENABLE);
	
	GLOBAL_INTERUPT_ENABLE;
	
    
}
