import time
import paho.mqtt.client as paho
from pynput.keyboard import Key, Controller
broker="127.0.0.1"
keyboard = Controller()
#define callback
def on_message(client, userdata, message):
    time.sleep(1)
    if(str(message.payload.decode("utf-8")) == "next"):
        print("next slide")
        keyboard.press(Key.right)
        keyboard.release(Key.right)
    elif(str(message.payload.decode("utf-8")) == "prev"):
        print("prev slide")
        keyboard.press(Key.left)
        keyboard.release(Key.left)
    elif(str(message.payload.decode("utf-8")) == "start"):
        print("start presentation")
        keyboard.press(Key.f5)
        keyboard.release(Key.f5)
    elif(str(message.payload.decode("utf-8")) == "tab"):
        print("tab button")
        keyboard.press(Key.alt)
        keyboard.press(Key.tab)
        keyboard.release(Key.tab)
    elif(str(message.payload.decode("utf-8")) == "enter"):
        print("enter button")
        keyboard.press(Key.enter)
        keyboard.release(Key.enter)
    elif(str(message.payload.decode("utf-8")) == "esc"):
        print("esc button")
        keyboard.press(Key.esc)
        keyboard.release(Key.esc)
        
client= paho.Client("client-001")
client.on_message=on_message

print("connecting to broker ",broker)
client.connect(broker)#connect

client.loop_start() #start loop to process received messages
print("subscribing ")
client.subscribe("presenter/action")#subscribe
time.sleep(1)

