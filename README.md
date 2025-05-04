# Socket-Programming
# TCP-Like Reliable Data Transfer Protocol

This project simulates a **TCP-like reliable data transfer protocol** by extending a basic **proxy-based network setup**. The goal is to replicate the behavior of TCP pipelining, byte-oriented sequence and acknowledgment numbers, and fast retransmit functionality in a simple network environment.

### **Features**
- **TCP-style pipelining** at sender and receiver (sliding window mechanism).
- **Byte-oriented sequence and acknowledgment numbers** (simulated using integers).
- **Fast retransmit functionality**: Detect and react to duplicate ACKs to ensure reliable data transfer.
- **Unidirectional data flow**: Data flows from Client to Receiver, but ACKs flow in both directions.

---

### **System Design Overview**

#### **1. Client (Browser Simulator)**
- The client simulates a browser that sends a stream of data packets with byte-based sequence numbers.
- Implements **TCP pipelining** by sending multiple packets concurrently within a sliding window.
- Handles **fast retransmission** when timeouts or duplicate ACKs occur.
  
#### **2. Proxy Server**
- Acts as an intermediary between the client and receiver.
- Forwards data packets and acknowledgment (ACK) messages.
- Simulates a real-world TCP relay, introducing realistic **delays or packet losses** to test the retransmission logic.

#### **3. Receiver**
- Receives the incoming packets and verifies the in-order delivery based on byte-sequence numbers.
- Sends **cumulative ACKs** back to the client through the proxy.
- Ensures **reliable data reassembly** in the receiver.

---

### **File Structure**

- `client.cpp`: Implements the Client (Browser Simulator), responsible for sending data packets with sliding window and handling fast retransmit.
- `proxy.cpp`: Implements the Proxy Server, forwarding packets and ACKs, while simulating packet loss and delays.
- `receiver.cpp`: Implements the Receiver, handling the reception of data packets, ensuring in-order delivery, and sending cumulative ACKs.
- **Screenshots**: Includes visual evidence of packet sending, ACKs, retransmissions, and successful delivery.

---

### **Compilation and Execution**

To compile and run the project, follow these steps:

1. **Clone the repository** (if applicable):
    ```bash
    git clone https://github.com/yourusername/tcp-reliable-transfer.git
    cd tcp-reliable-transfer
    ```

2. **Compile the C++ files**:
    ```bash
    g++ client.cpp -o client
    g++ proxy.cpp -o proxy
    g++ receiver.cpp -o receiver
    ```

3. **Run the system**:
    - Start the **receiver**:
      ```bash
      ./receiver
      ```
    - Start the **proxy server** (in a separate terminal):
      ```bash
      ./proxy
      ```
    - Start the **client** (in a separate terminal):
      ```bash
      ./client
      ```

   This will simulate the data flow from the client to the receiver via the proxy, with the sliding window mechanism, acknowledgments, and fast retransmission.

---

### **How it Works**

1. **Client** sends multiple packets concurrently within a sliding window, each packet containing a unique sequence number.
2. The **proxy** forwards the packets to the **receiver**, while also forwarding ACKs back to the client. The proxy may introduce delays or packet loss to test retransmission functionality.
3. The **receiver** validates the incoming packets based on their sequence numbers, and sends back cumulative ACKs.
4. If the client detects duplicate ACKs or timeouts, it **retransmits the lost packets** (simulating fast retransmit).

---

### **Deliverables**

1. **Source Code**:
   - `client.cpp`
   - `proxy.cpp`
   - `receiver.cpp`
   
2. **Screenshots**:
   - Packet sending logs.
   - ACK reception logs.
   - Retransmissions and successful delivery screenshots.

---

### **Acknowledgments**
- This project is a simplified version of a **TCP-like reliable data transfer protocol** and aims to simulate the basic behaviors of TCP in a controlled environment.
