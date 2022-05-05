#ifndef MQTT_H
#define MQTT_H

void mqtt_start();
void mqtt_send_message(char *message, char *topic);

#endif