# CanSerial

Serial port emulation over CAN bus.
Working on Raspberry Pi with MCP2515 module or other linux cansock compatible adapters.
Main goal - emulate serial port to add serial debugging to external can-connected modules (arduino)

Adding CAN bus to Raspberry Pi completely described here
https://www.raspberrypi.org/forums/viewtopic.php?f=44&t=141052

## Protocol

Only Base Frame format (CAN 2.0 B) implemented.
Master node periodically sends frame with ID 0x321 and zero length as UUID request.
Slave node responds to this frame with ID 0x323, containing 6 bytes of unique serial number.
Master will assign a port number and responds with ID 0x322, containig the port number.
After receiving port number slave node should ignore next UUID requests and accepts only 
frames with id 0x17F+(2 x PortNumber) 

After assigning port to slave, Emulator creates /tmp/ttyCAN0_xxxxxxxxxxxx node emulating serial port,
xxxxxxxxxxxx is the uniqueue ID reported by the board. 

## Build procedure

```
git clone https://github.com/bondus/CanSerial.git
cd CanSerial
cmake .
make
./canserial
```


If you get "Socket init error: xxx" - check your CAN configuration.


## Run CanSerial as service


```
$ sudo cp canserial.service /lib/systemd/system/canserial.service
$ sudo systemctl daemon-reload
$ sudo systemctl enable canserial.service
$ sudo systemctl start canserial.service
```
