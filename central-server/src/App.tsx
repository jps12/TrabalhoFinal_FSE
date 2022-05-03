import React from 'react';
import logo from './logo.svg';
import './App.css';
import { Message } from 'paho-mqtt';

import mqttClient from './clients/mqtt';

function App() {

  const devicesTopic = "fse2021/170141161/dispositivos/";

  // using paho-mqtt lib

  mqttClient.connect({onSuccess: () => {
    console.log("onConnect");
    mqttClient.subscribe(devicesTopic);

    let message = new Message("Im the central server !");
    message.destinationName = devicesTopic;
    mqttClient.send(message);

    return;
  }});

  mqttClient.onMessageArrived = (message) => {
    console.log("onMessageArrived: "+message.payloadString);
    return;
  };

  // using mqtt lib
  // mqttClient.on('connect', () => {
  //   mqttClient.subscribe(devicesTopic, (err, granted) => {
  //     if(err){
  //       console.log('MQTT ERROR: couldnt subscribe to ' + devicesTopic);
  //       return;
  //     }

  //     console.log('MQTT: successfully subscribed to ' + devicesTopic);

  //     mqttClient.publish(devicesTopic, 'Im the central server !');

  //     return;
  //   })
  // });

  // mqttClient.on('message', (topic, message) => {
  //   console.log('MQTT: new message -> ', message.toString());
  //   console.log('\tfrom topic ', topic.toString());

  //   return;
  // });

  return (
    <div className="App">
      <header className="App-header">
        <img src={logo} className="App-logo" alt="logo" />
        <p>
          Edit <code>src/App.tsx</code> and save to reload.
        </p>
        <a
          className="App-link"
          href="https://reactjs.org"
          target="_blank"
          rel="noopener noreferrer"
        >
          Learn React
        </a>
      </header>
    </div>
  );
}

export default App;
