// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>

//通信
const uint32_t APP_ID = 0x1234abcd;
const uint8_t CHANNEL = 13;
const char APP_FOURCHAR[] = "TECH";

//関数
void receive();

//読み取るときのやつ
uint8_t re_to_adrs; //送信先のアドレス 自分のアドレス
uint8_t re_from_adrs; //送信元のアドレス

uint8_t re_data[4] = {0,0,0,0}; //送信データ


void setup() {
	//ーーーーー通信設定ーーーーー
	the_twelite
		<< TWENET::appid(APP_ID) 
		<< TWENET::channel(CHANNEL) 
		<< TWENET::rx_when_idle(); 
	auto&& 	nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
					nwksmpl << NWK_SIMPLE::logical_id(0x00); //0x00→マスター 0xFE→スレーブ
	the_twelite.begin(); // start twelite!

	//ーーーーー起動ーーーーー
	Serial << "--- Parent act ---" << mwx::crlf;
}

void loop() {
	//ーーーーー受信ーーーーー
	while (the_twelite.receiver.available()) { //受信待ち 受信したデータがなくなるまで繰り返す
		receive();
		Serial 	<< "re_fr:" << format("0x%X", re_from_adrs) //送信元のアドレス 16進数で表示してます
						<< "  data: " << (int)re_data[0]
						<< " : " << (int)re_data[1]
						<< " : " << (int)re_data[2]
						<< " : " << (int)re_data[3]
						<< mwx::crlf << mwx::flush;
	}

	//ーーーーー送信ーーーーー
}

/*add function recive()*/
void receive() {
	auto&& rx = the_twelite.receiver.read(); //受信値をRXに読み込む
	char fourchars[5]{}; // 判定用
	auto&& np = expand_bytes(
								rx.get_payload().begin(), 
								rx.get_payload().end(), 
								make_pair((uint8_t*)fourchars, 4) //fourcharsに4バイトの文字列を格納
							);

	if (strncmp(APP_FOURCHAR, fourchars, 4)) {
		return;
	}
	expand_bytes(
		np,
		rx.get_payload().end(),
		re_from_adrs, //uint8_t
		re_data[0], //uint8_t
		re_data[1], //uint8_t
		re_data[2], //uint8_t
		re_data[3]  //uint8_t
	);
}


