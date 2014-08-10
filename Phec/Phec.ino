// beehive monitor
// v1.0
// help yourself
// don't blame me
// don't get stung
// other version for chip antenna narwal/112/5211
// this version for  uFL/105/5208
// have added 145uS delay to audio measurement to see whether there is noise or a 60Hz tone
// the first 50 odd frequency measurements seem to be anomalous - power settling or something - so take 50 measurements before saving
// add test for connection and toggle WiFi if connection is lost xxx removed - didn't help - still needs fixing
// slow sample rate - now see low frequency optical variation so switch off LED during optical read!
// 11/7 further slow optical sample rate from 345 to 745uS


//#include "application.h" //uncomment for USB programming
#include <math.h>
//pin definitions
const int led = D0;
int therm[6];              //  6 thermistors
const int micPin = A7;
const int optPin = A1;
uint16_t dynamicData[256]; //we are very short of memory so reuse this for optical and audio
int packetSize;
char UDPin[16];
UDP udp;
int i;

//
int v[6];                    //thermistor ADC readings
const float Rinf = 0.072;    //thermistor constants for 100k thermistor
const float B = 4190.0;
const float Rup = 47000.0;   //pullup resistor value

float t[6];                  //temperature measurement

char output[60];             //for spark variable

void setup() {
    therm[0] = A0;
    therm[1] = A2;
    therm[2] = A3;
    therm[3] = A4;
    therm[4] = A5;
    therm[5] = A6;
    Spark.variable("temperature", &output, STRING); 
    pinMode(led, OUTPUT);

    udp.begin(5208);
}

void loop() {
    if (udp.parsePacket()>1){
        digitalWrite(led, HIGH);   // Turn ON the LED to show packet received
        udp.read(UDPin,16);
        if(UDPin[0]=='s'){         // If sn received, sleep for n mins
            i = UDPin[1] - '0';
            if (i>0 && i<10){
                i*=600; //add a 00 for longer sleeps when debugged 600 = 10 mins times i
                Spark.sleep(SLEEP_MODE_DEEP, i);
            }
        }
        if(UDPin[0]=='d'){          // If d received, send monitoring data
            udp.beginPacket(udp.remoteIP(), udp.remotePort());
            if (UDPin[1]=='t'){
                //update temperatures
                for (i = 0; i<6;i++){
                    v[i] = analogRead(therm[i]);
                    t[i] = B / log(Rup * v[i]/(4096 - v[i])/Rinf) - 273;
                }
                sprintf(output,"%d, %d, %.1f, %.1f, %.1f, %.1f, %.1f, %.1f",v[0],Network.RSSI(),t[1],t[2],t[3],t[4],t[5],t[0]);
                //sprintf(output,"%d, %d, %d, %d, %d, %d",v[0],v[1],v[2],v[3],v[4],v[5]);
                udp.write((unsigned char*)&output,sizeof(output));
            }
            else if (UDPin[1]=='a'){
                //update sound
                for (i=-50;i<256;i++){
                    if (i >= 0) dynamicData[i]=analogRead(micPin);
                    else analogRead(micPin); //dummy read
                    delayMicroseconds(345);
                }
                udp.write((unsigned char*)&dynamicData[0],sizeof(dynamicData));
            }
            else if (UDPin[1]=='o'){
                RGB.control(true);
                RGB.brightness(0);
                for (i= -50;i<256;i++){
                    if (i >= 0) dynamicData[i]=analogRead(optPin);
                    else analogRead(optPin); //dummy read
                    delayMicroseconds(1545);
                }
                udp.write((unsigned char*)&dynamicData[0],sizeof(dynamicData));
                RGB.control(false);
            }
            udp.endPacket();
            digitalWrite(led, LOW);    // Turn OFF the LED
        }
    }

}

