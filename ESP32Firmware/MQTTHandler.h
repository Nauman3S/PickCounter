
uint8_t MQTTstatus = 0;
void getMacID(String MAC);
uint8_t mqttStatus()
{
    return MQTTstatus;
}

//broker used: broker.hivemq.com
const char *ordersTopic = "pickcounter/orders"; //send anything to this topic and it will reset the MDB bus
const char *weightsTopic = "pickcounter/weights";
const char *temp = "pickcounter/temp/temp";
int orderReceived = 0;
String orderValues[4] = {"", "", ""};
String calibTopic = "";
String DevID = "";
String calibValue = "";
String weiValue = "";
void setMACID(String MAC)
{
    DevID = MAC;
}

void setCalibrationValue(String v)
{
    calibValue = v;
}
int getCalibrationValue()
{
    return calibValue.toInt();
}

void setWeightValue(String v)
{
    weiValue = v;
}
String getWeightValue()
{
    return weiValue;
}
uint8_t NewOrderReceived()
{
    if (orderReceived == 1)
    {
        return 1;
    }
    else if (orderReceived == 2)
    {
        return 2;
    }
    else
    {
        return 0;
    }
}
String getOrderValues()
{
    orderReceived = 0;
    return (orderValues[0] + String(",") + orderValues[1] + String(",") + orderValues[2]);
}
void callback(char *topic, byte *payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    String payloadV = "";
    for (int i = 0; i < length; i++)
    {
        // Serial.print((char)payload[i]);
        payloadV = payloadV + String((char)payload[i]);
    }
    Serial.println(payloadV);
    Serial.println();
    if (String(topic) == String(ordersTopic))
    {
        String ordNumber = StringSeparator(payloadV, ',', 0);
        String productN = StringSeparator(payloadV, ',', 1);
        String amount = StringSeparator(payloadV, ',', 2);
        // Serial.print("LEDColor=");
        // Serial.println(LEDColor);
        // Serial.print("Value=");
        // Serial.println(Value);

        orderReceived = 1;
        orderValues[0] = ordNumber;
        orderValues[1] = productN;
        orderValues[2] = amount;
    }
    else if (String(topic) == String(calibTopic))
    {
        Serial.println("Calibration Data Received");
        setCalibrationValue(payloadV);
        Serial.print("Calibration Value set: ");
        Serial.println(getCalibrationValue());
    }

    else if (String(topic) == String(weightsTopic))
    {
        Serial.println("Weights Data Received");
        String wV = getWeightValue();

        Serial.print("Weight Value set: ");

        setWeightValue(payloadV);
        Serial.println(getWeightValue());
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
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        // Attempt to connect
        if (client.connect(clientId.c_str(), "bbniqtdq", "D87AAz6nsCdN"))
        {
            Serial.println("connected");

            // Once connected, publish an announcement...

            // ... and resubscribe
            client.subscribe(ordersTopic);
            calibTopic = String("pickcounter/calib/") + DevID;
            Serial.print("Calibration Topic: ");
            Serial.println(calibTopic);
            client.subscribe(calibTopic.c_str());
            client.subscribe(weightsTopic);
            client.subscribe(temp);
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