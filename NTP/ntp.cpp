/****************************************************************************
 *  NTP.cpp - gets current time from an NTP server or pool of servers.
 *		It is meant to be used with setSyncProvider from the Time library 
 *		 - see Example WifiClock
 *		This code is a modified version of the example "Time_NTP.pde" which 
 *		is included with the time library.
 *		modified by William Moeur and made into a library: 28 MAY 2015.
 *		This code is in the public domain.
 *****************************************************************************/

#include "ESP8266WiFi.h"
#include "arduino.h"
#include "ntp.h"

NTP::NTP(void)
{
}

void NTP::begin(const char* ntpServerName, int TimeZoneOffset)
{
	_serverName = ntpServerName;
	_timeZoneOffset = TimeZoneOffset;
	/* Start WiFiUDP socket, listening at local port LOCALPORT */
	UDP.begin(LOCALPORT);	
}

time_t NTP::getNtpTime(void)
{
  while (UDP.parsePacket() > 0) ; // discard any previously received packets
  //get a random server from the pool
  IPAddress timeServerIP;
  WiFi.hostByName(_serverName, timeServerIP); 
  sendNTPpacket(timeServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = UDP.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      UDP.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      time_t secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (time_t)packetBuffer[40] << 24;
      secsSince1900 |= (time_t)packetBuffer[41] << 16;
      secsSince1900 |= (time_t)packetBuffer[42] << 8;
      secsSince1900 |= (time_t)packetBuffer[43];
      time_t secsSince1970 = secsSince1900 - 2208988800UL;
      int8_t totalOffset = (int8_t)(_timeZoneOffset + DSToffset(secsSince1970));
      return secsSince1970 + (time_t)(totalOffset * SECS_PER_HOUR) ;
    }
    yield();
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void NTP::sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:                 
  UDP.beginPacket(address, 123); //NTP requests are to port 123
  UDP.write(packetBuffer, NTP_PACKET_SIZE);
  UDP.endPacket();
}

uint8_t NTP::DSToffset(time_t date)
{
  /*DST is between - 
   * 2nd Sunday of March @2AM 
   *        - and -
   * 1st Sunday of November @2AM
  */
  //get seconds for 2nd Sunday of March
  tmElements_t tm;
  tm.Month = 3; //MARCH
  tm.Year = year(date) - 1970;
  tm.Day = 7; //March 7th
  time_t beginDST = nextSunday( makeTime(tm) ) + 2*SECS_PER_HOUR;
  
  //get seconds for 1st Sunday in November
  tm.Month = 10; //October
  tm.Day = 31; //last day of October;
  time_t endDST = nextSunday( makeTime(tm) ) + 2*SECS_PER_HOUR;

  //now, if we are between the two times we are in DST
  return (((date >= beginDST) && (date < endDST))? 1: 0);
}

