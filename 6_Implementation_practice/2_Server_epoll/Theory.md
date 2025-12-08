**Objective:** Upgrade your server to handle multiple connections simultaneously using **Linux `epoll`**.
**Constraint:** Do NOT use threads. Use a Single Thread + Event Loop.

#### Step 1: The Reading (Input)

1.  **`man 2 fcntl`**
      * **Find:** `O_NONBLOCK`.
      * **Concept:** By default, `read()` freezes your program if there is no data. `O_NONBLOCK` tells the OS: *"If there is no data, return -1 and set errno to `EWOULDBLOCK` instead of sleeping."*
      
2.  **`man 7 epoll`** (The most important man page in Linux High Performance)
      * **Read:** The description of "Level-triggered and edge-triggered".
      * **Concept:**
          * **Level Triggered (LT):** "Hey, there is data\! Hey, there is data\!" (Keeps screaming until you read it). Easier.
          * **Edge Triggered (ET):** "Data arrived." (Tells you once. If you don't read *all* of it, it won't tell you again). Faster, used in HFT.
3.  **`man 2 epoll_create1`**
4.  **`man 2 epoll_ctl`** (Control Interface)
      * Arguments: `EPOLL_CTL_ADD` (Add a phone to watch).
5.  **`man 2 epoll_wait`** (The Loop)
      * This is the function that puts your CPU to sleep until *something* happens.

-----

#### Step 2: The Mental Model (The Architecture)

Building a **Reactor Pattern**:

1.  **Setup:** Create the `epoll` instance .
2.  **Register:** Add your `server_fd` (Listening Socket) to the receptionist's list.
3.  **Loop:**
      * **Wait:** Call `epoll_wait`. (Server sleeps).
      * **Wake Up:** A list of "Events" is returned.
      * **Check:**
          * Is the event on `server_fd`? -\> **New Client\!** Call `accept()`.
          * Is the event on `client_fd`? -\> **New Data\!** Call `read()`.

-----

#### Step 3: The Code

Create `server_epoll.cpp`.

#### Step 4: The Verification

1.  **Compile:** `g++ server_epoll.cpp -o server_epoll`
2.  **Test:**
      * Terminal 1: `./server_epoll`
      * Terminal 2: `nc localhost 6379` (Keep this open\!)
      * Terminal 3: `nc localhost 6379` (Keep this open\!)
3.  **Success Condition:** Both terminals connect. You can type in Terminal 2, get a reply, then type in Terminal 3, get a reply. **The server handles both without restarting.**
