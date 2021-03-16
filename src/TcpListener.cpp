#include "TcpListener.h"

#include "utility/w5500.h"
#include "utility/socket.h"

// Using the functions already implemented for EthernetClient, though that
// could be changed.
#include "Ethernet3.h"
#include "EthernetClient.h"

namespace {
bool IsListening


}//namespace



// Starts or continues listening for connections to 'tcp_port_', returns true if successfully
// configures the underlying socket.
// If the socket is already being used for
// another purpose (e.g. there is an existing connection or is listening to a different port) AND force== true, that is replaced
// with this new use (i.e. an existing connection is closed or broken).
bool TcpListener::listen(bool force) {
  if (force) {
    EthernetClient client(sock_num_);
    if (listener_state_ == kListening &&
        EthernetClass::_server_port[sock_num_] == port_ &&
        client.status() == SnSR::LISTEN) {
      // Already listening.
      return true;
    }
    // Close any existing connection.
    client.stop();
    // We can't continue using 'client' because it has cleared the socket number.
  } else {
    // Not forcing.
  }


}

// Returns true if currently connected (i.e. have already accepted a connection
// and returned a TcpServerConnection, and that connection has not yet
// been closed / stopped).
bool TcpListener::connected();

// Returns the connection to a *new* client. Only returns this on the
// first call to accept after a new connection is established, allowing
// the caller to distinguish between new and ongoing connections.
TcpServerConnection *TcpListener::accept();

// Returns again the current connection to a previously accepted client.
// Only returns non-nullptr after accept, and until the connection is closed.
TcpServerConnection *TcpListener::connection();

ListenerState TcpListener::listener_state() const { return listener_state_; }



// Returns the number of bytes available for reading, or -1 if a peer
// is not connected or has closed their side of the connection.
int TcpServerConnection::available() override;

// Returns the next byte from the connection. Returns -1 if there are
// no bytes available currently, or if the connection has been closed.
int TcpServerConnection::read() override;

// Returns the next byte from the stream without consuming it. The next
// call to read() will return the same value.
int TcpServerConnection::peek() override;

// Reads up to size bytes from the connection, writing them into buf.
// Returns the number of bytes read, i.e. zero or more. If none are
// available for reading and the connection has been closed by the peer,
// returns -1.
int TcpServerConnection::read(uint8_t *buf, size_t size);

// Write one byte to the connection, returns 1 if successful, 0 if
// unable to write.
// TODO Figure out whether and if so how to provide blocking I/O.
size_t TcpServerConnection::write(uint8_t) override;

// Write up to size bytes to the connection, returns the number of
// bytes written.
size_t TcpServerConnection::write(const uint8_t *buf, size_t size) override;

// Waits for the transmission of outgoing data to complete.
void TcpServerConnection::flush() override;

// Closes the connection.
void TcpServerConnection::stop();

// Returns true if the socket is connected to a peer.
bool TcpServerConnection::connected();
