# Ethernet5500 v1.5.5

Arduino Ethernet library for boards with the WIZnet W5500 networking chip. Based
on the
[Ethernet3 library developed by sstaub](https://github.com/sstaub/Ethernet3). It
is mostly the same initially, though I (jamessynge) have removed most of the
README.md and added method EthernetClass::setDhcp(DhcpClass*), which must be
called in order to use DHCP to request an address for the Arduino.

## Plans

*   Make writing non-blocking by default.
*   Add an DhcpClass& argument to EthernetClass::begin(mac) so that it isn't
    possible to forget to call setDhcp. The same could be done with
    EthernetClass::maintain. This could eliminate the _dhcp data member of
    EthernetClass.

## Issues

*   It seems to take two calls to begin(mac) in order to get an address assigned
    using DHCP. This used to work. What has happened?
