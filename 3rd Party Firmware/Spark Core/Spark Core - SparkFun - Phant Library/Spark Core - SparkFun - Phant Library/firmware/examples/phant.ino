#include "phant/phant.h"

Phant::Stream stream1("data.sparkfun.com", "rooNvrdyEziLb46x4wLL", "jkkwJ8NAlyIdq0xM0ndd", 80, PHANT_POST_METHOD);

void setup() {
    Serial.begin(9600);

    //Phant intitialization
    stream1.begin();
}

void loop() {
	//Adding a string field to the stream
	String sTest = "Hello World";
	stream1.add("string",sTest);

	//Adding a char field to the stream
	char cTest = 'e';
	stream1.add("char",cTest);

	//Adding a int field to the stream
	int iTest = rand();
	stream1.add("int",iTest);

	//Adding a byte field to the stream
	byte bTest = D0;
	stream1.add("byte",bTest);

	//Adding a long field to the stream
	long lTest = 186000L;
	stream1.add("long",lTest);

	//Adding a float field to the stream
	float fTest = 1.117;
	stream1.add("float",fTest);

	//Adding a double field to the stream
	double dTest = 11.23444556677;
	stream1.add("double",dTest);

	stream1.sendData();
    delay(1000);
}