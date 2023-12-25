#include <Arduino.h>

#include "./fled/fled.h"
#include <Adafruit_NeoPixel.h>
#define LED_A_PIN 32  // The data pin that the WS2812 strips are connected to.32
#define LED_B_PIN 14
#define LED_C_PIN 13
#define LED_D_PIN 33
#define LED_E_PIN 27
#define LED_NUM 60


Adafruit_NeoPixel rawpixel[5] = {
	Adafruit_NeoPixel(LED_NUM, LED_A_PIN, NEO_GRB + NEO_KHZ800),
	Adafruit_NeoPixel(LED_NUM*2, LED_B_PIN, NEO_GRB + NEO_KHZ800),
	Adafruit_NeoPixel(LED_NUM, LED_C_PIN, NEO_GRB + NEO_KHZ800),
	Adafruit_NeoPixel(LED_NUM*2, LED_D_PIN, NEO_GRB + NEO_KHZ800),
	Adafruit_NeoPixel(40, LED_E_PIN, NEO_GRB + NEO_KHZ800),
};

FLED led[6] = {
	FLED(&rawpixel[0], LED_NUM, 0, 59),
	FLED(&rawpixel[1], LED_NUM, 119, 60),
	FLED(&rawpixel[1], LED_NUM, 0, 59),
	FLED(&rawpixel[2], LED_NUM, 0, 59),
	FLED(&rawpixel[3], LED_NUM, 0, 59),
	FLED(&rawpixel[3], LED_NUM, 119, 60)
};


FLED circuit_led = FLED(&rawpixel[4], 40, 0, 5);


#include "./ser_ctrl/ser_ctrl.h"

HardwareSerial ctrl(2);
CTRL ser_ctrl(&ctrl);  

void setup() {
	for(int i=0; i<5; i++){
		rawpixel[i].begin();
		rawpixel[i].show();
	}

	for(int i=0; i<6; i++){
		led[i].init();
	}
	ser_ctrl.init();
	Serial.begin(115200);

	circuit_led.init();

}


uint32_t j = 0;
void loop(){
	circuit_led.clear();
	circuit_led.set_color_rgb(0 ,30, 0, 0);
	circuit_led.set_color_rgb(4 ,0, 15, 0);
	circuit_led.set_color_rgb(5 ,0, 15, 0);
	circuit_led.set_color_rgb(9 ,0, 0, 30);
	circuit_led.show();

	// for(int i=0; i<6; i++){
	// 	led[i].clear();
	// }
	// ser_ctrl.read();

	// byte kidou = ser_ctrl.data[0]; //10 起動 2 停止
	// byte phase = ser_ctrl.data[1] - 5; //1-4

	// for(int i=0; i<6; i++){
	// 	led[i].set_color_hsv_all(phase*60, 230, 40);
	// }

	// if(phase == 3){
	// 	for(int i=0; i<6; i++){
	// 		led[i].set_color_hsv(j	, 0, 150, 100);
	// 		led[i].set_color_hsv(j-1, 0, 100, 100);
	// 		led[i].set_color_hsv(j-2, 0, 70, 100);
	// 		led[i].set_color_hsv(j-3, 0, 40, 100);
	// 	}
	// }
	// j++;

	// for (int i=0; i<6; i++){
	// 	led[i].show();
	// }
	// delay(50);
}




