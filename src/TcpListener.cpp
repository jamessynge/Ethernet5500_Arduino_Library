#include "TcpListener.h"

#include "Ethernet3.h"
#include "EthernetClient.h"
#include "utility/socket.h"
#include "utility/w5500.h"

// bool TcpListener::is_listening() const {
//   if (listener_state_ != kListening ||
//       EthernetClass::_server_port[sock_num_] != port_) {
//     // *This* instance hasn't configured the socket to listen right now.
//     return false;
//   }
//   auto sr = status();
//   if (sr == SnSR::LISTEN)
//     Pretty definitely Not listening.
//       } &&
//   EthernetClient client(sock_num_);
//         status() == SnSR::LISTEN;
// }

bool TcpListener::listen(bool force) {
  if ((listener_state_ == kListening || listener_state_ == kConnected) &&
      EthernetClass::_server_port[sock_num_] == port_) {
    // Already configured to listen. May have a connection.
    return true;
  }
  if (!force && listener_state_ != kClosing) {
    // Is it already in use for some other purpose?
    auto hw_status = status();
    if (EthernetClass::_server_port[sock_num_] != 0 &&
        hw_status != SnSR::INIT && hw_status != SnSR::CLOSED) {
      // Apparently so.
      return false;
    }
  }
  // Close any existing connection,
  close();
  ::socket(sock_num_, SnMR::TCP, port_, 0);
  ::listen(sock_num_);
  // QUESTION: Should I check here whether the above worked?
  return true;
}

bool TcpListener::is_connected() const { return listener_state_ == kConnected; }

TcpServerConnection *TcpListener::accept() {
  if (listener_state_ == kListening) {
    // Has a connection been accepted by the hardware since the last call?
    auto hw_status = status();
    if (hw_status == SnSR::ESTABLISHED || hw_status == SnSR::CLOSE_WAIT) {
      // Yup, got a new connection.
      listener_state_ = kConnected;
      return &connection_;
    }
  }
  return nullptr;
}

// Returns again the current connection to a previously accepted client.
// Only returns non-nullptr after accept, and until the connection is closed.
TcpServerConnection *TcpListener::get_connection() {
  if (listener_state_ == kConnected) {
    return &connection_;
  }
  return nullptr;
}

void TcpListener::close() {
  EthernetClient client(sock_num_);
  client.stop();
  listener_state_ = kClosing;
}

uint8_t TcpListener::status() const { return w5500.readSnSR(sock_num_); }

int TcpServerConnection::available() {
  return w5500.getRXReceivedSize(listener_.sock_num_);
}

int TcpServerConnection::read() {
  EthernetClient client(listener_.sock_num_);
  return client.read();
}

int TcpServerConnection::peek() {
  EthernetClient client(listener_.sock_num_);
  return client.peek();
}

// Reads up to size bytes from the connection, writing them into buf.
// Returns the number of bytes read, i.e. zero or more. If none are
// available for reading and the connection has been closed by the peer,
// returns -1.
int TcpServerConnection::read(uint8_t *buf, size_t size) {
  return ::recv(listener_.sock_num_, buf, size);
}

// Write one byte to the connection, returns 1 if successful, 0 if
// unable to write.
// TODO Figure out whether and if so how to provide blocking I/O.
size_t TcpServerConnection::write(uint8_t b) {
  EthernetClient client(listener_.sock_num_);
  return client.write(b);
}

// Write up to size bytes to the connection, returns the number of
// bytes written.
size_t TcpServerConnection::write(const uint8_t *buf, size_t size) {
  EthernetClient client(listener_.sock_num_);
  return client.write(buf, size);
}

// Waits for the transmission of outgoing data to complete.
void TcpServerConnection::flush() { ::flush(listener_.sock_num_); }

// Closes the connection.
void TcpServerConnection::close() {
  listener_.close();
}

// Returns true if the socket is connected to a peer.
bool TcpServerConnection::connected() const {
  auto hw_status = listener_.status();
  return hw_status == SnSR::ESTABLISHED || hw_status == SnSR::CLOSE_WAIT;
}
