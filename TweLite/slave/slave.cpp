#include <TWELITE>
#include <NWK_SIMPLE>

#define TIME_UP 0
#define TIME_OFF -1
#define ZIG_WAIT_MAX 100
#define ADC_FLT_MAX 4

//通信
const uint32_t APP_ID = 0x1234abcd; // application ID
const uint8_t CHANNEL = 13; // channel 11-26
const char APP_FOURCHAR[] = "TECH";

//ピン
const uint8_t DIP0= mwx::PIN_DIGITAL::DIO13; //adrs1
const uint8_t DIP1= mwx::PIN_DIGITAL::DIO16; //adrs2
const uint8_t LED0= mwx::PIN_DIGITAL::DIO9; 
const uint8_t LED1= mwx::PIN_DIGITAL::DIO8; 

// application use
MWX_APIRET txreq_stat;

/*** Local function prototypes */
MWX_APIRET transmit();

uint8_t my_adrs; //自分のアドレス


void setup() {

	pinMode(DIP0,INPUT);
	pinMode(DIP1,INPUT);
	pinMode(LED0,OUTPUT);
	pinMode(LED1,OUTPUT);
	delay(10);
	my_adrs = digitalRead(DIP1) << 1;//子機のアドレスを計算
	my_adrs = my_adrs + digitalRead(DIP0); //子機のアドレスを計算
	digitalWrite(LED0,digitalRead(DIP0));
	digitalWrite(LED1,digitalRead(DIP1));

	Analogue.setup(
		true, // AD変換が安定するまで待つ
		ANALOGUE::KICK_BY_TICKTIMER // AD変換をTickTimer(1ms毎)で実行する
	);
	Analogue.begin(
		pack_bits(PIN_ANALOGUE::VCC) // AD変換対象のピン2つを指定
	);

	txreq_stat = MWX_APIRET(false,0);

	the_twelite
		<< TWENET::appid(APP_ID)    
		<< TWENET::channel(CHANNEL) 
		<< TWENET::rx_when_idle();      

	auto&&	nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
					nwksmpl << NWK_SIMPLE::logical_id(0xA0 + my_adrs); //子機をセット  
	the_twelite.begin();
	
	Serial << "--- wakup-com->start ---" << mwx::crlf;
}


bool b_transmit = false; //送信中かどうか
int16_t timzigwait = TIME_OFF; //ZigBeeの待ち時間

uint16_t vcc;

//読み取るときのやつ
// uint8_t re_to_adrs; //送信先のアドレス 自分のアドレス
// uint8_t re_from_adrs; //送信元のアドレス
//送るときのやつ
uint8_t se_from_adrs; //送信元のアドレス 自分のアドレス
uint8_t se_to_adrs = 0x00; //送信先のアドレス(マスター)

void loop() {

	//アナログの値を取得
	if(Analogue.available()){
		vcc = Analogue.read(PIN_ANALOGUE::VCC); //VCCの値
	}

	if(TickTimer.available()){ //1msごとに実行
		if(timzigwait > TIME_UP ){ //ZigBeeの待ち時間が終わったか
			timzigwait--;
		}
	}

	if (b_transmit) {
		//送信受付は完了
		if (the_twelite.tx_status.is_complete(txreq_stat.get_value())) {
			//送信完了
			timzigwait = TIME_OFF; //-1
			b_transmit = false; 
			the_twelite.sleep(5);
		}else{
			//送信未完了
			if(timzigwait == TIME_UP){
				//送信完了待ち時間が終わった
				timzigwait = TIME_OFF; //-1
				b_transmit = false;
				the_twelite.sleep(5);
			}else{
				//送信中	
			}
		}
	}else{
		//送信受付はしていない
		txreq_stat = transmit();
		if(txreq_stat){
			Serial << "..sleep2." << mwx::crlf << mwx::flush;
			b_transmit = true; //送信しよう！！
			timzigwait = ZIG_WAIT_MAX; //100msなら待ってあげる
		}else{
			//送信受け付け失敗
			Serial << "..chk2." << mwx::crlf << mwx::flush;
			b_transmit = false; //送信できないよ
			the_twelite.sleep(5);
		}
	}	
	
}


/*** transmit a packet */
MWX_APIRET transmit() {
	uint16_t send_value = 100;
	se_from_adrs = my_adrs;

	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
		Serial 	<< "fr_ad:"
						<< int(se_from_adrs) 
						<< " vcc:"
						<< int(vcc) 
						<< " temp:"
						<< int(send_value)
						<< mwx::crlf << mwx::flush;

		pkt << tx_addr(se_to_adrs) //送信先(0x00:親)
				<< tx_retry(0x1) //送信トライ回数
				<< tx_packet_delay(0,0,2);	//最低待ち時間、最長待ち時間、再送間隔
																		//直ちに再送、2ms待って再送
		//まとめる
		pack_bytes(
			pkt.get_payload(), //送信データ
			make_pair(APP_FOURCHAR, 4), //4バイトの文字列
			se_from_adrs, //自分のアドレス uint8_t
			vcc, //VCC電圧 uint16_t
			send_value //uint16_t
		);
		return pkt.transmit();
	}
	return MWX_APIRET(false, 0);
}


void wakeup(){
	// the_twelite.sleep(hoge); でスリープした後に呼び出される
}
