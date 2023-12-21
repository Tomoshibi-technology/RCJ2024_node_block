#include "./oled.h"

OLED::OLED(U8G2_SSD1306_128X64_NONAME_F_SW_I2C* ptr_myu8g2){
	myU8G2 = ptr_myu8g2;
}

void OLED::init(){
	myU8G2->begin();
	myU8G2->setFlipMode(0); //ディスプレイの上下を指定
}
void OLED::clear(){
	myU8G2->clearBuffer(); //ディスプレイのバッファをクリア
}

void OLED::show(){
	myU8G2->sendBuffer(); //ディスプレイにバッファの内容を反映
}

void OLED::display_version(float version){
	//myU8G2->setFont(u8g2_font_logisoso16_tf);
	myU8G2->setFont(u8g2_font_fub17_tr);
	byte x = myU8G2->getStrWidth("Tomoshibi");
	myU8G2->drawStr(64-(x/2),23,"Tomoshibi");	// write something to the internal memory
	x = myU8G2->getStrWidth("Technology");
	myU8G2->drawStr(64-(x/2),43,"Technology");	// write something to the internal memory

	myU8G2->setFont(u8g2_font_t0_11_mf);    //フォントを設定
	String str = "Version " + String(version);
	x = myU8G2->getStrWidth(str.c_str());
  myU8G2->drawStr(64-(x/2),55,str.c_str());

}

void OLED::half_display_num(String str0, String str1){
	//フォント指定
	myU8G2->setFont(u8g2_font_t0_11_mf);

	byte x = myU8G2->getStrWidth(str0.c_str());
  myU8G2->drawStr(64-(x/2),22,str0.c_str());
	x = myU8G2->getStrWidth(str1.c_str());
	myU8G2->drawStr(64-(x/2),33,str1.c_str());
}

void OLED::display_battary(float voltage, int percentage){
	//フォント指定
	myU8G2->setFont(u8g2_font_5x7_tr);
	//電圧表示
	String str = String(voltage) + "V";
	byte x = myU8G2->getStrWidth(str.c_str());
	myU8G2->drawStr(106-x,7,str.c_str());
	//バー
	myU8G2->drawFrame(108, 0, 20, 8);
	myU8G2->drawBox(108, 0, (int)(20.0*percentage/100), 8);
	
}

void OLED::display_title(String str){
	//フォント指定
	myU8G2->setFont(u8g2_font_5x7_tr);
	//タイトル表示
	byte x = myU8G2->getStrWidth(str.c_str());
	myU8G2->drawStr(0,7,str.c_str());
}



