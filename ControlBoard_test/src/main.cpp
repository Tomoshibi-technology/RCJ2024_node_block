#include <Arduino.h>

#define VERSION 0.25

#include "./speaker/speaker.h"
#define SPEAKER_PIN PB10
SPEAKER speaker(SPEAKER_PIN);

#include "./oled/oled.h"
#include <U8g2lib.h>
#define SDA_PIN PB9
#define SCL_PIN PB8
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,  SCL_PIN, SDA_PIN, U8X8_PIN_NONE);
OLED oled(&u8g2);

#include "./power/power.h"
#define VOL_PIN PB1
#define VOL_MAX 12.6
#define VOL_MIN 11.1
POWER power(VOL_PIN, VOL_MAX, VOL_MIN);

#include "./dip/dip.h"
#define DIP_PIN0 PA8
#define DIP_PIN1 PA11
#define DIP_PIN2 PA12
#define DIP_PIN3 PA15
DIP dip(DIP_PIN0, DIP_PIN1, DIP_PIN2, DIP_PIN3);

#include "./button/button.h"
#define BUTTON_PIN0 PB5
#define BUTTON_PIN1 PB6
#define BUTTON_PIN2 PB7
BUTTON button(BUTTON_PIN0, BUTTON_PIN1, BUTTON_PIN2);

const String name[16] = {
	"Center",
	"Display",
	"ArmA",
	"ArmB",
	"LEDPole",
	"Robot5",
	"Robot6",
	"Robot7",
	"Robot8",
	"Robot9",
	"Robot10",
	"Robot11",
	"Robot12",
	"Robot13",
	"Robot14",
	"Robot15"
};

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




void setup() {
	oled.init();
	delay(500);
	oled.display_version(VERSION);
	oled.show();

	power.init();
	
	dip.init();

	speaker.init();
	speaker.boot_music();

	button.init();

	led0.init();
	led0.set_color_rgb_all(50, 0, 0);
	led0.show();

	delay(1000);

	PC.begin(115200);
	TWE.begin(115200);
	PC.println("start");
}

unsigned long loop_timer = 10000;
byte receive_data[4] = {0, 0, 0, 0};

void loop() {
	//ーーーーーーーーーーループ計測ーーーーーーーーーー
	// PC.println(millis() - loop_timer);
	// loop_timer = millis();
	
	// PC.print(micros() - loop_timer);
	loop_timer = micros();

	//ーーーーーーーーーーボタンーーーーーーーーーーー
	bool btn_val[3] = {0, 0, 0};
	button.read(btn_val);

	//ーーーーーーーーーー無線ーーーーーーーーーー
	byte send_data = btn_val[0]*10 + btn_val[1] * 20 + btn_val[2] * 40;
	
	delay(3);
	if(TWE.available()>10){
		byte data = TWE.read();
		if(data == 250){
			byte raw_receive_data[4] = {0, 0, 0, 0};
			bool receive_bad_flg = 0; //受信失敗フラグ
			for(int i=0; i<4; i++){
				if(TWE.available()>0){
					raw_receive_data[i] = TWE.read();
					if(raw_receive_data[i] == 250){receive_bad_flg = 1; break;} //受信失敗フラグを立てる(250が来たら
				}else{
					receive_bad_flg = 1; break;
				}
			}
			if(!receive_bad_flg){
				for(int i=0; i<4; i++){
					receive_data[i] = raw_receive_data[i];
				}
				//せっかく更新したので、中身を表示
				PC.print(micros() - loop_timer);
				for(int i=0; i<4; i++){
					PC.print("  :");
					PC.print(receive_data[i]);
				}
				PC.println();
			}
		}
	}

	// if(TWE.available()>0){
	// 	while(true){
	// 		if(TWE.available()>0){
	// 			int data = TWE.read();
	// 			if(data==250){break;}
	// 		}
	// 	}
	// 	int k = 0;
	// 	byte raw_receive_data[4] = {0, 0, 0, 0};
	// 	bool receive_bad_flg = 0; //受信失敗フラグ
	// 	while(k<4){
	// 		if(TWE.available()>0){
	// 			raw_receive_data[k]=TWE.read();
	// 			k++;
	// 			if(raw_receive_data[k] == 250){receive_bad_flg = 1; break;} //受信失敗フラグを立てる(250が来たら
	// 		}
	// 	}
	// 	if(!receive_bad_flg){
	// 		for(int i=0; i<4; i++){
	// 			receive_data[i] = raw_receive_data[i];
	// 		}
	// 	}
	// }

	// if(receive_data[0] != 0){
	// 	PC.print(micros() - loop_timer);
	// 	for(int i=0; i<4; i++){
	// 		PC.print("  :");
	// 		PC.print(receive_data[i]);
	// 	}
	// 	PC.println();
	// }
	
	

	// //ーーーーーーーーーー効果音と光ーーーーーーーーーー
	led0.clear();


	bool led_flg = 0;
	if(btn_val[0] || receive_data[0] == 10){ 
		speaker.ring(C6);
		led0.set_color_hsv_all(70, 250, 20);
		led_flg = 1;
	}
	if(btn_val[1] || receive_data[0] == 20) {
		speaker.ring(E6);
		led0.set_color_hsv_all(150, 250, 20);
		led_flg = 1;
	}
	if(btn_val[2] || receive_data[0] == 40){
		speaker.ring(G6);
		led0.set_color_hsv_all(230, 250, 20);
		led_flg = 1;
	}
	if(!led_flg){
	 speaker.mute();
	 led0.set_color_hsv_all((millis()%2550)/10 , 150, 10);
	}

	led0.show();

	// //ーーーーーーーーーー表示ーーーーーーーーーー
	oled.clear();
	oled.display_title(name[dip.read_ID()]+" V" + String(VERSION));
	oled.display_battary(power.voltage(), power.percentage());
	oled.half_display_num(
		"S = "+String(send_data),
		"R = "+String(receive_data[0])
	);
	oled.half_display_3button(btn_val);
	oled.show();

	// // delay(10);



}
