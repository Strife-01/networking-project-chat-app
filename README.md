# Integration Project group 3 C++

Scuba Chat is a distributed, multi-hop, 4-nodes chat application designed for emulated underwater sound communication. It is relying on custom protocols for addressing, routing, and reliable data transfer. Each node dynamically selects an address via echo-based negotiation and maintains a list of reachable peers. Messages are fragmented and reassembled using a TCP-inspired stop-and-wait protocol. Reliable broadcasting is achieved through controlled flooding with deduplication and TTL. To avoid collisions, we implement CSMA/CD with exponential backoff. Routing is handled via vector-based protocols with split horizon and poison reverse, supporting topology changes through periodic updates, and Qt-based GUI provides user interaction and visualization.


TODO :
- increase table broadcast timeout after receiving a table that do not change anything
- slow down the rate at which we are broadcasting the table to let actual data be sent