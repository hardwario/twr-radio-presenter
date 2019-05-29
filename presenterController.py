import time
import paho.mqtt.client as paho
from pynput.keyboard import Key, Controller
broker="127.0.0.1"
keyboard = Controller()
#define callback
def on_message(client, userdata, message):
<<<<<<< HEAD
    message = str(message.payload.decode("utf-8"))
=======
    time.sleep(1)
    message = str(message.payload.decode("utf-8")
>>>>>>> f8828b7b69d790ef7e7e063a4ea472a8e246342e
    if(message == "next"):
        print("next slide")
        keyboard.press(Key.right)
        keyboard.release(Key.right)
<<<<<<< HEAD
        
=======
>>>>>>> f8828b7b69d790ef7e7e063a4ea472a8e246342e
    elif(message == "prev"):
        print("prev slide")
        keyboard.press(Key.left)
        keyboard.release(Key.left)
<<<<<<< HEAD
        
=======
>>>>>>> f8828b7b69d790ef7e7e063a4ea472a8e246342e
    elif(message == "start"):
        print("start presentation")
        keyboard.press(Key.f5)
        keyboard.release(Key.f5)
<<<<<<< HEAD
        
=======
>>>>>>> f8828b7b69d790ef7e7e063a4ea472a8e246342e
    elif(message == "tab"):
        print("tab button")
        keyboard.press(Key.alt)
        keyboard.press(Key.tab)
        keyboard.release(Key.tab)
<<<<<<< HEAD
        
=======
>>>>>>> f8828b7b69d790ef7e7e063a4ea472a8e246342e
    elif(message == "enter"):
        print("enter button")
        keyboard.release(Key.alt)
        keyboard.press(Key.enter)
        keyboard.release(Key.enter)
<<<<<<< HEAD
        
=======
>>>>>>> f8828b7b69d790ef7e7e063a4ea472a8e246342e
    elif(message == "esc"):
        print("esc button")
        keyboard.release(Key.alt)
        keyboard.press(Key.esc)
        keyboard.release(Key.esc)
<<<<<<< HEAD
        
=======
>>>>>>> f8828b7b69d790ef7e7e063a4ea472a8e246342e
    elif(message == "end"):
        print("end button")
        keyboard.press(Key.alt)
        keyboard.press(Key.f4)
        keyboard.release(Key.f4)
        keyboard.release(Key.alt)
<<<<<<< HEAD
        
=======
>>>>>>> f8828b7b69d790ef7e7e063a4ea472a8e246342e
    elif(message == "space"):
        print("space button")
        keyboard.release(Key.alt)
        keyboard.press(Key.space)
        keyboard.release(Key.space)
        
client= paho.Client("presenter-1")
client.on_message=on_message

print("connecting to broker ",broker)
client.connect(broker)#connect

client.loop_start() #start loop to process received messages
print("subscribing ")
client.subscribe("presenter/action")#subscribe
while 1:
<<<<<<< HEAD
    time.sleep(0.2)
=======
    time.sleep(1)
>>>>>>> f8828b7b69d790ef7e7e063a4ea472a8e246342e
