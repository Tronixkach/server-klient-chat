# Qt TCP Multi-Client Chat Application

## 📌 About the Project
This project is a multi-client chat application built with C++ and the Qt Framework (specifically the `Qt Network` module). It implements a robust Client-Server architecture over the TCP/IP protocol. 

The main goal of this project is to demonstrate an understanding of network socket programming, asynchronous data transmission, and state management between a central server and multiple concurrent client connections.

## 🚀 Key Features
- **Asynchronous Non-Blocking I/O:** Utilizes Qt's event-driven Signal/Slot mechanism (e.g., `readyRead`, `newConnection`) to handle network operations without blocking the main GUI thread.
- **Multi-Client Broadcasting:** The server maintains a dynamic list of active `QTcpSocket` connections and efficiently broadcasts incoming messages from one client to all other connected clients in real-time.
- **Connection Management:** Real-time tracking of online users. The server safely handles sudden client disconnects and cleans up resources (`deleteLater()`) to prevent memory leaks.
- **Graceful Shutdown:** If the server goes offline, it automatically notifies all active clients and securely drops the connections before shutting down the listening socket.
- **User-Friendly GUI:** Dedicated graphical interfaces for both the Server Control Panel (port configuration, live logs, active connections count) and the Client App (IP/Port configuration, nickname selection, chat window).

## 🛠 Tech Stack
- **Language:** C++17
- **Framework:** Qt 6
- **Modules:** Qt Network (`QTcpServer`, `QTcpSocket`), Qt Widgets.
- **Protocol:** TCP/IPv4

## ⚙️ Architecture (How it works)
1. **The Server** binds to a specified port and listens for incoming connections using `QTcpServer`. When a client connects, the server assigns a dedicated `QTcpSocket` descriptor and appends it to a `QList` of active sockets.
2. **The Client** connects to the server's IP and Port. Once authenticated (via a chosen nickname), it can send UTF-8 encoded string messages.
3. When the server receives a `readyRead` signal from any client socket, it reads the byte array and iterates through the active connection list, writing the data to all other sockets (Broadcasting).
