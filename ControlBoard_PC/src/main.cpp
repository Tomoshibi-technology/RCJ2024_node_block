#include <Arduino.h>


byte control_send_data[4] = {0, 0, 0, 0};

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

HardwareSerial TWE(PA1, PA0); //UART2 RX, TX

#include "./twelite/twelite.h"
TWELITE twelite(&TWE);


void setup() {
	PC.begin(115200);
	PC.println("start");
	
	led0.init();
	led0.set_color_rgb_all(50, 0, 0);
	led0.show();

	//ーーーこれは必須ーーーーー
	control_init();
	//ーーーーーまじでーーーーー
	twelite.init();
}

unsigned long loop_timer = 10000;


void loop() {
	//ーーーこれは必須ーーーーー
	control_loop();
	//ーーーーーまじでーーーーー

	//ーーーーーーーーーーループ計測ーーーーーーーーーー
	// PC.print(micros() - loop_timer);
	loop_timer = micros();


	control_send_data[0] = 100;
	control_send_data[1] = 150;
	control_send_data[2] = 200;
	control_send_data[3] = 250;
	twelite.send(control_send_data);	
	
	// PC.print(micros() - loop_timer);
	// 	for(int i=0; i<4; i++){
	// 		PC.print("  :");
	// 		PC.print(twelite.receive_data[i]);
	// 	}
	// PC.println();
	
	
	//ーーーーーーーーーーLED(自由に光らせてね)ーーーーーーーーーーー
	led0.clear();
	led0.set_color_hsv_all((millis()%2550)/10 , 150, 10);
	led0.show();
}
