##Arduino_ESP8266##

Libraries and examples for programming the _ESP8266_ from within the _Arduino IDE_.

###NTP Library###

gets the current time from an NTP server or pool of servers. This code is a modified version of the example _Time_NTP.pde_ which is included with the time library.


###TM1637 Library###

Unlike the ubiquitous _MAX7219_ LED display driver, the _TM1637_ can be run from 3.3V.  Do a search for "TM1637 Display" from ebay or aliexpress and you'll find a 4-digit module with a clock colon for under $1.50.  This library gives you access to one of these 4-digit modules to display time or integers. It requires two dedicated pins on the ESP8266 for communication.

###WiFiClock Example###

* This program requires the addition of the libraries:
  - NTP
  - tm1637
  - Time - [from Paul Stroffregen](https://github.com/PaulStoffregen/Time)

It uses the system clock to keep track of time. A _TM1637_ module connected to the _ESP8266_ will display this time. The time is kept current by hourly requests from an NTP server pool.



**WiFi connection**

You'll notice that there is no ssid or password hard wired into the code. If the _ESP8266_ has been previously connected to your network, it will remember your credentials and use them to reconnect. If this is the first time this module is to be connected to the network, this code waits to retrieve your ssid and password over the serial port.

A variation on this is to have the module switch to AP+STA mode and allow the user to connect from another WiFi device to configure the module.  This is what's done in [this HTTPD example](http://www.esp8266.com/viewtopic.php?f=6&t=376#p1629).
