
uint8_t MQTTstatus = 0;
void getMacID(String MAC);
uint8_t mqttStatus()
{
    return MQTTstatus;
}

//broker used: broker.hivemq.com
const char *ordersTopic = "pickcounter/orders"; //send anything to this topic and it will reset the MDB bus
int orderReceived = 0;
String orderValues[2] = {"", ""};
String DevID="";
void setMACID(String MAC){
    DevID=MAC;
}
uint8_t NewOrderReceived()
{
    if (orderReceived == 1)
    {
        return 1;
    }
    else if(orderReceived==2)
    {
        return 2;
    }
    else{
        return 0;
    }
}
String getOrderValues()
{
    orderReceived = 0;
    return (orderValues[0] + String(",") + orderValues[1]);
}
void callback(char *topic, byte *payload, unsigned int length)
{
    // Serial.print("Message arrived [");
    // Serial.print(topic);
    // Serial.print("] ");
    String payloadV = "";
    for (int i = 0; i < length; i++)
    {
        // Serial.print((char)payload[i]);
        payloadV = payloadV + String((char)payload[i]);
    }
    Serial.println();
    if (String(topic) == String(ordersTopic))
    {
        String LEDColor = StringSeparator(payloadV, ',', 0);
        String Value = StringSeparator(payloadV, ',', 1);
        String mID=StringSeparator(payloadV, ',', 2);
        // Serial.print("LEDColor=");
        // Serial.println(LEDColor);
        // Serial.print("Value=");
        // Serial.println(Value);
        if(mID==DevID){
        orderReceived = 1;
        orderValues[0] = LEDColor;
        orderValues[1] = Value;
        }
        else{
            orderReceived=2;
        }
    }

    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1')
    {
        digitalWrite(BUILTIN_LED, LOW); // Turn the LED on (Note that LOW is the voltage level
                                        // but actually the LED is on; this is because
                                        // it is active low on the ESP-01)
    }
    else
    {
        digitalWrite(BUILTIN_LED, HIGH); // Turn the LED off by making the voltage HIGH
    }
}

void publishValues(String topic, String msg)
{
    client.publish(topic.c_str(), msg.c_str());
}

void reconnect()
{
    // Loop until we're reconnected
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str(),"bbniqtdq","D87AAz6nsCdN"))
        {
            Serial.println("connected");

            // Once connected, publish an announcement...

            // ... and resubscribe
            client.subscribe(ordersTopic);
        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}