#define __LEDTCPSERVER_C

#include "TCPIPConfig.h"

#if defined(STACK_USE_LED_TCP_SERVER_EXAMPLE)

#include "TCPIP Stack/TCPIP.h"


// Defines which port the server will listen on
#define SERVER_PORT	9761


/*****************************************************************************
  Function:
	void LEDTCPServer(void)

  Summary:
	Implements a simple LED TCP Server.

  Description:
	This function implements a simple TCP server.  The function is invoked
	periodically by the stack to listen for incoming connections.  When a 
	connection is made, the server reads all incoming data, processes the 
	LEDs.
	
	This example can be used as a model for many TCP server applications.

  Precondition:
	TCP is initialized.

  Parameters:
	None

  Returns:
  	None
  ***************************************************************************/
void LEDTCPServer(void)
{
	WORD w;
	BYTE AppBuffer[8];
	char menuString[24];
	static TCP_SOCKET	MySocket;
	static enum _TCPServerState
	{
		SM_OPEN_SERVER_SOCKET = 0,
		SM_DISPLAY_MENU,
		SM_PROCESS,
		SM_CLOSE_SOCKET
	} TCPServerState = SM_OPEN_SERVER_SOCKET;

	switch(TCPServerState)
	{
		case SM_OPEN_SERVER_SOCKET: // Allocate a socket for this server to listen and accept connections on
			MySocket = TCPOpen(0, TCP_OPEN_SERVER, SERVER_PORT, TCP_PURPOSE_LED_SERVER);
			if(MySocket == INVALID_SOCKET) return;
			TCPServerState = SM_DISPLAY_MENU;
		break;
 
		case SM_DISPLAY_MENU: // Display the menu
			if(!TCPIsConnected(MySocket)) return;
			while((TCPIsPutReady(MySocket)) < sizeof("\x1b[2J \x1b[37m \x1b[1m"));
			TCPPutArray(MySocket, (BYTE*)"\x1b[2J \x1b[37m \x1b[1m", sizeof("\x1b[2J \x1b[37m \x1b[1m"));
			TCPFlush(MySocket);
			while((TCPIsPutReady(MySocket)) < sizeof("   LED SERVER MENU\n\r"));
			TCPPutArray(MySocket, (BYTE*)"   LED SERVER MENU\n\r", sizeof("   LED SERVER MENU\n\r"));
			TCPFlush(MySocket);
			while((TCPIsPutReady(MySocket)) < sizeof("1: LED0 ON\t2: LED0 OFF\n\r"));
			TCPPutArray(MySocket, (BYTE*)"1: LED0 ON\t2: LED0 OFF\n\r", sizeof("1: LED0 ON\t2: LED0 OFF\n\r"));
			TCPFlush(MySocket);
			while((TCPIsPutReady(MySocket)) < sizeof("3: LED1 ON\t4: LED1 OFF\n\r"));
			TCPPutArray(MySocket, (BYTE*)"3: LED1 ON\t4: LED1 OFF\n\r", sizeof("3: LED1 ON\t4: LED1 OFF\n\r"));
			TCPFlush(MySocket);
			while((TCPIsPutReady(MySocket)) < sizeof("5: LED2 ON\t6: LED2 OFF\n\r"));
			TCPPutArray(MySocket, (BYTE*)"5: LED2 ON\t6: LED2 OFF\n\r", sizeof("5: LED2 ON\t6: LED2 OFF\n\r"));
			TCPFlush(MySocket);
			while((TCPIsPutReady(MySocket)) < sizeof("7: LED3 ON\t8: LED3 OFF\n\r"));
			TCPPutArray(MySocket, (BYTE*)"7: LED3 ON\t8: LED3 OFF\n\r", sizeof("7: LED3 ON\t8: LED3 OFF\n\r"));
			TCPFlush(MySocket);
			while((TCPIsPutReady(MySocket)) < sizeof("a: PB0\ts: PB1\td: PB2\t\n\r\n\x1b[31m"));
			TCPPutArray(MySocket, (BYTE*)"a: PB0\ts: PB1\td: PB2\t\n\r\n\x1b[31m", sizeof("a: PB0\ts: PB1\td: PB2\t\n\r\n\x1b[31m"));
			TCPFlush(MySocket);
			TCPServerState = SM_PROCESS;
			break;
			
		case SM_PROCESS: // Display the menu
			if(!TCPIsConnected(MySocket)) return;
			w = TCPIsGetReady(MySocket);
			if(w == 0)
				return;
			TCPGet(MySocket, AppBuffer);
			int j;
			for(j = 0; j < w; j++)
			{
				if(AppBuffer[j] <= '8' && AppBuffer[j] >='1')
				{
					if(AppBuffer[j] == '1')
					{
						LED0_IO = 1;
						strcpy(menuString, (const char *)"\x1b[2K\rLED0  on");
					}	
					else if(AppBuffer[j] == '2')
					{
						LED0_IO = 0;
						strcpy(menuString, (const char *)"\x1b[2K\rLED0 off");
					}	
					else if(AppBuffer[j] == '3')
					{
						LED1_IO = 1;
						strcpy(menuString, (const char *)"\x1b[2K\rLED1  on");
					}	
					else if(AppBuffer[j] == '4')
					{
						LED1_IO = 0;
						strcpy(menuString, (const char *)"\x1b[2K\rLED1 off");
					}	
					else if(AppBuffer[j] == '5')
					{
						LED2_IO = 1;
						strcpy(menuString, (const char *)"\x1b[2K\rLED2  on");
					}	
					else if(AppBuffer[j] == '6')
					{
						LED2_IO = 0;
						strcpy(menuString, (const char *)"\x1b[2K\rLED2 off");
					}	
					else if(AppBuffer[j] == '7')
					{
						LED3_IO = 1;
						strcpy(menuString, (const char *)"\x1b[2K\rLED3  on");
					}	
					else if(AppBuffer[j] == '8')
					{
						LED3_IO = 0;
						strcpy(menuString, (const char *)"\x1b[2K\rLED3 off");
					}	
					while((TCPIsPutReady(MySocket)) < sizeof("\x1b[2K\rLED3 off"));
					TCPPutArray(MySocket, (BYTE*)menuString, sizeof("\x1b[2K\rLED3 off"));
					TCPFlush(MySocket);
				}	
				else if(AppBuffer[j] == 'a')
				{
					if(BUTTON0_IO == BUTTON_DOWN)
					{
						while((TCPIsPutReady(MySocket)) < sizeof("\x1b\r[2K\rButton 0 is Down"));
						TCPPutArray(MySocket, (BYTE*)"\x1b[2K\rButton 0 is Down", sizeof("\x1b[2K\rButton 0 is Down"));
						TCPFlush(MySocket);
					}	
					else
					{
						while((TCPIsPutReady(MySocket)) < sizeof("\x1b[2K\rButton 0 is Up"));
						TCPPutArray(MySocket, (BYTE*)"\x1b[2K\rButton 0 is Up", sizeof("\x1b[2K\rButton 0 is Up"));
						TCPFlush(MySocket);
					}
					return;
				}	
				else if(AppBuffer[j] == 's')
				{
					if(BUTTON1_IO == BUTTON_DOWN)
					{
						while((TCPIsPutReady(MySocket)) < sizeof("\x1b[2K\rButton 1 is Down"));
						TCPPutArray(MySocket, (BYTE*)"\x1b[2K\rButton 1 is Down", sizeof("\x1b[2K\rButton 1 is Down"));
						TCPFlush(MySocket);
					}	
					else
					{
						while((TCPIsPutReady(MySocket)) < sizeof("\x1b[2K\rButton 1 is Up"));
						TCPPutArray(MySocket, (BYTE*)"\x1b[2K\rButton 1 is Up", sizeof("\x1b[2K\rButton 1 is Up"));
						TCPFlush(MySocket);
					}
					return;
				}	
				else if(AppBuffer[j] == 'd')
				{
					if(BUTTON2_IO == BUTTON_DOWN)
					{
						while((TCPIsPutReady(MySocket)) < sizeof("\x1b[2K\rButton 2 is Down"));
						TCPPutArray(MySocket, (BYTE*)"\x1b[2K\rButton 2 is Down", sizeof("\x1b[2K\rButton 2 is Down"));
						TCPFlush(MySocket);
					}	
					else
					{
						while((TCPIsPutReady(MySocket)) < sizeof("\x1b[2K\rButton 2 is Up\r"));
						TCPPutArray(MySocket, (BYTE*)"\x1b[2K\rButton 2 is Up", sizeof("\x1b[2K\rButton 2 is Up"));
						TCPFlush(MySocket);
					}
					return;
				}					
				else if(AppBuffer[j] == 0x1B)
				{
					TCPServerState = SM_CLOSE_SOCKET;
					return;
				}	
			}
			break;
			
		case SM_CLOSE_SOCKET:
      TCPClose(MySocket);
			TCPServerState = SM_OPEN_SERVER_SOCKET;
			break;
	}
}

#endif //#if defined(STACK_USE_GENERIC_TCP_SERVER_EXAMPLE)
