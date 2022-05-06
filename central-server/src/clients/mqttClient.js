/* eslint-disable react-hooks/rules-of-hooks */
import { useEffect, useState } from 'react';
import mqtt from 'mqtt/dist/mqtt';

export const BASE_TOPIC_PATH = '/fse2021/180033743/';

/**
 * 
 * @param {String} topic : topico para se inscrever no broker
 * @returns status da conexao e array de mensagens
 */
export function mqttClient(topic) {
  const [connectionStatus, setConnectionStatus] = useState(false);
  const [message, setMessage] = useState([]);
  const [subscribed, setSubscribed] = useState(false);
  // const [client, setClient] = useState(null);

  const brokerURI = 'ws://127.0.0.1:8083/mqtt';

  const client = mqtt.connect(brokerURI);

  client.on('connect', () => {
    setConnectionStatus(true);
    subscribeTo(topic);
  });

  useEffect(
    () => {
      if(client !== null ){
        client.on('message', (topic, payload, packet) => {
          const newMessage = payload.toString();
          setMessage(newMessage);
  
          console.log(`MQTT: nova mensagem de ${topic} : ${newMessage}`);
        });
      }
    
    }, [message]);

    /**
   * 
   * @param {String} givenTopic 
   */
  function subscribeTo(givenTopic){
    client.subscribe(givenTopic, function (err) {
      if (err) {
        console.log(`MQTT ERROR: falha ao tentar inscrever-se no
        topico ${givenTopic} com erro = ${err}`);

        return;
      }

      setSubscribed(true);
    });
  };

  /**
   * 
   * @param {String} givenTopic 
   * @param {String} givenMessage 
   * @returns 
   */
  function sendMessageTo(givenTopic, givenMessage){
    client.publish(givenTopic, givenMessage, 0, (err) => {
      console.log(`MQTT ERROR: falha ao enviar mensagem=${givenMessage} 
      para o topico ${givenTopic} com erro = ${err}`);

      return;
    });

    return;
  }

  return {
    connectionStatus,
    message,
    subscribed,
    client
  };
};

// '/fse2021/180033743/sala/estado'