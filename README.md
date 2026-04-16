# ESP32 IoT Switch Controller

A lightweight IoT system for controlling 3 physical switches remotely via a mobile web interface — built with an ESP32 microcontroller and a Node.js backend.

---

## Overview

The ESP32 polls a local Node.js server every 2 seconds and reflects the latest switch states on its GPIO pins. A mobile-friendly web UI lets you toggle any of the 3 switches in real time from your phone or browser.

```
[ Mobile UI ] ──POST──▶ [ Node.js Server ] ◀──GET poll── [ ESP32 ]
                              (state store)                    │
                                                         [GPIO Pins]
                                                       SW1 | SW2 | SW3
```

---

## Features

- **3-channel switch control** — independently toggle each switch from the UI
- **ESP32 polling** — device checks server state every 2 seconds via HTTP GET
- **Mobile web UI** — served directly from the Node.js server, no app install needed
- **Zero dependencies on the device** — ESP32 uses plain HTTP, no MQTT broker required
- **Stateful server** — server holds the current switch state in memory; ESP32 always converges to it

---

## Project Structure

```
esp32-iot-switch/
├── server.js           # Express server — state store + API endpoints
├── public/
│   └── index.html      # Mobile web UI
├── firmware/
│   └── switch_controller.ino   # ESP32 Arduino sketch
├── package.json
└── README.md
```

---

## How It Works

### Server (`server.js`)

Two endpoints:

| Method | Endpoint     | Description                          |
|--------|--------------|--------------------------------------|
| `GET`  | `/api/state` | Returns current state of all 3 switches (polled by ESP32) |
| `POST` | `/api/led`   | Accepts `{ led, state }` — updates a switch state |
| `GET`  | `/`          | Serves the mobile UI                 |

State is stored in-memory as a simple object:
```js
{ led1: false, led2: false, led3: false }
```

### ESP32 Firmware (`firmware/switch_controller.ino`)

- Connects to Wi-Fi on boot
- Every 2 seconds, sends `GET /api/state` to the server
- Parses the JSON response and drives 3 GPIO output pins accordingly

### Mobile UI (`public/index.html`)

- Single-page interface with 3 toggle switches
- Each toggle fires a `POST /api/led` with the switch name and new state
- No frameworks — plain HTML/CSS/JS

---

## Getting Started

### Prerequisites

- Node.js v16+
- Arduino IDE with ESP32 board support
- ESP32 dev board
- Both the server machine and ESP32 on the **same local network**

### 1. Clone the repo

```bash
git clone https://github.com/YOUR_USERNAME/esp32-iot-switch.git
cd esp32-iot-switch
```

### 2. Start the server

```bash
npm install
node server.js
# Server running on http://localhost:3000
```

### 3. Flash the ESP32

Open `firmware/switch_controller.ino` in Arduino IDE.

Update these lines with your network details:
```cpp
const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* serverIP = "192.168.x.x";  // Your machine's local IP
```

Flash to the ESP32. Open Serial Monitor at 115200 baud to verify connection.

### 4. Open the UI

On your phone or browser (connected to the same Wi-Fi):
```
http://<your-machine-ip>:3000
```

Toggle switches — the ESP32 will reflect changes within 2 seconds.

---

## API Reference

### `GET /api/state`
Returns the current switch states.

**Response:**
```json
{
  "led1": false,
  "led2": true,
  "led3": false
}
```

### `POST /api/led`
Updates the state of one switch.

**Request body:**
```json
{
  "led": "led1",
  "state": true
}
```

**Response:**
```json
{
  "ok": true,
  "ledState": {
    "led1": true,
    "led2": true,
    "led3": false
  }
}
```

---

## Notes & Limitations

- **State is in-memory** — restarting the server resets all switches to `false`
- **No authentication** — anyone on the local network can control the switches; not intended for public deployment as-is
- **Polling delay** — there's up to a 2-second lag between a UI toggle and the ESP32 reacting
- **Local network only** — server runs on `localhost:3000`; remote access requires port forwarding or deployment (planned)

---

## Planned Improvements

- [ ] Persist state to a file or SQLite so server restarts don't reset switches
- [ ] Deploy server (Railway / Render / VPS) for remote access
- [ ] Replace polling with WebSockets for instant response
- [ ] Add basic password protection to the UI
- [ ] Named switches (editable labels in the UI)

---

## License

MIT
