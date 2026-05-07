import * as readline from 'readline' // read input from terminal

// ── Ask (terminal input → chat → display) ──
export async function fetchResponse() {
    // 1. ask input in terminal
    const question = await promptTerminal('Enter your question >>> ')
    if (!question) return { line1: 'No input', line2: '' }

    // 2. send to chat
    const answer = await askChat(question)
    return answer;
}

// terminal handler
function promptTerminal(prompt) {
    // display prompt, read input, send out
    const rl = readline.createInterface({ input: process.stdin, output: process.stdout })
    return new Promise(resolve => {
        rl.question(prompt, input => {
            rl.close() // once resolve close readline
            resolve(input.trim())
        })
    })
}

async function askChat(question) {
    // fetching response
    // const res = await fetch('https://api.openai.com/v1/chat/completions', {
    //     method: 'POST', // post method to send prompt
    //     headers: {
    //         "Authorization": `Bearer ${process.env.OPENAI_API_KEY}`,
    //         "Content-Type": "application/json"
    //     },
    //     body: JSON.stringify({
    //         model: "gpt-4o-mini", // model name
    //         messages: [{ 
    //             role: 'system', content: "回答控制在20字以内, 只给结论"
    //         }, {
    //             role: 'user', content: question 
    //         }], // actual questions here
    //         max_tokens: 60,
    //     })
    // })

    
    // fake response for testing
    const text = {
        id: "chatcmpl-mock",
        object: "chat.completion",
        created: 1710000000,
        model: "gpt-4o-mini",
        choices: [{
            index: 0,
            message: {
                role: "assistant",
                content: "It looks like a good day to go outside!"
            },
            finish_reason: "stop"
        }]
    }

    const res = {
        ok: true,
        status: 200,
        statusText: "OK",
        json: async () => (text)
    }
    

    if (!res.ok) throw new Error(`Claude error: ${res.statusText}`) // fetch error handling
    const data = await res.json() // format into JSON
    return data.choices?.[0]?.message?.content?.trim() ?? 'No answer' 
    // choice is response, first one, get message and content
    // trim turn into string
}

/* 
testing
const res = await fetchResponse();
console.log(res);
*/