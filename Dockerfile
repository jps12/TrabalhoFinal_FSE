FROM node:17-slim

WORKDIR /central-server/

COPY ./install.sh /central-server/install.sh

RUN chmod 777 /central-server/install.sh

RUN /central-server/install.sh

CMD npm install && npm start
