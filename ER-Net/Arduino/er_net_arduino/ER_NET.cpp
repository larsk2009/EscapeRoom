#include "ER_NET.h"

// ethernet mac address - must be unique on your network
byte ErNet::mac[] = { 0x70,0x69,0x69,0x2D,0x30,0x31 };
byte ErNet::controlUnitIP[4];

EthernetUDP udp;
EthernetClient tcpSender;
//EthernetServer tcpReceiver(46667);

void ErNet::Setup() {
    Serial.begin(115200);

    Ethernet.begin(mac);
    //tcpReceiver.begin();
    int success = udp.begin(discoveryPort);
    Serial.print("initialize: ");
    Serial.println(success ? "success" : "failed");
    Serial.println(Ethernet.localIP());
}

void ErNet::Loop() {
    //check for new udp-packet:
    int size = udp.parsePacket();
    if (size > 0) {
        //Serial.println(size);
        do
            {
            char* msg = (char*)malloc(size + 1);
            int len = udp.read(msg, size + 1);
            msg[len] = 0;
            // Serial.print("received: '");
            // Serial.println(msg);
            udpSerialReceived(udp.remoteIP(), msg, size);
            free(msg);
            }
        while ((size = udp.available()) > 0);
        //finish reading this packet:
        udp.flush();
        // Serial.println("'");
        // Serial.print("remote ip: ");
        // Serial.println(udp.remoteIP());
        // Serial.print("remote port: ");
        // Serial.println(udp.remotePort());

        udp.stop();

        //restart with new connection to receive packets from other clients
        Serial.print("restart connection: ");
        Serial.println (udp.begin(discoveryPort) ? "success" : "failed");
    }
   /*  if(EthernetClient client = tcpReceiver.available()) {
        size_t size;
        while((size = client.available()) > 0) {
            char *msg = (char*)malloc(size);
            size = client.read(msg, size);
            Serial.println("Received tcp message:");
            Serial.write(msg, size);
            free(msg);
        }
        client.stop();
    } */
}

//callback that prints received packets to the serial port
void ErNet::udpSerialReceived(IPAddress ip, char *data, uint16_t len){
    StaticJsonDocument<100> doc;
    Serial.println(data + 7);
    DeserializationError err = deserializeJson(doc, data + 7);
    if (err) {
        Serial.print("deserializeJson() failed with code ");
        Serial.println(err.c_str());
    } else {
        Serial.println("Succesfull deserialization");
        const char* type = doc["MessageType"];
        Serial.println(type);
        if(strcmp(type, "Discovery") == 0) {
            onDiscovery(ip);
        }
    }
}

void ErNet::onDiscovery(IPAddress ip) {
    controlUnitIP[0] = ip[0];
    controlUnitIP[1] = ip[1];
    controlUnitIP[2] = ip[2];
    controlUnitIP[3] = ip[3];

    StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
    doc["Guid"] = "8fb5d28b-6d33-422a-a041-2d11b39c3051";
    doc["MessageType"] = "DiscoveryAcknowledge";

    char json[128];

    serializeJson(doc, json);

    Serial.print("Response: ");
    Serial.println(json);

    Serial.print("Sending to ip ");
    Serial.println(ip);

    //Send string back using tcp
    if(tcpSender.connect(ip, responsePort)) {
        tcpSender.print("ER-NET\n");
        tcpSender.print(json);
        tcpSender.stop();
    } else {
        Serial.println("TCP failed");
    }
}