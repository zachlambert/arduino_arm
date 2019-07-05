
import serial
import numpy as np

ser = serial.Serial("COM3",9600)

def read_serial():
    return ser.readline()

def send_serial(text):
    ser.write(bytes(text)) #Need to check if this works

latest = np.zeros(10)

n=0
max_n = 10
while n<max_n:
    print(read_serial())
    n+=1
        
ser.close()
