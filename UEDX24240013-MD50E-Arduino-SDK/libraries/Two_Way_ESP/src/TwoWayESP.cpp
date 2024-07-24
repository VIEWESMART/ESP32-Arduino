/* TODO: Make a nice looking header */

#include "TwoWayESP.h"
#include <Arduino.h>

String TwoWayESP::m_Output = "";
bool TwoWayESP::m_CanRead = false;


esp_now_peer_info_t TwoWayESP::m_Peer;
TwoWayESP::Packet TwoWayESP::s_OutgoingPacket;
TwoWayESP::Packet TwoWayESP::s_IncomingPacket;

bool TwoWayESP::Begin(const uint8_t macAddress[6]) {
    // Set WiFi mode to STA
	WiFi.mode(WIFI_STA);
    Serial.print("My mac address: ");
    Serial.println(WiFi.macAddress());
    

	// Init ESP-NOW
	if (esp_now_init() != ESP_OK){
		Serial.println("[ESPNOW] Error: Could not init!!!");
		return false;
	}

	// Register the send cb
	esp_now_register_send_cb(cb_OnDataSent);
    esp_now_register_recv_cb(cb_OnDataRecv);

    m_Peer = esp_now_peer_info_t();
	memcpy(m_Peer.peer_addr, macAddress, 6);
	m_Peer.channel = 0;
	m_Peer.encrypt = false;

	if(esp_now_add_peer(&m_Peer) != ESP_OK) {
		Serial.println("[ESPNOW] Failed to add peer. Is your MAC address correct?");
		return false;
	}

    s_OutgoingPacket.size = 0;
    memset((void*)s_OutgoingPacket.message, 0, sizeof(s_OutgoingPacket.message));  

    return true;
}


void TwoWayESP::SendBytes(void* src, uint8_t size) {
    s_OutgoingPacket.size = size > 256 ? 256 : size;
    memset(s_OutgoingPacket.message, 0, sizeof(s_OutgoingPacket.message));
    memcpy(s_OutgoingPacket.message, src, s_OutgoingPacket.size);

	esp_err_t result = esp_now_send(m_Peer.peer_addr, (uint8_t*)&s_OutgoingPacket, sizeof(uint8_t) + size);
}

void TwoWayESP::SendString(String input) {
    uint8_t size = input.length() + 1;
    s_OutgoingPacket.size = size;

    memset(s_OutgoingPacket.message, 0, sizeof(s_OutgoingPacket.message));
    input.getBytes(s_OutgoingPacket.message, input.length());

	esp_err_t result = esp_now_send(m_Peer.peer_addr, (uint8_t*)&s_OutgoingPacket, sizeof(uint8_t) + size);

    if (result != ESP_OK) {
        Serial.println("[TwoWayESP] There was an error during send!");
    }
}

void TwoWayESP::cb_OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
	if (status != ESP_NOW_SEND_SUCCESS) {
        Serial.println("[TwoWayESP](cb) There was an error during send!");
    }
}

void TwoWayESP::cb_OnDataRecv(const uint8_t* mac_addr, const uint8_t* incomingData, int len) {

	if (m_CanRead) {
        return;
    }
    
    // Get size of incoming message
    memset(&s_IncomingPacket, 0, sizeof(s_IncomingPacket));
    memcpy(&s_IncomingPacket, incomingData, len);

    m_Output = "";
	s_IncomingPacket.size = *incomingData;
    for (uint8_t i = 0; i < s_IncomingPacket.size; i++) {
        char curChar = s_IncomingPacket.message[i];
        m_Output += String(curChar);
    }
    m_CanRead = true;
}