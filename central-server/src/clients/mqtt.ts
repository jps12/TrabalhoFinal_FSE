// import mqtt from 'precompiled-mqtt';
// import * as mqtt from 'mqtt';
import * as mqtt from 'paho-mqtt';

// export const mqttClient = mqtt.connect('mqtt://test.mosquitto.org');

const mqttConfig = {
   uri: 'test.mosquitto.org',
   port: 80, // WebSockets port
   path: '',
   clientId: '170141161-centralServer'
};

const mqttClient = new mqtt.Client(
  mqttConfig.uri,
  mqttConfig.port,
  mqttConfig.path,
  mqttConfig.clientId
);

const devicesTopic = "fse2021/170141161/dispositivos/";

// // using paho-mqtt lib

// mqttClient.connect({onSuccess: () => {
//    console.log("onConnect");
//    mqttClient.subscribe(devicesTopic);
//    let message = new mqtt.Message("Im the central server !");
//    message.destinationName = devicesTopic;
//    mqttClient.send(message);
// }});

// mqttClient.onMessageArrived = (message) => {
//    console.log("onMessageArrived: "+message.payloadString);
// };

export default mqttClient;