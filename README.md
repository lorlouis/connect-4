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
| a or l | d or h | enter | q |

### Local Multiplayer

simply run con4

### Online Multiplayer

#### Server

run `con4 -s <Port Number>`

If you intend to play outside of your LAN
make sure to forward the port on your router

#### Client

Once the server is waiting for player 2,

run `con4 -c <The server's IP>:<Port Number>`
