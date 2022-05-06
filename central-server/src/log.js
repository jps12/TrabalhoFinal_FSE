import fs from 'fs';

const LOG_FILE = 'logs.csv'

export default function writeDataToCsvLog(config) {
   // const { id, room, modeName, action, value } = config;
   const date = new Date().toString();
   //   const log = ${date}, ${id}, ${room}, ${modeName}, ${action}, ${value}\n;
   const log = `${date}, ${config}`;
   fs.writeFile(LOG_FILE, log, { flag: 'a+' }, () => {});

   return;
}