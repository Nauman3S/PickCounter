
import random
import json
import base64
import time
from datetime import datetime
import paho.mqtt.client as mqtt



msgV=""
topicV=""

# def postReq(server,header,jsonV,userPass):
#     final_dictionary = json.loads(jsonV)
#     print(final_dictionary)
#     h=header.split(':')
#     h1=h[0]
#     h2=h[1].strip()
#     #['http://44.195.192.158:3000/v1/jobOperations', 'Content-Type: application/x-www-form-urlencoded', '{\n"Tracker_ID__c":"240AC4AFD0D0C0",\n"Tracker_DateTime__c":"2021-8-30T0:41:5.531"\n}']
#     req = request.Request(server, method="POST")
#     req.add_header(h1, h2)
#     data = final_dictionary
#     data = json.dumps(jsonV)
#     data = data.encode()
#     userPassV=userPass.split(',')
#     print(userPassV)
#     try:
#         base64string = base64.b64encode((userPassV[0] + ":" + userPassV[1]).encode("ascii"))
#         request.add_header("Authorization", "Basic {}".format(base64string.decode("ascii")))
#         r = request.urlopen(req, data=data)
#         content = r.read()
#         decodedC=content.decode()
#         print(decodedC)
#         jj=json.loads(decodedC)
#         print(jj['status'])
#         return jj['status']
#     except Exception as e:
#         return 400
def on_connect(client, userdata, rc):
    #print("Connected with result code "+str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("pickcounter/orderstatus")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    global dataFreq
    global msgV,topicV
    #print(msg.topic+" "+str(msg.payload))
    topicV=str(msg.topic)
    msgV=str((msg.payload).decode('utf-8'))

    if(topicV=='pickcounter/orderstatus'):
        print('Order Status Response from ESP32',msgV)
        
    
clientID_prefix=""
for i in range(0,6):
    clientID_prefix=clientID_prefix + str(random.randint(0,99999))


client = mqtt.Client("C1"+clientID_prefix)
#client.on_connect = on_connect
client.on_message = on_message
client.username_pw_set('bbniqtdq','D87AAz6nsCdN')
client.connect("smart-pilot.cloudmqtt.com", 1883, 60)
client.subscribe("pickcounter/orderstatus")


client.loop_start()
devID=""
while 1:
    devID=input("Enter Device ID: ")
    ledColor=input("Enter LED Color to send: ")
    valueV=input("Enter Value to send: ")
    client.publish('pickcounter/orders',str(ledColor)+','+str(valueV)+','+str(devID))
    print("Order Sent!")
    time.sleep(1)
    
