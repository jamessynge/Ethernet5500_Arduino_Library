#ifndef SRC_TCPLISTENER_H
#define SRC_TCPLISTENER_H

#include <inttypes.h>

// #include "Arduino.h"	
// #include "Print.h"
#include "Stream.h"

class TcpListener;

class TcpServerConnection : public Stream
{
public:
  // Returns the number of bytes available for reading, or -1 if a peer
  // is not connected or has closed their side of the connection.
  int available() override;

  // Returns the next byte from the connection. Returns -1 if there are
  // no bytes available currently, or if the connection has been closed.
  int read() override;

  // Returns the next byte from the stream without consuming it. The next
  // call to read() will return the same value.
  int peek() override;

  // Reads up to size bytes from the connection, writing them into buf.
  // Returns the number of bytes read, i.e. zero or more. If none are
  // available for reading and the connection has been closed by the peer,
  // returns -1.
  int read(uint8_t *buf, size_t size);

  // Write one byte to the connection, returns 1 if successful, 0 if
  // unable to write.
  // TODO Figure out whether and if so how to provide blocking I/O.
  size_t write(uint8_t b) override;

  // Write up to size bytes to the connection, returns the number of
  // bytes written.
  size_t write(const uint8_t *buf, size_t size) override;

  // Waits for the transmission of outgoing data to complete.
  void flush() override;

  // Closes the connection.
  void close();

  // Returns true if the socket is connected to a peer.
  bool connected() const;

private:
  TcpServerConnection(TcpListener& listener)
      : listener_(listener) {}

  TcpListener& listener_;

  friend class TcpListener;
};

class TcpListener
{
public:
  enum ListenerState : uint8_t { kUninitialized, kListening, kConnected, kClosing };

  // A TcpListener will listen on hardware socket 'sock_num' for TCP connections to
  // TCP port number 'port'. The constructor does NOT start the listening process.
  TcpListener(uint16_t port, uint8_t sock_num)
      : connection_(*this), port_(port), sock_num_(sock_num),
        listener_state_(kUninitialized) {}

  // Returns True if this instance has configured the underlying socket to listen for new connections,
  // and the caller has not accepted a new connection. I.e. this is true from the
  // time that listen() is called, and until accept() returns a pointer to a TcpServerConnection
  // instance.
  // bool is_listening() const;

  // Starts or continues listening for connections to 'tcp_port_', returns true if successfully
  // configures the underlying socket.
  // If the socket is already being used for
  // another purpose (e.g. there is an existing connection or is listening to a different port) AND force== true, that is replaced
  // with this new use (i.e. an existing connection is closed or broken).
  bool listen(bool force);

  // Returns true if currently connected (i.e. have already accepted a connection
  // and returned a TcpServerConnection, and that connection has not yet
  // been closed / stopped).
  bool is_connected() const;

  // Returns the connection to a *new* client. Only returns this on the
  // first call to accept after a new connection is established, allowing
  // the caller to distinguish between new and ongoing connections.
  TcpServerConnection *accept();

  // Returns the current accepted connection. Only returns non-nullptr after accept, and until the connection is closed.
  TcpServerConnection *get_connection();

  ListenerState listener_state() const { return listener_state_; }

  // Close the listener socket and the current active connection, if there is one.
  void close();

private:
  // Value of the Socket 'sock_num_' Status Register (SnSR).
  uint8_t status() const;

  // Embedded connection object returned by accept and connection.
  TcpServerConnection connection_;

  // The TCP port being listened to.
  const uint16_t port_;

  // The socket number that is listening.
  const uint8_t sock_num_;

  // State of the listener. This is not the state of the underlying socket.
  ListenerState listener_state_;

  friend class TcpServerConnection;
};

#endif // SRC_TCPLISTENER_H
