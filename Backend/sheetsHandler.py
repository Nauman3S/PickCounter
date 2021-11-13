import gspread
import random
import time
import json
import base64
from oauth2client.service_account import ServiceAccountCredentials
from datetime import datetime
import paho.mqtt.client as mqtt

msgV = ""
topicV = ""
sendNextOrder=1
ordersIndex=1
deleteCompleted=0

clientID_prefix = ""
for i in range(0, 6):
    clientID_prefix = clientID_prefix + str(random.randint(0, 99999))


mqtt_client = mqtt.Client("SADC0"+clientID_prefix)


def getCalibrationData():
    global mqtt_client
    try:
        # use creds to create a client to interact with the Google Drive API
        scope = ['https://www.googleapis.com/auth/spreadsheets',
                 'https://www.googleapis.com/auth/drive']
        #scope = ['https://spreadsheets.google.com/feeds']
        #scope = ['https://www.googleapis.com/auth/spreadsheets']
        creds = ServiceAccountCredentials.from_json_keyfile_name(
            'client_secret.json', scope)
        client = gspread.authorize(creds)

        # Find a workbook by name and open the first sheet
        # Make sure you use the right name here.
        sheet = client.open("Orders_Calibrations").sheet1

        # Extract and print all of the values
        list_of_hashes = sheet.get_all_records()
        print(list_of_hashes)

        dateTimeObj = datetime.now()
        timestampStr = dateTimeObj.strftime("%d-%b-%Y (%H:%M:%S.%f)")

        g = sheet.get_all_values()
        print(len((g)))
        print(g[1][0])  # esp1
        for i in range(1, len(g)-1):
            mqtt_client.publish('pickcounter/calib/'+g[i][0], g[i][1])
    except Exception as e:

        print('something bad happened: ', e)


def getProductWeights():
    global mqtt_client
    try:
        # use creds to create a client to interact with the Google Drive API
        scope = ['https://www.googleapis.com/auth/spreadsheets',
                 'https://www.googleapis.com/auth/drive']
        #scope = ['https://spreadsheets.google.com/feeds']
        #scope = ['https://www.googleapis.com/auth/spreadsheets']
        creds = ServiceAccountCredentials.from_json_keyfile_name(
            'client_secret.json', scope)
        client = gspread.authorize(creds)

        # Find a workbook by name and open the first sheet
        # Make sure you use the right name here.
        sheet = client.open("Orders_ProductWeights").sheet1

        # Extract and print all of the values
        list_of_hashes = sheet.get_all_records()
        print(list_of_hashes)

        dateTimeObj = datetime.now()
        timestampStr = dateTimeObj.strftime("%d-%b-%Y (%H:%M:%S.%f)")

        g = sheet.get_all_values()
        print(len((g)))
        print(g[1][0])  # esp1
        
        weightsData=''
        for i in range(1, len(g)-1):
            weightsData=weightsData+g[i][0]+','+g[i][1]+';'
        weightsData=weightsData[:-1]
        mqtt_client.publish('pickcounter/weights', weightsData)

    except Exception as e:

        print('something bad happened: ', e)


def getOrders():
    global mqtt_client, ordersIndex, sendNextOrder, deleteCompleted
    try:
        # use creds to create a client to interact with the Google Drive API
        scope = ['https://www.googleapis.com/auth/spreadsheets',
                 'https://www.googleapis.com/auth/drive']
        #scope = ['https://spreadsheets.google.com/feeds']
        #scope = ['https://www.googleapis.com/auth/spreadsheets']
        creds = ServiceAccountCredentials.from_json_keyfile_name(
            'client_secret.json', scope)
        client = gspread.authorize(creds)

        # Find a workbook by name and open the first sheet
        # Make sure you use the right name here.
        sheet = client.open("Orders_OrdList").sheet1

        # Extract and print all of the values
        list_of_hashes = sheet.get_all_records()
        print(list_of_hashes)

        dateTimeObj = datetime.now()
        timestampStr = dateTimeObj.strftime("%d-%b-%Y (%H:%M:%S.%f)")

        g = sheet.get_all_values()
        print(len((g)))
        print(g[1][0])  # esp1
        print(g[1][1])  # esp1
        print(g[1][2])  # esp1
        if(sendNextOrder==1):
            if(ordersIndex>len(g)):
                ordersIndex=0
            if(len(g)>1):
                orderStr=g[ordersIndex][0]+','+g[ordersIndex][1]+','+g[ordersIndex][2]
                mqtt_client.publish('pickcounter/orders',orderStr)
                sendNextOrder=0
                ordersIndex=ordersIndex+1
            if(deleteCompleted==1):
                if(len(g)>=1):
                    sheet.delete_rows(ordersIndex-1)
                deleteCompleted=0
        

    except Exception as e:

        print('something bad happened: ', e)


def on_connect(client, userdata, rc):
    print("Connected with result code "+str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("pickcounter/orderstatus")


# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):

    global msgV, topicV, sendNextOrder, deleteCompleted
    print(msg.topic+" "+str(msg.payload))
    topicV = str(msg.topic)
    msgV = str((msg.payload).decode('utf-8'))
    if(topicV == 'pickcounter/orderstatus'):
        print('Order Status Response from ESP32', msgV)
        if('Completed' in msgV):
            sendNextOrder=1
            deleteCompleted=1


mqtt_client.on_message = on_message
#client.on_connect = on_connect
mqtt_client.username_pw_set('bbniqtdq', 'D87AAz6nsCdN')
mqtt_client.connect("smart-pilot.cloudmqtt.com", 1883, 60)
mqtt_client.subscribe("pickcounter/orderstatus")


mqtt_client.loop_start()
# updateSheet()
devID = ""

while 1:
    getCalibrationData()
    getProductWeights()
    getOrders()
    # devID=input("Enter Device ID: ")
    # ledColor=input("Enter LED Color to send: ")
    # valueV=input("Enter Value to send: ")
    # client.publish('pickcounter/orders',str(ledColor)+','+str(valueV)+','+str(devID))
    # print("Order Sent!")
    
    time.sleep(3)
