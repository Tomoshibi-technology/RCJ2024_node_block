import serial
import time
import serial.tools.list_ports

from playsound import playsound


# playsound("World2.mp3")

print('Hello, World!')

def auto_detect_serial_port():
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        print(p)
        if 'USB-SERIAL' in p.description:
            print(p.device)
            return p.device
    return None

ser = serial.Serial(auto_detect_serial_port(), 115200, timeout=0.1)

time.sleep(5)

while True:
	ser.write(b"1")
	time.sleep(.05)
	ser.write(b"0")
	time.sleep(.05)
	print("aaaaaaaaaaaa")
     
    fdsa









