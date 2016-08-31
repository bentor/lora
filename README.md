# lora
A Lora networking toolbox for testing and troubleshooting various settings

## Intended Use
This program is a quick and dirty test suite for two lora enabled microcontrollers, e.g., two Arduinos. I needed a way to test various bandwidth frequencies combined with payload size, without having to re-compile and upload code for each permutation for each microcontroller. So, instead here is a "lora shell" with a limited set of commands for testing. Interaction with the lora shell is through the Serial Monitor in Arduino Studio (with the "No line ending" option selected and 9600 baud) or just through the usb serial.     

```
? : show available commands
p : toggle (start/stop) pinging other nodes
b<n> : set bandwidth. n = {0,..,7}
r<n> : set local and remote node's bandwidth. n = {0,..,7}
s : perform bandwidth scan
f : perform frequency scan
g : perform packet size scan
t : perform throughput test

lora>
```

Here is the output after turning on ping with `p`:

```
lora>p
Sending time: 44
got reply: pong
RSSI: -24
Ping time: 90
```

All times are in milli seconds.

Here is the output after a bandwidth scan with `s`:

```
lora>s
got reply: ready

Starting bandwidth scan
-----------------------
Bw7_8Cr45Sf128
Sending time: 691
No reply, is lora shell running on remote node?

Bw10_4Cr45Sf128
Sending time: 518
got reply: scan pong
RSSI: -32
Ping time: 1114

Bw15_6Cr45Sf128
Sending time: 346
got reply: scan pong
RSSI: -34
Ping time: 744

Bw31_25Cr48Sf128
Sending time: 173
got reply: scan pong
RSSI: -33
Ping time: 373

Bw31_25Cr45Sf512
Sending time: 608
got reply: scan pong
RSSI: -35
Ping time: 1320

Bw62_5Cr45Sf1024
Sending time: 526
got reply: scan pong
RSSI: -36
Ping time: 1160

Bw125Cr45Sf2048
Sending time: 526
got reply: scan pong
RSSI: -36
Ping time: 1162

Bw125Cr45Sf128
Sending time: 44
got reply: scan pong
RSSI: -32
Ping time: 95

Bw125Cr45Sf128
lora>
```

Here is the output after a packet size scan with `g`:


```
lora>g
got reply: unknown

Starting packet size scan
-----------------------
set_size: 0
Sending time: 34
got reply: unknown
RSSI: -33
Ping time: 81

set_size: 25
Sending time: 75
got reply: unknown
RSSI: -33
Ping time: 122

set_size: 50
Sending time: 111
got reply: unknown
RSSI: -33
Ping time: 169

set_size: 75
Sending time: 148
got reply: unknown
RSSI: -34
Ping time: 232

set_size: 100
Sending time: 183
got reply: unknown
RSSI: -32
Ping time: 294

set_size: 125
Sending time: 219
got reply: unknown
RSSI: -32
Ping time: 356

set_size: 150
Sending time: 256
got reply: unknown
RSSI: -32
Ping time: 420

set_size: 175
Sending time: 292
got reply: unknown
RSSI: -32
Ping time: 482

set_size: 200
Sending time: 332
got reply: unknown
RSSI: -32
Ping time: 548

set_size: 225
Sending time: 368
got reply: unknown
RSSI: -32
Ping time: 610

set_size: 250
Sending time: 405
got reply: unknown
RSSI: -32
Ping time: 663

lora>
```

Note that in the U.S. the FCC has a 400ms transmission time limit restriction for the 902-928MHz ISM bands. 

# Dependencies
Remember to get the excellent RadioHead library. Get it from here: http://www.airspayce.com/mikem/arduino/RadioHead/


# Author
Bent G Christensen <bentor@gmail.com>

