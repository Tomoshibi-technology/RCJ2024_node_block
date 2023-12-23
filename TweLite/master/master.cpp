// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>

/*** Config part */
const uint32_t APP_ID = 0x1234abcd;
const uint8_t CHANNEL = 13;

// application use
const char APP_FOURCHAR[] = "TECH";
uint8_t u8devid = 0;

/*** function prototype */
void receive();

//読み取るときのやつ
uint8_t re_to_adrs; //送信先のアドレス 自分のアドレス
uint8_t re_from_adrs0; //送信元のアドレス
uint8_t re_from_adrs1; //送信元のアドレス
//送るときのやつ
// uint8_t se_to_adrs; //送信先のアドレス
// uint8_t se_from_adrs; //送信元のアドレス 自分のアドレス

uint16_t vcc;
uint16_t temp;

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
	while (the_twelite.receiver.available()) { //受信待ち 受信したデータがなくなるまで繰り返す
		receive();
	}
	
	Serial 	<< " r_fr0:" << format("0x%X", re_from_adrs0) //送信元のアドレス 16進数で表示してます
					<< " r_fr1:" << format("0x%X", re_from_adrs1) //送信先のアドレス(自分のアドレス)
					<< " vcc:" << (int)vcc
					<< " tem:" << (int)temp
					<< mwx::crlf << mwx::flush;
}

/*add function recive()*/
void receive() {

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
		re_from_adrs0,
		vcc,
		temp
	);
	re_from_adrs1 = rx.get_addr_src_lid(); //送信元のアドレスを格納
}

MWX_APIRET transmit(void) {
    uint8_t txadrs;
    
    //送信先を選択する処理(省略)
    if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
	pkt << tx_addr(txadrs)  //送信先を指定
            << tx_retry(0x3) 
	    << tx_packet_delay(0,0,2);

        pack_bytes(pkt.get_payload(), make_pair(APP_CHAR, 3));//ヘッダー
        pack_bytes(pkt.get_payload(),do_out); //do_outをパッケージ化する

	return pkt.transmit(); //パケットを送信
    }
}
