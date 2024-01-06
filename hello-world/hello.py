import serial
import time
import serial.tools.list_ports
import pygame

# from playsound import playsound


# playsound("World2.mp3")

print('Hello, World!')

def alarm():
    pygame.mixer.init(frequency = 44100)    # 初期設定
    pygame.mixer.music.load("World2.mp3")     # 音楽ファイルの読み込み
    pygame.mixer.music.play(1)              # 音楽の再生回数(1回)
    # while(1):
    #     a = input("Finish? --->")
    #     if(a is 'y'): break
    time.sleep(3)
    pygame.mixer.music.stop()               # 再生の終了
    return 0



def auto_detect_serial_port():
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        print(p)
        if 'USB-SERIAL' in p.description:
            print(p.device)
            return p.device
    return None

alarm()

auto_detect_serial_port()
# ser = serial.Serial(auto_detect_serial_port(), 115200, timeout=0.1)
ser = serial.Serial("/dev/ttyUSB0", 115200, timeout=0.1)

time.sleep(5)

while True:
	ser.write(b"1")
	time.sleep(.05)
	ser.write(b"0")
	time.sleep(.05)
	print("aaaaaaaaaaaa")









