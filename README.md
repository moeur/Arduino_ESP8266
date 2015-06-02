Arduino_ESP8266
Libraries and Examples for programming the ESP8266 from within the Arduino IDE.

NTP - gets current time from an NTP server or pool of servers.
		This code is a modified version of the example "Time_NTP.pde" which 
		is included with the time library.
		
tm1637 - Library for accessing a 4-digit 7-segment display that uses the tm1637 chip
			requires two pins on the ESP8266 for communication

WiFiClock
    This program requires the addition of the libraries:
      NTP, tm1637, and Time
  It uses the system clock to keep track of time.
  A tm1637 module connected to the ESP8266 will display this time.
  The time is kept current by hourly requests from an NTP server pool.

  WiFi connection
    You'll notice that there is no ssid or password hard wired into the code
    If the ESP8266 has been previously connected to your network, it will
    remember your credentials and use then to reconnect.
    If this is the first time this module is to be connected to the network,
    this code waits to retrieve your ssid and password over the serial port.

