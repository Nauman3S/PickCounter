

#include "WiFiCreds.h"
#include <PubSubClient.h>
#include "SStack.h"

#define BUILTIN_LED 2

const char *mqtt_server = "broker.hivemq.com";
String MAC = "";
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;
#include "MQTTHandler.h"

void setup_wifi()
{

    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void processOrderAndSendResponse(uint8_t n)
{

    //process the order here
    String orderStatus[3] = {"Completed", "Timeout", "Failed"};
    String responseJSON = "{\"Status\":\"" + orderStatus[n] + String("\"}");
    publishValues("pickcounter/orderstatus", responseJSON);
}

void setup()
{
    pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output
    //setupADC();

    Serial.print("Device ID: ");
    //MAC = String(WiFi.macAddress());
    Serial.println(getMacAddress());
    setMACID(getMacAddress());
    Serial.begin(115200);
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void loop()
{

    if (!client.connected())
    {
        reconnect();
    }
    client.loop();

    if (NewOrderReceived() == 1)
    {   Serial.println("New Order Received.");
        Serial.print("LEDColor,Value = ");
        Serial.println(getOrderValues());
        processOrderAndSendResponse(0);//0=completed, 1=timeout, 2=failed
    }
    else if(NewOrderReceived() == 2){
       Serial.println("Wrong Device ID");
       getOrderValues();//failed, wrong mac
    }

    unsigned long now = millis();
    if (now - lastMsg > 2000)
    { //data update frequency == 2000mSec or 2sec
        lastMsg = now;

        // put normal processing code here
    }
}
