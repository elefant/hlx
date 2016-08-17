# Build
  $ cmake .
  $ make all

# Run
- Server

  $ bin/TestServer

- Client

  $ bin/TestClient [tcp/udp] [username] [password]

# Notes

1. This project depends on cmake and boost. So, please ensure them or run

   $ sudo apt-get install cmake libboost-all-dev

2. libdnet/ is the library I wrote from scrath for other project a couple of
   years ago. At the time of writing, I followed some other coding standard,
   which is different from what I am using right now in mozilla. The source
   files in src/ are ones I wrote for this test. I also removed and did a small
   tweak to libdent to make it more suitable for the test.

3. I didn't finish all the requirements including
_
a) The connection via UDP doesn't require an username/password authentication since
   the UDP socket is connection-less so I have to maintain the connection for
   authentication on my own. I don't have bunch of time to implement that but
   it should be straighforward since the ip/port is easily to get by "recvFrom".

b) I hard-code the IP addresses and ports so the client/server couldn't be
   deployed on different machine.

4. Some improvements:

a) The username/password could be read from a file or even a database.

b) All the communcation is line-based so there's a one-byte overhead for each packet.
   We can further reduce the overhead by pre-defining the strcuture or introducing
   third-party library like protobuf. However, for UDP, we actually can do a
   chunk based communication since it's datagram so that we don't need to
   split the inbound data.