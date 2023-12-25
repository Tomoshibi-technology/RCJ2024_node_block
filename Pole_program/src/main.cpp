#include <Arduino.h>
#include <Wire.h>
#include <FastLED.h>
#include "SSD1306.h"//ディスプレイ用ライブラリを読み込み
#include "music_scale.h"

// #include <WiFi.h>
// #include <WiFiUdp.h>
// #include <ArtnetWifi.h>

// const char* ssid = "TomoshibiTechnology_IoT";
// const char* password = "All_outlook";

// const char* ssid = "OPPO Reno5 A (eSIM)";
// const char* password = "w3cc33mb";

// const char* ssid = "TP-Link_0D38_2.4G";
// const char* password = "04701842";


HardwareSerial CTRL(2);

#define SDA_PIN 21
#define SCL_PIN 22
SSD1306  display(0x3c, SDA_PIN, SCL_PIN); //SSD1306インスタンスの作成（I2Cアドレス,SDA,SCL）

#define BUZZER_PIN 4              // ブザーのピン番号
#define BUZZER_CHANEL 13           // ブザーのチャネル番号

#define ID_PIN_A 25
#define ID_PIN_B 18
#define ID_PIN_C 19
#define ID_PIN_D 23
void id_begin();
int id_read();

// LED Strip
const int numLeds = 120; 
const int numleds_of_around = 40; // Change if your setup has more or less LED's
#define LED_A_PIN 32  // The data pin that the WS2812 strips are connected to.32
#define LED_B_PIN 14
#define LED_C_PIN 13
#define LED_D_PIN 33
#define LED_E_PIN 27

// NefryBT D4=GPIO18
CRGB led_A[numLeds];
CRGB led_B[numLeds];
CRGB led_C[numLeds];
CRGB led_D[numLeds];
CRGB led_E[numleds_of_around];

// Artnet settings
// ArtnetWifi artnet;
// const int startUniverse = 2;

//bool sendFrame = 1;
int previousDataLength = 0;

int pre_beat;
int pre_beat_od_around;

// boolean ConnectWifi(void);
// void onDmxFrame(uint16_t universe, uint16_t length, uint8_t sequence, uint8_t* data);

void leds_set_rgb(int r, int g, int b);
void leds_set_hsv(int h, int s, int v);
void led_delay(int colorvalue, int delayvalue);

void receive_show(void);

int fog_random[3];
int star_count = 0;



void setup() {
	Serial.begin(115200);
	CTRL.begin(115200);
	
  FastLED.addLeds<WS2812, LED_A_PIN, GRB>(led_A, numLeds);
  FastLED.addLeds<WS2812, LED_B_PIN, GRB>(led_B, numLeds);
  FastLED.addLeds<WS2812, LED_C_PIN, GRB>(led_C, numLeds);
  FastLED.addLeds<WS2812, LED_D_PIN, GRB>(led_D, numLeds);
  FastLED.addLeds<WS2812, LED_E_PIN, GRB>(led_E, numleds_of_around);

  ledcSetup(BUZZER_CHANEL, 12000, 8);
	ledcAttachPin(BUZZER_PIN, BUZZER_CHANEL);

	id_begin();

  display.init();		//ディスプレイを初期化
	display.flipScreenVertically();		//ディスプレイを上下反転
  // display.setFont(ArialMT_Plain_16);    //フォントを設定
  display.setFont(ArialMT_Plain_24);    //フォントを設定
	display.setTextAlignment(TEXT_ALIGN_CENTER); //テキストを中央揃え

  display.drawString(64, 5, "Tomoshibi");
  display.drawString(64, 27, "Technology");	

	display.setFont(ArialMT_Plain_10);    //フォントを設定
  display.drawString(64, 53,"v = 1.3");

  display.display();   //指定された情報を描画

	ledcWriteTone(BUZZER_CHANEL, C5); // ド
	led_delay(50, 80);
	ledcWriteTone(BUZZER_CHANEL, G5); // ソ
	led_delay(70, 80);
	ledcWriteTone(BUZZER_CHANEL, D5); // レ
	led_delay(90, 80);
	ledcWriteTone(BUZZER_CHANEL, A5); // ラ
	led_delay(110, 80);
	ledcWriteTone(BUZZER_CHANEL, 0);    // 消音
	leds_set_hsv(0,0,0);
	delay(1000);

  // ConnectWifi();
  // artnet.begin();
	pinMode(2,OUTPUT);
	digitalWrite(2,HIGH);
	display.resetDisplay();
	
	ledcWriteTone(BUZZER_CHANEL, C5); 
	for(int i=0;i<100;i+=2){
		if(i == 40){
			ledcWriteTone(BUZZER_CHANEL, D5);    // 消音
		}else if(i == 70){
			ledcWriteTone(BUZZER_CHANEL, 0);    // 消音
		}
		leds_set_hsv(170, 120,i);
		delay(6);
	}
	ledcWriteTone(BUZZER_CHANEL, 0);    // 消音

	///LED初期化
  for(int i=0; i<numLeds; i++){
    led_A[i]=CRGB(17,12,5);
    led_B[i]=CRGB(17,12,5);
    led_C[i]=CRGB(17,12,5);
    led_D[i]=CRGB(17,12,5);
  }
	FastLED.show();

	
	fog_random[0] = random(10,43);
	fog_random[1] = random(44,77);
	fog_random[2] = random(85,115);

	for(int i=0; i<3; i++){
		// fog_random[i] = 10 + random(100);
		Serial.println(fog_random[i]);
	}

	// artnet.setArtDmxCallback(onDmxFrame);

}

unsigned long dis_time = 0; //ディスプレイ
unsigned long star_time = 0; 
int mode = 0;

void loop() {
	if(millis() - dis_time > 5000){
		display.resetDisplay();
		display.init();		//ディスプレイを初期化
		display.setFont(ArialMT_Plain_24);
		display.flipScreenVertically();		//ディスプレイを上下反転
		display.setTextAlignment(TEXT_ALIGN_CENTER); //テキストを中央揃え
		display.drawString(64, 8,"id = " + String(id_read()));
		display.drawString(64, 32,"mode = " + String(mode));
		display.display();   //指定された情報を描画
		dis_time = millis();
	}

	if(star_count > 0 && millis()-star_time > 20){
		star_count += 1;
		star_time = millis();
	}
	
	// artnet.read();

	// int a = CTRL.available();
	// Serial.println(a);
	// Serial.print(" ");
	// if(CTRL.available()>30){
	// 	Serial.println(CTRL.read());
	// }

	receive_show();
	
}




void receive_show(void){
	byte data[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
	while(CTRL.available()>26){
		byte n = CTRL.read();
	}
	if(CTRL.available()){
		byte mydata = CTRL.read();
		if(mydata == 250){
			byte raw_receive_data[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
			bool receive_bad_flg = 0; //受信失敗フラグ
			for(int i=0; i<13; i++){
				if(CTRL.available()){
					raw_receive_data[i] = CTRL.read();
					if(raw_receive_data[i] == 250){receive_bad_flg = 1; break;} //受信失敗フラグを立てる(250が来たら
				}else{
					receive_bad_flg = 1; break;
				}
			}
			if(!receive_bad_flg){
				for(int i=0; i<13; i++){
					data[i] = raw_receive_data[i];
				}
				for(int i=0; i<13; i++){
					Serial.print(data[i]);
					Serial.print(" ");
				}
				Serial.println();
			}
		}
	}
	// uint8_t data[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
  mode = data[0];

  int beat = data[1]/2.2; //255を120に
  int beat_of_around = data[1]/7; //255を40に
	float rate = 0.8;
  beat = (beat*rate) + (pre_beat*(1-rate));
  beat_of_around = (beat_of_around*rate) + (pre_beat_od_around*(1-rate));
	pre_beat = beat;
	pre_beat_od_around = beat_of_around;

	int H = data[2];
	int S = data[3];
	int V = data[4];
	int H2 = data[5];
	int S2 = data[6];
	int V2 = data[7];
	int updownA = data[8];
	int updownB = data[9];
	int updownC = data[10];

	int star_trigger = data[11];
	int randomID = data[12];
	if(star_count == 0 && (star_trigger == 5 && randomID == id_read())){
		star_count = 1;
	}

	Serial.print(star_trigger);
	Serial.print(" ");
	Serial.print(randomID);
	Serial.print(" ");
	Serial.println(star_count);

//リセット
  for(int i=0; i<numLeds; i++){
    led_A[i]=CRGB(0,0,0);
    led_B[i]=CRGB(0,0,0);
    led_C[i]=CRGB(0,0,0);
    led_D[i]=CRGB(0,0,0);
    led_E[i]=CRGB(0,0,0);
  }

//値入れ
	if(mode == 1){ //beatで高さ IDによって色が変化
		H += 25 * id_read();
		H = H%255;
		for(int i=0; i<beat; i++){
			led_A[i] = CHSV(H,max(255-(i*3),0),V);
			led_B[i] = CHSV(H,max(255-(i*3),0),V);
			led_C[i] = CHSV(H,max(255-(i*3),0),V);
			led_D[i] = CHSV(H,max(255-(i*3),0),V);
		}
		for(int i=0; i<beat_of_around; i++){
			led_E[i] = CHSV(H,255-(i*6),V);
		}

	}else if(mode == 2){ //beatで高さ 奇数偶数で色が変化
		if(id_read()%2 == 0){
			for(int i=0; i<beat; i++){
				led_A[i] = CHSV(H,max(255-(i*3),0),V);
				led_B[i] = CHSV(H,max(255-(i*3),0),V);
				led_C[i] = CHSV(H,max(255-(i*3),0),V);
				led_D[i] = CHSV(H,max(255-(i*3),0),V);
			}
			for(int i=0; i<beat_of_around; i++){
				led_E[i] = CHSV(H,255-(i*6),V);
			}
		}else{
			for(int i=0; i<beat; i++){
				led_A[i] = CHSV(H2,max(255-(i*3),0),V);
				led_B[i] = CHSV(H2,max(255-(i*3),0),V);
				led_C[i] = CHSV(H2,max(255-(i*3),0),V);
				led_D[i] = CHSV(H2,max(255-(i*3),0),V);
			}
			for(int i=0; i<beat_of_around; i++){
				led_E[i] = CHSV(H2,255-(i*6),V);
			}
		}
	}else if(mode == 3){ //もわもわ
		for(int i=0; i<beat; i++){
			int myS = max(255-(i*2),0);
			led_A[i] = CHSV(H,myS,V);
			led_B[i] = CHSV(H,myS,V);
			led_C[i] = CHSV(H,myS,V);
			led_D[i] = CHSV(H,myS,V);
		}
		for(int num=0; num<3; num++){
			//この下、1乱数あたりのモワモワ
			int mowa_center;
			if(num==0){
				mowa_center = fog_random[num] + (updownA-5);
			}else if(num==1){
				mowa_center = fog_random[num] + (updownB-5);	
			}else{
				mowa_center = fog_random[num] + (updownC-5);
			}
			for(int roop=0; roop<3; roop++){
				int myV = ((5-roop*roop)/4.0)*V2;
				int led_id = mowa_center+roop;
				led_A[led_id] = CHSV(H2,S2,myV);
				led_B[led_id] = CHSV(H2,S2,myV);
				led_C[led_id] = CHSV(H2,S2,myV);
				led_D[led_id] = CHSV(H2,S2,myV);

				led_id = mowa_center-roop;
				led_A[led_id] = CHSV(H2,S2,myV);
				led_B[led_id] = CHSV(H2,S2,myV);
				led_C[led_id] = CHSV(H2,S2,myV);
				led_D[led_id] = CHSV(H2,S2,myV);
			}
		}
		//ここから飛ばすやつ
		if(star_count > 115){
			star_count = 0;
			//なにもしない
		}else if(star_count > 0){
			//115まで
			for(int roop=0; roop<5; roop++){
				int myV = 150 - ((17-roop*roop)/16.0*100 );
				//myV = 100;
				int led_id = star_count + roop;
				led_A[led_id] = CHSV(12,100,myV);
				led_B[led_id] = CHSV(12,100,myV);
				led_C[led_id] = CHSV(12,100,myV);
				led_D[led_id] = CHSV(12,100,myV);
			}
		}	

	}else{
		//知らないModeだったときは、beatにのる
		for(int i=0; i<beat; i++){
			int myS = max(255-(i*3),0);
			led_A[i] = CHSV(H,myS,V);
			led_B[i] = CHSV(H,myS,V);
			led_C[i] = CHSV(H,myS,V);
			led_D[i] = CHSV(H,myS,V);
		}
		for(int i=0; i<beat_of_around; i++){
			led_E[i] = CHSV(H,255-(i*6),V);
		}
	}
  
  //previousDataLength = length;
  FastLED.show();
}

void leds_set_hsv(int h, int s, int v) {
	for(int i = 0; i < numleds_of_around; i++) {
		led_E[i] = CHSV(h, s, v);
	}
	FastLED.show();
}

void leds_set_rgb(int r, int g, int b) {
	for(int i = 0; i < numleds_of_around; i++) {
		led_E[i] = CRGB(r, g, b);
	}
	FastLED.show();
}

void led_delay(int colorvalue, int delayvalue){
	for (int i= -1*(delayvalue/2); i<=delayvalue/2; i++)	{
		leds_set_hsv(colorvalue, 200, 80-50*abs(i)/float(delayvalue/2));
		delay(1);
	}
}

void id_begin(){
	pinMode(ID_PIN_A, INPUT);
	pinMode(ID_PIN_B, INPUT);
	pinMode(ID_PIN_C, INPUT);
	pinMode(ID_PIN_D, INPUT);
}

int id_read(){
	int id = digitalRead(ID_PIN_A) + digitalRead(ID_PIN_B)*2 + digitalRead(ID_PIN_C)*4 + digitalRead(ID_PIN_D)*8;
	return id;
}


