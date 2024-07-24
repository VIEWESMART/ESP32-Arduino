#include <TwoWayESP.h>

// Make this the OTHER ESPs mac address
// The mac address can be found by uploading this program
// It will be output on the serial terminal
const uint8_t otherESPAddr[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


// A struct to organize data to send
// CANNOT BE > 256 BYTES
struct DataToTransmit {
	int someInt;
	float someFloat;
	bool someBool;
	int arr[3];
};

void setup() {
	Serial.begin(115200);

	// Begin TwoWayESP with the other ESPs mac
	TwoWayESP::Begin(otherESPAddr);
}

void loop() {
	// Check if there is an available read
	if(TwoWayESP::Available()) {
		DataToTransmit recv;
		TwoWayESP::GetBytes(&recv, sizeof(DataToTransmit));
		Serial.println("[Incoming] Struct data: ");
		Serial.print("\t INT: "); Serial.println(recv.someInt);
		Serial.print("\t FLOAT: "); Serial.println(recv.someFloat);
		Serial.print("\t BOOL: "); Serial.println(recv.someBool);
		Serial.print("\t ARR: [0] = "); Serial.print(recv.arr[0]);
		Serial.print(", [1] = "); Serial.print(recv.arr[1]);
		Serial.print(", [2] = "); Serial.println(recv.arr[2]);
	}
	
	// Build up a packet to send (Can be from sensor values)
	DataToTransmit send;
	send.someInt = 25;
	send.someFloat = 3.14159265;
	send.someBool = false;
	send.arr[0] = 0;
	send.arr[1] = 1;
	send.arr[2] = 2;

	// Send some bytes to the other ESP
	TwoWayESP::SendBytes(&send, sizeof(DataToTransmit));

	// Delay to not overrun the memory limit
	delay(2000);
}