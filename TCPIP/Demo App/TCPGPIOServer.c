#include "TCPIPConfig.h"

#if defined(STACK_USE_TCP_GPIO_SERVER)

#include "TCPIP Stack/TCPIP.h"
#include <ctype.h>
#include <string.h>
#include "PortConfig.h"
#include "TCPGPIOServer.h"

// State variables
state myState = SM_OPEN_SERVER_SOCKET;
command myCommand = DO_NO_COMMAND;
parsedCommand executeCommand = INVALID;
int menuState = 0;
BOOL promptDisplayed = FALSE;

const char *menu[] = {"\n\n    Enter a command to interact with board, or enter q to quit\n\n",
                      "LED1 on: LED1      LED1 off: ~LED1      LED2 on: LED2      LED2 off: ~LED2\n",
                      "LED3 on: LED3      LED3 off: ~LED3      LED4 on: LED4      LED4 off: ~LED4\n",
                      "Get BTN1 state: BTN1       Get BTN2 state: BTN2       Get BTN3 state: BTN3\n"};


/*****************************************************************************
  Function:
        void TCPGPIOServer(void)

  Summary:
        Implements a simple TCP Server, which accepts a set of commands from the
        user to turn LEDs on the board on/off, and monitor push buttons.
  
  Description:
        Implements a simple TCP Server, which accepts a set of commands from the
        user to turn LEDs on the board on/off, and monitor push buttons.

  Precondition:
        TCP is initialized.

  Parameters:
        None

  Returns:
        None
 ***************************************************************************/
void TCPGPIOServer(void) {
    static TCP_SOCKET mySocket;
    WORD numBytes = 0;
    BYTE userCmd[MAX_CMD_LENGTH + 1];
    int size;

    switch (myState) {
        // Open a server socket
        case SM_OPEN_SERVER_SOCKET:
            mySocket = TCPOpen(0, TCP_OPEN_SERVER, TCP_GPIO_SERVER_PORT, TCP_PURPOSE_TCP_GPIO_SERVER);
            if (mySocket == INVALID_SOCKET) {
                return;
            }
            myState = SM_LISTEN_FOR_CLIENT_CONNECTION;
            break;
        // Listen for a client to connect
        case SM_LISTEN_FOR_CLIENT_CONNECTION:
            if (TCPIsConnected(mySocket) == TRUE) {
                myState = SM_DISPLAY_MENU;
            }
            break;
        // Display the menu
        case SM_DISPLAY_MENU:
            // Disconnect if the client is disconnects
            if (TCPIsConnected(mySocket) == FALSE) {
                myState = SM_DISCONNECT_CLIENT;
                return;
            }
            
            size = strlen(menu[menuState]);
            
            if (TCPIsPutReady(mySocket) < size) {
                return;
            }
            
            TCPPutArray(mySocket, (BYTE*)menu[menuState++], size);
            
            TCPFlush(mySocket);
            
            if (menuState == 4) {
                myState = SM_FIND_COMMAND;
            }
            
            break;
        // Find the user's command
        case SM_FIND_COMMAND:
            // If the user has disconnected, somehow, close the connection
            if (TCPIsConnected(mySocket) == FALSE) {
                myState = SM_DISCONNECT_CLIENT;
                return;
            }
            
            if (promptDisplayed == FALSE) {
                size = strlen("\x1B[33m $ \x1B[37m");

                // If the socket is not ready to put, return
                if (TCPIsPutReady(mySocket) < size) {
                    return;
                }

                // Display a prompt
                TCPPutArray(mySocket, (BYTE*)"\x1B[33m $ \x1B[37m", size);
                promptDisplayed = TRUE;
            }
            
            // Get the number of bytes in the 'GET' queue
            numBytes = TCPIsGetReady(mySocket);
            
            // If there are zero bytes in the queue, don't do anything
            if (numBytes == 0) {
                myCommand = DO_NO_COMMAND;
            } else if (numBytes > MAX_CMD_LENGTH + 2) {
                userCmd[0] = (BYTE) '\0';
                myCommand = DO_FIND;
                TCPDiscard(mySocket);
            }
            // Otherwise, get the user's command they sent, q is quit
            else {
                // All commands have a max length
                TCPGetArray(mySocket, (BYTE*)&userCmd, numBytes - 2);
                // So if there's anything else in there, discard it
                TCPDiscard(mySocket);
                // Null terminate command string
                userCmd[numBytes - 2] = '\0';
                
                if (numBytes == 3 && (userCmd[0] == 'q' || userCmd[0] == 'Q')) {
                    myCommand = DO_QUIT;
                }
                else {
                    myCommand = DO_FIND;
                }
            }
            
            // Process the user's command
            switch (myCommand) {
                case DO_NO_COMMAND:
                    break;
                case DO_QUIT:
                    promptDisplayed = FALSE;
                    // User quit, change state to DISCONNECT
                    myState = SM_DISCONNECT_CLIENT;
                    break;
                case DO_FIND:
                    // User entered a command, find it to process
                    executeCommand = findCommand((BYTE*)&userCmd);
                    myState = SM_PROCESS_COMMAND;
                    break;
            }
            break;
        // Act on the user's command
        case SM_PROCESS_COMMAND:
            // Biggest command response length
            size = strlen("\x1B[32m BTN1 is NOT pressed\n");

            // If the socket is not ready to put, return
            if (TCPIsPutReady(mySocket) < size) {
                return;
            }
            
            switch (executeCommand) {
                case LED1:
                    ledOn(1, TRUE);
                    TCPPutArray(mySocket, (BYTE*)"\x1B[32m LED1 is ON\n", strlen("\x1B[32m LED1 is ON\n"));
                    break;
                case NOT_LED1:
                    ledOn(1, FALSE);
                    TCPPutArray(mySocket, (BYTE*)"\x1B[32m LED1 is OFF\n", strlen("\x1B[32m LED1 is OFF\n"));
                    break;
                case LED2:
                    ledOn(2, TRUE);
                    TCPPutArray(mySocket, (BYTE*)"\x1B[32m LED2 is ON\n", strlen("\x1B[32m LED2 is ON\n"));
                    break;
                case NOT_LED2:
                    ledOn(2, FALSE);
                    TCPPutArray(mySocket, (BYTE*)"\x1B[32m LED2 is OFF\n", strlen("\x1B[32m LED2 is OFF\n"));
                    break;
                case LED3:
                    ledOn(3, TRUE);
                    TCPPutArray(mySocket, (BYTE*)"\x1B[32m LED3 is ON\n", strlen("\x1B[32m LED3 is ON\n"));
                    break;
                case NOT_LED3:
                    ledOn(3, FALSE);
                    TCPPutArray(mySocket, (BYTE*)"\x1B[32m LED3 is OFF\n", strlen("\x1B[32m LED3 is OFF\n"));
                    break;
                case LED4:
                    ledOn(4, TRUE);
                    TCPPutArray(mySocket, (BYTE*)"\x1B[32m LED4 is ON\n", strlen("\x1B[32m LED4 is ON\n"));
                    break;
                case NOT_LED4:
                    ledOn(4, FALSE);
                    TCPPutArray(mySocket, (BYTE*)"\x1B[32m LED4 is OFF\n", strlen("\x1B[32m LED4 is OFF\n"));
                    break;
                case BTN1:
                    if (buttonPressed(1) == TRUE) {
                        TCPPutArray(mySocket, (BYTE*)"\x1B[32m BTN1 is pressed\n", strlen("\x1B[32m BTN1 is pressed\n"));
                    } else {
                        TCPPutArray(mySocket, (BYTE*)"\x1B[32m BTN1 is NOT pressed\n", strlen("\x1B[32m BTN1 is NOT pressed\n"));
                    }
                    break;
                case BTN2:
                    if (buttonPressed(2) == TRUE) {
                        TCPPutArray(mySocket, (BYTE*)"\x1B[32m BTN2 is pressed\n", strlen("\x1B[32m BTN2 is pressed\n"));
                    } else {
                        TCPPutArray(mySocket, (BYTE*)"\x1B[32m BTN2 is NOT pressed\n", strlen("\x1B[32m BTN2 is NOT pressed\n"));
                    }
                    break;
                case BTN3:
                    if (buttonPressed(3) == TRUE) {
                        TCPPutArray(mySocket, (BYTE*)"\x1B[32m BTN3 is pressed\n", strlen("\x1B[32m BTN3 is pressed\n"));
                    } else {
                        TCPPutArray(mySocket, (BYTE*)"\x1B[32m BTN3 is NOT pressed\n", strlen("\x1B[32m BTN3 is NOT pressed\n"));
                    }
                    break;
                default:
                    TCPPutArray(mySocket, (BYTE*)"\x1B[32m Invalid Command\n", strlen("\x1B[32m Invalid Command\n"));
                    break;
            }
            
            promptDisplayed = FALSE;
            myState = SM_FIND_COMMAND;
            break;
        // Disconnect the client   
        case SM_DISCONNECT_CLIENT:
            TCPDiscard(mySocket);
            TCPDisconnect(mySocket);
            myState = SM_LISTEN_FOR_CLIENT_CONNECTION;
            menuState = 0;
            break;
    }
}

parsedCommand findCommand(BYTE* unparsedCommand) {
    char *usersCommand = (char*) unparsedCommand;
    parsedCommand cmd;
     
    if (strcmp(usersCommand, "LED1") == 0) { cmd = LED1; }
    else if (strcmp(usersCommand, "LED2") == 0) { cmd = LED2; }
    else if (strcmp(usersCommand, "LED3") == 0) { cmd = LED3; }
    else if (strcmp(usersCommand, "LED4") == 0) { cmd = LED4; }
    else if (strcmp(usersCommand, "~LED1") == 0) { cmd = NOT_LED1; }
    else if (strcmp(usersCommand, "~LED2") == 0) { cmd = NOT_LED2; }
    else if (strcmp(usersCommand, "~LED3") == 0) { cmd = NOT_LED3; }
    else if (strcmp(usersCommand, "~LED4") == 0) { cmd = NOT_LED4; }
    else if (strcmp(usersCommand, "BTN1") == 0) { cmd = BTN1; }
    else if (strcmp(usersCommand, "BTN2") == 0) { cmd = BTN2; }
    else if (strcmp(usersCommand, "BTN3") == 0) { cmd = BTN3; }
    else { cmd = INVALID; }
    
    return cmd;
}

#endif //#if defined(STACK_USE_TCP_TO_UPPER_SERVER)
