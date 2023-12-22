// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>

#define CNT_MAX 1000

/*** Config part */
const uint32_t APP_ID = 0x1234abcd;
const uint8_t CHANNEL = 13;
// const uint8_t PIN_DI1 = mwx::PIN_DIGITAL::DIO10; 
// const uint8_t PIN_DO1 = mwx::PIN_DIGITAL::DIO9; 

// application use
int16_t cnt1ms;
uint32_t ts;
bool rcvflg;
const char APP_FOURCHAR[] = "TECH";
uint8_t u8devid = 0;

/*** function prototype */
void receive();

/*** setup procedure (run once at cold boot) */
void setup() {
	the_twelite
		<< TWENET::appid(APP_ID) 
		<< TWENET::channel(CHANNEL) 
		<< TWENET::rx_when_idle(); 

	auto&& 	nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
					nwksmpl << NWK_SIMPLE::logical_id(0x00); //0x00→マスター 0xFE→スレーブ
	the_twelite.begin(); // start twelite!

	Serial << "--- Parent act ---" << mwx::crlf;
}

void loop() {
	if(TickTimer.available()){
		cnt1ms++; //1msカウント
	}

	while (the_twelite.receiver.available()) { //受信待ち 受信したデータがなくなるまで繰り返す
		receive();
		rcvflg = true;
	}

	if(cnt1ms >= CNT_MAX){ //1000msごとに実行
		cnt1ms -= CNT_MAX; 
		ts = (ts + 1) % 100000; //秒数をカウントアップ

		if(rcvflg){
			rcvflg = false;
		}else{
			Serial << "ts:" <<  int(ts) << mwx::crlf << mwx::flush;;
		}
	}
}

/*add function recive()*/
void receive() {
	uint8_t adrs;
	uint16_t vcc;
	uint16_t temp;
	double temp2;
	double vcc2;

	auto&& rx = the_twelite.receiver.read(); //受信値をRXに読み込む
	char fourchars[5]{}; // 判定用
	
	auto&& np = expand_bytes(
								rx.get_payload().begin(), 
								rx.get_payload().end(), 
								make_pair((uint8_t*)fourchars, 4)  // 4bytes of msg
								//fourcharsに4バイトの文字列を格納
							);

	if (strncmp(APP_FOURCHAR, fourchars, 4)) {
		return;
	} //文字列チェック

	expand_bytes(
		np,
		rx.get_payload().end(),
		adrs,
		vcc,
		temp
	);

	temp2 = (double)(temp-500)/10; //電圧値から温度換算
	vcc2 = (double)vcc/1000; //電圧値から電源電圧換算

	Serial	<< "ts:" << int(ts) 
					<< "：" << int(rx.get_addr_src_lid()) //送信元の論理ID <NWK_SIMPLE>で指定してるやつ
					<< " adrs: " 	<< (int)adrs //
					<< " vcc: " << vcc2 << "V"
					<< " temp: " << temp2 << "deg" 
					<< mwx::crlf << mwx::flush;

	Serial 	<< int (rx.get_addr_src_lid())
					<< "," << (int)adrs
					<< "," << (int)vcc
					<< "," << (int)temp
					<< mwx::crlf << mwx::flush;
}