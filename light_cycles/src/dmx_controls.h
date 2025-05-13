// dmx_controls.h

#ifndef SERIAL_DMX_H
#define SERIAL_DMX_H

#include "serialib.h"

#define DMX_CHANNELS 25

int open_dmx_port(serialib *serial, const char *port);
//int init_dmx(serialib *serial, const char *port_name);
void close_dmx_port(serialib *serial);
void send_dmx_data(serialib *serial, TitanTube *tubes, size_t num_tubes);

#endif

