#include "TCPIPConfig.h"
#include <string.h>

#if defined(STACK_USE_TCP_TO_UPPER_SERVER)

#include "TCPIP Stack/TCPIP.h"
#include <ctype.h>

// Defines which port the server will listen on
#define TCP_TO_UPPER_SERVER_PORT    7777

static enum _myState {
    SM_OPEN_SERVER_SOCKET = 0,
    SM_LISTEN_FOR_CLIENT_CONNECTION,
    SM_DISPLAY_MENU,
    SM_FIND_COMMAND,
    SM_PROCESS_COMMAND,
    SM_DISCONNECT_CLIENT
} myState = SM_OPEN_SERVER_SOCKET;

static enum _commandEnums {
    DO_NO_COMMAND = 0,
    DO_QUIT,
    DO_FIND,
} myCommand = DO_NO_COMMAND;

static enum _parsedCommand {
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
} executeCommand = INVALID;

typedef enum _parsedCommand parsedCommand;

const char *menu[] = {"\n\n    Enter a command to interact with board, or enter q to quit\n\n",
                      "LED1 on: LED1      LED1 off: ~LED1      LED2 on: LED2      LED2 off: ~LED2\n",
                      "LED3 on: LED3      LED3 off: ~LED3      LED4 on: LED4      LED4 off: ~LED4\n",
                      "Get BTN1 state: BTN1       Get BTN2 state: BTN2       Get BTN3 state: BTN3\n"};

int menuState = 0;

const int MAX_CMD_LENGTH = 5;

parsedCommand findCommand (char *u);


/*****************************************************************************
  Function:
        void TCP_To_Upper_Server(void)

  Summary:
        Implements a simple TCP Server, which inputs a character form a client, converts the received character to upper case, and sends it back to the client.

  Description:
        Implements a simple TCP Server, which inputs a character form a client, converts the received character to upper case, and sends it back to the client.
	
        This example can be used as a model for many TCP server applications.

  Precondition:
        TCP is initialized.

  Parameters:
        None

  Returns:
        None
 ***************************************************************************/
void TCPToUpperServer(void) {
    static TCP_SOCKET mySocket;
    WORD numBytes = 0;
    BYTE userCmd[MAX_CMD_LENGTH];

    switch (myState) {
        // Open a server socket
        case SM_OPEN_SERVER_SOCKET:
            mySocket = TCPOpen(0, TCP_OPEN_SERVER, TCP_TO_UPPER_SERVER_PORT, TCP_PURPOSE_TCP_TO_UPPER_SERVER);
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
            
            int size = strlen(menu[menuState]);
            
            if (TCPIsPutReady(mySocket) < size) {
                return;
            }
            
            TCPPutArray(mySocket, (BYTE*)menu[menuState++], size);
            
            TCPFlush(mySocket);
            
            if (menuState == 4) {
                myState = SM_FIND_COMMAND;
            }
            
            break;
        // Process the user's commands
        case SM_FIND_COMMAND:
            // If the user has disconnected, somehow, close the connection
            if (TCPIsConnected(mySocket) == FALSE) {
                myState = SM_DISCONNECT_CLIENT;
                return;
            }
            // If the socket is not ready to put, return
            if (TCPIsPutReady(mySocket) < (WORD) 1) {
                return;
            }
            
            // Get the number of bytes in the 'GET' queue
            numBytes = TCPIsGetReady(mySocket);
            
            // If there are zero bytes in the queue, don't do anything
            if (numBytes == 0) {
                myCommand = DO_NO_COMMAND;
            }
            // Otherwise, get the user's command they sent, q is quit
            else {
                // All commands have a max length
                TCPGetArray(mySocket, &userCmd, MAX_CMD_LENGTH);
                // So if there's anything else in there, discard it
                TCPDiscard(mySocket);
                
                if (userCmd[0] == 'q' || userCmd[0] == 'Q') {
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
                    // User quit, change state to DISCONNECT
                    myState = SM_DISCONNECT_CLIENT;
                    break;
                case DO_FIND:
                    // User entered a command, find it to process
                    executeCommand = findCommand(&userCmd);
                    myState = SM_PROCESS_COMMAND;
                    break;
            }
            break;
        // Disconnect the client
        case SM_PROCESS_COMMAND:
            
            switch (executeCommand) {
                default:
                    break;
            }
            
            myState = SM_FIND_COMMAND;
            break;
            
        case SM_DISCONNECT_CLIENT:
            TCPDiscard(mySocket);
            TCPDisconnect(mySocket);
            myState = SM_LISTEN_FOR_CLIENT_CONNECTION;
            menuState = 0;
            break;
    }
}

parsedCommand findCommand(char *usersCommand) {
    parsedCommand cmd;
     
    if (strncmp(usersCommand, "LED1", 4) == 0) { cmd = LED1; }
    else if (strncmp(usersCommand, "LED2", 4) == 0) { cmd = LED2; }
    else if (strncmp(usersCommand, "LED3", 4) == 0) { cmd = LED3; }
    else if (strncmp(usersCommand, "LED4", 4) == 0) { cmd = LED4; }
    else if (strncmp(usersCommand, "~LED1", 5) == 0) { cmd = NOT_LED1; }
    else if (strncmp(usersCommand, "~LED2", 5) == 0) { cmd = NOT_LED2; }
    else if (strncmp(usersCommand, "~LED3", 5) == 0) { cmd = NOT_LED3; }
    else if (strncmp(usersCommand, "~LED4", 5) == 0) { cmd = NOT_LED4; }
    else if (strncmp(usersCommand, "BTN1", 4) == 0) { cmd = BTN1; }
    else if (strncmp(usersCommand, "BTN2", 4) == 0) { cmd = BTN2; }
    else if (strncmp(usersCommand, "BTN3", 4) == 0) { cmd = BTN3; }
    else { cmd = INVALID; }
    
    return cmd;
}

#endif //#if defined(STACK_USE_TCP_TO_UPPER_SERVER)
