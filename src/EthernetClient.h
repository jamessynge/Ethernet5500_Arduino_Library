#ifndef ETHERNET5500_SRC_ETHERNETCLIENT_H_
#define ETHERNET5500_SRC_ETHERNETCLIENT_H_
#include <Arduino.h>
#include <Client.h>
#include <IPAddress.h>
#include <Print.h>

class EthernetClient : public Client {
 public:
  EthernetClient();
  EthernetClient(uint8_t sock);

  uint8_t status() const;
  virtual int connect(IPAddress ip, uint16_t port);
  virtual int connect(const char *host, uint16_t port);
  virtual size_t write(uint8_t);
  virtual size_t write(const uint8_t *buf, size_t size);
  virtual int available();
  virtual int read();
  virtual int read(uint8_t *buf, size_t size);
  virtual int peek();
  virtual void flush();
  virtual void stop();
  virtual uint8_t connected();
  virtual operator bool();
  virtual bool operator==(const bool value) { return bool() == value; }
  virtual bool operator!=(const bool value) { return bool() != value; }
  virtual bool operator==(const EthernetClient &);
  virtual bool operator!=(const EthernetClient &rhs) {
    return !this->operator==(rhs);
  }
  uint8_t getSocketNumber() const;

  // Return the IP address of the host who sent the current incoming packet
  virtual void remoteIP(uint8_t *ip);
  // Return the MAC address of the host who sent the current incoming packet
  virtual void remoteMAC(uint8_t *mac);

  // get the value of the socket mode register
  virtual uint8_t getSocketMode();
  // set the 'no Delayed ACK' option
  virtual void setNoDelayedACK(bool ack = false);
  // get the state of 'no Delayed ACK' option
  virtual bool getNoDelayedACK();

  friend class EthernetServer;

  using Print::write;

 private:
  static uint16_t _srcport;
  uint8_t _sock;
};

#endif  // ETHERNET5500_SRC_ETHERNETCLIENT_H_
