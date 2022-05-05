/* eslint-disable react-hooks/rules-of-hooks */
import { useEffect, useState } from 'react';
import mqtt from 'mqtt/dist/mqtt';

/**
 * 
 * @param {String} topic : topico para se inscrever no broker
 * @returns status da conexao e array de mensagens
 */
export function mqttClient(topic) {
  const [connectionStatus, setConnectionStatus] = useState(false);
  const [messages, setMessages] = useState([]);
  const [subscribed, setSubscribed] = useState(false);
  const [client, setClient] = useState(null);

  const brokerURI = 'ws://127.0.0.1:8083/mqtt';

  setClient(mqtt.connect(brokerURI));

  useEffect(
    () => {
      if(client !== null ){
        client.on('connect', () => {
          setConnectionStatus(true);
          subscribeTo(topic);
        });
  
        client.on('message', (topic, payload, packet) => {
          const newMessage = payload.toString();
          setMessages(messages.concat(newMessage));
  
          console.log(`MQTT: nova mensagem de ${topic} : ${newMessage}`);
        });
      }
    // eslint-disable-next-line react-hooks/exhaustive-deps
    }, [messages]);

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
    messages,
    subscribed
  };
};

// '/fse2021/180033743/sala/estado'