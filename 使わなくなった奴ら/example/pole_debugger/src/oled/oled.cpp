#include "./oled.h"

OLED::OLED(SSD1306* ptr_ssd){
	SSD = ptr_ssd;
}

void OLED::init(){
	SSD->init(); //ディスプレイの初期化
	SSD->flipScreenVertically(); //ディスプレイを上下反転
	SSD->setTextAlignment(TEXT_ALIGN_CENTER); //テキストを中央揃えに
	SSD->setContrast(255); //コントラストを設定(最大255)
}

void OLED::display_version(int version){
	SSD->setFont(ArialMT_Plain_24);    //フォントを設定
	SSD->drawString(64, 5, "Tomoshibi");
  SSD->drawString(64, 27, "Technology");	
	SSD->setFont(ArialMT_Plain_10);    //フォントを設定
  SSD->drawString(64, 53,"Version " + String(version));
}

void OLED::display_num(String str0, int num0, String str1, int num1){
	SSD->setFont(ArialMT_Plain_24);
	SSD->drawString(64, 8,  str0 + String(num0));
	SSD->drawString(64, 32, str1 + String(num1));
}

void OLED::half_display_num(String str0, int num0, String str1, int num1){
	SSD->setFont(ArialMT_Plain_16);
	SSD->drawString(64, 4,  str0 + String(num0));
	SSD->drawString(64, 20, str1 + String(num1));
}

void OLED::half_display_3button(bool n0, bool n1, bool n2){
	if(n0 == 1){
		SSD->fillRect(2, 40, 40, 22);  //0
	}
	if(n1 == 1){
		SSD->fillRect(44, 40, 40, 22);  //1
	}
	if(n2 == 1){
		SSD->fillRect(86, 40, 40, 22);  //2
	}
}

void OLED::clear(){
	SSD->clear(); //ディスプレイをクリア
}

void OLED::show(){
	SSD->display();   //指定された情報を描画
}



