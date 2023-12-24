#include <Arduino.h>

#include "./control/control.h"

#include "./fled/fled.h"
#include <Adafruit_NeoPixel.h>
#define LED_PIN0 PA6
#define LED_NUM 300
Adafruit_NeoPixel neopixel0 = Adafruit_NeoPixel(LED_NUM, LED_PIN0, NEO_GRB + NEO_KHZ800);
FLED led0(&neopixel0, LED_NUM);

#include <HardwareSerial.h>
HardwareSerial PC(PA10, PA9); //UART1 RX, TX
//HardwareSerial ARM(PC7, PC6); //UART6 RX, TX
HardwareSerial POLE(PD2, PC12)

void setup() {
	PC.begin(115200);
	PC.println("start");

	POLE.begin(115200);
	
	led0.init();
	led0.set_color_rgb_all(50, 0, 0);
	led0.show();

	//ーーーこれは必須ーーーーー
	control_init();
	//ーーーーーまじでーーーーー
}

unsigned long loop_timer = 10000;

void loop() {
	//ーーーこれは必須ーーーーー
	control_loop();
	//ーーーーーまじでーーーーー

	//ーーーーーーーーーーループ計測ーーーーーーーーーー
	// PC.print(micros() - loop_timer);
	loop_timer = micros();

	//ーー無線の内容が見れます(別に消して大丈夫)ーー
	if(twelite.read()){ //tweliteから受信成功したら1を返す
		PC.print(micros() - loop_timer);
		for(int i=0; i<4; i++){
			PC.print("  :");
			PC.print(twelite.receive_data[i]);
		}
		PC.println();
	}
	
	//ーーーーーーーーーーLED(自由に光らせてね)ーーーーーーーーーーー
	led0.clear();
	led0.set_color_hsv_all((millis()%2550)/10 , 150, 10);
	led0.show();
}
