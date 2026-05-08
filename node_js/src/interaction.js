// interaction.js - response handling logic
// trigger, run fetch, format the result return

import { fetchDate, fetchWeather } from './fetchDate.js'
import { execFile } from 'child_process' // to execute external file
import { promisify } from 'util'
import { fetchResponse } from './textHandler.js'

const execFileAsync = promisify(execFile)
// const OLED_MAX = 21

// ST7735 landscape 160x128, setTextSize(2): 13 char per line
const TFT_MAX = 13

// ── Date + Weather ──
export async function handleDate() {
    // parallel handling
    const [dateInfo, weather] = await Promise.all([fetchDate(), fetchWeather()])

    // hard code for testing
    // const dateInfo = { time: '11:41 AM', date: '05/07/2026' }
    // const weather = { celsius: 13.4, fahrenheit: 56.12 }


    // format into two lines
    // date, time
    // weather F, C
    return {
        line1: truncate(`${dateInfo.date}  ${dateInfo.time}`),
        line2: truncate(`${weather.fahrenheit}F / ${weather.celsius}C`)
    }
}

// testing handleDate
// const res = await handleDate();
// console.log(res)

// ── Todo ──
export async function handleTodo() {
    const pythonPath = '.venv/bin/python3';
    const { stdout } = await execFileAsync(pythonPath, ['fetchTodo.py']) // execute py file into output
    let tasks = []
    // parse JSON
    // if error return message
    try { tasks = JSON.parse(stdout) } catch {
        return { line1: 'Todo error', line2: '' }
    }

    // filter out two unfinished todo
    const pending = tasks.filter(t => t.status !== 'completed').slice(0, 2)
    // if none, all done message
    if (pending.length === 0) return { line1: 'All done!', line2: '' } 

    // return name of the first two todo
    return {
        line1: truncate(pending[0]?.title ?? ''),
        line2: truncate(pending[1]?.title ?? '')
    }
}

// ── Ask (terminal input → Claude → display) ─────────────────
export async function handleAsk() {
    const response = await fetchResponse(); // get response

    // fake response for testing

    return wrapToDisplay(response); // format into 
}

// testing handleAsk
// const res = await handleAsk();
// console.log(res)

// text formatter
function wrapToDisplay(text) {
    const words = text.split(' ') // split by space
    let line1 = '', line2 = '' // two line placeholder

    // fill in two lines
    for (const w of words) {
        if (line1.length + w.length + 1 <= TFT_MAX)
            line1 += (line1 ? ' ' : '') + w
        else if (line2.length + w.length + 1 <= TFT_MAX)
            line2 += (line2 ? ' ' : '') + w
        else break
    }
    return { line1, line2 }
}

// make sure each line is under width
function truncate(str) {
    return str.slice(0, TFT_MAX)
}