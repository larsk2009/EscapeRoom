#ifndef ER_NET_H
#define ER_NET_H

#include "Arduino.h"
#include <EtherCard.h>
#include <IPAddress.h>


class ErNet {
    public:
        void Setup();
        void Loop();

    private:
        static void udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len);\
        
        const static int discoveryPort = 46666;
        bool IsSetup = false;
        static byte mymac[];
};

#endif