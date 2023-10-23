#ifndef ETHERNET5500_SRC_ETHERNETSERVER_H_
#define ETHERNET5500_SRC_ETHERNETSERVER_H_

#include "Server.h"

class EthernetClient;

class EthernetServer : public Server {
 private:
  uint16_t _port;
  void accept();

 public:
  EthernetServer(uint16_t);
  EthernetClient available();
  virtual void begin();
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
  using Print::write;
};

#endif  // ETHERNET5500_SRC_ETHERNETSERVER_H_
