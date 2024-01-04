#include <stddef.h>

#define EXMEM_ADD 0x1100
//--CS-define----------//
#define CS0 0
#define CS1 1
#define CS2 2
#define CS3 3
#define CS4 4
#define CS5 5
#define CS6 6
#define CS7 7
#define CS8 8
#define CS9 9
#define CS10 10
#define CS11 11
#define CS12 12
#define CS13 13
#define CS14 14
#define CS15 15
#define CS16 16
#define CS17 17
#define CS18 18
#define CS19 19



//------led_don----------//


#define LED0 *(unsigned char *) (EXMEM_ADD+CS0)
#define LED1 *(unsigned char *) (EXMEM_ADD+CS1)
#define LED2 *(unsigned char *) (EXMEM_ADD+CS2)
#define LED3 *(unsigned char *) (EXMEM_ADD+CS3)

//--------LED_7_DOAN-------------//



#define LED7D0 *(unsigned char *) (EXMEM_ADD+CS4)
#define LED7D1 *(unsigned char *) (EXMEM_ADD+CS5)
#define LED7D2 *(unsigned char *) (EXMEM_ADD+CS6)
#define LED7D3 *(unsigned char *) (EXMEM_ADD+CS7)
//---------LED_7_DOAN_SCAN----------//
#define LED7SCAN_CTR *(unsigned char *) (EXMEM_ADD+CS8)
#define LED7SCAN_DATA *(unsigned char *) (EXMEM_ADD+CS9)


//---------relay----------//

struct RelayBit{
	unsigned char RL0 : 1;
	unsigned char RL1 : 1;
	unsigned char RL2 : 1;
	unsigned char RL3 : 1;
	unsigned char RL4 : 1;
	unsigned char RL5 : 1;
	unsigned char RL6 : 1;
	unsigned char RL7 : 1;
};

union Relay{
	unsigned char relayAll ;
	struct RelayBit relayBit;
};
union Relay C_Relays;

#define RELAYMAP *( volatile unsigned char*) (EXMEM_ADD+CS10)
#define RELAY_ACTIVATION (RELAYMAP=*( unsigned char*)(&C_Relays))//Assign C_Relays value to External Memory


//---------Button----------//

struct Button
{
	unsigned char LEFT : 1;
	unsigned char RIGHT : 1;
	unsigned char UP : 1;
	unsigned char DOWN : 1;
	unsigned char GO : 1;
	unsigned char BTN1 : 1;
	unsigned char BTN2 : 1;
	unsigned char BTN3 : 1;

};
struct Button C_Buttons ;

#define BUTTONMAP *(volatile unsigned char*) (EXMEM_ADD+CS14)
#define READKEY (*(unsigned char*)(&C_Buttons)= BUTTONMAP) //Assign from External Memory to C_Buttons


//---------Motor----------//

struct Motor
{
	unsigned char DC1 :2;
	unsigned char DC2 :2;
	unsigned char DCfree :4;
};

struct Motor C_Motors ;

#define MOTORMAP *( volatile unsigned char*) (EXMEM_ADD+CS13)
#define MOTOR_ACTIVATION (MOTORMAP=*( unsigned char*)(&C_Motors))

#define STOP 0
#define FORWARD 1
#define REWARD 2


//---------LCD----------//
#define LCDE_H (PORTG |= (1<<3))
#define LCDE_L (PORTG &= ~(1<<3))

#define LCD_DATA *(unsigned char *) (EXMEM_ADD+CS11)
#define LCD_INS *(unsigned char *) (EXMEM_ADD+CS12)




/* ---------One_Wire---------- */

#define DSB18B20 0
#define ONEWIRE_HIGH (PORTB !=(1<<0))
#define ONEWIRE_LOW (PORTB &= ~(1<<0))
#define ONEWIRE_MASTER (DDRB !=(1<<0))
#define ONEWIRE_FREE (DDRB&= ~(1<<0))
#define ONEWIRE_STATE (PINB&(1<<0))



/* ---------Menu_Display---------- */

#define ON 1
#define OFF 0

#define Device_Relay 0
#define Device_Motor1 1
#define Device_Motor2 2
#define Device_LED 3

void ActuatorsActivate(char Devices, char Status);


typedef struct Linker {
	char MenuID;
	struct Linker *m_pPre;
	char m_sTitle[20];

	char m_sList1[20];
	struct Linker *m_pMenulist1; void (*ActivationON)(char, char);

	char m_sList2[20];
	struct Linker *m_pMenulist2; void (*ActivationOFF)(char, char);

	char m_sList3[20];
	struct Linker *m_pMenulist3
} Menu;

Menu C_MainMenu, C_SensorMenu, C_ActuatorsMenu;
Menu C_MotorsMenu, C_Motor1Menu, C_Motor2Menu;

Menu C_MainMenu ={
	NULL,
	NULL,
	"    MAIN MENU ",
	
	"    SENSOR ", &C_SensorMenu,NULL,
	"    ACTUATOR ", &C_ActuatorsMenu,NULL,
	"    SETTING ", NULL,
};

Menu C_ActuatorsMenu ={
	NULL,
	&C_MainMenu,
	"    Actuators ",
	
	"    Relays ", NULL, NULL,
	"    Motors ", &C_MotorsMenu, NULL,
	"    Leds ", NULL,
};

Menu C_MotorsMenu ={
	NULL,
	&C_ActuatorsMenu,
	"    MOTORS ",
	"    motor1 ", &C_Motor1Menu,NULL,
	"    motor2 ", &C_Motor2Menu,NULL,
	NULL, NULL,
};


Menu C_Motor1Menu ={
	Device_Motor1,
	&C_MotorsMenu,
	"    Motor1 ",
	
	"    ON ",NULL, &ActuatorsActivate,
	"    OFF ",NULL, &ActuatorsActivate,
	NULL, NULL,
};

Menu C_Motor2Menu ={
	Device_Motor2,
	&C_MainMenu,
	"    Motor2 ",
	"    ON ", NULL, &ActuatorsActivate,
	"    OFF ", NULL, &ActuatorsActivate,
	NULL, NULL,
};






Menu C_SensorMenu ={
	NULL,
	&C_MainMenu,
	"    SENSORS MENU ",
	"    Temperature ", NULL,NULL,
	"    Humidity ", NULL,NULL,
	"    Date & Time ", NULL,NULL
};





