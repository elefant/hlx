#include "net/TcpServer.h"
#include "net/UdpSocket.h"

namespace {

//----------------------------------------------------------
// TCP server
//----------------------------------------------------------

class TestTcpServer final : public net::TcpServer
{
public:
  TestTcpServer(short aPort)
    : TcpServer(aPort, 10)
  {
  }

private:
  virtual void onClientAccepted(net::TcpSocketPtr aClientSocket) override
  {
    // Would be run onn off-main thread.
    std::string line;
    while (aClientSocket->receiveLine(line)) {
      printf("Received %s\n", line.c_str());
      const std::string ACK = "ACK\n";
      aClientSocket->send(ACK.c_str(), ACK.length());
    }
  }
};

//----------------------------------------------------------
// UDP server
//----------------------------------------------------------

class TestUdpServer final
{
public:
  TestUdpServer(short aPort)
    : mUdpSocket(net::UdpSocket::Create())
  {
    mUdpSocket->bind(aPort, "");
  }

  void run()
  {
    // Would be run on main thread.

    while (1) {
      std::string hostIp;
      short hostPort;
      char buffer[512];
      if (-1 == mUdpSocket->receivFrom(buffer, sizeof(buffer), hostIp, hostPort)) {
        return;
      }

      std::string ACK = "ACK\n";
      mUdpSocket->sendTo(ACK.c_str(), ACK.length(), hostIp, hostPort);
    }
  }

private:
  net::UdpSocketPtr mUdpSocket;
};

} // end of unnamed namespace.

int main()
{
  TestTcpServer tcpServer(8888);
  tcpServer.run(false); // non-blocking

  TestUdpServer udpServer(9999);
  udpServer.run(); // blocking
}
