#ifndef ER_NET_H
#define ER_NET_H

#include "Arduino.h"
#include <UIPEthernet.h>
#include <IPAddress.h>
#include <ArduinoJson.h>


class ErNet {
    public:
        void                Setup(const char* _puzzleName, char mac_0, char mac_1, char mac_2, char mac_3, char mac_4, char mac_5);
        void                Loop();

        void                SetSolved(bool isSolved);
        bool                GetDisplayNumber(int *number);
        bool                GetSolution(int *solution);

        void                SetResetCallback(void (*callback)(void));
        static byte         mac[];

    private:
        void                ParseUdpMessage(IPAddress ip, char *data, uint16_t len);
        static void         onDiscovery(IPAddress ip);
        void                ParseTcpMessage(char *message, uint16_t len);
        
        bool                IsSetup = false;
        bool                receivedDisplayNumber = false;
        bool                receivedSolution = false;
        unsigned int        displayNumber;
        unsigned int        solution;

        void                (*ResetCallback)(void);
        bool                IsResetCallbackSet = false;
        
        const static int    communicationPort = 46667;
        static byte         controlUnitIP[4] = {0, 0, 0, 0};
};

#endif