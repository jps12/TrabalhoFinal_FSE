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
   const [espTest, setEspTest] = useState(new Esp({id: 1, mac: 'abc', mqttClient: client1, room: 'sala', type: 1}));

   //test
   let message = JSON.stringify({ mac:"novoMAC"});
   dispositivosClient.client.publish(BASE_TOPIC_PATH + 'dispositivos', message);
   // end test

   useEffect(() => {
      // console.log(`esp: ${espTest}`);
      try {
         var message = JSON.parse(dispositivosClient.message);
         console.log(`message: ${message.mac}`);
         espTest.mac = message.mac;
         setEspTest(espTest);
      } catch (e) {
         console.log(e);
      }
   // eslint-disable-next-line react-hooks/exhaustive-deps
   }, [dispositivosClient.message]);

   espTest.mqttClient.client.subscribe(
      BASE_TOPIC_PATH + espTest.room + '/temperatura'
   );

   espTest.mqttClient.client.on('message', (topic, payload, packet) => {
      const newMessage = payload.toString();
      if(topic === (BASE_TOPIC_PATH + espTest.room + '/temperatura')){
         console.log(`nova temperatura: ${newMessage}`);
         espTest.output = newMessage;
         return;
      }
    });
   
   setInterval(() => {
      console.log(`nova temperatura: ${espTest.output}`);
   }, 1000*10)

   return (
      <div>
         Novo dispositivo encontrado: ESP MAC ADDR {dispositivosClient.message}
         <h3>Olá, sou a esp de MAC = {espTest.mac}</h3>
         <table>
            <thead>
               <tr>
                  <th>Entrada</th>
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

// export default EspInterface;