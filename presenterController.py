import time
import paho.mqtt.client as paho
from pynput.keyboard import Key, Controller
#Here goes IP address of your mqtt broker
#If you are running it on this computer leave it as is
broker="127.0.0.1"
keyboard = Controller()
#define callback
def on_message(client, userdata, message):
    message = str(message.payload.decode("utf-8"))
    if(message == "next"):
        print("next slide")
        keyboard.press(Key.right)
        keyboard.release(Key.right)
        
    elif(message == "prev"):
        print("prev slide")
        keyboard.press(Key.left)
        keyboard.release(Key.left)
        
    elif(message == "start"):
        print("start presentation")
        keyboard.press(Key.f5)
        keyboard.release(Key.f5)
        
    elif(message == "tab"):
        print("tab button")
        keyboard.press(Key.alt)
        keyboard.press(Key.tab)
        keyboard.release(Key.tab)
        keyboard.release(Key.alt)
        
    elif(message == "enter"):
        print("enter button")
        keyboard.press(Key.enter)
        keyboard.release(Key.enter)
        
    elif(message == "esc"):
        print("esc button")
        keyboard.press(Key.esc)
        keyboard.release(Key.esc)
        
    elif(message == "end"):
        print("end button")
        keyboard.press(Key.alt)
        keyboard.press(Key.f4)
        keyboard.release(Key.f4)
        keyboard.release(Key.alt)
        
    elif(message == "space"):
        print("space button")
        keyboard.press(Key.space)
        keyboard.release(Key.space)
        
    elif(message == "tabNext"):
        print("next tab button")
        keyboard.press(Key.tab)
        keyboard.release(Key.tab)

    elif(message == "tabPrev"):
        print("prev tab button")
        keyboard.press(Key.shift)
        keyboard.press(Key.tab)
        keyboard.release(Key.tab)
        keyboard.release(Key.shift)
        
client= paho.Client("presenter-1")
client.on_message=on_message

print("connecting to broker ",broker)
client.connect(broker)#connect

client.loop_start() #start loop to process received messages
print("subscribing ")
client.subscribe("presenter/action")#subscribe
while 1:
    time.sleep(0.2)
