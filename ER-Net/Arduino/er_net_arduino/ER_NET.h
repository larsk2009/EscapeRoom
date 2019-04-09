#ifndef ER_NET_H
#define ER_NET_H

#include "Arduino.h"
#include <UIPEthernet.h>
#include <IPAddress.h>
#include <ArduinoJson.h>


class ErNet {
    public:
        void                Setup();
        void                Loop();

        void                SetSolved(bool isSolved);
        bool                GetDisplayNumber(int *number);

    private:
        static void         udpSerialReceived(IPAddress ip, char *data, uint16_t len);
        static void         onDiscovery(IPAddress ip);
        static void         ParseTcpMessage(char *message, uint16_t len);
        
        bool                IsSetup = false;
        
        const static int    discoveryPort = 46666;
        const static int    responsePort = 46667;
        static byte         controlUnitIP[4] = {0, 0, 0, 0};
        static byte         mac[];
};

#endif