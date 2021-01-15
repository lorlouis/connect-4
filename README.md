# Connect 4 online

## How to build

### Linux / OSX (probably)

`cd` to the project's root and run `make`

### Windows

Open the .sln file in visualstudio and click on build

## How to play

### Controls

| Move Left | Move Right | Select | Quit |
|:---------:|:----------:|:------:|:----:|
| a or h | d or l | space | q |

### Local Multiplayer

simply run con4

### Online Multiplayer

#### Dedicated Server

run `con4 -s <Port Number>`

If you intend to play outside of your LAN
make sure to forward the port on your router

#### Client

Once the server is running, connect to it by running

`con4 -c <The server's IP>:<Port Number>`

If the server is running on the same machine you want
to run the client(s) connect to it via 127.0.0.1.
