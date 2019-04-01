#include "Arduino.h"
#include "ER_NET.h"

byte Ethernet::buffer[1000]; // tcp/ip send and receive buffer
// ethernet mac address - must be unique on your network
byte ErNet::mymac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };

void ErNet::Setup() {
    Serial.begin(115200);

    if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
    {
        Serial.println(F("Failed to access Ethernet controller"));
    }
    if (!ether.dhcpSetup()) {
        Serial.println(F("DHCP failed"));
    }

    ether.printIp("IP:  ", ether.myip);
    ether.printIp("GW:  ", ether.gwip);
    ether.printIp("DNS: ", ether.dnsip);

    ether.enableBroadcast(0);

    //register udpSerialPrint() to port 46666
    ether.udpServerListenOnPort(&ErNet::udpSerialPrint, discoveryPort);
}

void ErNet::Loop() {
    ether.packetLoop(ether.packetReceive());
}

//callback that prints received packets to the serial port
void ErNet::udpSerialPrint(uint16_t dest_port, uint8_t src_ip[IP_LEN], uint16_t src_port, const char *data, uint16_t len){
  IPAddress src(src_ip[0],src_ip[1],src_ip[2],src_ip[3]);
  
  Serial.print("dest_port: ");
  Serial.println(dest_port);
  Serial.print("src_port: ");
  Serial.println(src_port);
  
  
  Serial.print("src_port: ");
  ether.printIp(src_ip);
  Serial.println();
  Serial.println("data: ");
  Serial.println(data);
}