#include "TCPIPConfig.h"

#if defined(STACK_USE_TCP_TO_UPPER_SERVER)

#include "TCPIP Stack/TCPIP.h"
#include <ctype.h>

// Defines which port the server will listen on
#define TCP_TO_UPPER_SERVER_PORT    7777

static enum _myState {
    SM_OPEN_SERVER_SOCKET = 0,
    SM_LISTEN_FOR_CLIENT_CONNECTION,
    SM_PROCESS_COMMAND,
    SM_DISCONNECT_CLIENT
} myState = SM_OPEN_SERVER_SOCKET;

static enum _commandEnums {
    DO_NO_COMMAND = 0,
    DO_QUIT,
    DO_TO_UPPER,
} myCommand = DO_NO_COMMAND;

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
    BYTE theChar = 0;

    switch (myState) {
        case SM_OPEN_SERVER_SOCKET:
            mySocket = TCPOpen(0, TCP_OPEN_SERVER, TCP_TO_UPPER_SERVER_PORT, TCP_PURPOSE_TCP_TO_UPPER_SERVER);
            if (mySocket == INVALID_SOCKET)
                return;
            myState = SM_LISTEN_FOR_CLIENT_CONNECTION;
            break;

        case SM_LISTEN_FOR_CLIENT_CONNECTION:
            if (TCPIsConnected(mySocket) == FALSE)
                return;
            else {
                myState = SM_PROCESS_COMMAND;
                break;
            }
        case SM_PROCESS_COMMAND:
            if (TCPIsConnected(mySocket) == FALSE) {
                myState = SM_DISCONNECT_CLIENT;
                return;
            }
            if (TCPIsPutReady(mySocket) < (WORD) 1)
                return;
            if ((numBytes = TCPIsGetReady(mySocket)) == 0)
                myCommand = DO_NO_COMMAND;
            else
                TCPGet(mySocket, &theChar);
                if (theChar == 'q')
                    myCommand = DO_QUIT;
                else
                    myCommand = DO_TO_UPPER;
            switch (myCommand) {
                case DO_NO_COMMAND:
                    break;
                case DO_QUIT:
                    myState = SM_DISCONNECT_CLIENT;
                    break;
                case DO_TO_UPPER:
                    TCPPut(mySocket, toupper(theChar));
                    break;
            }
            break;
        case SM_DISCONNECT_CLIENT:
            TCPDisconnect(mySocket);
            myState = SM_LISTEN_FOR_CLIENT_CONNECTION;
            break;
    }
}

#endif //#if defined(STACK_USE_TCP_TO_UPPER_SERVER)
