import { useState, useEffect } from 'react';

import { mqttClient, BASE_TOPIC_PATH } from '../clients/mqttClient'
import { Esp } from '../classes/esp';


/**
 * 
 * @param {Esp} espDevice objeto da classe src/classes/esp
 * @returns 
 */
export const EspInterface = () => {
   const client1 = mqttClient(BASE_TOPIC_PATH);
   const dispositivosClient = mqttClient(BASE_TOPIC_PATH + 'dispositivos');
   const [espTest, setEspTest] = useState(new Esp({ id: 1, mac: 'abc', mqttClient: client1, room: 'sala', type: 1 }));

   useEffect(() => {
      try {
         var message = JSON.parse(dispositivosClient.message);
         console.log(`message: ${message.mac}`);
         espTest.mac = message.mac;
         setEspTest(espTest);
      } catch (e) {
         console.log(e);
      }
   }, [dispositivosClient.message]);


   const updateVariables = () => {

      espTest.mqttClient.client.subscribe(
         BASE_TOPIC_PATH + espTest.room + '/temperatura'
      );

      espTest.mqttClient.client.on('message', (topic, payload, packet) => {
         const newMessage = payload.toString();
         if (topic === (BASE_TOPIC_PATH + espTest.room + '/temperatura')) {
            console.log(`nova temperatura: ${newMessage}`);
            espTest.output = newMessage;
            return;
         }
      });


      espTest.mqttClient.client.subscribe(
         BASE_TOPIC_PATH + espTest.room + '/umidade'
      );

      espTest.mqttClient.client.on('message', (topic, payload, packet) => {
         const newMessage = payload.toString();
         if (topic === (BASE_TOPIC_PATH + espTest.room + '/umidade')) {
            console.log(`nova umidade: ${newMessage}`);
            espTest.input = newMessage;
            return;
         }
      });


      espTest.mqttClient.client.subscribe(
         BASE_TOPIC_PATH + espTest.room + '/estado'
      );
      const lastState = espTest.state;
      espTest.mqttClient.client.on('message', (topic, payload, packet) => {
         const newMessage = payload.toString();
         if (topic === (BASE_TOPIC_PATH + espTest.room + '/estado')) {
            console.log(`nova estado: ${newMessage}`);
            espTest.state = newMessage;
            return;
         }
      });

   }


   setInterval(() => {
      updateVariables()
      console.log(`nova temperatura: ${espTest.output}`);
   }, 1000 * 10)

   return (
      <div>
         Novo dispositivo encontrado: ESP MAC ADDR {dispositivosClient.message}
         <h3>Olá, sou a esp de MAC = {espTest.mac}</h3>
         <table>
            <thead>
               <tr>
                  <th>Umidade</th>
                  <th>Temperatura</th>
                  <th>Estado</th>
               </tr>
            </thead>

            <tbody>
               <tr>
                  <td>{espTest.input}</td>
                  <td>{espTest.output}</td>
                  <td>{espTest.state}</td>
               </tr>
            </tbody>

         </table>
      </div>
   );
};
