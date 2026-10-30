import random
import paho.mqtt.client as mqtt

# MQTT Broker
broker = 'mq.nl.eu.org'

# Port
port = 8883

# Topic
topic = "stop/1"

# Client ID
client_id = f'python-mqtt-{random.randint(0, 1000)}'

# Username and Password
username = 'connectedsystems'
password = 'tincos'

def on_connect(client, userdata, flags, rc):
    if rc==0:
        print("connected OK")   
    else:
        print("Bad connection Returned code=",rc)

client = mqtt.Client(client_id)
client.username_pw_set(username, password)
client.on_connect = on_connect

print("Connecting to broker ",broker)
client.connect(broker, port)

# subscribe to the topic
client.subscribe("move/1")

# print the message when received
def on_message(client, userdata, msg):
    print(msg.payload.decode("utf-8"))  # Decode the bytes to a string

# client.on_message = on_message

# publish the message
client.publish(topic, f"stop")
client.publish(f"stop/2", f"stop")
client.publish(f"stop/3", f"stop")
client.publish(f"stop/4", f"stop")

client.loop_forever()