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
HardwareSerial POLE(PD2, PC12);

#define BCD(c) 5 * (5 * (5 * (5 * (5 * (5 * (5 * (c & 128) + (c & 64)) + (c & 32)) + (c & 16)) + (c & 8)) + (c & 4)) + (c & 2)) + (c & 1)


void setup() {
	PC.begin(115200);
	PC.println("start");

	POLE.begin(115200);
	
	// led0.init();
	// led0.set_color_rgb_all(50, 0, 0);
	// led0.show();

	//ーーーこれは必須ーーーーー
	control_init();
	//ーーーーーまじでーーーーー
}

unsigned long loop_timer = 10000;
unsigned long sec_timer = 0;

uint32_t mycount = 0;

uint8_t kaeru[9] = {44, 46, 48, 49, 48, 46, 44, 200, 200};
uint8_t doremi[9] = {44, 46, 48, 49, 51, 53, 55, 200, 200};

void loop() {
	//ーーーこれは必須ーーーーー
	// control_loop();
	//ーーーーーまじでーーーーー


	//ーーーーーーーーーーループ計測ーーーーーーーーーー
	// PC.print(micros() - loop_timer);
	loop_timer = micros();
	if(sec_timer+500 < millis()){
		sec_timer = millis();
		mycount++;
		// PC.printf("count: %08d\n", mycount);
	}

	uint8_t send_data[12] = {0,0, ((mycount/1000)+55)%240,200,50,30,200,60,50,30,20,1};

	//送信相手選択 bit目がid番号
	send_data[0] = 0b00000111; //0-7 
	send_data[1] = 0b00000000; //8-15

	//モード選択
	send_data[2] = 0x1; //メロディ ドミソ
	//未定義の何か
	send_data[2] |= 0xA << 4;

	//音を鳴らす
	send_data[3] = kaeru[mycount%9]; //音の高さ
	// send_data[3] = doremi[mycount%9]; //音の高さ

	// if(mycount%9 == 0){
	// 	send_data[3] = 44; //音の高さ
	// }else{
	// 	send_data[3] = 200; //音の高さ
	// }

	
	// if(twelite.receive_data[2]==200 ){ //スタートスイッチの有無
	// 	send_data[0] = 10;
	// }else{
	// 	send_data[0] = 2;
	// }

	// send_data[1] = twelite.receive_data[3]; //フェーズ 6-9

	//有線送信
	POLE.write(250);
	for(int i=0; i<12; i++){
		POLE.write(send_data[i]);
		// PC.print(send_data[i]);
	}
	

	//ーー無線の内容が見れます(別に消して大丈夫)ーー
	// if(twelite.read()){ //tweliteから受信成功したら1を返す
	// 	PC.print(micros() - loop_timer);
	// 	for(int i=0; i<4; i++){
	// 		PC.print("  :");
	// 		PC.print(twelite.receive_data[i]);
	// 	}
	// 	PC.println();
	// }
	
	//ーーーーーーーーーーLED(自由に光らせてね)ーーーーーーーーーーー
	// led0.clear();
	// led0.set_color_hsv_all((millis()%2550)/10 , 150, 10);
	// led0.show();
}
