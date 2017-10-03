/* 
 * File:   TCPGPIOServer.h
 * Author: loveboat
 *
 * Created on October 1, 2017, 5:33 PM
 */

#ifndef TCPGPIOSERVER_H
#define	TCPGPIOSERVER_H

// Defines which port the server will listen on
#define TCP_GPIO_SERVER_PORT    7777
#define MAX_CMD_LENGTH 5

typedef enum _myState {
    SM_OPEN_SERVER_SOCKET = 0,
    SM_LISTEN_FOR_CLIENT_CONNECTION,
    SM_DISPLAY_MENU,
    SM_FIND_COMMAND,
    SM_PROCESS_COMMAND,
    SM_DISCONNECT_CLIENT
} state;

typedef enum _commandEnums {
    DO_NO_COMMAND = 0,
    DO_QUIT,
    DO_FIND,
} command;

typedef enum _parsedCommand {
    INVALID = 0,
    LED1,
    LED2,
    LED3,
    LED4,
    NOT_LED1,
    NOT_LED2,
    NOT_LED3,
    NOT_LED4,
    BTN1,
    BTN2,
    BTN3
} parsedCommand;

// typedef enum _myState state;
//typedef enum _commandEnums command;
//typedef enum _parsedCommand parsedCommand;

parsedCommand findCommand (BYTE* u);

#endif	/* TCPGPIOSERVER_H */

