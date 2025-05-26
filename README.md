# 💬 PingMe

**PingMe** is a terminal-based chat client written in **C**, designed to allow multiple clients to communicate with each other in real-time over a server. It's a simple, low-level messaging application built from the ground up using sockets — perfect for learning about networking, multiplexing, and systems programming.

---

## 🚀 Project Status

**🔧 Ongoing** — Actively in development. Contributions and feedback are welcome!

---

## ✨ Features

* 🔄 **Multi-client chat support** — Multiple users can connect to the server and exchange messages in real time.
* 🖥️ **Terminal-based UI** — Clean, minimal terminal interface designed for UNIX systems.
* 🧠 **Educational Focus** — Ideal for learning sockets, file descriptors, and `select()` system calls in C.
* 🧪 **Modular Codebase** — Separated into logical files and functions for clarity and extensibility.
* 📡 **Server-Client Architecture** — Written entirely in C with no external dependencies.
* 📜 **Logging** — Basic error logging for debugging and stability.

---

## 🧩 Planned Features

* 🧑‍💻 **Username-based chat formatting**
* 🧾 **JSON-based message structure**
* 📥 **Message history buffer**
* 🔒 **Basic authentication or nickname registration**
* 💬 **Private messaging (DMs)**

---

## 🧰 Tech Stack

* **Language**: C
* **Networking**: POSIX Sockets
* **Multiplexing**: `select()`
* **Platform**: Linux / macOS (UNIX-based systems)

---

## 📦 How to Build

```bash
git clone https://github.com/yourusername/PingMe.git
cd PingMe
gcc server.c server_utils.c -o server
gcc client.c client_utils.c -o client
```

---

## 🧪 Preview

Run the server first:

```bash
./server
```

Then launch multiple clients:

```bash
./client
```

Clients can then type messages that are broadcasted to all others.

---

## 🤝 Contributing

Contributions are welcome! Fork the repository and create a pull request.

1. Fork the repo
2. Create your feature branch (`git checkout -b feature/fooBar`)
3. Commit your changes (`git commit -am 'Add some fooBar'`)
4. Push to the branch (`git push origin feature/fooBar`)
5. Create a new Pull Request

---

## 📄 License

This project is licensed under the MIT License.

---

Feel free to ⭐ the repo and follow the project for updates!
