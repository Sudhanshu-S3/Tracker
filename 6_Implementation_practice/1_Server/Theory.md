# Simple Server Explanation (main.cc)

This code creates a basic server that listens for messages and replies back. Think of it like setting up a customer service phone line.

## 1. Create the Phone (Socket)
`socket(AF_INET, SOCK_STREAM, 0)`
- We buy a phone.
- `AF_INET`: We are using IPv4 (standard internet addresses).
- `SOCK_STREAM`: We want a reliable connection (TCP), like a phone call, not a text message.

## 2. Set Options
`setsockopt(...)`
- This fixes a common annoyance where the OS says "Port is busy" if you restart the server too quickly. It lets us reuse the address immediately.

## 3. Assign a Number (Bind)
`bind(...)`
- We attach a phone number (Port 8080) to our phone.
- `INADDR_ANY`: We are willing to answer calls from any of our network cards (Wi-Fi, Ethernet, etc.).

## 4. Turn on the Ringer (Listen)
`listen(server_fd, 10)`
- We switch the phone on and wait.
- The `10` is the "backlog": if 10 people call at once, they wait in line. The 11th gets a busy signal.

## 5. Wait for Calls (Accept Loop)
`while(true) { accept(...) }`
- The server sits in an infinite loop, waiting forever.
- `accept`: When the phone rings, we pick it up.
- **Important:** This creates a *new* file descriptor (`client_fd`).
    - `server_fd`: The main phone line that rings.
    - `client_fd`: The specific conversation with *this* one caller.

## 6. Talk (Read/Write)
- `read`: We listen to what the caller says and store it in a `buffer`.
- `write`: We speak back. Here, we say "Hello this is sudhanshu's server".

## 7. Hang Up (Close)
`close(client_fd)`
- We finish the conversation with this specific person so we can go back to waiting for the next call.

---
**Summary:**
1. **Socket**: Buy phone.
2. **Bind**: Get number.
3. **Listen**: Wait for ring.
4. **Accept**: Pick up.
5. **Read/Write**: Chat.
6. **Close**: Hang up.

Problems are :

Blocking in nature.
