const express = require('express');
const app = express();
app.use(express.json());

// Shared state
let ledState = { led1: false, led2: false, led3: false };

// Mobile calls this to toggle LEDs
app.post('/api/led', (req, res) => {
  const { led, state } = req.body;  // { led: "led1", state: true }
  ledState[led] = state;
  res.json({ ok: true, ledState });
});

// ESP32 polls this every 500ms
app.get('/api/state', (req, res) => {
  res.json(ledState);
});

// root directory
app.get('/', (req, res) => {
  res.sendFile(__dirname+"/public/index.html");
});

//runs locally on port 3000
app.listen(3000, () => console.log('Server on :3000'));