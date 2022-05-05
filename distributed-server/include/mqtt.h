#ifndef MQTT_H
#define MQTT_H

void mqtt_start();
void mqtt_envia_mensagem(char *message, char *topic);
void mqtt_envia_mac(char *mac_address);
void mqtt_envia_estado_botao(int estado_led);

#endif