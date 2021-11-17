

#include "WiFiCreds.h"
#include <PubSubClient.h>
#include "SStack.h"

#define BUILTIN_LED 2

const char *mqtt_server = "smart-pilot.cloudmqtt.com";
String MAC = "";
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (512)
char msg[MSG_BUFFER_SIZE];
int value = 0;
uint8_t WCAvailable = 0; //weight calibration data
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

void processOrderAndSendResponse(uint8_t n, String amt, String id)
{

    //process the order here
    String orderStatus[3] = {"Completed", "Timeout", "Failed"};
    String responseJSON = String("{\"Status\":\"" + orderStatus[n]) + String("\",\"Amount\":\"") + String(amt);
    responseJSON += String("\",\"id\":\"") + String(id);
    responseJSON += String("\"}");
    publishValues("pickcounter/orderstatus", responseJSON);
}

void setup()
{
    Serial.begin(115200);
    pinMode(BUILTIN_LED, OUTPUT); // Initialize the BUILTIN_LED pin as an output

    setup_wifi();

    Serial.print("Device ID: ");
    //MAC = String(WiFi.macAddress());

    Serial.println(getMacAddress());
    setMACID(getMacAddress());
    client.setBufferSize(1024);
    client.setKeepAlive(30);
    client.setServer(mqtt_server, 1883);

    client.setCallback(callback);
    // delay(3000);
    // publishValues("pickcounter/getWCdata","getData");
}

void loop()
{

    if (!client.connected())
    {
        reconnect();
    }
    client.loop();

    if (NewOrderReceived() == 1)
    {
        Serial.println("New Order Received.");
        Serial.print("Order Number, Product, Amount = ");
        String orderValues = getOrderValues();
        Serial.println(orderValues);
        String amt = StringSeparator(orderValues, ',', 2);
        String ordID = StringSeparator(orderValues, ',', 0);
        if (orderValues.indexOf("timeoutTestProduct") >= 0)
        {
            processOrderAndSendResponse(1, "0", ordID); //0=completed, 1=timeout, 2=failed
        }
        if (orderValues.indexOf("failedTestProduct") >= 0)
        {
            processOrderAndSendResponse(2, "0", ordID); //0=completed, 1=timeout, 2=failed
        }
        processOrderAndSendResponse(0, amt, ordID); //0=completed, 1=timeout, 2=failed
        Serial.println("Order processed and completed");
    }
    else if (NewOrderReceived() == 2)
    {
        Serial.println("Wrong Device ID");
        String orderValues = getOrderValues();
        String ordID = StringSeparator(orderValues, ',', 0);

        processOrderAndSendResponse(2, "0", ordID);
    }

    unsigned long now = millis();
    if (now - lastMsg > 4000)
    { //data update frequency == 2000mSec or 2sec
        if (WCAvailable == 0)
        {
            WCAvailable = 1;
            publishValues(String("pickcounter/getWCdata"), String("getData"));
        }
        lastMsg = now;

        // put normal processing code here
    }
}
