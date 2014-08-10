// v0.01
// help yourself
// don't blame me
// don't get stung

#include "application.h"
#include <math.h>
//pin definitions
int led = D0;
int therm[2];         //two thermistor for demonstration expandable to 6
int micPin = A7;
int optPin = A0;
int dynamicData[128]; //we are very short of memory so reuse this for optical and audio
int packetSize;
char UDPin[16];
UDP udp;

//
int v[2];                    //thermistor ADC readings
const float Rinf = 0.06;     //thermistor constants for 57k thermistor
const float B = 3990.0;
const float Rup = 47000.0;   //pullup resistor value

float t[2];                  //temperature measurement

char output[48];             //for spark variable

void setup() {
    therm[0] = A1;
    therm[1] = A2;
    Spark.variable("temperature", &output, STRING); 
    pinMode(led, OUTPUT);
    udp.begin(5208);
}

void loop() {
    if (udp.parsePacket()>1){
        digitalWrite(led, HIGH);   // Turn ON the LED to show packet received
        udp.read(UDPin,16);
        if(UDPin[0]=='s'){         // If sn received, sleep for n mins
            int i = UDPin[1] - '0';
            if (i>0 && i<10){
                i*=60; //add a 0 for longer sleeps when debugged
                Spark.sleep(SLEEP_MODE_DEEP, i);
            }
        }
        if(UDPin[0]=='d'){          // If d received, send monitoring data
            udp.beginPacket(udp.remoteIP(), udp.remotePort());
            if (UDPin[1]=='t'){
                //update temperatures
                for (int i = 0; i<2;i++){
                    v[i] = analogRead(therm[i]);
                    t[i] = B / log(Rup * v[i]/(4096 - v[i])/Rinf) - 273;
                }
                sprintf(output,"%d, %d, %.1f, %.1f",v[0],Network.RSSI(),t[0],t[1]);
                udp.write((unsigned char*)&output,sizeof(output));
            }
            else if (UDPin[1]=='a'){
                //update sound
                for (int i=0;i<128;i++){
                    dynamicData[i]=analogRead(micPin);
                }
                udp.write((unsigned char*)&dynamicData[0],sizeof(dynamicData));
            }
            else if (UDPin[1]=='o'){
                for (int i=0;i<128;i++){
                    dynamicData[i]=analogRead(optPin);
                }
                udp.write((unsigned char*)&dynamicData[0],sizeof(dynamicData));
            }
            udp.endPacket();
            digitalWrite(led, LOW);    // Turn OFF the LED
        }
    }
}
