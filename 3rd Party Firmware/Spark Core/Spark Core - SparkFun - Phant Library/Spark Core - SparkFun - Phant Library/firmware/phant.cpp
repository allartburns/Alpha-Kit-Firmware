#include "phant.h"

// Constructor
Phant::Stream::Stream(String host, String publicKey, String privateKey, unsigned int port, unsigned int method)
{
	_host = host;
	_pub = publicKey;
	_prv = privateKey;
	_params = "";
	_port = port;
	_method = method;
}
//	get()
//		function returns formated request to send data with HTTP GET method
String Phant::Stream::get() {
	String result = "GET /input/" + _pub + "?private_key" + _prv + _params;
	result += "\nUser-Agent: SparkCoreLibrary";
	result += "Host: " + _host + "\n";

	return result;
}

//	get()
//		function returns formated request to send data with HTTP GET method
String Phant::Stream::post() {
	String params = _params.substring(1);
	String result = "POST /input/" + _pub + " HTTP/1.1\n";
	result += "User-Agent: SparkCoreLibrary\n";
	result += "Host: " + _host + "\n";
	result += "Connection: close\n";
	result += "Phant-Private-Key: " + _prv + "\n";
	result += "Content-Type: application/x-www-form-urlencoded\n";
	result += "Content-Length: " + String(params.length()) + "\n\n";
	result += params;

	return result;
}

// Initializers that should be called only once in the `setup()` function
int Phant::Stream::begin(){
	_client = TCPClient(PHANT_SOCKET_NUMBER);
	return 1;
}

// Main API functions that the library provides
// typically called in `loop()` or `setup()` functions

//	add(field, string)
//		function adds a String field to the sending queue.
//		Warning: string type data needs to be urlencoded beforehand
void Phant::Stream::add(String field, String data) {
	_params += "&" + field + "=" + data;
}

//	add(field, char)
//		function adds a char field to the sending queue.
void Phant::Stream::add(String field, char data) {
	_params += "&" + field + "=" + String(data);
}

//	add(field, int)
//		function adds a int field to the sending queue.
void Phant::Stream::add(String field, int data) {
	_params += "&" + field + "=" + String(data);
}

//	add(field, byte)
//		function adds a byte field to the sending queue.
void Phant::Stream::add(String field, byte data) {
	_params += "&" + field + "=" + String(data);
}

//	add(field, long)
//		function adds a long field to the sending queue.
void Phant::Stream::add(String field, long data) {
	_params += "&" + field + "=" + String(data);
}

//	add(field, unsignedInt)
//		function adds a unsigned int field to the sending queue.
void Phant::Stream::add(String field, unsigned int data) {
	_params += "&" + field + "=" + String(data);
}

//	add(field, unsignedLong)
//		function adds a unsigned long field to the sending queue.
void Phant::Stream::add(String field, unsigned long data) {
	_params += "&" + field + "=" + String(data);
}

//	add(field, double)
//		function adds a double field to the sending queue.
void Phant::Stream::add(String field, double data) {
	char tmp[30];
	dtostrf(data, 1, 4, tmp);
	_params += "&" + field + "=" + String(tmp);
}

//	add(field, float)
//		function adds a float field to the sending queue.
void Phant::Stream::add(String field, float data) {
	char tmp[30];
	dtostrf(data, 1, 4, tmp);
	_params += "&" + field + "=" + String(tmp);
}

//convert double to ascii
char *Phant::Stream::dtostrf (double val, signed char width, unsigned char prec, char *sout) {
	char fmt[20];
	sprintf(fmt, "%%%d.%df", width, prec);
	sprintf(sout, fmt, val);
	return sout;
}

//	sendData()
//		function send pending data to the server and clears the queue
//
//	TODO: read plain text response from the server for success/failure assertion

int Phant::Stream::sendData() {
	if (_method == PHANT_POST_METHOD){
		int length = _host.length()+1;
		char charBuffer[length];
		_host.toCharArray(charBuffer,length);
		if(_client.connect(charBuffer,_port)) {
			String request = post();
			_client.println(request);
			delay(150);
			if(!_client.connected()){
				_client.stop();
			}
			_client.flush();
			_client.stop();
			
			_params = "";
			
			return 1;
		}
	}
	return 0;
}

//	clearStream()
//		function clears data stream content
//

int Phant::Stream::clearStream() {
	String request = "DELETE /input/" + _pub + " HTTP/1.1\n";
	request += "User-Agent: SparkCoreLibrary\n";
	request += "Host: " + _host + "\n";
	request += "Connection: close\n";
	request += "Phant-Private-Key: " + _prv + "\n";
	
	int length = _host.length()+1;
	char charBuffer[length];
	_host.toCharArray(charBuffer,length);
	if(_client.connect(charBuffer,_port)) {
		_client.println(request);
		delay(150);
		if(!_client.connected()){
			_client.stop();
		}
		_client.flush();
		_client.stop();
		return 1;
	}
	
	return 0;
}

//	deleteStream(deleteKey)
//		function takes a deleteKey and completely remove the data stream
//

int Phant::Stream::deleteStream(String deleteKey) {
	String request = "DELETE /streams/" + _pub + " HTTP/1.1\n";
	request += "User-Agent: SparkCoreLibrary\n";
	request += "Host: " + _host + "\n";
	request += "Connection: close\n";
	request += "Phant-Delete-Key: " + deleteKey + "\n";
	
	int length = _host.length()+1;
	char charBuffer[length];
	_host.toCharArray(charBuffer,length);
	if(_client.connect(charBuffer,_port)) {
		_client.println(request);
		delay(150);
		if(!_client.connected()){
			_client.stop();
		}
		_client.flush();
		_client.stop();
		return 1;
	}
	
	return 0;
}
