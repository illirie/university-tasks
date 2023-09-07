import keyboard
from time import sleep

def action(key: str):
    keyboard.press(key)
    sleep(1)
    keyboard.release(key)