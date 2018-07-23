# ECE3740-P1.3
Project 1.3 for ECE3740, using telnet client to interface with MX7CK board

This program runs a TCP/IP server on an MX7CK board. Connecting to it with a Telnet client allows the user to turn the board's LEDs 
on and off, as well as monitor the states of the push buttons.

To test, first compile the project in MPLAB X IDE and load it onto a Microchip MX7CK board. Once the server is running, connect an ethernet cable to the board and to a computer, and use a terminal on the computer to open a telnet client at IP address 192.168.1.214, port 7777.

```
telnet 192.168.1.214 7777
```

The server will return a menu on how to turn LEDs on/off and monitor the buttons.

---
___NOTE__: If you would like more detailed instructions on how to run the project, the included TPP document runs you through 
exactly how to load the project onto the MX7CK board and how it should work._
