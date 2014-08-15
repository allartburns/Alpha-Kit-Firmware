#include <OneWire.h> 
// SPI and the pair of SFE_CC3000 include statements are required
// for using the CC300 shield as a client device.
#include <SPI.h>
#include <SFE_CC3000.h>
#include <SFE_CC3000_Client.h>
// Progmem allows us to store big strings in flash using F().
// We'll sacrifice some flash for extra DRAM space.
#include <Progmem.h>



////////////////////////////////////
// CC3000 Shield Pins & Variables //
////////////////////////////////////
// Don't change these unless you're using a breakout board.
#define CC3000_INT      2   // Needs to be an interrupt pin (D2/D3)
#define CC3000_EN       7   // Can be any digital pin
#define CC3000_CS       10  // Preferred is pin 10 on Uno
#define IP_ADDR_LEN     4   // Length of IP address in bytes

int DS18S20_Pin = 3; //DS18S20 Signal pin on digital 2

////////////////////
// WiFi Constants //
////////////////////
char ap_ssid[] = "Shower Curtain";                // SSID of network
char ap_password[] = "poopedon";        // Password of network
unsigned int ap_security = WLAN_SEC_WPA2; // Security of network
// ap_security can be any of: WLAN_SEC_UNSEC, WLAN_SEC_WEP, 
//  WLAN_SEC_WPA, or WLAN_SEC_WPA2
unsigned int timeout = 30000;             // Milliseconds
char server[] = "data.sparkfun.com";      // Remote host site


// Initialize the CC3000 objects (shield and client):
SFE_CC3000 wifi = SFE_CC3000(CC3000_INT, CC3000_EN, CC3000_CS);
SFE_CC3000_Client client = SFE_CC3000_Client(wifi);

//Temperature chip i/o
OneWire ds(DS18S20_Pin);  // on digital pin 2

/////////////////
// Phant Stuff //
/////////////////
const String publicKey = "ZGGZ56yvQKS4jrAEqQX4";
const String privateKey = "2mmV8qn0wPHBYRzX9qxB";
const byte NUM_FIELDS = 3;
const String fieldNames[NUM_FIELDS] = {"hive_number", "sensor_name", "temp"};
String fieldData[NUM_FIELDS];


void setup(void) {
  Serial.begin(9600);

  // Set Up WiFi:
  setupWiFi();
}

void loop(void) {
  float temperature = getTemp();
  Serial.println(temperature);
  
  //fieldData[2] = getTemp();
  fieldData[0]="test_hive";
  fieldData[1]="temp_0";
  fieldData[2] = floatToString("ASDF", getTemp(), 4, 6);
  
  postData(); // the postData() function does all the work, 
                // check it out below.
  
  delay(5000); //just here to slow down the output so it is easier to read
  
}


float getTemp(){
  //returns the temperature from one DS18S20 in DEG Celsius

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
      //no more sensors on chain, reset search
      ds.reset_search();
      return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
      Serial.print("Device is not recognized");
      return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end

  byte present = ds.reset();
  ds.select(addr);    
  ds.write(0xBE); // Read Scratchpad

  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  
  ds.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;
  
  return TemperatureSum;
  
}



void postData()
{

  // Make a TCP connection to remote host
  if ( !client.connect(server, 80) )
  {
    // Error: 4 - Could not make a TCP connection
    Serial.println(F("Error: 4"));
  }

  // Post the data! Request should look a little something like:
  // GET /input/publicKey?private_key=privateKey&light=1024&switch=0&time=5201 HTTP/1.1\n
  // Host: data.sparkfun.com\n
  // Connection: close\n
  // \n
  client.print("GET /input/");
  client.print(publicKey);
  client.print("?private_key=");
  client.print(privateKey);
  for (int i=0; i<NUM_FIELDS; i++)
  {
    client.print("&");
    client.print(fieldNames[i]);
    client.print("=");
    client.print(fieldData[i]);
  }
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println();

  while (client.connected())
  {
    if ( client.available() )
    {
      char c = client.read();
      Serial.print(c);
    }      
  }
  Serial.println();
}

void setupWiFi()
{
  ConnectionInfo connection_info;
  int i;

  // Initialize CC3000 (configure SPI communications)
  if ( wifi.init() )
  {
    Serial.println(F("CC3000 Ready!"));
  }
  else
  {
    // Error: 0 - Something went wrong during CC3000 init!
    Serial.println(F("Error: 0"));
  }

  // Connect using DHCP
  Serial.print(F("Connecting to: "));
  Serial.println(ap_ssid);
  if(!wifi.connect(ap_ssid, ap_security, ap_password, timeout))
  {
    // Error: 1 - Could not connect to AP
    Serial.println(F("Error: 1"));
  }

  // Gather connection details and print IP address
  if ( !wifi.getConnectionInfo(connection_info) ) 
  {
    // Error: 2 - Could not obtain connection details
    Serial.println(F("Error: 2"));
  }
  else
  {
    Serial.print(F("My IP: "));
    for (i = 0; i < IP_ADDR_LEN; i++)
    {
      Serial.print(connection_info.ip_address[i]);
      if ( i < IP_ADDR_LEN - 1 )
      {
        Serial.print(".");
      }
    }
    Serial.println();
  }
}

char * floatToString(char * outstr, double val, byte precision, byte widthp){
  char temp[16];
  byte i;

  // compute the rounding factor and fractional multiplier
  double roundingFactor = 0.5;
  unsigned long mult = 1;
  for (i = 0; i < precision; i++)
  {
    roundingFactor /= 10.0;
    mult *= 10;
  }
  
  temp[0]='\0';
  outstr[0]='\0';

  if(val < 0.0){
    strcpy(outstr,"-\0");
    val = -val;
  }

  val += roundingFactor;

  strcat(outstr, itoa(int(val),temp,10));  //prints the int part
  if( precision > 0) {
    strcat(outstr, ".\0"); // print the decimal point
    unsigned long frac;
    unsigned long mult = 1;
    byte padding = precision -1;
    while(precision--)
      mult *=10;

    if(val >= 0)
      frac = (val - int(val)) * mult;
    else
      frac = (int(val)- val ) * mult;
    unsigned long frac1 = frac;

    while(frac1 /= 10)
      padding--;

    while(padding--)
      strcat(outstr,"0\0");

    strcat(outstr,itoa(frac,temp,10));
  }

  // generate space padding 
  if ((widthp != 0)&&(widthp >= strlen(outstr))){
    byte J=0;
    J = widthp - strlen(outstr);
    
    for (i=0; i< J; i++) {
      temp[i] = ' ';
    }

    temp[i++] = '\0';
    strcat(temp,outstr);
    strcpy(outstr,temp);
  }
  
  return outstr;
}
