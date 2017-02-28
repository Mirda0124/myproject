#include <EEPROM.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <ESP8266mDNS.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <FS.h>
#include <Adafruit_SSD1306.h>
#include <WebSocketsServer.h>
#include <Wire.h>
#include <TimeLib.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESP8266httpUpdate.h>
#include <ESP.h>
#include <WebSocketClient.h>
#include <ESP8266HTTPClient.h>

//#include <SoftwareSerial.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
//char server[] = "192.168.1.192:9000";
//WebSocketClient client;
unsigned long aktualniMillis; //aktualni cas
unsigned long predchoziMillis; //cas poseldni akce


bool mobil;


#define nextion Serial // port pro komunikaci s displejem Nextion
#define ONE_WIRE_BUS D6
#define TEMPERATURE_PRECISION 10
#define OLED_RESET LED_BUILTIN
#define DBG_OUTPUT_PORT Serial
Adafruit_SSD1306 display(OLED_RESET);

// NTP Servers:
static const char ntpServerName0[] = "us.pool.ntp.org";
static const char ntpServerName1[] = "time.windows.com";
static const char ntpServerName2[] = "time-nw.nist.gov";
static const char ntpServerName3[] = "time-a.nw.nist.gov";
static const char ntpServerName4[] = "time.nist.gov";
//String httpserver = "192.168.1.192"; 

const int timeZone = 2;     // Central European Time


const byte  but1 = 14;  //leva prava
const byte  but2 = 2;


const byte Rele_1 = 16;
const byte Rele_2 = 0;
const byte Rele_3 = 13;
const byte Rele_4 = 15;

const unsigned int localPort = 8888;  // local port to listen for UDP packets

static unsigned char PROGMEM logo16_glcd_bmp[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x04, 0x00, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x04, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0C, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0C, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x04, 0x00, 0x0F, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x04, 0x20, 0x1F, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x31, 0x1E, 0x4E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x3B, 0x3E, 0x4E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x73, 0x3C, 0xC6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x77, 0xFC, 0xC7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x02, 0x7F, 0xFF, 0xC2, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x06, 0x7F, 0xFF, 0xE0, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x07, 0xFF, 0xFF, 0xE0, 0x0C, 0x03, 0xE3, 0xF0, 0x07, 0xCF, 0xC0, 0x7E, 0x30, 0x60, 0x00,
  0x01, 0x03, 0xFF, 0xFF, 0xE0, 0x08, 0x0F, 0xF7, 0xFC, 0x1F, 0xCF, 0xE3, 0xFE, 0x70, 0xE3, 0x00,
  0x01, 0x23, 0xFF, 0xFF, 0xE1, 0x98, 0x1E, 0x07, 0x9E, 0x38, 0x0E, 0x77, 0xCE, 0x70, 0xE1, 0x00,
  0x00, 0x93, 0xFF, 0xFF, 0xE3, 0x10, 0x1C, 0x07, 0x0E, 0x70, 0x0E, 0x77, 0x0E, 0x30, 0xE0, 0x00,
  0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0x00, 0x3F, 0xE3, 0x0E, 0x7F, 0xCE, 0x67, 0x0E, 0x30, 0xE0, 0x00,
  0x00, 0x1F, 0xFF, 0xFF, 0xFE, 0x00, 0x3F, 0xF7, 0x0E, 0x7F, 0xCE, 0x06, 0x0E, 0x70, 0xE0, 0x00,
  0x00, 0x0F, 0xFF, 0xFF, 0xFE, 0x00, 0x3C, 0x03, 0x0E, 0x70, 0x0E, 0x07, 0x0E, 0x30, 0xE0, 0x00,
  0x00, 0x0F, 0xFF, 0xFF, 0xFE, 0x00, 0x1C, 0x07, 0x0E, 0x70, 0x0E, 0x07, 0x0E, 0x30, 0xE0, 0x00,
  0x00, 0x8F, 0xFF, 0xCF, 0xFC, 0x00, 0x1E, 0x07, 0x0E, 0x38, 0x0E, 0x07, 0x8E, 0x38, 0xE0, 0x00,
  0x00, 0x4F, 0xFC, 0x03, 0xFC, 0x40, 0x0F, 0xF7, 0x0E, 0x3F, 0xCE, 0x03, 0xFE, 0x3F, 0xE0, 0x00,
  0x00, 0x7F, 0xF0, 0x01, 0xFE, 0x80, 0x03, 0xF3, 0x0E, 0x0F, 0xCC, 0x00, 0xFE, 0x0F, 0xE0, 0x00,
  0x00, 0x7F, 0xF0, 0x00, 0xFF, 0x88, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0xE0, 0x00,
  0x00, 0x7F, 0xE0, 0x00, 0xFF, 0x88, 0x01, 0x40, 0x00, 0x00, 0x00, 0x03, 0xFE, 0x3F, 0xE0, 0x00,
  0x00, 0x7F, 0xE0, 0x00, 0x7F, 0x98, 0x07, 0xC0, 0x00, 0x00, 0x00, 0x03, 0xFC, 0x3F, 0xC0, 0x00,
  0x00, 0x3F, 0xE0, 0x00, 0x7F, 0x80, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x3F, 0xC0, 0x00, 0x3F, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x1F, 0xF0, 0x00, 0x3E, 0x00, 0x3F, 0xCF, 0xF0, 0x1F, 0x87, 0xE0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0F, 0xF8, 0x18, 0x38, 0x00, 0x3F, 0xCF, 0xF8, 0x7F, 0x8F, 0xE0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x07, 0xFE, 0x3C, 0x3B, 0x00, 0x07, 0x00, 0x3C, 0x70, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x13, 0xFE, 0x3C, 0x3F, 0x00, 0x07, 0x00, 0x1C, 0xE0, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x1F, 0xE2, 0x00, 0x7E, 0x00, 0x07, 0x07, 0xFC, 0xE0, 0x3F, 0xE0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x1F, 0xE6, 0x00, 0x7C, 0x00, 0x07, 0x0F, 0xFC, 0xE0, 0x3F, 0xF0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0F, 0xE6, 0x00, 0xF8, 0x00, 0x07, 0x1C, 0x1C, 0xE0, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0F, 0xE6, 0x00, 0xF8, 0x00, 0x07, 0x1C, 0x1C, 0xE0, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x07, 0xFF, 0x00, 0xF0, 0x00, 0x07, 0x1E, 0x1C, 0x7F, 0x1F, 0x60, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x03, 0xFF, 0x00, 0xF0, 0x00, 0x07, 0x0F, 0xFC, 0x3F, 0x8F, 0xF0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xFE, 0x00, 0xE0, 0x00, 0x06, 0x03, 0xFC, 0x0F, 0x01, 0xE0, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0xFF, 0xC1, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0xFF, 0x81, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x7E, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x3E, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x1F, 0xDF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x0F, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//int zacatek=0;

unsigned long previousMillis;
int interval = 5000;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress OWadr1, OWadr2, OWadr3, OWadr4, OWadr5, OWadr6;
ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);
File fsUploadFile;
WiFiUDP Udp;
WiFiClient client;

bool reset_hesla;
bool zobraz_displej;
bool pripojeno[10];
bool sendStatSolar[10];
bool sendStatschema[10];
bool neprirazeno;

bool HledatCidla;

bool buttonup;
bool buttondown;
//digitalní výstupy

bool DO_Cerpadlo1;
bool DO_Cerpadlo2;
bool DO_Cerpadlo3;
bool DO_Cerpadlo4;
bool RunCerp1;
bool RunCerp2;
bool RunCerp3;
bool RunCerp4;
bool restart;
bool priorita=0;


uint8_t odpoj = 0;

byte NTP_server_number = 0;

uint8_t pocet_pripojeni = 0;
uint8_t o = 0;

unsigned wifisetposition;
unsigned wifisetchar;
unsigned wifisettype;
unsigned wifisettypeselect;

unsigned wifisetcharpos[16];
int writewifieeprom = 0;




byte test;
byte i;
byte b;

byte lastweek;

byte butstate1;
byte butstate2;


byte butstatelast1;
byte butstatelast2;


byte lastminute;
byte wifidelay;


byte   controlooa1;
byte   controlooa2;
byte   controlooa3;
byte   controlooa4;
byte   controlooalast1;
byte   controlooalast2;
byte   controlooalast3;
byte   controlooalast4;

int adress = 0;
int writeAddress = 0;
int eread;


float diffON;
float diffOFF;
float maxON;
float maxOFF;
float collMax;



float diffONlast;
float diffOFFlast;
float maxONlast;
float maxOFFlast;
float collMaxlast;


//-----------HODNOTY PRO DEFAULTNI NASTAVENI
float DSdiffON = 8.0;
float DSdiffOFF = 4.0;
float DSmaxON = 90.0;
float DSmaxOFF = 95.0;
float DScollMax = 130.0;
int   DScontrolooa = 0;


float term1;
float term2;
float term3;
float term4;
float term5;
float term6;

byte pocetsnimacu;
byte pocetsnimaculast;

byte stepper;

byte setprirazeni1;
byte setprirazeni2;
byte setprirazeni3;
byte setprirazeni4;
byte setprirazeni5;
byte setprirazeni6;

byte setprirazeni1last;
byte setprirazeni2last;
byte setprirazeni3last;
byte setprirazeni4last;
byte setprirazeni5last;
byte setprirazeni6last;



bool zmenaadres = 0;

float T_kolektor;
float T_zasdole;
float T_zasnahore;
float T_mereni1;
float T_mereni2;
float T_mereni3;


//pocet wifis
byte n;

//String Index;

char ssid[20];
char password[20];

String statCid1 = "nnnnnnnnnnn";
String statCid2 = "nnnnnnnnnnn";
String statCid3 = "nnnnnnnnnnn";
String statCid4 = "nnnnnnnnnnn";
String statCid5 = "nnnnnnnnnnn";
String statCid6 = "nnnnnnnnnnn";

String ST_Cid1Prirad="nnnnnnnnnnn";
String ST_Cid2Prirad="nnnnnnnnnnn";
String ST_Cid3Prirad="nnnnnnnnnnn";
String ST_Cid4Prirad="nnnnnnnnnnn";
String ST_Cid5Prirad="nnnnnnnnnnn";
String ST_Cid6Prirad="nnnnnnnnnnn";


String ST_kolektor="nnnnnnnn";
String ST_zasdole="nnnnnnnn";
String ST_zasnahore="nnnnnnnn";
String ST_mereni1="nnnnnnnn";
String ST_mereni2="nnnnnnnn";
String ST_mereni3="nnnnnnnn";

String ST_DiffOn="nnnnnnnn";
String ST_DiffOff="nnnnnnnn";
String ST_MaxZasOn="nnnnnnnn";
String ST_MaxZasOff="nnnnnnnn";
String ST_MaxKolektor="nnnnnnnn";

String prirazeni1="nnnnnnnnnnn";
String prirazeni2="nnnnnnnnnnn";
String prirazeni3="nnnnnnnnnnn";
String prirazeni4="nnnnnnnnnnn";
String prirazeni5="nnnnnnnnnnn";
String prirazeni6="nnnnnnnnnnn";

String  typeprirazeni0 = "neprirazen";
String  typeprirazeni1 = "kolektory";
String  typeprirazeni2 = "zas. dole";
String  typeprirazeni3 = "zas. nahore";
String  typeprirazeni4 = "mereni 1";
String  typeprirazeni5 = "mereni 2";
String  typeprirazeni6 = "mereni 3";


String onoffauto1="nnnnnnn";
String onoffauto2="nnnnnnn";
String onoffauto3="nnnnnnn";
String onoffauto4="nnnnnnn";


String Web_Cerpadlo1="nnnnnnn";
String Web_Cerpadlo2="nnnnnnn";
String Web_Cerpadlo3="nnnnnnn";
String Web_Cerpadlo4="nnnnnnn";

String Graf_cas="nnnnnnn";
String PocetSnimacu="nnnnnnn";
String WriteWeek="nnnnnnn";
//String string_text;
String Sssid="nnnnnnnnnnnnn";



time_t getNtpTime();
void digitalClockDisplay();
void printDigits(int digits);
void sendNTPpacket(IPAddress &address);

void digitalClockDisplay()
{
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(".");
  Serial.print(month());
  Serial.print(".");
  Serial.print(year());
  Serial.println();
  Serial.println(weekday());



int n = hour();
     Serial.print(n);
    n = minute();
     Serial.print(n);
    n = second();
     Serial.println(n);

     
}

void printDigits(int digits)
{
  // utility for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

/*-------- NTP code ----------*/

const int NTP_PACKET_SIZE = 48;     // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
   if (WiFi.status() == WL_CONNECTED){
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
                     if(NTP_server_number==0) { 
                           WiFi.hostByName(ntpServerName0, ntpServerIP);Serial.print(ntpServerName0); Serial.print(": ");Serial.println(ntpServerIP);}
                     if(NTP_server_number==1) { 
                           WiFi.hostByName(ntpServerName1, ntpServerIP);Serial.print(ntpServerName1); Serial.print(": ");Serial.println(ntpServerIP);}
                     if(NTP_server_number==2) { 
                           WiFi.hostByName(ntpServerName2, ntpServerIP);Serial.print(ntpServerName2); Serial.print(": ");Serial.println(ntpServerIP);}
                     if(NTP_server_number==3) {
                           WiFi.hostByName(ntpServerName3, ntpServerIP);Serial.print(ntpServerName3); Serial.print(": ");Serial.println(ntpServerIP);}
                     if(NTP_server_number==4) {
}       
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      setSyncInterval(6000000);
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;      
    }
  }
  Serial.println("No NTP Response :-(");
     NTP_server_number++;
 if (NTP_server_number>=4){NTP_server_number=0;}
  }
  return 0; // return 0 if unable to get the time
  
}
// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
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
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

//HODINY Z WEBU KONEC



//***************************************************************************************************

void HledejCidla() {
  sensors.begin();
  sensors.isParasitePowerMode();
  oneWire.reset_search();
  oneWire.search(OWadr1);
  oneWire.search(OWadr2);
  oneWire.search(OWadr3);
  oneWire.search(OWadr4);
  oneWire.search(OWadr5);
  oneWire.search(OWadr6);

  //nastav rozli�en�
  sensors.setResolution(OWadr1, TEMPERATURE_PRECISION);
  sensors.setResolution(OWadr2, TEMPERATURE_PRECISION);
  sensors.setResolution(OWadr3, TEMPERATURE_PRECISION);
  sensors.setResolution(OWadr4, TEMPERATURE_PRECISION);
  sensors.setResolution(OWadr5, TEMPERATURE_PRECISION);
  sensors.setResolution(OWadr6, TEMPERATURE_PRECISION);

  setprirazeni1 = 0;
  setprirazeni2 = 0;
  setprirazeni3 = 0;
  setprirazeni4 = 0;
  setprirazeni5 = 0;
  setprirazeni6 = 0;
  zmenaadres = 1;

  // ST_kolektor =  String(T_kolektor,1);



  delay(2000);

}

void DefaultNastav() {
  EEPROM.begin(512);
  writeAddress = 0;               EEPROM.put(writeAddress, DSdiffON);
  writeAddress += sizeof(float);  EEPROM.put(writeAddress, DSdiffOFF);
  writeAddress += sizeof(float);  EEPROM.put(writeAddress, DSmaxON);
  writeAddress += sizeof(float);  EEPROM.put(writeAddress, DSmaxOFF);
  writeAddress += sizeof(float);  EEPROM.put(writeAddress, DScollMax);
  writeAddress += sizeof(float);  EEPROM.put(writeAddress, DScontrolooa);
  EEPROM.end();

  EEPROM.begin(512);
  adress = 0;               EEPROM.get(adress, diffON);
  adress += sizeof(float);  EEPROM.get(adress, diffOFF);
  adress += sizeof(float);  EEPROM.get(adress, maxON);
  adress += sizeof(float);  EEPROM.get(adress, maxOFF);
  adress += sizeof(float);  EEPROM.get(adress, collMax);
  adress += sizeof(float);  EEPROM.get(adress, controlooa1);
  adress += sizeof(float);  EEPROM.get(adress, controlooa2);
  adress += sizeof(float);  EEPROM.get(adress, controlooa3);
  adress += sizeof(float);  EEPROM.get(adress, controlooa4);
  EEPROM.end();

  delay(2000);
}

void SmazGraf() {
  WriteWeek =  String(weekday());
SPIFFS.remove("/DataGraf1"+WriteWeek+".json");
SPIFFS.remove("/DataGraf2"+WriteWeek+".json");
SPIFFS.remove("/DataGraf3"+WriteWeek+".json");
SPIFFS.remove("/DataGraf4"+WriteWeek+".json");
SPIFFS.remove("/DataGraf5"+WriteWeek+".json");
SPIFFS.remove("/DataGraf6"+WriteWeek+".json");
Serial.println("/DataGraf1"+WriteWeek+".json");  
Serial.println("mazu grafy"); 
}

void WriteGraph() {
  
           WriteWeek =  String(weekday());
           Graf_cas  =  String(hour());
           Graf_cas += ".";
           Graf_cas +=  String(minute());
           if(minute()>9){Graf_cas +=  "1";}
           
     
    // open file for writing
    File f = SPIFFS.open("/DataGraf1"+WriteWeek+".json", "a");
  if (!f) { Serial.println("file open failed");} 
    f.println("{'x':"+Graf_cas+",'y':"+ST_kolektor+"},");
    f.close();
    
   f = SPIFFS.open("/DataGraf2"+WriteWeek+".json", "a");
  if (!f) {Serial.println("file open failed"); } 
    f.println("{'x':"+Graf_cas+",'y':"+ST_zasdole+"},");
    f.close();

   f = SPIFFS.open("/DataGraf3"+WriteWeek+".json", "a");
  if (!f) {Serial.println("file open failed"); } 
    f.println("{'x':"+Graf_cas+",'y':"+ST_zasnahore+"},");
    f.close();
    
   f = SPIFFS.open("/DataGraf4"+WriteWeek+".json", "a");
  if (!f) {Serial.println("file open failed"); } 
    f.println("{'x':"+Graf_cas+",'y':"+ST_mereni1+"},");
    f.close();

   f = SPIFFS.open("/DataGraf5"+WriteWeek+".json", "a");
  if (!f) {Serial.println("file open failed"); } 
    f.println("{'x':"+Graf_cas+",'y':"+ST_mereni2+"},");
    f.close();

   f = SPIFFS.open("/DataGraf6"+WriteWeek+".json", "a");
  if (!f) {Serial.println("file open failed"); } 
    f.println("{'x':"+Graf_cas+",'y':"+ST_mereni3+"},");
    f.close();


    Serial.println("====== Writing to SPIFFS file =========");   
    Serial.println(Graf_cas+"#"+ST_kolektor+"#");
    Serial.println("/DataGraf1"+WriteWeek+".json");
              
            }
//-----------------Komunikace vzdálený server------------------
void POST(void){                                                          
                                                                                                                                                                                                                                                                                        
String WebData;                                                                                      
String ID=String(ESP.getChipId());

WebData= "chipid="+ID+"&kolektor="+ST_kolektor +"&zasdole="+ ST_zasdole +"&zasnahore="+ ST_zasnahore +"&mereni1="+ ST_mereni1 +"&mereni2="+ ST_mereni2 +"&mereni3="+ ST_mereni3+"&statDiffOn="+ST_DiffOn+" °C"+"&statDiffOff="+ST_DiffOff+"&statMaxZasOff="+ST_MaxZasOff+"&statMaxZasOn="+ST_MaxZasOn+" °C+&statMaxKolektor="+ST_MaxKolektor+" °C&output1="+Web_Cerpadlo1+"&onoffauto1="+onoffauto1+"&onoffauto2="+onoffauto2+"&onoffauto3="+onoffauto3
+"&onoffauto4="+onoffauto4+"&cid1="+prirazeni1 + "&cid2=" + prirazeni2 + "&cid3=" + prirazeni3 + "&cid4=" + prirazeni4 + "&cid5=" + prirazeni5 + "&cid6=" + prirazeni6 + "&HledatCidla="+HledatCidla+ "&PocetSnimacu="+PocetSnimacu+"&cas="+millis2time() ;  
HledatCidla=false;


           HTTPClient http;
           http.begin("http://energyface.eu:80/EspIN.php");
           http.addHeader("Content-Type", "application/x-www-form-urlencoded");
           http.POST(WebData);
//           http.writeToStream(payload);
String payload=http.getString();
 // Serial.println(ESP.getFreeHeap()); 
           http.end();
int len;
if (payload.length()>100) {len=100;} else {len=payload.length();}
char payload1[WebData.length()];



                             payload.toCharArray(payload1, len);
String webText="";
 bool nalezena_data=false;
 bool nalezeno_Wifi_Heslo;
 bool nalezeno_Wifi_Jmeno;
 String HesloWifi;
 String JmenoWifi;
 bool NastavWifi=false;
// Serial.println(len);

            nalezeno_Wifi_Heslo=false;
   for (int i=0; i <=len-1; i++){ 

   
         webText+=payload1[i];
    if ( payload1[i]=='#'){webText="";nalezeno_Wifi_Heslo=false;nalezeno_Wifi_Jmeno=false;}
    if (nalezeno_Wifi_Heslo==true){HesloWifi+=payload1[i];}
    if (nalezeno_Wifi_Jmeno==true) {JmenoWifi+=payload1[i];}
        
       if (webText=="OUT1=ON")  {controlooa1=1;}
       if (webText=="OUT1=OFF") {controlooa1=2;}
       if (webText=="OUT1=AUTO"){controlooa1=0;}
       if (webText=="OUT2=ON")  {controlooa2=1;}
       if (webText=="OUT2=OFF") {controlooa2=2;}
       if (webText=="OUT2=AUTO"){controlooa2=0;}
       if (webText=="OUT3=ON")  {controlooa3=1;}
       if (webText=="OUT3=OFF") {controlooa3=2;}
       if (webText=="OUT3=AUTO"){controlooa3=0;}
       if (webText=="OUT4=ON")  {controlooa4=1;}
       if (webText=="OUT4=OFF") {controlooa4=2;}
       if (webText=="OUT4=AUTO"){controlooa4=0;}
    
       if (webText=="BUT1=u"){diffON++;}     
       if (webText=="BUT1=d"){diffON--;}
       if (webText=="BUT2=u"){diffOFF++;}
       if (webText=="BUT2=d"){diffOFF--;}
       if (webText=="BUT3=u"){maxOFF++;}
       if (webText=="BUT3=d"){maxOFF--;}
       if (webText=="BUT4=u"){maxON++;}
       if (webText=="BUT4=d"){maxON--;}                        
       if (webText=="BUT5=u"){collMax++;}
       if (webText=="BUT5=d"){collMax--;}  

       if (webText=="CID1=0"){setprirazeni1 = 0;} 
       if (webText=="CID1=1"){setprirazeni1 = 1;} 
       if (webText=="CID1=2"){setprirazeni1 = 2;} 
       if (webText=="CID1=3"){setprirazeni1 = 3;} 
       if (webText=="CID1=4"){setprirazeni1 = 4;} 
       if (webText=="CID1=5"){setprirazeni1 = 5;}     
       if (webText=="CID1=6"){setprirazeni1 = 6;}
        
       if (webText=="CID2=0"){setprirazeni2 = 0;} 
       if (webText=="CID2=1"){setprirazeni2 = 1;}
       if (webText=="CID2=2"){setprirazeni2 = 2;}
       if (webText=="CID2=3"){setprirazeni2 = 3;}
       if (webText=="CID2=4"){setprirazeni2 = 4;}
       if (webText=="CID2=5"){setprirazeni2 = 5;}
       if (webText=="CID2=6"){setprirazeni2 = 6;} 

       if (webText=="CID3=0"){setprirazeni3 = 0;}                        
       if (webText=="CID3=1"){setprirazeni3 = 1;}
       if (webText=="CID3=2"){setprirazeni3 = 2;}
       if (webText=="CID3=3"){setprirazeni3 = 3;}
       if (webText=="CID3=4"){setprirazeni3 = 4;}
       if (webText=="CID3=5"){setprirazeni3 = 5;}
       if (webText=="CID3=6"){setprirazeni3 = 6;}

       if (webText=="CID4=0"){setprirazeni4 = 0;}
       if (webText=="CID4=1"){setprirazeni4 = 1;}
       if (webText=="CID4=2"){setprirazeni4 = 2;}
       if (webText=="CID4=3"){setprirazeni4 = 3;}
       if (webText=="CID4=4"){setprirazeni4 = 4;}
       if (webText=="CID4=5"){setprirazeni4 = 5;}
       if (webText=="CID4=6"){setprirazeni4 = 6;}

       if (webText=="CID5=0"){setprirazeni5 = 0;}
       if (webText=="CID5=1"){setprirazeni5 = 1;}
       if (webText=="CID5=2"){setprirazeni5 = 2;}
       if (webText=="CID5=3"){setprirazeni5 = 3;}
       if (webText=="CID5=4"){setprirazeni5 = 4;}
       if (webText=="CID5=5"){setprirazeni5 = 5;}
       if (webText=="CID5=6"){setprirazeni5 = 6;}

       if (webText=="CID6=0"){setprirazeni6 = 0;}
       if (webText=="CID6=1"){setprirazeni6 = 1;}
       if (webText=="CID6=2"){setprirazeni6 = 2;}
       if (webText=="CID6=3"){setprirazeni6 = 3;}
       if (webText=="CID6=4"){setprirazeni6 = 4;}
       if (webText=="CID6=5"){setprirazeni6 = 5;}
       if (webText=="CID6=6"){setprirazeni6 = 6;}       
  //     Serial.println(webText);
       if (webText=="Default_nastav=1"){DefaultNastav();}
       if (webText=="hledej_cidla=1"){HledejCidla();HledatCidla=true;Serial.println("jj");} 
       if (webText=="WifiHeslo="){nalezeno_Wifi_Heslo=true;}
       if (webText=="WifiJmeno="){nalezeno_Wifi_Jmeno=true;NastavWifi=true;}
       if (webText=="ButtUpdate="){display.println("Aktualizuji..");display.display();   t_httpUpdate_return ret = ESPhttpUpdate.update("www.energyface.eu",80,"/update.php");} 
       
   Serial.println(payload);          
  }
//  Serial.println(webText);
//  Serial.println(ESP.getFreeHeap()); // volani funkce a zobrazeni hodnoty

     if (NastavWifi==true){
                               JmenoWifi.toCharArray(ssid, 11);
                               HesloWifi.toCharArray(password, 11); 
                               reset_hesla==false;  
                               WiFi.disconnect();
                               delay(200);                 
                               WiFi.begin(ssid, password);
                               writewifieeprom = 1;
                               Serial.println(HesloWifi);    
                               Serial.println(JmenoWifi);
                               NastavWifi==false;
                               
     }
}
//-----------------Komunikace klient------------------
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:
      pripojeno[num] = false;
      sendStatSolar[num]=false;      
      pocet_pripojeni = 0;
      i = 0;
      while (i <= 10) {
        if (pripojeno[i] == true) {
          pocet_pripojeni++;
        } i++;
      }

      //   if (odpoj>=3){odpoj=0;}
      //    if(pocet_pripojeni>=3) {webSocket.sendTXT(odpoj, "MaxConnected");Serial.println("příliš mnoho připojení");odpoj++;}

      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        webSocket.sendTXT(num, "Connected");
        pripojeno[num] = true;
        pocet_pripojeni = 0;
        i = 0;
        while (i <= 10) {
          if (pripojeno[i] == true) {
            pocet_pripojeni++;
          } i++;
        }
        if (pocet_pripojeni >= 10) {
          if (num == odpoj) {
            odpoj++;
          } if (odpoj == 3) {
            odpoj = 0;
          }  webSocket.sendTXT(odpoj, "MaxConnected#");
          odpoj++;
        }
      }
      break;
    case WStype_TEXT:

//      if (payload[0] == 'a') {webSocket.sendTXT(num, "StatCid#" + prirazeni1 + "#" + prirazeni2 + "#" + prirazeni3 + "#" + prirazeni4 + "#" + prirazeni5 + "#" + prirazeni6 + "#" + PocetSnimacu + "#");}

     // začni odesílat data o nastaveni solaru do klienta
      if (payload[0] == 'd') { sendStatSolar[num]=true;
                               webSocket.sendTXT(num, "StatCid#" + prirazeni1 + "#" + prirazeni2 + "#" + prirazeni3 + "#" + prirazeni4 + "#" + prirazeni5 + "#" + prirazeni6 + "#" + PocetSnimacu + "#");}
      if (payload[0] == 'v') { sendStatSolar[num]=false;}
      if (payload[0] == 'l') { sendStatschema[num]=true;}
      if (payload[0] == 'k') { sendStatschema[num]=false;}
      if (payload[0] == 'u') { Serial.println("update soft");}      
      if (payload[0] == 'p') { Serial.println("update spiff");}

      // Tlačitka nastavení Soláru
      if (payload[0] == 'b') {
        if (payload[1] == '0') {
          if (payload[2] == '0') {
            diffON++;
          }

          if (payload[2] == '1') {
            diffON-- ;
          }
          if (payload[2] == '2') {
            diffOFF++;
          }
          if (payload[2] == '3') {
            diffOFF-- ;
          }
          if (payload[2] == '4') {
            maxOFF++ ;
          }
          if (payload[2] == '5') {
            maxOFF-- ;
          }
          if (payload[2] == '6') {
            maxON++  ;
          }
          if (payload[2] == '7') {
            maxON--  ;
          }
          if (payload[2] == '8') {
            collMax++;
          }
          if (payload[2] == '9') {
            collMax--;
          }
        }



        // Tlačítka Cerpadla a Továrního nastavení
        if (payload[1] == '1') {
          if (payload[2] == '0') {controlooa1 = 0;}
          if (payload[2] == '1') {controlooa1 = 1;}
          if (payload[2] == '2') {controlooa1 = 2;}

          if (payload[2] == '3') { controlooa2 = 0;}
          if (payload[2] == '4') { controlooa2 = 1;}
          if (payload[2] == '5') { controlooa2 = 2;}
          
          if (payload[2] == '6') { controlooa3 = 0;}
          if (payload[2] == '7') { controlooa3 = 1;}
          if (payload[2] == '8') { controlooa3 = 2;}
          
          if (payload[2] == '9') { controlooa4 = 0;}

        }

      if (payload[1] == '2') {

          if (payload[2] == '0') {controlooa4 = 1;}

          if (payload[2] == '1') {controlooa4 = 2;}

          
          if (payload[2] == '2') {
            DefaultNastav();
            webSocket.sendTXT(num, "DefaultNastav#");
          }
          if (payload[2] == '3') {
            webSocket.sendTXT(num, "HledamPocetCidel#");
            HledejCidla();   pocetsnimacu = sensors.getDeviceCount(); PocetSnimacu  =  String(sensors.getDeviceCount());  webSocket.sendTXT(num, "NaselPocetCidel#" + PocetSnimacu + "#");
            webSocket.sendTXT(num, "StatCid#" + prirazeni1 + "#" + prirazeni2 + "#" + prirazeni3 + "#" + prirazeni4 + "#" + prirazeni5 + "#" + prirazeni6 + "#" + PocetSnimacu + "#");
          }
        }



      }
      // Senzory
      if (payload[0] == 's') {
        if (payload[1] == '0') {
          if (payload[2] == '0') {
            setprirazeni1 = 0;
          }
          if (payload[2] == '1') {
            setprirazeni1 = 1;
          }
          if (payload[2] == '2') {
            setprirazeni1 = 2;
          }
          if (payload[2] == '3') {
            setprirazeni1 = 3;
          }
          if (payload[2] == '4') {
            setprirazeni1 = 4;
          }
          if (payload[2] == '5') {
            setprirazeni1 = 5;
          }
          if (payload[2] == '6') {
            setprirazeni1 = 6;
          }
        }
        if (payload[1] == '1') {
          if (payload[2] == '0') {
            setprirazeni2 = 0;
          }
          if (payload[2] == '1') {
            setprirazeni2 = 1;
          }
          if (payload[2] == '2') {
            setprirazeni2 = 2;
          }
          if (payload[2] == '3') {
            setprirazeni2 = 3;
          }
          if (payload[2] == '4') {
            setprirazeni2 = 4;
          }
          if (payload[2] == '5') {
            setprirazeni2 = 5;
          }
          if (payload[2] == '6') {
            setprirazeni2 = 6;
          }
        }
        if (payload[1] == '2') {
          if (payload[2] == '0') {
            setprirazeni3 = 0;
          }
          if (payload[2] == '1') {
            setprirazeni3 = 1;
          }
          if (payload[2] == '2') {
            setprirazeni3 = 2;
          }
          if (payload[2] == '3') {
            setprirazeni3 = 3;
          }
          if (payload[2] == '4') {
            setprirazeni3 = 4;
          }
          if (payload[2] == '5') {
            setprirazeni3 = 5;
          }
          if (payload[2] == '6') {
            setprirazeni3 = 6;
          }
        }
        if (payload[1] == '3') {
          if (payload[2] == '0') {
            setprirazeni4 = 0;
          }
          if (payload[2] == '1') {
            setprirazeni4 = 1;
          }
          if (payload[2] == '2') {
            setprirazeni4 = 2;
          }
          if (payload[2] == '3') {
            setprirazeni4 = 3;
          }
          if (payload[2] == '4') {
            setprirazeni4 = 4;
          }
          if (payload[2] == '5') {
            setprirazeni4 = 5;
          }
          if (payload[2] == '6') {
            setprirazeni4 = 6;
          }
        }
        if (payload[1] == '4') {
          if (payload[2] == '0') {
            setprirazeni5 = 0;
          }
          if (payload[2] == '1') {
            setprirazeni5 = 1;
          }
          if (payload[2] == '2') {
            setprirazeni5 = 2;
          }
          if (payload[2] == '3') {
            setprirazeni5 = 3;
          }
          if (payload[2] == '4') {
            setprirazeni5 = 4;
          }
          if (payload[2] == '5') {
            setprirazeni5 = 5;
          }
          if (payload[2] == '6') {
            setprirazeni5 = 6;
          }
        }
        if (payload[1] == '5') {
          if (payload[2] == '0') {
            setprirazeni6 = 0;
          }
          if (payload[2] == '1') {
            setprirazeni6 = 1;
          }
          if (payload[2] == '2') {
            setprirazeni6 = 2;
          }
          if (payload[2] == '3') {
            setprirazeni6 = 3;
          }
          if (payload[2] == '4') {
            setprirazeni6 = 4;
          }
          if (payload[2] == '5') {
            setprirazeni6 = 5;
          }
          if (payload[2] == '6') {
            setprirazeni6 = 6;
          }
        }

      }
  
    // heslo wifi
   
     if (payload[0] == 'w') {
        if (payload[1] == 's') {   ssid[0]= payload[2];  ssid[1]=payload[3]; ssid[2]= payload[4]; ssid[3]=payload[5];ssid[4]=payload[6];ssid[5]=payload[7];ssid[6]=payload[8];ssid[7]=payload[9];ssid[8]=payload[10];ssid[9]=payload[11];ssid[10]=payload[12];ssid[11]=payload[13];ssid[12]=payload[14];ssid[13]=payload[15];ssid[14]=payload[16];ssid[17]=payload[19];ssid[18]=payload[20];ssid[19]=payload[21];ssid[20]=payload[22];    }
        if (payload[1] == 'p') {  password[0]= payload[2];password[1]= payload[3];password[2]= payload[5];password[3]= payload[5];password[4]= payload[6];password[5]= payload[7];password[6]= payload[8];password[7]= payload[9];password[8]= payload[10];password[9]= payload[11];password[10]= payload[12];    writewifieeprom = 1;
    WiFi.disconnect();}

                            }

    break;
                            
  }



}

String getContentType(String filename) {
  if (server.hasArg("download"))          return "application/octet-stream";
  else if (filename.endsWith(".htm"))     return "text/html";
  else if (filename.endsWith(".html"))    return "text/html";
  else if (filename.endsWith(".css"))     return "text/css";
  else if (filename.endsWith(".js"))      return "application/javascript";
  else if (filename.endsWith(".png"))     return "image/png";
  else if (filename.endsWith(".gif"))     return "image/gif";
  else if (filename.endsWith(".jpg"))     return "image/jpeg";
  else if (filename.endsWith(".ico"))     return "image/x-icon";
  else if (filename.endsWith(".xml"))     return "text/xml";
  else if (filename.endsWith(".pdf"))     return "application/x-pdf";
  else if (filename.endsWith(".zip"))     return "application/x-zip";
  else if (filename.endsWith(".js.gz"))   return "application/javascript";
  else if (filename.endsWith(".css.gz"))  return "text/css";
  else if (filename.endsWith(".html.gz")) return "text/html";
  else if (filename.endsWith(".htm.gz"))  return "text/html";
  else if (filename.endsWith(".gz"))      return "application/x-gzip";
  return "text/plain";
}

bool handleFileRead(String path) {

//  DBG_OUTPUT_PORT.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
        priorita=true;
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();


    return true;
  }
   priorita=false;
  return false;
}

String millis2time() {
  String Time = "";
  unsigned long ss;
  byte mm, hh;
  ss = millis() / 1000;
  hh = ss / 3600;
  mm = (ss - hh * 3600) / 60;
  ss = (ss - hh * 3600) - mm * 60;
  if (hh < 10)Time += "0";
  Time += (String)hh + ":";
  if (mm < 10)Time += "0";
  Time += (String)mm + ":";
  if (ss < 10)Time += "0";
  Time += (String)ss;
  return Time;
}

//----------------Autorizace webserver--------------------
void handleLogin() {
Serial.println("root");
  if (server.hasHeader("Cookie")) {
    String cookie = server.header("Cookie");
    Serial.println(cookie);

int len=cookie.length();
char payload[len];
String webText="";
                             cookie.toCharArray(payload, len);
   for (int i=0; i <=len-1; i++){ 

   
         webText+=payload[i];
    if ( payload[i]==';'){webText="";}

        
       if (webText=="device=mobil")    {mobil=true;}
       if (webText=="device=desktop")  {mobil=false;}
  }
 
  }
  if (server.hasArg("DISCONNECT")) {Serial.println("hovno");
    String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=0\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
    if (server.arg("USERNAME") == "admin" &&  server.arg("PASSWORD") == "admin" ) {
      String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=1\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
      server.sendContent(header);
      //      msg = "alert('Špatne heslo nebo jmeno');";

      return;
    }
   if (server.arg("USERNAME") == "uzivatel" &&  server.arg("PASSWORD") == "1234" ) {
      String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=2\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
      server.sendContent(header);
      //      msg = "alert('Špatne heslo nebo jmeno');";

      return;
    }

   if (server.arg("USERNAME") == "propuls" &&  server.arg("PASSWORD") == "3479" ) {Serial.println("no funguje...");
      String header = "HTTP/1.1 301 OK\r\nSet-Cookie: ESPSESSIONID=3\r\nLocation: /\r\nCache-Control: no-cache\r\n\r\n";
      server.sendContent(header);
      //      msg = "alert('Špatne heslo nebo jmeno');";

      return;
    }   

    server.send(200, "text/xml", "<datafeeder><WrongLogin>Špatne heslo nebo jmeno</WrongLogin></datafeeder>");

    
  }

  // open file for reading
  File f = SPIFFS.open("/login.html", "r");
  size_t sent = server.streamFile(f, "text/html");
  f.close();
}

//root page can be accessed only if authentification is ok
void handleRoot() {
  String header;
  String content;
  if (!is_authentified()) {
    Serial.println("neutorizovano");
    header = "HTTP/1.1 301 OK\r\nLocation: /login\r\nCache-Control: no-cache\r\n\r\n";
    server.sendContent(header);
    return;
  }

  if (server.hasHeader("Cookie")) {
    String cookie = server.header("Cookie");
    Serial.println(cookie);

int len=cookie.length();
char payload[len];
String webText="";
                             cookie.toCharArray(payload, len);
   for (int i=0; i <=len-1; i++){ 

   
         webText+=payload[i];
    if ( payload[i]==';'){webText="";}

        
       if (webText==" device=mobil")    {mobil=true;}
       if (webText==" device=desktop")  {mobil=false;}
       if (webText=="device=mobil")    {mobil=true;}
       if (webText=="device=desktop")  {mobil=false;}
   Serial.println(webText);
  }
   if (mobil==false){
                       File f = SPIFFS.open("/index.htm", "r");
                       size_t sent = server.streamFile(f, "text/html");
                       f.close();
  }
  if (mobil==true){
                       File f = SPIFFS.open("/m.index.htm", "r");
                       size_t sent = server.streamFile(f, "text/html");
                       f.close();
  }
}
}

//no need authentification
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

bool is_authentified() {
  if (server.hasHeader("Cookie")) {
    String cookie = server.header("Cookie");
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {return true;}
    if (cookie.indexOf("ESPSESSIONID=2") != -1) {return true;}
    if (cookie.indexOf("ESPSESSIONID=3") != -1) {return true;}
  return false;
}
}

void JSON(){
  File f = SPIFFS.open("/test.json", "r");
  size_t sent = server.streamFile(f, "text/json");
  f.close();
          }

void handleXML()
{
String XML;
 
  XML = "<?xml version='1.0'?>";
  XML += "<datafeeder>";
  XML += "<tempIN>50</tempIN>";
  XML += "<tempOUT>10</tempOUT>";
  XML += "<runtime>15</runtime>";
  XML += "</datafeeder>";

  server.send(200, "text/xml", XML);
}

//--------------Dotykový displej---------------------------
void touch_Return(String page_ID, String component_ID, String touch_event) {
  if (page_ID == "0") {
    if (component_ID == "4") {
 //     if (digitalRead(13)) { // kdy je dioda rozsvicena
//          digitalWrite(13, LOW); // zhasni diodu
//          visible("b1", true); // zobraz ikolu zhasnute led
//          test66 = test66+1; 
      }
      else {
  //      digitalWrite(13, HIGH);test66 = test66+1; // rozsvit diodu
 //       visible("b1", false);
      }
    }
  }
//}
//------------------------------------------------------------------
void page_Return(String page_ID) {}
//------------------------------------------------------------------
String Nextion_receive(boolean read_data) { //returns generic

  boolean answer = false; // znacka
  char bite; // promenna pro ulozeni znaku
  String cmd; // promenna pro ulozeni textu
  byte countEnd = 0; // pocitadlo
  unsigned long previous; // cas spusteni
  int timeout = 1000; // doba po kterou se ceka na prichozi data
  previous = millis();

  do { // cekani na spravnou odpoved
    if (nextion.available() > 0) { // kdyz jsou k dispozici data, precti data
      bite = nextion.read();
      cmd += bite;
      display.println(bite);
      display.println(cmd);
      if ((byte)bite == 0xff) countEnd++;
      if (countEnd == 3) answer = true;
    }
  }
  while (!answer && !((unsigned long)(millis() - previous) >= timeout)); // ceka na spravnou hodnotu, nebo uplynuti casu

  if (read_data) { // read general data
    if (cmd[0] == 0x65) { // Touch event return data
      // 0X65 + Page ID + Component ID + TouchEvent + End
      touch_Return(String(cmd[1], DEC), String(cmd[2], DEC), String(cmd[3], DEC));
    }
    else if (cmd[0] == 0x66) { // Current page ID number returns
      // 0X66 + Page ID + End
      page_Return(String(cmd[1], DEC));
    }
    else if (cmd[0] == 0x67) { // Touch coordinate data returns
      // 0X67++ Coordinate X High-order+Coordinate X Low-order+Coordinate Y High-order+Coordinate Y Low-order+TouchEvent State+End
    }
    else if (cmd[0] == 0x68) { // Touch Event in sleep mode
      // 0X68++Coordinate X High-order+Coordinate X Low-order+Coordinate Y High-order+Coordinate Y Low-order+TouchEvent State+End
    }
  }
  else { //read get data
    if (cmd[0] == 0x70) { // String variable data returns
      // X70+Variable Content in ASCII code+End
      return cmd;
    }
    else if (cmd[0] == 0x71) { // Numeric variable data returns
      // 0X71+variable binary data(4 bytes little endian mode, low in front)+End
      return cmd;
    }
  }
}
//------------------------------------------------------------------
void check_display() { // kontrola prijatych dat
  if (nextion.available() > 0) // kontroluje obsah pameti, pokud nen nic odeslano, dalsi cast programu se neprovede
  {
    Nextion_receive(true); // precist hodnoty z serial portu
  }
}
//------------------------------------------------------------------
void nextion_init(int speed_init) { // nastaveni pri spusteni displeje
  nextion.begin(speed_init);
}
//------------------------------------------------------------------
void visible(String componentID, boolean visible) { // skryti/zobrazeni komponenty
  String vis = "vis " + componentID + "," + String(visible);
  send_Command(vis.c_str());             
   vis = "N1.val=50";
  send_Command(vis.c_str());  

  
}
//------------------------------------------------------------------
void send_Command(const char* cmd) { // odeslani dat do displeje
  nextion.print(cmd);
  nextion.write(0xFF);
  nextion.write(0xFF);
  nextion.write(0xFF);
  nextion.flush();
}




void setup(){
Serial.begin(9600);

delay(500);
// httpServer.begin();
  server.on("/xml", handleXML);
  server.on("/index.htm", handleRoot);
  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/inline", []() {
  server.send(200, "text/plain", "this works without need of authentification");
  });
// kontrola hesla a už. jména
  server.onNotFound(handleNotFound);
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  //ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize );


  pinMode(but1, INPUT);
  pinMode(but2, INPUT);
 // pinMode(but3, INPUT);
 // pinMode(but4, INPUT);
  pinMode(Rele_1, OUTPUT);
  pinMode(Rele_2, OUTPUT);
  pinMode(Rele_3, OUTPUT);
  pinMode(Rele_4, OUTPUT);

 digitalWrite(Rele_1,LOW);
 digitalWrite(Rele_2,LOW);
 digitalWrite(Rele_3,LOW);
 digitalWrite(Rele_4,LOW);

  EEPROM.begin(512);
  adress = 0;               EEPROM.get(adress, diffON);
  adress += sizeof(float);  EEPROM.get(adress, diffOFF);
  adress += sizeof(float);  EEPROM.get(adress, maxON);
  adress += sizeof(float);  EEPROM.get(adress, maxOFF);
  adress += sizeof(float);  EEPROM.get(adress, collMax);
  adress += sizeof(float);  EEPROM.get(adress, controlooa1);
  adress += sizeof(float);  EEPROM.get(adress, controlooa2);
  adress += sizeof(float);  EEPROM.get(adress, controlooa3);
  adress += sizeof(float);  EEPROM.get(adress, controlooa4);
  adress += sizeof(float);
  EEPROM.get(adress, OWadr1); adress += sizeof(OWadr1);
  EEPROM.get(adress, OWadr2); adress += sizeof(OWadr2);
  EEPROM.get(adress, OWadr3); adress += sizeof(OWadr3);
  EEPROM.get(adress, OWadr4); adress += sizeof(OWadr4);
  EEPROM.get(adress, OWadr5); adress += sizeof(OWadr5);
  EEPROM.get(adress, OWadr6); adress += sizeof(OWadr6);
  EEPROM.get(adress, pocetsnimacu); adress += sizeof(int);

  adress = 76;

  EEPROM.get(adress, setprirazeni1); adress += sizeof(int);
  EEPROM.get(adress, setprirazeni2); adress += sizeof(int);
  EEPROM.get(adress, setprirazeni3); adress += sizeof(int);
  EEPROM.get(adress, setprirazeni4); adress += sizeof(int);
  EEPROM.get(adress, setprirazeni5); adress += sizeof(int);
  EEPROM.get(adress, setprirazeni6); adress += sizeof(int);
  adress = 104;
  adress = 110;
  EEPROM.get(adress, ssid); adress += sizeof(ssid);
  EEPROM.get(adress, password);

  EEPROM.end();

  WiFi.begin(ssid, password);

  //File system INIT
  SPIFFS.begin();
  //  server.onNotFound(handleNotFound);
  server.onNotFound([]() {
    if (!handleFileRead(server.uri()))  server.send(404, "text/plain", "FileNotFound");
  });

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  sensors.begin();
  sensors.isParasitePowerMode();
  server.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);



  PocetSnimacu = String( sensors.getDeviceCount());


  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
  
  nextion_init(9600);


  



                              
  display.clearDisplay();
  display.drawBitmap(0, 0, (const uint8_t *) logo16_glcd_bmp, 128, 64, WHITE);
  display.display();
  delay(4000);

}


void loop()
{


//Serial.println(ESP.getFreeHeap()); // volani funkce a zobrazeni hodnoty

  if (WiFi.status() == WL_CONNECTED) {
    aktualniMillis = millis();
    if(aktualniMillis - predchoziMillis > 1000) {predchoziMillis = aktualniMillis;POST();}
  }
//if (priorita==1){
  server.handleClient();webSocket.loop();check_display();

if (weekday()!=lastweek){if(restart==1){SmazGraf();} restart=1;lastweek=weekday(); }
if (timeStatus() != timeNotSet) { if (minute()!=lastminute) {lastminute=minute(); digitalClockDisplay();WriteGraph(); } }


  if (WiFi.status() != WL_CONNECTED) {
    if (wifidelay == 0) {
      WiFi.begin("Propuls","aaaaa12345")
      ;//(ssid, password);
    }
    wifidelay++;
    if (wifidelay > 500) {
      wifidelay = 0;
    }
  }

  //odesílání dat klientovi
  if (pocet_pripojeni > 0) {
    if (o >= 10) {o = 0;}
                    while (o < 10) {
                      if (pripojeno[o] == true)        { webSocket.sendTXT(o,"RunTime#"+millis2time() + "#");
                          if (sendStatSolar[o] == true){ webSocket.sendTXT(o, "Nastaveni#" + ST_DiffOn + " °C#" + ST_DiffOff + " °C#" + ST_MaxZasOff + " °C#" + ST_MaxZasOn + " °C#" + ST_MaxKolektor + " °C#" + onoffauto1+"#"+ onoffauto2+"#"+ onoffauto3+"#"+ onoffauto4+"#");}
                          if (sendStatschema[o] == true){webSocket.sendTXT(o, "Cidla#" + ST_kolektor + "#" + ST_zasdole + "#" + ST_zasnahore + "#" + ST_mereni1 + "#" + ST_mereni2 + "#" + ST_mereni3 + "#" + Web_Cerpadlo1 + "#" + millis2time() + "#");}
                     o++; 
                     break;                                 }
                     o++;          }
                          }


if (sensors.getDeviceCount()>0) {
  if (stepper > 50) {
  Serial.println("cekuju cidla");
    
    stepper = 0;
    sensors.requestTemperaturesByAddress(OWadr1);
 //   sensors.requestTemperatures();
    if (pocetsnimacu > 0) {
      term1= sensors.getTempCByIndex(0);
      //term1 = sensors.getTempC(OWadr1);
      statCid1 = "Nalezeno";
    } else {
      statCid1 = "Nenalezeno";
    }
    if (pocetsnimacu > 1) {
      term2 = sensors.getTempC(OWadr2);
      statCid2 = "Nalezeno";
    } else {
      statCid2 = "Nenalezeno";
    }
    if (pocetsnimacu > 2) {
      term3 = sensors.getTempC(OWadr3);
      statCid3 = "Nalezeno";
    } else {
      statCid3 = "Nenalezeno";
    }
    if (pocetsnimacu > 3) {
      term4 = sensors.getTempC(OWadr4);
      statCid4 = "Nalezeno";
    } else {
      statCid4 = "Nenalezeno";
    }
    if (pocetsnimacu > 4) {
      term5 = sensors.getTempC(OWadr5);
      statCid5 = "Nalezeno";
    } else {
      statCid5 = "Nenalezeno";
    }
    if (pocetsnimacu > 5) {
      term6 = sensors.getTempC(OWadr6);
      statCid6 = "Nalezeno";
    } else {
      statCid6 = "Nenalezeno";
    }
  }
  stepper++;
      }

  T_kolektor =  400.0;
  T_zasdole =   400.0;
  T_zasnahore = 400.0;
  T_mereni1 =   400.0;
  T_mereni2 =   400.0;
  T_mereni3 =   400.0;

  switch (setprirazeni1) {
    case 0: prirazeni1 = typeprirazeni0; break;
    case 1: prirazeni1 = typeprirazeni1; T_kolektor =  term1; break;
    case 2: prirazeni1 = typeprirazeni2; T_zasdole =   term1; break;
    case 3: prirazeni1 = typeprirazeni3; T_zasnahore = term1; break;
    case 4: prirazeni1 = typeprirazeni4; T_mereni1 =   term1; break;
    case 5: prirazeni1 = typeprirazeni5; T_mereni2 =   term1; break;
    case 6: prirazeni1 = typeprirazeni6; T_mereni3 =   term1; break;
  }
  switch (setprirazeni2) {
    case 0: prirazeni2 = typeprirazeni0; break;
    case 1: prirazeni2 = typeprirazeni1; T_kolektor =  term2; break;
    case 2: prirazeni2 = typeprirazeni2; T_zasdole =   term2; break;
    case 3: prirazeni2 = typeprirazeni3; T_zasnahore = term2; break;
    case 4: prirazeni2 = typeprirazeni4; T_mereni1 =   term2; break;
    case 5: prirazeni2 = typeprirazeni5; T_mereni2 =   term2; break;
    case 6: prirazeni2 = typeprirazeni6; T_mereni3 =   term2; break;
  }
  switch (setprirazeni3) {
    case 0: prirazeni3 = typeprirazeni0; break;
    case 1: prirazeni3 = typeprirazeni1; T_kolektor =  term3; break;
    case 2: prirazeni3 = typeprirazeni2; T_zasdole =   term3; break;
    case 3: prirazeni3 = typeprirazeni3; T_zasnahore = term3; break;
    case 4: prirazeni3 = typeprirazeni4; T_mereni1 =   term3; break;
    case 5: prirazeni3 = typeprirazeni5; T_mereni2 =   term3; break;
    case 6: prirazeni3 = typeprirazeni6; T_mereni3 =   term3; break;
  }
  switch (setprirazeni4) {
    case 0: prirazeni4 = typeprirazeni0; break;
    case 1: prirazeni4 = typeprirazeni1; T_kolektor =  term4; break;
    case 2: prirazeni4 = typeprirazeni2; T_zasdole =   term4; break;
    case 3: prirazeni4 = typeprirazeni3; T_zasnahore = term4; break;
    case 4: prirazeni4 = typeprirazeni4; T_mereni1 =   term4; break;
    case 5: prirazeni4 = typeprirazeni5; T_mereni2 =   term4; break;
    case 6: prirazeni4 = typeprirazeni6; T_mereni3 =   term4; break;
  }
  switch (setprirazeni5) {
    case 0: prirazeni5 = typeprirazeni0; break;
    case 1: prirazeni5 = typeprirazeni1; T_kolektor =  term5; break;
    case 2: prirazeni5 = typeprirazeni2; T_zasdole =   term5; break;
    case 3: prirazeni5 = typeprirazeni3; T_zasnahore = term5; break;
    case 4: prirazeni5 = typeprirazeni4; T_mereni1 =   term5; break;
    case 5: prirazeni5 = typeprirazeni5; T_mereni2 =   term5; break;
    case 6: prirazeni5 = typeprirazeni6; T_mereni3 =   term5; break;
  }
  switch (setprirazeni6) {
    case 0: prirazeni6 = typeprirazeni0; break;
    case 1: prirazeni6 = typeprirazeni1; T_kolektor =  term6; break;
    case 2: prirazeni6 = typeprirazeni2; T_zasdole =   term6; break;
    case 3: prirazeni6 = typeprirazeni3; T_zasnahore = term6; break;
    case 4: prirazeni6 = typeprirazeni4; T_mereni1 =   term6; break;
    case 5: prirazeni6 = typeprirazeni5; T_mereni2 =   term6; break;
    case 6: prirazeni6 = typeprirazeni6; T_mereni3 =   term6; break;
  }

  ST_kolektor =  String(T_kolektor, 1);
  ST_zasdole =   String(T_zasdole, 1);
  ST_zasnahore = String(T_zasnahore, 1);
  ST_mereni1 =   String(T_mereni1, 1);
  ST_mereni2 =   String(T_mereni2, 1);
  ST_mereni3 =   String(T_mereni3, 1);

  ST_DiffOn       =  String(diffON, 1);
  ST_DiffOff      =  String(diffOFF, 1);
  ST_MaxZasOn     =  String(maxON, 1);
  ST_MaxZasOff    =  String(maxOFF, 1);
  ST_MaxKolektor  =  String(collMax, 1);

  ST_Cid1Prirad   =  String(setprirazeni1, 1);

  if (T_kolektor == 400.0 and  T_zasdole == 400.0 and T_zasnahore == 400.0 and T_mereni1 == 400.0 and T_mereni2 == 400.0 and T_mereni3 == 400.0) {
    neprirazeno = HIGH;
  } else {
    neprirazeno = LOW;
  }

   if (controlooa1 < 0)  {controlooa1 = 0;}
   if (controlooa1 > 2)  {controlooa1 = 2;}
   if (controlooa1 == 0) {onoffauto1 = "AUTO1";DO_Cerpadlo1 = RunCerp1;} 
   if (controlooa1 == 1) {onoffauto1 = "ON1";DO_Cerpadlo1 = true;}
   if (controlooa1 == 2) {onoffauto1 = "OFF1";DO_Cerpadlo1 = false;}

  if (DO_Cerpadlo1) { Web_Cerpadlo1 = "RUN";} else { Web_Cerpadlo1 = "STOP";}

  if (controlooa2 < 0)  {controlooa2 = 0;} 
  if (controlooa2 > 2)  {controlooa2 = 2;}
  if (controlooa2 == 0) {onoffauto2 = "AUTO2";DO_Cerpadlo2 = RunCerp2;}
  if (controlooa2 == 1) {onoffauto2 = "ON2";DO_Cerpadlo2 = true;}
  if (controlooa2 == 2) {onoffauto2 = "OFF2";DO_Cerpadlo2 = false;}

  if (controlooa3 < 0)  {controlooa3 = 0;}
  if (controlooa3 > 2)  {controlooa3 = 2;}
  if (controlooa3 == 0) {onoffauto3 = "AUTO3";DO_Cerpadlo3 = RunCerp3;} 
  if (controlooa3 == 1) {onoffauto3 = "ON3";DO_Cerpadlo3 = true;}
  if (controlooa3 == 2) {onoffauto3 = "OFF3";DO_Cerpadlo3 = false;}

  if (controlooa4 < 0)  {controlooa4 = 0;} 
  if (controlooa4 > 2)  {controlooa4 = 2;}
  if (controlooa4 == 0) {onoffauto4 = "AUTO4";DO_Cerpadlo4 = RunCerp4;} 
  if (controlooa4 == 1) {onoffauto4 = "ON4";DO_Cerpadlo4 = true;} 
  if (controlooa4 == 2) {onoffauto4 = "OFF4";DO_Cerpadlo4 = false;}

 if (DO_Cerpadlo1) {  digitalWrite(Rele_1,HIGH); }else { digitalWrite(Rele_1,LOW);  } 
 if (DO_Cerpadlo2) {  digitalWrite(Rele_2,HIGH); }else { digitalWrite(Rele_2,LOW);  } 
 if (DO_Cerpadlo3) {  digitalWrite(Rele_3,HIGH); }else { digitalWrite(Rele_3,LOW);  } 
 if (DO_Cerpadlo4) {  digitalWrite(Rele_4,HIGH); }else { digitalWrite(Rele_4,LOW);  } 

  butstate1 = digitalRead(but1);
  butstate2 = digitalRead(but2);


      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0); 



//if (butstate2==true){   String Default_ssid ="Propuls"; 
//                        String Default_heslo="aaaaa12345";                      
//                               Default_ssid.toCharArray(ssid, 20);
 //                              Default_heslo.toCharArray(password, 20);
 //                              reset_hesla==false;    
  //                             WiFi.disconnect();
  //                             delay(100);                 
  //                             WiFi.begin(ssid, password);
  //                             writewifieeprom = 1;
  //                             Serial.println("jede smejd");
  //                 }

//if (butstate1==true){ // display.println("Aktualizuji..");display.display(); t_httpUpdate_return ret=ESPhttpUpdate.update("192.168.1.192",80,"/arduino.bin"); display.clearDisplay();display.println(test2); display.println("Hotovo!");display.display() ;
                      //         switch(ret) {
                      //                       case HTTP_UPDATE_FAILED:
                      //                             test2=ESPhttpUpdate.getLastErrorString();
                      //                       break;
                      //
                      //                       case HTTP_UPDATE_NO_UPDATES:
                      //                            test2="NO_UPDATES";
                      //                       break;
                      //
                      //                       case HTTP_UPDATE_OK:
                      //                            test2="UPDATE_OK";
                      //                       break;
                      //                     }
//                      display.println("SSID:");
//                      display.println(ssid); 
//                      display.println("Heslo:"); 
//                      display.println(password); 
//                      display.println(ST_DiffOn);
//}

//if (butstate1==false){      
   if (WiFi.status() != WL_CONNECTED) { display.println("Nepripojen");   }
   if (WiFi.status() == WL_CONNECTED) { display.println("IP:");  
                                        display.println(WiFi.localIP());}
                                        display.setTextSize(1);
                                        display.print("ID:");
                                        display.println(ESP.getChipId());
                                        
//                                      }                                                              
                                              
 Serial.println(WiFi.localIP());           
display.display();
  

//Serial.println(butstate1);
//Serial.println(butstate2);







  //===============================================================================================================================================================

  //zaloha nasstaveni parametru programu
  if (diffONlast != diffON or diffOFFlast != diffOFF or maxONlast != maxON or maxOFFlast != maxOFF or collMaxlast != collMax or controlooalast1 != controlooa1 or controlooalast2 != controlooa2 or controlooalast3 != controlooa3 or controlooalast4 != controlooa4) {

    EEPROM.begin(512);
    writeAddress = 0;               EEPROM.put(writeAddress, diffON);
    writeAddress += sizeof(float);  EEPROM.put(writeAddress, diffOFF);
    writeAddress += sizeof(float);  EEPROM.put(writeAddress, maxON);
    writeAddress += sizeof(float);  EEPROM.put(writeAddress, maxOFF);
    writeAddress += sizeof(float);  EEPROM.put(writeAddress, collMax);
    writeAddress += sizeof(float);  EEPROM.put(writeAddress, controlooa1);
    writeAddress += sizeof(float);  EEPROM.put(writeAddress, controlooa2);
    writeAddress += sizeof(float);  EEPROM.put(writeAddress, controlooa3);
    writeAddress += sizeof(float);  EEPROM.put(writeAddress, controlooa4);

    EEPROM.end();
  }


  //zaloha nastaveni adres cidel
  if (zmenaadres != 0) {

    EEPROM.begin(512);
    writeAddress = 24;

    EEPROM.put(writeAddress, OWadr1); writeAddress += sizeof(OWadr1);
    EEPROM.put(writeAddress, OWadr2); writeAddress += sizeof(OWadr2);
    EEPROM.put(writeAddress, OWadr3); writeAddress += sizeof(OWadr3);
    EEPROM.put(writeAddress, OWadr4); writeAddress += sizeof(OWadr4);
    EEPROM.put(writeAddress, OWadr5); writeAddress += sizeof(OWadr5);
    EEPROM.put(writeAddress, OWadr6); writeAddress += sizeof(OWadr6);
    EEPROM.put(writeAddress, pocetsnimacu); writeAddress += sizeof(int); //76

    zmenaadres = 0;
    EEPROM.end();
  }

  //zaloha nastaveni prirazeni cidel
  if (setprirazeni1last != setprirazeni1 or setprirazeni2last != setprirazeni2 or setprirazeni3last != setprirazeni3 or setprirazeni4last != setprirazeni4 or setprirazeni5last != setprirazeni5 or setprirazeni6last != setprirazeni6) {

    EEPROM.begin(512);
    writeAddress = 76;

    EEPROM.put(writeAddress, setprirazeni1); writeAddress += sizeof(int);
    EEPROM.put(writeAddress, setprirazeni2); writeAddress += sizeof(int);
    EEPROM.put(writeAddress, setprirazeni3); writeAddress += sizeof(int);
    EEPROM.put(writeAddress, setprirazeni4); writeAddress += sizeof(int);
    EEPROM.put(writeAddress, setprirazeni5); writeAddress += sizeof(int);
    EEPROM.put(writeAddress, setprirazeni6); writeAddress += sizeof(int);


    EEPROM.end();
  }


  if (writewifieeprom > 0) {
    EEPROM.begin(512);
    writeAddress = 110;
    EEPROM.put(writeAddress, ssid); writeAddress += sizeof(ssid);
    EEPROM.put(writeAddress, password); writeAddress += sizeof(password);
    EEPROM.end();
    writewifieeprom = 0;
  }


  setprirazeni1last = setprirazeni1;
  setprirazeni2last = setprirazeni2;
  setprirazeni3last = setprirazeni3;
  setprirazeni4last = setprirazeni4;
  setprirazeni5last = setprirazeni5;
  setprirazeni6last = setprirazeni6;


  pocetsnimaculast = pocetsnimacu;

  diffONlast = diffON;
  diffOFFlast = diffOFF;
  maxONlast = maxON;
  maxOFFlast = maxOFF;
  collMaxlast = collMax;
  controlooalast1 = controlooa1;
  controlooalast2 = controlooa2;
  controlooalast3 = controlooa3;
  controlooalast4 = controlooa4;
}

//}




