# 💬 Multi-Client Chat System (`chat-server`)

A simple multi-client chat system written in C, using TCP sockets and multithreading on Linux.  
Supports multiple clients connecting simultaneously, sending messages with usernames, timestamps, and a clean chat-style UI.

---

## 🚀 Features

- Multiple clients connect and chat concurrently  
- Username support for each client  
- Timestamped messages for context  
- Clear input line and formatted output for readability  
- Graceful client connection and disconnection handling  
- Colored usernames for better message clarity (using ANSI escape codes)  

---

## 🗂️ Project Structure
```
chat-server/
├── server.c # Server program source code
├── client.c # Client program source code
├── chat-server # Compiled server executable (ignored in .gitignore)
├── chat-client # Compiled client executable (ignored in .gitignore)
├── README.md # This project documentation
└── .gitignore # Git ignore rules

```
 
---

## 🛠️ How to Compile

Make sure you have GCC installed. Compile the server and client with:

```bash
gcc server.c -o chat-server -pthread
gcc client.c -o chat-client -pthread

```
## ▶️ How to Run

1. Start the server in a terminal:
```bash
./chat-server
``` 
2. In separate terminal windows, start clients:
```bash
./chat-client
```
3.Enter your username when prompted and start chatting!

## 📤 Demo Outputs
![image](https://github.com/user-attachments/assets/b8a6b856-d90a-489d-81d0-7a442e87aa11)
![image](https://github.com/user-attachments/assets/a5e1b9fa-11b2-427b-9642-54364347c81f)

## 🎮 Usage Controls
-GCC compiler
-Unix/Linux environment (tested on Kali Linux and Ubuntu)
-Basic terminal supporting ANSI escape codes (most Linux terminals) 

## 📦 Requirements
- GCC compiler  
- Unix/Linux environment (tested on Kali Linux and Ubuntu)
- Basic terminal supporting ANSI escape codes (most Linux terminals)

## 👨‍💻 Author

**Vigneshwaran Murugan**  
[🔗 GitHub Profile](https://github.com/VigneshwaranMurugan16/)

---

## 📝 License

This project is open-source and licensed under the MIT License.
