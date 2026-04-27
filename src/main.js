// main.js - serial port event handler for Arduino/ESP32
/*
1. set up connection
2. event listener
3. event handler
4. send back response
*/

import { SerialPort } from 'serialport'
import { ReadlineParser } from '@serialport/parser-readline'


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
function handleInput(line) {
    console.log(line);
    try {
        const msg = JSON.parse(line);

        if (msg.type === "voice") {
            console.log("voice");
            sendCommand({ action: 'voice' });
        }

        if (msg.type === "date") {
            console.log("date");
            sendCommand({ action: 'date' });
        }

        if (msg.type === "todo") {
            console.log("todo");
            sendCommand({ action: 'todo' });
        }
    } catch (e) {
        console.log("not JSON");
    }
}

// send response back to port
function sendCommand(command) {
    const payload = typeof command === 'string' ? command : JSON.stringify(command)
    port.write(payload + '\n', (err) => {
        if (err) {
            console.error('Write error:', err.message)
        }
    })
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