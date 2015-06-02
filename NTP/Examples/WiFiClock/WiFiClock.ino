#include <ntp.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TM1637.h>
#include <Ticker.h>
#include <Time.h>

bool startWiFi(void);
time_t getNTPtime(void);
void readLine(char buffer[], uint8_t size);
void digitalClockDisplay();
void toggleColon(void);

NTP NTPclient;
tm1637 display(14, 13);
Ticker clock;
bool colon = true;
bool updateTime = true;

#define PST -8 //pacific standard time

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  while (!startWiFi()){delay(1500);}
  
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  display.setBrightness(4);

  NTPclient.begin("time.nist.gov", PST);
  setSyncInterval(SECS_PER_HOUR);
  setSyncProvider(getNTPtime);
  
  clock.attach(0.5, toggleColon);

}

void loop() {
   if (updateTime){
      updateTime = false;
      if (timeStatus() != timeNotSet) 
        digitalClockDisplay();  
   }
}

time_t getNTPtime(void)
{
  return NTPclient.getNtpTime();
}

void digitalClockDisplay()
{
  // digital clock display of the time
    display.writeTime(hourFormat12(), minute(), colon);
    colon = !colon;
}


bool startWiFi(void)
{
  uint8_t i;
  //check for persistent wifi connection
 for (i=0;i<10;i++){
   if (WiFi.status() == WL_CONNECTED) return true;
   delay(500);
   Serial.print(".");
 }

  /*didn't work: so ask user to enter credentials over Serial Port */
  #define maxSSIDlength 20
  #define maxPASSlength 40
  char ssid[maxSSIDlength];
  char pass[maxPASSlength];
  
 //prompt the user for his ssid

  Serial.print("Enter ssid name: ");
  readLine(ssid, maxSSIDlength);
  Serial.println();
  Serial.print("Enter pass phrase: ");
  readLine(pass, maxPASSlength);
  Serial.println();

  Serial.print("Attempting to Connect");
  if (WiFi.begin(ssid, pass) != WL_CONNECTED) {
     for (i=0;i<10;i++){
       if (WiFi.status() == WL_CONNECTED) return true;
       delay(500);
       Serial.print(".");
     }
  }
  Serial.print("Failed to connect to: ");
  Serial.println(ssid);
  
  Serial.print("using pass phrase: ");
  Serial.println(pass);

  return false;
 }

void readLine(char buffer[], uint8_t size)
{
    uint8_t i = 0;
    char c;

    do {
      while (!Serial.available()){yield();} //wait for input
      c = Serial.read();
      Serial.write(c); //echo characters back to user.
      if (c == '\r') break;
      if (c != '\n')
        buffer[i++] = c;      
    } while (i < size-1);
    buffer[i] = '\0';
}

void toggleColon(void)
{
   updateTime = true;  
}
