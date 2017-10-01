#include "PortConfig.h"

int ledPort;
int ledBit;

void getPortAndBitName(unsigned int led);

// Set the LEDS to digital output
void portInit() {
    PORTSetPinsDigitalOut(LED1_IOPORT, LED1_BIT); //Set the pin connected to LED1 as output
    PORTSetPinsDigitalOut(LED2_IOPORT, LED2_BIT); //Set the pin connected to LED2 as output
    PORTSetPinsDigitalOut(LED3_IOPORT, LED3_BIT); //Set the pin connected to LED3 as output
    PORTSetPinsDigitalOut(LED4_IOPORT, LED4_BIT); //Set the pin connected to LED4 as output

    PORTSetPinsDigitalIn(BUTTON1_IOPORT, BUTTON1_BIT); // Set the pin connected to BUTTON1 as input
    PORTSetPinsDigitalIn(BUTTON2_IOPORT, BUTTON2_BIT); // Set the pin connected to BUTTON2 as input
    PORTSetPinsDigitalIn(BUTTON3_IOPORT, BUTTON3_BIT); // Set the pin connected to BUTTON3 as input

    PORTClearBits(LED1_IOPORT, LED1_BIT); // LED1 off
    PORTClearBits(LED2_IOPORT, LED2_BIT); // LED2 off
    PORTClearBits(LED3_IOPORT, LED3_BIT); // LED3 off
    PORTClearBits(LED4_IOPORT, LED4_BIT); // LED4 off
    
    DDPCONbits.JTAGEN = 0;
}

// Turn a specific LED on for a time interval specified by delay
void ledBlink(unsigned int led, unsigned int delay) {
    unsigned int i;
    
    if (led >= 1 && led <= 4) {
        getPortAndBitName(led);

        PORTSetBits(ledPort, ledBit);
        for (i = 0; i < delay; i++); // Delay some time
        PORTClearBits(ledPort, ledBit);
    }
}

// Set the correct port name and bit name for the LED
void getPortAndBitName(unsigned int led) {
    switch (led) {
        case (1):
            ledPort = LED1_IOPORT;
            ledBit  = LED1_BIT;
            break;
        case (2):
            ledPort = LED2_IOPORT;
            ledBit  = LED2_BIT;
            break;
        case (3):
            ledPort = LED3_IOPORT;
            ledBit  = LED3_BIT;
            break;
        case (4):
            ledPort = LED4_IOPORT;
            ledBit  = LED4_BIT;
            break;
        default:
            break;
    }
}

// Determine if a button is being pressed
BOOL buttonPressed(unsigned int button) {
    BOOL buttonPressed = FALSE;
    char portRead;
    
    if (button >= 1 && button <= 3) {
        
        switch (button) {
            case 1:
                portRead = PORTReadBits(BUTTON1_IOPORT, BUTTON1_BIT);
                break;
            case 2:
                portRead = PORTReadBits(BUTTON2_IOPORT, BUTTON2_BIT);
                break;
            case 3:
                portRead = PORTReadBits(BUTTON3_IOPORT, BUTTON3_BIT);
                break;
        }
        
        if (portRead != 0) {
            buttonPressed = TRUE;
        } else {
            buttonPressed = FALSE;
        }
    }
    
    return buttonPressed;
}

// Turn a specific LED on or off. Pass True for ON, False for OFF
void ledOn(unsigned int led, BOOL on) {
    if (led >= 1 && led <= 4) {
        getPortAndBitName(led);
        
        if (on == TRUE) {
            PORTSetBits(ledPort, ledBit);
        } else {
            PORTClearBits(ledPort, ledBit);
        }
    }
}
