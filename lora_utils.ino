
// Using the excellent RadioHead lora library
#include <RH_RF95.h>

RH_RF95 rf95(10, 2);
//RH_RF95 rf95(2,3);

char c;
uint8_t bw = 0;
uint8_t scan_bw = 0;
uint8_t scan_freq = 0;

bool mode_ping = false;
bool mode_bw_scan = false;
bool mode_freq_scan = false;
char *ping_data = (char *)malloc(RH_RF95_MAX_MESSAGE_LEN);


void setup()
{
  Serial.begin(9600);
  if (!rf95.init()) {
    Serial.println("init failed");
  }
  else {
    Serial.println("init success");
  }

  rf95.setTxPower(23, false);
  delay(2000);


  sprintf(ping_data, "ping");
  print_help();
  Serial.print("lora>");
}

uint8_t bandwidths = 8;

void set_bw(uint8_t n) {

  if(!mode_bw_scan)
     bw = n;

  rf95.setModeIdle();

  switch (n) {
    case 1:
      rf95.setModemConfig(rf95.Bw7_8Cr45Sf128);
      Serial.println("Bw7_8Cr45Sf128");
      break;
    //    case 2:
    //      rf95.setModemConfig(rf95.Bw7_8Cr45Sf512);
    //      Serial.println("Bw7_8Cr45Sf512");
    //      break;
    case 2:
      rf95.setModemConfig(rf95.Bw10_4Cr45Sf128);
      Serial.println("Bw10_4Cr45Sf128");
      break;
    case 3:
      rf95.setModemConfig(rf95.Bw15_6Cr45Sf128);
      Serial.println("Bw15_6Cr45Sf128");
      break;
    //    case 5:
    //      rf95.setModemConfig(rf95.Bw20_8Cr45Sf512);
    //      Serial.println("Bw20_8Cr45Sf512");
    //      break;
    case 4:
      rf95.setModemConfig(rf95.Bw31_25Cr45Sf128);
      Serial.println("Bw31_25Cr48Sf128");
      break;
    case 5:
      rf95.setModemConfig(rf95.Bw31_25Cr45Sf512);
      Serial.println("Bw31_25Cr45Sf512");
      break;
    //    case 7:
    //      rf95.setModemConfig(rf95.Bw31_25Cr45Sf1024);
    //      Serial.println("Bw31_25Cr45Sf1024");
    //      break;
    //    case 8:
    //      rf95.setModemConfig(rf95.Bw41_7Cr45Sf1024);
    //      Serial.println("Bw41_7Cr45Sf1024");
    //      break;
    case 6:
      rf95.setModemConfig(rf95.Bw62_5Cr45Sf1024);
      Serial.println("Bw62_5Cr45Sf1024");
      break;
    //    case 10:
    //      rf95.setModemConfig(rf95.Bw62_5Cr45Sf2048);
    //      Serial.println("Bw62_5Cr45Sf2048");
    //      break;
    case 7:
      rf95.setModemConfig(rf95.Bw125Cr45Sf2048);
      Serial.println("Bw125Cr45Sf2048");
      break;
    //    case 12:
    //      rf95.setModemConfig(rf95.Bw125Cr48Sf4096);
    //      Serial.println("Bw125Cr48Sf4096");
    //      break;
    default:
      rf95.setModemConfig(rf95.Bw125Cr45Sf128);
      Serial.println("Bw125Cr45Sf128");
      break;
  }

}


void set_size(uint8_t n) {
  Serial.print("set_size: ");
  Serial.println(n);

  memset(ping_data, '$', n);
  ping_data[n] = 0;

  //Serial.print(ping_data);
  //Serial.println(".");
}

void set_freq(uint8_t n) {

  rf95.setModeIdle();

  switch (n) {
    case 1:
      rf95.setFrequency(903.0);
      Serial.println("903.0");
      break;
    case 2:
      rf95.setFrequency(906.0);
      Serial.println("906.0");
      break;
    case 3:
      rf95.setFrequency(909.0);
      Serial.println("909.0");
      break;
    case 4:
      rf95.setFrequency(912.0);
      Serial.println("912.0");
      break;
    case 5:
      rf95.setFrequency(915.0);
      Serial.println("915.0");
      break;
    default:
      rf95.setFrequency(900.0);
      Serial.println("900.0");
      break;
  }

}


void listen() {

  if (rf95.available())
  {
    // Should be a message for us now
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf95.recv(buf, &len))
    {
      Serial.print("got request: ");
      Serial.println((char*)buf);

      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);

      // Send a reply
      if (strcmp((char *)buf, "ping") == 0) {
        // ping
        uint8_t data[] = "pong";
        rf95.send(data, sizeof(data));

      } else if (strcmp((char *)buf, "bw_scan") == 0) {
        // scan
        uint8_t data[] = "ready";
        mode_bw_scan = true;
        rf95.send(data, sizeof(data));
      } else if (strcmp((char *)buf, "freq_scan") == 0) {
        // scan
        uint8_t data[] = "ready";
        mode_freq_scan = true;
        rf95.send(data, sizeof(data));
      } else if (strcmp((char *)buf, "remote_bw") == 0) {
        // remote_bw
        uint8_t data[] = "ready";
        rf95.send(data, sizeof(data));

        // expecting bw
        if (rf95.waitAvailableTimeout(5000)) {
          // Should be a reply message for us now
          if (rf95.recv(buf, &len)) {
            set_bw(buf[0]);
          }
        }

      } else {

        // unknown
        uint8_t data[] = "unknown";
        rf95.send(data, sizeof(data));
      }

      rf95.waitPacketSent();
      Serial.println("Sent a reply");
    }
    else
    {
      Serial.println("recv failed");
    }
  }
}


void scan_listen() {

  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(5000))
  {
    // Should be a reply message for us now
    if (rf95.recv(buf, &len))
    {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);

      uint8_t data[] = "scan pong";
      unsigned long t = millis();

      rf95.send(data, sizeof(data));
      rf95.waitPacketSent();
      Serial.println();

    }
    else
    {
      Serial.println("recv failed");
    }
  }
}


void send_stream() {
  uint8_t data[] = "ping";
  unsigned long t = millis();

  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  for (int i = 0; i < 100; i++) {
    rf95.send(data, sizeof(data));
    rf95.waitPacketSent();

    // Now wait for a reply

    if (rf95.waitAvailableTimeout(5000))
    {
      // Should be a reply message for us now
      if (rf95.recv(buf, &len))
      {
        Serial.print(".");
      }
      else
      {
        Serial.print("*");
      }
    }
    else
    {
      Serial.print("|");
    }

  }
  Serial.println();
  Serial.print("100 packets: ");
  Serial.println(millis() - t);
}



bool send_ping() {
  bool pong_received = false;
  unsigned long t = millis();

  rf95.send((uint8_t *)ping_data, strlen(ping_data) + 1);

  rf95.waitPacketSent();
  Serial.print("Sending time: ");
  Serial.println(millis() - t);


  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(5000))
  {
    // Should be a reply message for us now
    if (rf95.recv(buf, &len))
    {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      Serial.print("Ping time: ");
      Serial.println(millis() - t);
      pong_received = true;
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is lora shell running on remote node?");
  }

  Serial.println();
  return pong_received;
}

bool send_scan_request(uint8_t data[], uint8_t data_len) {

  rf95.send(data, data_len);
  rf95.waitPacketSent();

  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (rf95.waitAvailableTimeout(5000))
  {
    // Should be a reply message for us now
    if (rf95.recv(buf, &len))
    {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
      return true;
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is lora shell running on remote node?");
  }

  return false;

}


void bw_scan() {

  uint8_t data[] = "bw_scan";

  // send scan request to other node
  if (send_scan_request(data, sizeof(data))) {

    mode_bw_scan = true;

    Serial.println();
    Serial.println("Starting bandwidth scan");
    Serial.println("-----------------------");

    // go through the bandwidths
    for (uint8_t i = 1; i <= bandwidths; i++) {
      set_bw(i);
      send_ping();
    }

    // set original bw
    set_bw(bw);
    mode_bw_scan = false;
  }
}


void freq_scan() {

  uint8_t data[] = "freq_scan";

  // send scan request to other node
  if (send_scan_request(data, sizeof(data))) {

    Serial.println();
    Serial.println("Starting frequency scan");
    Serial.println("-----------------------");

    // go through the frequency
    for (uint8_t i = 0; i <= 5; i++) {
      set_freq(i);
      send_ping();
    }

    // set original freq
    rf95.setFrequency(900.0);
  }
}

void size_scan() {

  uint8_t data[] = "size_scan";

  // send scan request to other node
  if (send_scan_request(data, sizeof(data))) {

    Serial.println();
    Serial.println("Starting packet size scan");
    Serial.println("-----------------------");

    // go through the sizes
    for (uint8_t i = 0; i <= 10; i++) {
      set_size(i * (uint8_t)(RH_RF95_MAX_MESSAGE_LEN / 10));
      send_ping();
    }

    // set original size
    sprintf(ping_data, "ping");
  }
}


void send_remote_bw(uint8_t n) {

  uint8_t data[] = "remote_bw";

  // send scan request to other node
  if (send_scan_request(data, sizeof(data))) {

    Serial.println();
    Serial.println("Setting local and remote node's bandwidth");
    Serial.println("-----------------------------------------");

    rf95.send(&n, sizeof(n));
    rf95.waitPacketSent();

    set_bw(n);
  }

}


void print_help() {
  Serial.println();
  Serial.println("? : show available commands");
  Serial.println("p : toggle (start/stop) pinging other nodes");
  Serial.println("b<n> : set bandwidth. n = {0,..,7}");
  Serial.println("r<n> : set local and remote node's bandwidth. n = {0,..,7}");
  Serial.println("s : perform bandwidth scan");
  Serial.println("f : perform frequency scan");
  Serial.println("g : perform packet size scan");
  Serial.println("t : perform throughput test");
  Serial.println();
}

void loop() {

  // Input shell

  if (Serial.available() > 0) {
    c = Serial.read();
    Serial.println(c);

    switch (c) {
      case '?':
        print_help();
        break;
      case 'p':
        mode_ping = !mode_ping;
        break;
      case 'b':
        while (Serial.available() == 0);
        c = Serial.read();
        set_bw(c - '0');
        break;
      case 's':
        bw_scan();
        break;
      case 'f':
        freq_scan();
        break;
      case 't':
        send_stream();
        break;
      case 'r':
        while (Serial.available() == 0);
        c = Serial.read();
        send_remote_bw(c - '0');
        break;
      case 'g':
        size_scan();
        break;
    }

    Serial.print("lora>");
  }

  // Action loop
  if (mode_ping) {

    send_ping();
    delay(4000);

  } else if (mode_bw_scan) {

    // receiving node
    scan_bw++;
    set_bw(scan_bw);
    scan_listen();

    // done scanning
    if (scan_bw >= bandwidths) {
      mode_bw_scan = false;
      scan_bw = 0;
      set_bw(bw);
    }

  } else if (mode_freq_scan) {

    // receiving node
    set_freq(scan_freq++);
    scan_listen();

    // done scanning
    if (scan_freq >= 6) {
      mode_freq_scan = false;
      scan_freq = 0;
      set_freq(0);
    }

  } else {
    // default state
    listen();
  }
}

