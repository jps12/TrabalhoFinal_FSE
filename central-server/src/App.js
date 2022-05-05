import './App.css';

// TRYING MQTT

import React, { useEffect } from 'react';
import mqtt from 'mqtt/dist/mqtt';

function App () {
  const [connectionStatus, setConnectionStatus] = React.useState(false);
  const [messages, setMessages] = React.useState([]);

  useEffect(() => {
    const client = mqtt.connect('ws://127.0.0.1:8083/mqtt');
    client.on('connect', () => {
      setConnectionStatus(true);

      client.subscribe('/fse2021/180033743/sala/estado', function (err) {
        if (!err) {
          client.publish('/fse2021/180033743/sala/estado', 'Hello my friend')
        }
      });
    });
    client.on('message', (topic, payload, packet) => {
      setMessages(messages.concat(payload.toString()));

      console.log(payload.toString())
      client.end();
    });
  }, []);

  return (
    <>
    {
      messages.map((message) => (
        <h2>{message}</h2>
      ))
    }
      </>
  );
}

export default App;