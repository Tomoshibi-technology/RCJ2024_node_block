#include <TWELITE>
#include <NWK_SIMPLE>

#define TIME_UP 0
#define TIME_OFF -1
#define ZIG_WAIT_MAX 100
#define ADC_FLT_MAX 4

/*** Config part */
const uint32_t APP_ID = 0x1234abcd; // application ID
const uint8_t CHANNEL = 13; // channel 11-26
/*** application defs */
const char APP_FOURCHAR[] = "TECH";

struct ADC_MEAN{
	uint8_t wp; //書き込み位置
	uint16_t buf[ADC_FLT_MAX]; //温度センサーの値を格納
	uint16_t dat; //温度センサーの計算値
};

const uint8_t PIN_DO1 = mwx::PIN_DIGITAL::DIO18; //Boot
const uint8_t PIN_DO2 = mwx::PIN_DIGITAL::DIO9; //TEMP
const uint8_t PIN_DI1= mwx::PIN_DIGITAL::DIO12; //adrs1
const uint8_t PIN_DI2= mwx::PIN_DIGITAL::DIO13; //adrs2
const uint8_t PIN_AD1 = mwx::PIN_ANALOGUE::A1; //温度

// application use
MWX_APIRET txreq_stat;
uint16_t au16AI[2];
ADC_MEAN adTemp; //温度センサーの値を格納
bool b_transmit = false;
bool ad_finish = false;
uint8_t u8devid = 0x00;	//parent
bool b_senser_started;
int16_t timzigwait = TIME_OFF; //ZigBeeの待ち時間
uint8_t adrs;

/*** Local function prototypes */
void sleepNow();
void napNow();
MWX_APIRET transmit();

/*** the setup procedure (called on boot) */
void setup() {

	pinMode(PIN_DO1, OUTPUT_INIT_LOW); //BOOTをLOW
	pinMode(PIN_DO2, OUTPUT_INIT_HIGH); //TEMP OFF
	pinMode(PIN_DI1,INPUT_PULLUP);
	pinMode(PIN_DI2,INPUT_PULLUP);

	delay(10);
	adrs = digitalRead(PIN_DI2) << 1;//子機のアドレスを計算
	adrs = adrs + digitalRead(PIN_DI1); //子機のアドレスを計算

	for(auto&& x : au16AI) x = 0xFFFF;			//au16AIの値を初期化
	for(auto&& x : adTemp.buf) x = 0xFFFF;	//adTemp.bufの値を初期化

	Analogue.setup(
		true, // AD変換が安定するまで待つ
		ANALOGUE::KICK_BY_TICKTIMER // AD変換をTickTimer(1ms毎)で実行する
	);
	Analogue.begin(
		pack_bits(PIN_AD1,PIN_ANALOGUE::VCC) // AD変換対象のピン2つを指定
	);

	txreq_stat = MWX_APIRET(false,0);

	the_twelite
		<< TWENET::appid(APP_ID)    
		<< TWENET::channel(CHANNEL) 
		<< TWENET::rx_when_idle();      

	auto&&	nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
					nwksmpl << NWK_SIMPLE::logical_id(0xFE); //子機をセット  
	the_twelite.begin();
	
	Serial << "--- wakup-com->start ---" << mwx::crlf;
}


/*** the loop procedure (called every event) */
void loop() {
	//TIME_OFF = -1
	//TIME_UP = 0
	//ZIG_WAIT_MAX = 100

	if(TickTimer.available()){ //1msごとに実行
		if(timzigwait > TIME_UP ){ //ZigBeeの待ち時間が終わったか
			timzigwait--;
		}
	}
	if (b_transmit) { //送信受け付け完了したか
		if (the_twelite.tx_status.is_complete(txreq_stat.get_value())) { //送信が完了したか	
			//Serial << "..transmit complete." << mwx::crlf << mwx::flush;
			timzigwait = TIME_OFF;
			sleepNow();
		}else{
			if(timzigwait == TIME_UP){
				timzigwait = TIME_OFF;
				sleepNow();
			}
		}
	}else{
		txreq_stat = transmit();
		if(txreq_stat){
			//Serial << "..sleep2." << mwx::crlf << mwx::flush;
			b_transmit = true;
			timzigwait = ZIG_WAIT_MAX;
		}else{
			Serial << "..chk2." << mwx::crlf << mwx::flush;
			sleepNow();
		}
	}

	//温度センサーの値を取得
	if(Analogue.available()){
		au16AI[0] = Analogue.read(PIN_ANALOGUE::VCC); //VCCの値
		au16AI[1] = Analogue.read(PIN_AD1); //温度センサーの値

		adTemp.buf[adTemp.wp] = Analogue.read(PIN_AD1); //温度センサーの値を格納 
																										// .wpがどこまで格納したかを示す
		uint16_t sum;
		ad_finish = true;

		for(uint8_t i=0; i<ADC_FLT_MAX; i++){ //ADC_FLT_MAXは4
			if(adTemp.buf[i] == 0xFFFF){ //値が格納されていない場合
				ad_finish = false;
				break;
			}else{
				sum += adTemp.buf[i]; //合計を算出
			}
		}
		if(ad_finish){
			adTemp.dat = sum>>2; //4で割る(2回右にシフトする)
		}
		if(adTemp.wp++ >= ADC_FLT_MAX){ //値の格納場所を更新する
			// ADC_FLT_MAXは4
			adTemp.wp = 0;
		}
	}
}

/* callback wakeup */
void wakeup(){
	if (!b_senser_started) { //スリープからのウェークアップ
		//falseだったら
		b_senser_started = true;
		digitalWrite(PIN_DO2,LOW);
		napNow();
	}else{
		b_transmit = false;
	}
}

// perform sleeping
void sleepNow() {
	uint32_t u32ct = 4900; //5000ms+ランダム時間
	//Serial << int(adrs) << "..sleeping " << int(u32ct) << "ms." << mwx::crlf << mwx::flush;
	b_senser_started = false;
	digitalWrite(PIN_DO2,HIGH); //温度センサーをOFF
	the_twelite.sleep(u32ct);
}

/* perform short period sleep */
void napNow() {
	uint32_t u32ct = 100;
	//Serial << "..nap " << int(u32ct) << "ms." << mwx::crlf << mwx::flush;
	the_twelite.sleep(
		u32ct, //スリープ時間
		false, //起床時間を再検証するか
		false, //RAMを保持しないスリープにするか
		TWENET::SLEEP_WAKETIMER_SECONDARY //ウェイクアップタイマーの設定
	);
}


/*** transmit a packet */
MWX_APIRET transmit() {
	if (auto&& pkt = the_twelite.network.use<NWK_SIMPLE>().prepare_tx_packet()) {
		//Serial << "analogue"<< int(au16AI[0]) << ".."<< int(adTemp.dat)   << mwx::crlf << mwx::flush;
		pkt << tx_addr(u8devid) //送信先(0x00:親)
				<< tx_retry(0x1) //送信トライ回数
				<< tx_packet_delay(0,0,2);	//最低待ち時間、最長待ち時間、再送間隔
																		//直ちに再送、2ms待って再送
		//まとめる
		pack_bytes(
			pkt.get_payload(), //送信データ
			make_pair(APP_FOURCHAR, 4), //4バイトの文字列
			adrs, //自分のアドレス
			au16AI[0], //VCC電圧
			adTemp.dat //温度センサーの計算済みの値
		);
		return pkt.transmit();
	}
	return MWX_APIRET(false, 0);
}

