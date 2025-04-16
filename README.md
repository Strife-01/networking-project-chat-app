# Scuba Chat - Underwater Network System

Scuba Chat is a distributed, multi-hop, four-node chat application designed for emulated underwater sound communication. The system includes custom-built protocols for routing, addressing, and reliable data transmission, paired with a Qt-based GUI for user interaction.

---

## ⚙️ System Overview

Scuba Chat relies on echo-based address negotiation, multi-hop routing (Bellman-Ford), stop-and-wait data transfer, and controlled flooding for broadcasting. The MAC layer uses CSMA/CD with exponential backoff and randomized collision avoidance.  

**Highlights:**
- Dynamic address negotiation using vector routing
- CSMA/CD-based channel access
- Reliable stop-and-wait ARQ with fragmentation/reassembly
- Controlled flooding for broadcast
- Qt GUI for user-friendly interaction

---

## 🚀 Build & Run

```bash

# sudo apt install qt6-base-dev qt6-tools-dev qt6-tools-dev-tools cmake g++


# Clone the repository
git clone https://github.com/Strife-01/networking-project-chat-app.git

cd networking-project-chat-app/src/frontend/Chatroom/build/Desktop-Debug

make clean

make

./Chatroom

```

## 🗂️ File Structure

```bash
├── Backend/
│   ├── addressing/         # Address assignment and resolution
│   ├── channel_state/      # Channel status tracking
│   ├── fragmentation/      # Fragmentation and reassembly logic
│   ├── mac/                # Medium Access Control (CSMA/CD)
│   ├── multihop-relay/     # Multi-hop message forwarding
│   ├── network/            # Interface between frontend and backend
│   ├── routing/            # Routing logic using vector tables
│   ├── transport/          # Stop-and-Wait protocol logic
│   └── MessageQueue/       # Application-layer message buffer
├── Frontend/
│   └── ChatRoom/                # Application entry point
│       └── main.cpp             # Qt GUI
└── Utils/
    ├── BlockingQueue.h     # Thread-safe queue
    ├── Message.h           # Message structure
    ├── MessageType.h       # Message types enum
    ├── Random.h            # Random generator
    ├── packet_header.*     # Packet header bitfield and utils
    └── test_framework.h    # Testing utilities
```

```Backend```
- `MessageQueue` – Manages application-layer message buffer
- `addressing` – Handles dynamic address assignment and resolution
- `channel_state` – Tracks channel transmission status
- `fragmentation` – Splits and reassembles long messages
- `mac` – Implements CSMA/CD logic
- `multihop-relay` – Forwards messages across multiple hops
- `network` – Bridges frontend and backend logic
- `routing` – Implements Bellman-Ford routing
- `transport` – Stop-and-wait ARQ logic

```Frontend```
- `Chat` – GUI implemented using Qt

```Utils```
- `BlockingQueue.h` – Thread-safe message queue
- `Message.h`, `MessageType.h` – Defines message formats and types
- `Random.h` – Pseudo-random number generator
- `packet_header.*` – Bitfield-based packet header structure
- `test_framework.h` – Shared testing infrastructure

```main.cpp``` – Entry point

---

## 📡 Protocols & Functionalities

### Medium Access Control: CSMA/CD

```2.1.1 Carrier Sensing```
- Nodes check if the medium is busy/free/sending/idle before transmission.

```2.1.2 Collision Detection```
- No physical detection; fallback on timeouts and destination checking.

```2.1.3 Exponential Backoff```
- Backoff after echo requests: wait ∈ [0, 2^exp], exp ∈ [0, 10].

```2.1.4 Collision Avoidance```
- Double-check before sending with randomized delay.

### Fragmentation and Assembly

```- Max payload per packet: 28 bytes (32 bytes - 4-byte header)```
```- Fragmenter::fragmentMessage()``` slices long messages.
```- Reassembler::insertFragment()``` reorders and joins received fragments.
```- Fragment ID``` and ```message ID``` used for tracking and reassembly.
```Packet Header:```
`| src(3) | next_hop(3) | dest(3) | MF(1) | msg_id(3) | frag_id(11) | type(3) | len(5) |`

### Reliable Data Transfer: Stop-and-Wait ARQ

```- StopAndWaitSender```: Sends fragments, waits for ACK, retries on timeout.
```- StopAndWaitReceiver```: Sends ACKs, forwards, and reassembles.
```- Timeout: 25 seconds``` | ```Max retries: configurable```

```TransportManager``` glues sender and receiver logic.

### Multi-Hop Forwarding

Routing is done using vector routing (Bellman-Ford) with:
- Echo requests to share routing tables
- No split horizon/poison reverse
- TTL & hop-limit to prevent loops
- “Freeze” mode during data transmission to reduce interference

### Dynamic Addressing

- Based on available echo requests at startup
- Selects unused addresses dynamically
- Falls back to random address selection with retries

### User Interface (Qt-based)

- Global chat on the left
- Active user list on the right
- Private chat opens on click
- Notification popups on message receipt
- Built using Qt Creator for cross-platform ease

---

## 🛠️ Build Instructions

1. Open the project in **Qt Creator**.
2. Configure using the default `qmake` setup.
3. Build and run the application directly via the IDE.

---
