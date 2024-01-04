/*
* standard.c
*
* Created: 11/17/2021 3:55:48 PM
* Author : Nhan
*/
#include <stdbool.h>
#include <avr/io.h>
#include "userdef.h"
#define F_CPU 1000000
#include<util/delay.h>

int i;


//------led_don----------//

unsigned long led = 0x55555555;

void led_display (unsigned long led)
{
	LED0 = led&0x000000ff;
	LED1 = (led>>8)&0x000000ff;
	LED2 = (led>>16)&0x000000ff;
	LED3 = (led>>24)&0x000000ff;
}

//--------LED_7_DOAN-------------//

unsigned char SegCode[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,
0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};

void SevenSegDisplay(unsigned int value)
{
	LED7D0 = SegCode[value%10];
	LED7D1 = SegCode[(value/10)%10];
	LED7D2 = SegCode[(value/100)%10%10];
	LED7D3 = SegCode[(value/1000)%10%10];
}

void SevenSegDisplayScan(unsigned int value)
{
	LED7SCAN_DATA = SegCode[value%10];
	LED7SCAN_CTR = 0b11111110; _delay_ms(50); LED7SCAN_CTR = 0xff;
	LED7SCAN_DATA = SegCode[(value/10)%10];
	LED7SCAN_CTR = 0b11111101; _delay_ms(50); LED7SCAN_CTR = 0xff;
	LED7SCAN_DATA = SegCode[(value/100)%10%10];
	LED7SCAN_CTR = 0b11111011; _delay_ms(50); LED7SCAN_CTR = 0xff;
	LED7SCAN_DATA = SegCode[(value/1000)%10%10];
	LED7SCAN_CTR = 0b11110111; _delay_ms(50); LED7SCAN_CTR = 0xff;
}

//---------LCD----------//

void LcdInit()
{
	LCD_INS = 0x38 ; LCDE_H ; _delay_us(1); LCDE_L  ; _delay_us(1);
	_delay_us(200);
	LCD_INS = 0x0C ; LCDE_H ; _delay_us(1); LCDE_L  ; _delay_us(1);
	_delay_us(200);
	LCD_INS = 0x06 ; LCDE_H ; _delay_us(1); LCDE_L  ; _delay_us(1);
	_delay_us(200);
	LCD_INS = 0x01 ; LCDE_H ; _delay_us(1); LCDE_L  ; _delay_us(1);
	_delay_us(200);

}
void PrintL(const char *str, unsigned line, unsigned col)
{
	unsigned char add;
	switch(line)
	{
		case 0: add = 0x80; break ;
		case 1: add = 0xC0; break ;
		case 2: add = 0x94; break ;
		case 3: add = 0xD4; break ;
	}
	LCD_INS = add + col;  LCDE_H ; _delay_us(1); LCDE_L  ; _delay_us(1);
	_delay_us(50);
	while( *str != '\0')
	{LCD_DATA = *str++; LCDE_H ; _delay_us(1); LCDE_L  ; _delay_us(1);
		_delay_us(10);
	}
}


// ---------Menu Display---------- //

void MenuDisplay(Menu *C_pMenu, unsigned char select){
	LcdInit();
	PrintL(C_pMenu->m_sTitle,0,0);
	
	PrintL(C_pMenu->m_sList1,1,0);
	PrintL(C_pMenu->m_sList2,2,0);
	PrintL(C_pMenu->m_sList3,3,0);
	
	PrintL(">",select,0);
}

void ActuatorsActivate(char Devices, char Status){
	switch (Devices){
		case Device_Relay: if(Status) {C_Relays.relayAll=0xff;}else{C_Relays.relayAll=0x00;}
		RELAY_ACTIVATION;
		break;
		case Device_Motor1: C_Motors.DC1=Status; MOTOR_ACTIVATION;
		break;
		case Device_Motor2: C_Motors.DC2=Status; MOTOR_ACTIVATION;
		break;
		case Device_LED: if(Status) {LED0=0xff;}else{LED0=0x00;}
	}
}


/* ---------One_Wire---------- */

char OneWireReset(void){
	char Presene;
	ONEWIRE_MASTER; ONEWIRE_LOW; _delay_us(480); ONEWIRE_FREE; _delay_us(80);
	
	Presene = ONEWIRE_STATE;	_delay_us(60);
	return 1;
}

void OneWireWriteByte(unsigned char Byte){
	unsigned char i;
	for(i=0;i<8;i++){
		ONEWIRE_MASTER;
		if(Byte&0x01){ONEWIRE_LOW;	_delay_us(15);	ONEWIRE_FREE;_delay_us(45);	_delay_us(50);} 
		else{ ONEWIRE_LOW;	_delay_us(15);_delay_us(45);	ONEWIRE_FREE;	_delay_us(30); }
		Byte= Byte>>1;
	}
}

unsigned char OneWireReadByte(void){
	unsigned char i;
	unsigned int Byte =0;
	for(i=0;i<8;i++){
		ONEWIRE_MASTER; 
		ONEWIRE_LOW;	_delay_us(15);	ONEWIRE_FREE;	_delay_us(15);
		Byte=Byte|(ONEWIRE_STATE<<7); _delay_us(30);
	}
	return Byte;
}


unsigned int ReadTemp(void){
	unsigned char data[2];
	if(OneWireReset()){
		OneWireWriteByte(0xCC);
		OneWireWriteByte(0x44);
		ONEWIRE_FREE; _delay_ms(750);
		data[0]=OneWireReset();
		OneWireWriteByte(0xCC);
		OneWireWriteByte(0xBE);
		data[0]=OneWireReadByte();
		data[1]=OneWireReadByte();
		return (data[0]+data[1]*256)*0.0625;
	}
	else{return 0;}
}






void main(void)
{
	MCUCR |= 0x80;
	PORTG = 0x0F; //LCD enable
	XMCRA =0;
	DDRG = 0x0f;
	LcdInit();

	SevenSegDisplay(4321);
	SevenSegDisplay(0);
	
	PrintL("    NGUYEN TRONG NHAN",0,0);
	PrintL("    1311066",2,0);
	_delay_ms(1000);


	unsigned int count=0;
	char count_stattus =1;
/*
	while(1){
		
		READKEY;


		READKEY;
		if(!C_Buttons.DOWN){
			while(!C_Buttons.DOWN) READKEY;
			count_stattus=(0)?1:0;
			}
			
		C_Relays.relayBit.RL0=count_stattus; RELAY_ACTIVATION;
		SevenSegDisplay(count);
		_delay_ms(50);
		
	}
*/
	while(1){
		READKEY;
		if(!C_Buttons.DOWN){
			while(!C_Buttons.DOWN) READKEY;
			count_stattus=(0)?1:0;
			
		}
		C_Relays.relayBit.RL0=count_stattus; RELAY_ACTIVATION;

		if(count_stattus) {
			if(count==99){count=0;}else{count++;}
			}else{
			if(count==0){count=99;}else{count--;}
		}
		SevenSegDisplay(count);
		_delay_ms(100);
		}
		


	

/*	


	unsigned int Temp;

	if(OneWireReset()){PrintL("DSB18B20 OK",0,3);}else{PrintL("DSB18B20 FAILED",0,3);}
		
	while (1)
	{
		Temp=ReadTemp();
		SevenSegDisplay(Temp);
	}



//-------------------------Menu---------------------------//


	Menu *C_pMenu;
	C_pMenu = &C_MainMenu;
	unsigned char select =1;
	MenuDisplay(C_pMenu, select);
	while(1){
		READKEY;
		if(!C_Buttons.DOWN){
			while(!C_Buttons.DOWN) READKEY;
			select= (select==3)?1:select+1;
			MenuDisplay(C_pMenu, select);
			
		}
		
		if(!C_Buttons.UP){
			while(!C_Buttons.UP) READKEY;
			select= (select==1)?3:select-1;
			MenuDisplay(C_pMenu, select);
			
		}
		
		
		if(!C_Buttons.RIGHT){
			READKEY;
			while(!C_Buttons.RIGHT){ READKEY;}
			switch (select){
				case 1: C_pMenu=(C_pMenu->m_pMenulist1 ==NULL)?C_pMenu:C_pMenu->m_pMenulist1; break;
				case 2: C_pMenu=(C_pMenu->m_pMenulist2 ==NULL)?C_pMenu:C_pMenu->m_pMenulist2; break;
				case 3: C_pMenu=(C_pMenu->m_pMenulist3 ==NULL)?C_pMenu:C_pMenu->m_pMenulist3; break;
				default: break;
			}

			MenuDisplay(C_pMenu, select);
			
		}

		if(!C_Buttons.LEFT){
			READKEY;
			while(!C_Buttons.LEFT){ READKEY;}
			C_pMenu=(C_pMenu->m_pPre ==NULL)?C_pMenu:C_pMenu->m_pPre;
			MenuDisplay(C_pMenu, select);
			
		}
		
		
		if(!C_Buttons.GO){
			READKEY;
			while(!C_Buttons.GO){ READKEY;}
			switch (select){
				case 1: if(C_pMenu->ActivationON==NULL) break;
					C_pMenu->ActivationON(C_pMenu->MenuID,ON); break;
				case 2: if(C_pMenu->ActivationON==NULL) break;
					C_pMenu->ActivationON(C_pMenu->MenuID,OFF); break;

			}

		MenuDisplay(C_pMenu, select);
			
		}		
		
	}

/*	
	
//-------------------------1_2_led_don----------------------//
	
	while(1)
	for(i=0;i<32;i++)
	{
		led = (unsigned long) 1<<i;
		led_display(led);
		_delay_ms(200);
	}
	

//---------------------------Relay--------------------------//
	for(i=0;i<8;i++)
	{
		C_Relays.relayAll = (unsigned char) 1<<i;
		RELAY_ACTIVATION;
		_delay_ms(200);
	}
	C_Relays.relayAll = 0x00;
	C_Relays.relayBit.RL0 =1;


//-------------------------Motor---------------------------//

	while(1)
	{
		READKEY;
		if(!C_Buttons.LEFT)
		{
			while(!C_Buttons.LEFT) READKEY;
			C_Motors.DC1 = FORWARD;
		}
		if(!C_Buttons.RIGHT)
		{
			while(!C_Buttons.RIGHT) READKEY;
			C_Motors.DC1 = REWARD;
		}
		MOTOR_ACTIVATION;
	
	}
	
	
*/	
	
	return;
}

