/* 
 * File:   PortConfig.h
 * Author: Daniel Lovegrove
 *
 * Created on September 17, 2017, 4:47 PM
 */

#ifndef PORTCONFIG_H
#define	PORTCONFIG_H

#include <plib.h>

// IOPORT bit masks can be found in ports.h
#define LED1_IOPORT	IOPORT_G
#define	LED1_BIT	BIT_12
#define LED2_IOPORT	IOPORT_G
#define	LED2_BIT	BIT_13
#define LED3_IOPORT	IOPORT_G
#define	LED3_BIT	BIT_14
#define LED4_IOPORT	IOPORT_G
#define	LED4_BIT	BIT_15

#define BUTTON1_IOPORT IOPORT_G
#define BUTTON1_BIT    BIT_6
#define BUTTON2_IOPORT IOPORT_G
#define BUTTON2_BIT    BIT_7
#define BUTTON3_IOPORT IOPORT_A
#define BUTTON3_BIT    BIT_0


// Initialize board ports
void portInit();
// Blink an led
void ledBlink(unsigned int a, unsigned int b);
// Turn an LED on
void ledOn(unsigned int a, BOOL b);
// Determine if a button is pressed
BOOL buttonPressed(unsigned int a);

#endif	/* PORTCONFIG_H */

