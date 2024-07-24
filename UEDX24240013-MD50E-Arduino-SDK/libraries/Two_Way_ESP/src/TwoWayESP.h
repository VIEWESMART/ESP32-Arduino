/* TODO: Make a nice looking header */
#pragma once

#include <esp_now.h>
#include <WiFi.h>

class TwoWayESP {
public:
    bool static Begin(const uint8_t macAddress[6]);
    
    bool static Available() { return m_CanRead; }
    String static GetString() { m_CanRead = false; return m_Output; }
    void static GetBytes(void* dest, uint8_t size) { memcpy(dest, s_IncomingPacket.message, size > 256 ? 256 : size); }

    void static SendString(String input);
    void static SendBytes(void* src, uint8_t size);



private:
    void static cb_OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
    void static cb_OnDataRecv(const uint8_t* mac_addr, const uint8_t* incomingData, int len);
    
    typedef struct Packet {
        uint8_t size = 0;
        unsigned char message[257];
    } Packet;

    static String m_Output;
    static bool m_CanRead;

    static esp_now_peer_info_t m_Peer;
    static Packet s_OutgoingPacket;
    static Packet s_IncomingPacket;
};