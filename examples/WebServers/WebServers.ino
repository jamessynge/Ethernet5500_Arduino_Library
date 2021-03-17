// WebServers
//
// This demonstrates using multiple TcpListener instances to listen
// for multiple simultaneous connections to multiple ports.
//
// This is based on the WebServer.ino example, originally by David A. Mellis.

#include <Ethernet3.h>
#include <SPI.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 1, 177);

class SocketHandler {
 public:
  SocketHandler(uint16_t port, uint8_t sock_num) : listener_(port, sock_num) {}

  void setup() {
    if (listener_.listen(true)) {
      Serial.print("Socket ");
      Serial.print(listener_.sock_num());
      Serial.print(" is listening for connections to port ");
      Serial.println(listener_.port());
    } else {
      Serial.print("Failed to start listening on socket ");
      Serial.print(listener_.sock_num());
      Serial.print(" to port ");
      Serial.println(listener_.port());
    }
  }

  void loop() {
    TcpServerConnection* conn = listener_.accept();
    if (conn) {
      Serial.print("Accepted connection to port ");
      Serial.print(listener_.port());
      Serial.print(" on socket ");
      Serial.println(listener_.sock_num());
      // Our goal is to skip over the entire HTTP request header, which
      // means that we should see some non-whitespace, followed ultimately
      // by "\r\n\r\n".
      at_start_of_line_ = false;
    } else {
      conn = listener_.get_connection();
      if (!conn) {
        return;
      }
    }

    // To better demonstrate the handling of multiple connections at the same
    // time, we only read a limited number of bytes on each loop.
    for (int ndx = 0; ndx < 10; ++ndx) {
      int c = conn->read();
      if (c == -1) {
        if (!conn->connected()) {
          Serial.print("Lost connection on socket ");
          Serial.println(listener_.sock_num());
          conn->close();
        }
      }
      if (c == '\n') {
        if (at_start_of_line_) {
          Serial.print("Skipped over HTTP request to port ");
          Serial.print(listener_.port());
          Serial.print(" on socket ");
          Serial.println(listener_.sock_num());
          generate_response(conn);
          return;
        } else {
          at_start_of_line_ = true;
        }
      } else if (c != '\r') {
        at_start_of_line_ = false;
      }
    }
  }

 private:
  void generate_response(TcpServerConnection* conn) {
    // send a standard http response header
    conn->println("HTTP/1.1 200 OK");
    conn->println("Content-Type: text/html");
    // Tell the client that they can't reuse the connection for another request,
    // i.e. that the connection will be closed after completion of the response
    conn->println("Connection: close");
    // Refresh the page automatically every 5 seconds. Note that this is not a
    // standardized feature of HTTP.
    conn->println("Refresh: 5");
    conn->println();
    conn->println("<!DOCTYPE HTML>");
    conn->println("<html>");
    // output the value of each analog input pin
    for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
      int sensorReading = analogRead(analogChannel);
      conn->print("analog input ");
      conn->print(analogChannel);
      conn->print(" is ");
      conn->print(sensorReading);
      conn->println("<br />");
    }
    conn->println("</html>");

    Serial.print("Closed connection on socket ");
    Serial.println(listener_.sock_num());
    conn->close();
  }

  TcpListener listener_;
  boolean at_start_of_line_ = true;
};

SocketHandler handlers[] = {
    SocketHandler(80, 0),   SocketHandler(80, 1),   SocketHandler(80, 2),
    SocketHandler(80, 3),   SocketHandler(8080, 4), SocketHandler(8080, 5),
    SocketHandler(8080, 6), SocketHandler(8080, 7),
};
size_t kNumHandlers = sizeof(handlers) / sizeof(handlers[0]);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }

  Serial.print("Ethernet address is ");
  Serial.println(Ethernet.localIP());

  for (size_t ndx = 0; ndx < kNumHandlers; ++ndx) {
    handlers[ndx].setup();
  }
}

void loop() {
  for (size_t ndx = 0; ndx < kNumHandlers; ++ndx) {
    handlers[ndx].loop();
  }
}
