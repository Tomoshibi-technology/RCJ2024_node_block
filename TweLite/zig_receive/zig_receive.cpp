// use twelite mwx c++ template library
#include <TWELITE>
#include <NWK_SIMPLE>

#define CNT_MAX 1000

/*** Config part */
const uint32_t APP_ID = 0x1234abcd;
const uint8_t CHANNEL = 13;
const uint8_t PIN_DI1 = mwx::PIN_DIGITAL::DIO10; 
const uint8_t PIN_DO1 = mwx::PIN_DIGITAL::DIO9; 

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

    auto&& nwksmpl = the_twelite.network.use<NWK_SIMPLE>();
	   nwksmpl << NWK_SIMPLE::logical_id(0xFE); // set Logical ID. (0x00 means parent device)

    the_twelite.begin(); // start twelite!
    Serial << "--- Parent act ---" << mwx::crlf;
}

/*** loop procedure (called every event) */
void loop() {

    if(TickTimer.available()){
	++cnt1ms;
    }

    while (the_twelite.receiver.available()) {
	receive();
	rcvflg = true;
    }

    if( cnt1ms >= CNT_MAX){
	cnt1ms -= CNT_MAX;

	ts = (ts + 1 ) % 100000;

	if(rcvflg){
	    rcvflg = false;
	}else{
	    // Serial << "ts:" <<  int(ts) << mwx::crlf << mwx::flush;;
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

		uint8_t hoge[4]; // init all elements as default (0).
   
    auto&& rx = the_twelite.receiver.read();
    char fourchars[5]{}; // init all elements as default (0).
    auto&& np = expand_bytes(
			rx.get_payload().begin(), rx.get_payload().end()
		, make_pair((uint8_t*)fourchars, 4)  // 4bytes of msg
                );

    if (strncmp(APP_FOURCHAR, fourchars, 4)) {return; } //文字列チェック

    expand_bytes(	np,
									rx.get_payload().end(),
									adrs,
									hoge[0],
									hoge[1],
									hoge[2],
									hoge[3]
								);
    temp2 = (double)( temp - 500 ) / 10; //電圧値から温度換算
    vcc2 = (double)vcc/1000; //電圧値から電源電圧換算
    Serial 	<< "ts:" << int(ts) 
						<< "：" << int(rx.get_addr_src_lid())
						<< " adrs: " << (int)adrs
						<< " : " << (int)hoge[0]
						<< " : " << (int)hoge[1]
						<< " : " << (int)hoge[2]
						<< " : " << (int)hoge[3]
						<< mwx::crlf << mwx::flush;
}