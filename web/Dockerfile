FROM node:lts

WORKDIR /node/app

COPY . .

RUN npm i

RUN make -C ./compiler

CMD [ "npm", "run", "start" ]