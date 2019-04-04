#ifndef ER_NET_H
#define ER_NET_H

#include "Arduino.h"
#include <UIPEthernet.h>
#include <IPAddress.h>
#include <ArduinoJson.h>


class ErNet {
    public:
        void Setup();
        void Loop();

    private:
        static void udpSerialReceived(IPAddress ip, char *data, uint16_t len);
        static void onDiscovery(IPAddress ip);
        
        bool IsSetup = false;
        
        const static int discoveryPort = 46666;
        const static int responsePort = 46667;
        static byte controlUnitIP[4];
        static byte mac[];
};

#endif