import { mqttClient, BASE_TOPIC_PATH } from '../clients/mqttClient'

export const ESP_BAT = 1;
export const ESP_ENE = 0;

export class Esp {
   /**
    * 
    * @param {{mac: String, id: Int, room: String, type: ESP_BAT | ESP_ENE, mqttClient: mqttClient}} config 
    */
   constructor(config){
      this.mac          = config.mac         ?? null;
      this.id           = config.id          ?? null;
      this.room         = config.room        ?? '(comodo não informado)';
      this.type         = config.type        ?? ESP_ENE;
      this.mqttClient   = config.mqttClient  ?? null;

      this.roomTopic    = null;
      this.input        = '';
      this.output       = '';
      this.state        = '';
   };

   /**
    * 
    * @param {String} roomName nome do comodo onde a Esp esta
    */
   setRoom(roomName){
      this.room = roomName;
   }

   configMqttClient(){
      this.roomTopic = BASE_TOPIC_PATH + this.room;
      this.mqttClient = mqttClient(this.roomTopic);
   }

   getMac(){
      return this.mac;
   }
}