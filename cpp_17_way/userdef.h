#include <stddef.h>
#include <cstdint>

#define EXMEM_ADD 0x1100 //ATMEGA



typedef struct RelayBit{
	unsigned char RL0 : 1;
	unsigned char RL1 : 1;
	unsigned char RL2 : 1;
	unsigned char RL3 : 1;
	unsigned char RL4 : 1;
	unsigned char RL5 : 1;
	unsigned char RL6 : 1;
	unsigned char RL7 : 1;
} RelayBit;

typedef union RelayRegister{
	volatile char relay_all;
	volatile RelayBit relay_bit;
} RelayRegister;



class Relay{
private:
    RelayRegister *const m_register;

public:
    Relay(uint32_t addr){
        m_register = reinterpret_cast<RelayRegister *>(addr);
    }

};

