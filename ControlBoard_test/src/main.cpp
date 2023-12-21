#include <Arduino.h>

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


HardwareSerial Serial1(PA10, PA9); //UART1 TX, RX

void setup() {
	oled.init();

	delay(1000);
	oled.display_version(1.1);
	oled.show();

	power.init();
	
	dip.init();

	speaker.init();
	speaker.boot_music();

	button.init();

	delay(1000);

	Serial1.begin(115200);
}

unsigned long loop_timer = 10000;

void loop() {
	Serial1.println(millis() - loop_timer);
	loop_timer = millis();

	bool val[3] = {0, 0, 0};
	button.read(val);

	if(val[0]) speaker.ring(C6);
	if(val[1]) speaker.ring(E6);
	if(val[2]) speaker.ring(G6);
	if(!val[0] && !val[1] && !val[2]) speaker.mute();
	

	oled.clear();
	oled.half_display_3button(val);
	oled.half_display_num(
		"S = "+String(dip.read_ID()),
		"R = "+String(power.percentage())+"%" // max,min
	);

	oled.display_battary(power.voltage(), power.percentage());
	oled.display_title(name[dip.read_ID()]+" V" + String(1.13));
	oled.show();

	// delay(10);
	//speaker.beat(C5, 0.5, 240);

}
