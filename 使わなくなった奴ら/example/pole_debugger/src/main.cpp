#include <Arduino.h>

#include "./oled/oled.h"
#include "SSD1306.h"//ディスプレイ用ライブラリを読み込み

#define SDA_PIN 21
#define SCL_PIN 22
SSD1306 ssd1306(0x3c, SDA_PIN, SCL_PIN); //SSD1306インスタンスの作成（I2Cアドレス,SDA,SCL）
OLED oled(&ssd1306);

#include "./button/button.h"
BUTTON button(34, 5, 15);

void setup(){
	oled.init(); //version表示
	oled.display_version(1);
	oled.show();

	button.init();

	Serial.begin(115200);
	delay(1000);
}

int i = 0;
bool val[3] = {0, 0, 0};
int send_value = 0;
int receive_value = 0;
	
void loop(){
	button.read(val);
	// printf("%d %d %d\n", val[0], val[1], val[2]);
	
	send_value = val[0]*30+val[1]*40+val[2]*50;
	Serial.print(send_value);
	
	if(Serial.available()){
		receive_value = Serial.read();
	}else{
		receive_value = 0;
	}

	oled.clear();
	oled.half_display_num("S = ", send_value, "R = ", receive_value);
	oled.half_display_3button(val[0], val[1], val[2]);
	oled.show();
}

