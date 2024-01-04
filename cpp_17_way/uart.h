#include <stddef.h>
#include <cstdint>


typedef uint32_t volatile device_register; 

enum UART_ID
{
    UART0, UART1, UART2, UART_INVALID
};

typedef struct UART_CONFIG {
    device_register UDR;
    device_register UBRRH;
    device_register UBRRL;
    device_register UCSRA;
    device_register UCSRB;
    device_register UCSRC;
} UART_CONFIG;


class UART{
private:
    enum REGISTER_ADDR{
        UART0_UCSRA = 0xC0,
        UART1_UCSRA = 0xC8,
        UART2_UCSRA = 0xD0,
    };

    device_register &UDR;
    device_register &UBRRH;
    device_register &UBRRL;
    device_register &UCSRA;
    device_register &UCSRB;
    device_register &UCSRC;

public:
    UART(std::size_t idx ){ 
        
        
        switch(idx) {
            case UART_ID::UART0 :
                &UCSRA = *reinterpret_cast<device_register *>(REGISTER_ADDR::UART0_UCSRA);

                break;
            case UART_ID::UART1 :
                &UCSRA = *reinterpret_cast<device_register *>(REGISTER_ADDR::UART1_UCSRA);

                break;
            case UART_ID::UART2 :
                &UCSRA = *reinterpret_cast<device_register *>(REGISTER_ADDR::UART2_UCSRA);

                break;
        }

        
    }

};