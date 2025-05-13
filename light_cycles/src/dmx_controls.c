#include"serialib.h"
#include "titan_tube.h"
#include "dmx_controls.h"



int open_dmx_port(serialib *serial, const char *port) {
    if (serial->openDevice(port, 57600) != 1) {
        printf("Erreur d'ouverture du port DMX\n");
        return -1;
    }
    return 0;
}


// 0 0 0 0 0 0 
// HHHH aaaa bbbb c ccc d  ddd ... 0xE7
// 0123 4567 8    12    16

void send_dmx_data(serialib *serial, TitanTube *tubes, size_t num_tubes) {
    unsigned char message[DMX_CHANNELS + 5]; // 512 canaux + 5 octets header et footer
    message[0] = 0x7E; // Début de message
    message[1] = 0x06; // Label
    message[2] = 0x19; // Longueur (low byte)
    message[3] = 0x00; // Longueur (high byte)

    // Canaux DMX avec couleurs des tubes
    size_t current_tube = 0;
    for (size_t i = 0; i < num_tubes * 4; i+=4) {
        message[4 + i + 0] = tubes[current_tube].color.r; // Canal 1 - Rouge
        message[4 + i + 1] = tubes[current_tube].color.g; // Canal 2 - Vert
        message[4 + i + 2] = tubes[current_tube].color.b; // Canal 3 - Bleu
        message[4 + i + 3] = 0; // Canal 3 - White
        printf("%zu : %d, %d, %d \n", current_tube, message[i], message[i+1], message[i+2]);
        // printf("%zu \n",  4 + i);
        current_tube++;
    }

    message[DMX_CHANNELS + 4] = 0xE7; // Fin de message

    // Envoi des données DMX
    serial->writeBytes(message, sizeof(message));
}

// Ferme le port série après l'envoi des données
void close_dmx_port(serialib *serial) {
    serial->closeDevice();
}

