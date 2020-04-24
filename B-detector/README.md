# Arduino UNO + ESP8266 + AWS Iot project

### Env
###### Software
- Ardiono IDE
- AWS Console
###### Hardware
- UNO
- ESP8266
- LEDs, resisters, ultrasonic sensor, wires etc
###### Firmware
- NA


### Dev Note
###### To test ESP8266 communication
- When flushing Uno board, disconnect ESP power first to prevent sync error
- For pc side serial comm: 5v, board built in rx/tx(0/1), baudrate 115200
- For comm between uno and esp: 3.3v, donot connect reset pin, do not use board built in rx/tx, baudrate 115200

### To Flush ESP8266
```
rx-rx
tx-tx
3.3v - chen, 3.3
gnd - gnd, gpio0
```

using espressif download tool
if seeing syncing issue, attach RST to GND for a while then release

```
v1.6.2
boot1.7 bin 0x00000
at/user1 bin 0x01000
default08 bin 0xfc000
blank bin 0xfe000
blank bin 0x7e000

26M/40MHz
Dout mode
8Mbit
baud 115200
```

### TODOs
- Wiring diagram memo
