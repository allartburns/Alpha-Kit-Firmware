#define SPARKFUN_DATA_URL "data.sparkfun.com"
#define SPARKFUN_DATA_PORT 80
#define SPARKFUN_INPUT_PATH "input"
#define PRIVATE_KEY "<<privatekey>>"
#define PUBLIC_KEY "<<publickey>>"
#define STREAM_NAME "<<streamname>>"

void sendToSparkfunData(float temperature){
    char szData[16];

    if(client.connect(SPARKFUN_DATA_URL, SPARKFUN_DATA_PORT)){
        sprintf(szData, "%.2f", temperature);

        client.print("GET /");
        client.print(SPARKFUN_INPUT_PATH);
        client.print("/");
        client.print(PUBLIC_KEY);
        client.print("?");
        client.print("private_key=");
        client.print(PRIVATE_KEY);
        client.print("&");
        client.print(STREAM_NAME);
        client.print("=");
        client.println(szData);
        client.print("Host: ");
        client.println(SPARKFUN_DATA_URL);
        client.println("Connection: close");
        client.println();
        client.flush();

        while(client.available()){
            sprintf(szData, "%c", client.read());

            Serial.print(szData);
        }

        client.stop();
    }
    else{
        Serial.println("Cannot connect to Sparkfun Data");
    }
}
