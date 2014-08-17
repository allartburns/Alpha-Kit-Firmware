#include "DHT22.h"
#include "elapsedMillis.h"

DHT dht(D0, DHT22);
char humidity[10] = "-1";
char temperature[10] = "-274";
char temp_humid[21];


void setup() {
	dht.begin();

	Spark.variable("humidity", &humidity, STRING);
	Spark.variable("temperature", &temperature, STRING);
	Spark.variable("temp_humid", &temp_humid, STRING);

	strcpy(temp_humid, temperature);
	strcat(temp_humid, ",");
	strcat(temp_humid, humidity);
}


void loop() {
	dht.poll();
	
	if(dhtElapsed>=2000) {
        String(dht.readHumidity()).toCharArray(humidity, 10);
        String(dht.readTemperature(true)).toCharArray(temperature, 10);
        
        strcpy(temp_humid, temperature);
        strcat(temp_humid, ",");
        strcat(temp_humid, humidity);
        
        dhtElapsed = 0;
    }
}