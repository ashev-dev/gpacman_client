## Build instructions for CMake on Debian

### Update system

    apt-get update
    apt-get upgrade

### Prepare system

    apt-get install build-essential

    apt-get install git cmake libsfml-dev 

    apt-get install libboost-dev libboost-program-options-dev libboost-thread-dev

### Clone this repo

    git clone ...

### Create Makefile & check system requirements

Go to the 'gpacman_server' or 'gpacman_client' directory and run

    cmake .
    
### Building

    make

### Run gpacman_server

    ./gpacman_server
    
### Run 2 clients

    ./gpacman_client
    
