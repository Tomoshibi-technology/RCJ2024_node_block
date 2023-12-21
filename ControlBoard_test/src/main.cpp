#include <Arduino.h>

#include "./speaker/speaker.h"
#define BUZZER_PIN PB10

SPEAKER speaker(BUZZER_PIN);



#include "./oled/oled.h"
#include <U8g2lib.h>
#define SDA_PIN PB9
#define SCL_PIN PB8
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,  SCL_PIN, SDA_PIN, /* reset=*/ U8X8_PIN_NONE);
OLED oled(&u8g2);

#include "./voltage/voltage.h"
#define VOL_PIN PB1
VOLTAGE voltage(VOL_PIN);

#include "./dip/dip.h"
#define DIP_PIN0 PA8
#define DIP_PIN1 PA11
#define DIP_PIN2 PA12
#define DIP_PIN3 PA15
DIP dip(DIP_PIN0, DIP_PIN1, DIP_PIN2, DIP_PIN3);

const String name[16] = {
	"Center",
	"Display",
	"RobotArmA",
	"RobotArmB",
	"LEDPole",
	"Pro5",
	"Pro6",
	"Pro7",
	"Pro8",
	"Pro9",
	"Pro10",
	"Pro11",
	"Pro12",
	"Pro13",
	"Pro14",
	"Pro15"
};



void setup() {
	pinMode(BUZZER_PIN, OUTPUT);
	digitalWrite(BUZZER_PIN, HIGH);
	oled.init();
	oled.display_version(1.1);
	oled.show();

	voltage.init();
	
	dip.init();

	delay(1000);
}

void loop() {
	oled.clear();
	oled.half_display_num(
		"S = "+String(dip.read_ID()),
		"R = "+String(voltage.percentage(12.5,11.1))+"%" // max,min
	);

	oled.display_battary(voltage.read(), voltage.percentage(12.6,11.1));
	oled.display_title(name[dip.read_ID()]+" V" + String(1.13));
	oled.show();

	delay(1000);  
	tone(BUZZER_PIN, 500);
	delay(1000);
	noTone(BUZZER_PIN);
	delay(1000);
}
