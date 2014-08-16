#ifndef _PHANT_LIBRARY
#define _PHANT_LIBRARY


// Make library cross-compatiable
// with Arduino, GNU C++ for tests, and Spark.
//#if defined(ARDUINO) && ARDUINO >= 100
//#include "Arduino.h"
//#elif defined(SPARK)
//#include "application.h"
//#endif

// TEMPORARY UNTIL the stuff that supports the code above is deployed to the build IDE
#include "application.h"

//
#define PHANT_DEFAULT_PORT	80
#define PHANT_GET_METHOD	1
#define PHANT_POST_METHOD	2
#define PHANT_SOCKET_NUMBER	6


namespace Phant
{
	class Stream
	{
	private:
		String _pub;
		String _prv;
		String _host;
		unsigned int _port;
		unsigned int _method;
		String _params;
		TCPClient _client;
		
		String get();
		String post();
		
	public:
		Stream(String host, String publicKey, String privateKey, unsigned int port = PHANT_DEFAULT_PORT, unsigned int method = PHANT_POST_METHOD);
		int begin();
		void add(String field, String data);
		void add(String field, char data);
		void add(String field, int data);
		void add(String field, byte data);
		void add(String field, long data);
		void add(String field, unsigned int data);
		void add(String field, unsigned long data);
		void add(String field, float data);
		void add(String field, double data);
		
		char* dtostrf (double val, signed char width, unsigned char prec, char *sout);
		
		int sendData();
		int clearStream();
		int deleteStream(String deleteKey);
		
	};
}

#endif
