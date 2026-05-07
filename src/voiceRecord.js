//
// enable mediaRecorder, get access
// start recording
// audio stream, chunks send to 
// once stop, reassemble the voice file from chunks
// send to js


let recorder;
let chunks = [];

async function startRecording() {
    const stream = await navigator.mediaDevices.getUserMedia({ audio: true});

    recorder = new MediaRecorder(stream);
    recorder.ondataavailable = (e) => {
        chunks.push(e.data);
    }

    // once stop, collect chunks
    // send to backend for logic handle
    recorder.onstop = async () => {
        const audioBlob = new Blob(chunks, { type: "audio/webm"});
        const formData = new FormData();
        formData.append("audio", audioBlob);

        
    }


}