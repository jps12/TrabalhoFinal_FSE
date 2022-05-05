#ifndef MQTT_H
#define MQTT_H

void mqtt_start();
void mqtt_send_message(char *message, char *topic);
void mqtt_send_mac(char *mac_address);
void send_estado_botao_mqtt(int estado_led);

#endif