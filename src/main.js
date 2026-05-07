// main.js - serial port event handler for Arduino/ESP32
/*
1. set up connection
2. event listener
3. event handler
4. send back response
*/

import { SerialPort } from 'serialport'
import { ReadlineParser } from '@serialport/parser-readline'
import { fetchDate, fetchWeather } from './fetchDate'

// serial port info
// transfer baud rate
const SERIAL_PATH = '/dev/cu.usbserial-xxxx'
const BAUD_RATE = 115200

// create my board
const port = new SerialPort({
  path: SERIAL_PATH,
  baudRate: BAUD_RATE,
  autoOpen: false
})

// parse messages from the board
const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }))

// connecting to the board
function openPort() {
    return new Promise((resolve, reject) => {
        port.open((err) => {
            if (err) {
                reject(new Error('Failed to open serial port: ' + err.message));
            } else {
                console.log('Serial port opened:', SERIAL_PATH);
                resolve();
            }
        });
    });
}

// event listener call back for parser.on
//needs work -- currently placeholder
async function handleInput(line) {
    console.log(line);
    try {
        const msg = JSON.parse(line);

        //voice interaction is button 3
        if (msg.type === "voice") {
            console.log("still need to do, once done will sendCommand");
            sendCommand({ action: 'voice' });
            //await sendCommand(voice());
        }

        //date and weather is button 1  
        if (msg.type === "date") {
            await sendCommand(fetchDate());
            await sendCommand(fetchWeather());

        }

        //todo list is button 2
        if (msg.type === "todo") {
            //await sendCommand(fetchTodo());
            console.log("currently toDo is a python file, need it to be JS")
        }
    } catch (e) {
        console.log("Invalid JSON:", line);
    }
}

// send response back to port
//command should be a Promise
async function sendCommand(command) {
    try{
        const resolved = await command;
        const payload = typeof resolved === 'string' ? resolved : JSON.stringify(resolved);

        port.write(payload + '\n', (err) => {
            if (err) {
                console.error('Write error:', err.message)
            }
        })
    } catch(err) {
        console.error("Command failed: ", err);
    }
}

function portConnectionCheck() {
    port.on('close', () => {
        console.log("port closed (ESP32 disconnected)");
    });
    port.on('error', (err) => {
        console.log("serial error", err.message);
    })
};

// main logic
async function main() {
    await openPort();
    portConnectionCheck();
    parser.on('data', handleInput);
}

main();