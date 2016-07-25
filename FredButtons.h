#ifndef FREDBUTTONS_H
#define FREDBUTTONS_H
template<int length> class BitField{
	//#define bitStuff //bitstuff is currently broken due to a compiler bug it seems
	#ifndef bitStuff
	bool bits[length];
	public:
	bool get(int index){
		return bits[index];
	}
	void setTo(int index, bool value){
		bits[index] = value;
	}
	
	bool operator[](int index){
		return get(index);
	}
	#else
	uint8_t bytes[length/8];
	public:
	bool get(int index){
		return bitRead(bytes[index/8], index%8);
	}
	void setTo(int index, bool value){
		Serial.print("set ");
		Serial.print(index);
		Serial.print(" (index/8=");
		Serial.print(index/8);
		Serial.print(") to ");
		Serial.println(value);
		bitWrite(bytes[index/8],index%8,value);
	}
	
	bool operator[](int index){
		return get(index);
	}
	#endif
};

struct ButtonGroup{
	uint8_t btn1,btn2;
};

template<int buttons, int buttonGroups> class FredButtons{
	BitField<buttons> btnHeld;
	BitField<buttons> btnProcessed;
	
	public:
	uint8_t pins[buttons];
	ButtonGroup groups[buttonGroups];
	void (*buttonPressed)(int);
    void (*groupPressed)(int);
	
	void begin(){
		for(int i = 0; i < buttons; i++){
			pinMode(pins[i],INPUT);
		}
	}
	
	void process(){
		for(int i = 0; i < buttons; i++){
			bool btnHeldNew = digitalRead(pins[i]);
			if(btnHeldNew && !btnHeld[i]){ //on btn down
				btnProcessed.setTo(i,false);
			}else if(!btnHeldNew && btnHeld[i] && !btnProcessed[i]){//on btn up, if not processed for a double press
				if(buttonPressed){
					buttonPressed(i);
				}
			}
			btnHeld.setTo(i,btnHeldNew);
		}
		
		for(int i = 0; i < buttonGroups; i++){
			uint8_t b1 = groups[i].btn1;
			uint8_t b2 = groups[i].btn2;
			if(btnHeld[b1] && !btnProcessed[b1] && btnHeld[b2] && !btnProcessed[b2]){
				btnProcessed.setTo(b1,true);
				btnProcessed.setTo(b2,true);
				if(groupPressed){
					groupPressed(i);
				}
			}
		}
	}
};
#endif