// main.js - serial port event handler for ESP32 communication
/*
1. set up connection
2. event listener
3. event handler
4. send back response
*/

import 'dotenv/config' // to read private api key, more secure
import { SerialPort } from 'serialport'
import { ReadlineParser } from '@serialport/parser-readline' // parse serialport message
import { handleDate, handleTodo, handleAsk } from './interaction.js'

const SERIAL_PATH = '/dev/cu.usbserial-xxxx'  // makerbroad path
const BAUD_RATE = 115200

// initialize serialport connection
const port = new SerialPort({ path: SERIAL_PATH, baudRate: BAUD_RATE, autoOpen: false })
// parse serial port message by line
const parser = port.pipe(new ReadlineParser({ delimiter: '\r\n' }))

// open port for transfer
function openPort() {
    return new Promise((resolve, reject) => {
        port.open(err => err
            ? reject(new Error('Failed to open port: ' + err.message))
            : resolve()
        )
    })
}

// convert command (obj) to JSON
// write to makerbroad, send response back
function sendCommand(command) {
    const payload = JSON.stringify(command)
    port.write(payload + '\n', err => {
        if (err) console.error('Write error:', err.message)
        else console.log('[→ ESP32]', payload)
    })

    // testing
    // console.log(payload);
}

// while waiting, display loading message
function sendLoading(hint) {
    sendCommand({ action: 'loading', hint })
}


// different response based on different message from makerbroad
async function handleInput(line) {
    console.log('[← ESP32]', line)
    // parse the JSON from makerbroad
    let msg
    try { msg = JSON.parse(line) } catch { return }

    // if date, first load
    // get result, send response back, or error message
    if (msg.type === 'date') {
        sendLoading('Fetching INFO...')
        try {
            const result = await handleDate()
            sendCommand({ action: 'date', ...result }) // include everything in result, key: value
        } catch (e) {
            // error message limited to only 13, due to size constrains
            sendCommand({ action: 'date', line1: 'Error', line2: e.message.slice(0, 13) })
        }
    }

    // if todo, first load
    // get result, send response back, or error message
    if (msg.type === 'todo') {
        sendLoading('Loading TODO...')
        try {
            const result = await handleTodo()
            sendCommand({ action: 'todo', ...result })
        } catch (e) {
            sendCommand({ action: 'todo', line1: 'Error', line2: e.message.slice(0, 13) })
        }
    }

    if (msg.type === 'ask') {
        sendLoading('Waiting Response...')
        try {
            const result = await handleAsk()
            sendCommand({ action: 'ask', ...result })
        } catch (e) {
            sendCommand({ action: 'ask', line1: 'Error', line2: e.message.slice(0, 13) })
        }
    }
}

// check if port connection is on or closed
function portConnectionCheck() {
    port.on('close', () => console.log('port closed'))
    port.on('error', err => console.error('serial error:', err.message))
}

// basic logic of monitoring
async function main() {
    await openPort()
    console.log('Serial port opened:', SERIAL_PATH)
    portConnectionCheck()
    parser.on('data', handleInput)

    // testing
    // const fakeInput = JSON.stringify({
    //     type: "date", // 3 types to check, "date", "todo", "ask"
    //     device: "esp32",
    //     timestamp: 1710000000,
    //     payload: {
    //         question: "what is weather?"
    //     }
    // })
    // handleInput(fakeInput)
}

main()