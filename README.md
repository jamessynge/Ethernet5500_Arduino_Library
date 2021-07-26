# Ethernet5500 v0.5.0

Arduino Ethernet library for boards with the WIZnet W5500 networking chip. Based on the [Ethernet3 library developed by sstaub](https://github.com/sstaub/Ethernet3). It is mostly the same initially, though I (jamessynge) have removed most of the README.md and added method EthernetClass::setDhcp(DhcpClass*), which must be called in order to use DHCP to request an address for the Arduino.

## Plans

*   Make writing non-blocking by default.
