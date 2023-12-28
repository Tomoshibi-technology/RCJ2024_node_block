#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

int NUMPIXELS = 256;
int PIN = 9;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int make_id(int x, int y);


float kansuu(float x){
	return 5.5*sin(x/2)+7.5;
}

float kansuu2(float x){
	return 4.3*sin(2*x/3)+7.5;
}


void setup(){
	pixels.begin();
	Serial.begin(115200);
}

float i = -15;

void loop(){
	pixels.clear();

	for(int x=0;x<16;x++){
		for(int y=0;y<16;y++){
			// if(x == y -15 + i%32){
			float a = i/3;
			float b = 1.4;
			float c = 1.8;
			

			//--------------------------円-------------------------------			
			// int x2 = sqrt(10+(int(i)%125)-(x-7.5)*(x-7.5));
			// int x3 = sqrt(10+((int(i)+60)%125)-(x-7.5)*(x-7.5));

			// if(((y >= x2 +7.5 -b && y <= x2 +7.5 +b)||(y >= -x2 +7.5 -b && y <= -x2 +7.5 +b))&& x2 != 0){
			// 	pixels.setPixelColor(make_id(x,y), pixels.Color(10,5,0)); //red
			// }
			// if(((y >= x3 +7.5 -b && y <= x3 +7.5 +b)||(y >= -x3 +7.5 -b && y <= -x3 +7.5 +b))&& x3 != 0){
			// 	pixels.setPixelColor(make_id(x,y), pixels.Color(0,10,5)); //red
			// }

			// --------------------------三相交流-------------------------------
			// if(y >= kansuu(x+a)-b && y <= kansuu(x+a)+b){
			// 	int value = map(abs(y-kansuu(x+a)),0,1.8,30,0);
			// 	pixels.setPixelColor(make_id(x,y), pixels.Color(5,min(value,50),0)); //green
			// } else if(y >= kansuu(x+a+4.18)-b && y <= kansuu(x+a+4.18)+b){
			// 	pixels.setPixelColor(make_id(x,y), pixels.Color(0,5,10)); //blue
			// } else if(y >= kansuu(x+a+8.36)-b && y <= kansuu(x+a+8.36)+b){
			// 	pixels.setPixelColor(make_id(x,y), pixels.Color(10,5,0)); //red
			// } else {
			// 	pixels.setPixelColor(make_id(x,y), pixels.Color(1,0,1));
			// }
			int value = min((int)(14.0-abs(y-kansuu2(x+a))),50); //関数は最大13とか
			value = pow(2.0, value/2.5);
			if(value> 200){value = 0;}else if(value < 0){value = 0;}
			pixels.setPixelColor(make_id(x,y), pixels.Color(3,value*0.5,2)); //green
			
		}
	}
	i++;

	pixels.show();
	delay(50);

}


int make_id(int x, int y){
	if(y%2 == 0){
		return y*16 + x;
	}else{
		return y*16 + 15 - x;
	}
}








// #include <Arduino.h>
// // 以下はサーボ用
// #include <SCServo.h>

// SCSCL sc;
// HardwareSerial Serial1(PA10, PA9);

// void setup(){
//   // Serial.begin(115200);
//   Serial1.begin(1000000);
//   sc.pSerial = &Serial1;
//   delay(1000);
// 	pinMode(PA12, OUTPUT);
// }

// void loop(){
// 	// int i = 2;
// 	for(int i=0;i<5;i++){
//   	sc.WritePos(i, 1000, 0, 1500);//舵机(ID1)以最高速度V=1500步/秒,运行至P1=1000
// 		delay(100);
//   }
// 	digitalWrite(PA12, HIGH);
// 	delay(754);//[(P1-P0)/V]*1000+100
  
// 	for(int i=0;i<5;i++){
//   	sc.WritePos(i, 20, 0, 1500);//舵机(ID1)以最高V=1500步/秒,运行至P1=20
// 		delay(100);
//   }
// 	digitalWrite(PA12, LOW);
// 	delay(754);//[(P1-P0)/V]*1000+100
// }





