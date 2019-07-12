
import serial

try:
    ser = serial.Serial("COM3",9600)
except:
    pass

def read_serial():
    return ser.readline()

n=0
max_n = 10;
while n<max_n:
    print(ser.readline().decode("utf-8"));
    n+=1
    
ser.close()
