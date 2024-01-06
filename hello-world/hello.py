import serial
import time
import serial.tools.list_ports
import pygame

# from playsound import playsound


# playsound("World2.mp3")

print('Hello, World!')

pygame.mixer.init(frequency = 44100)    # 初期設定
pygame.mixer.music.load("World2.mp3")     # 音楽ファイルの読み込み


def auto_detect_serial_port():
    ports = list(serial.tools.list_ports.comports())
    for p in ports:
        print(p)
        if 'USB-SERIAL' in p.description:
            print(p.device)
            return p.device
    return None

# alarm()

auto_detect_serial_port()
# ser = serial.Serial(auto_detect_serial_port(), 115200, timeout=0.1)
ser = serial.Serial("/dev/ttyUSB0", 115200, timeout=0.0001)

ser.close()
ser.open()
ser.reset_input_buffer()

time.sleep(3)
print("dd")

n = 0
music_flg = 0
while True:
	ser.write(b"1")
	# receive = ser.readline()
	time.sleep(.05)

	ser.write(b"0")
	receive = ser.readline()
	time.sleep(.05)

	receive = ser.readline()
	
	receive = receive.strip()
	receive = receive.decode("utf-8")
	print(n)
	print(receive)
	n += 1
	if receive == 'D' and music_flg == 0:
		music_flg = 1
		n = 0
		pygame.mixer.music.play(1)              # 音楽の再生回数(1回)
	if receive == 'E':
		pygame.mixer.music.stop()               # 再生の終了
		music_flg = 0
		n = 0









