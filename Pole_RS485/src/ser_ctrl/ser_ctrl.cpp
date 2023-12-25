
#include "./ser_ctrl.h"

CTRL::CTRL(HardwareSerial* ptr_serial){
	SER = ptr_serial;
}

void CTRL::init(void){
	SER->begin(115200);
}

void CTRL::read(void){
	while(SER->available()>26){
		byte n = SER->read();
	}
	if(SER->available()){
		byte mydata = SER->read();
		if(mydata == 250){
			byte raw_receive_data[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
			bool receive_bad_flg = 0; //受信失敗フラグ
			for(int i=0; i<13; i++){
				if(SER->available()){
					raw_receive_data[i] = SER->read();
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
}