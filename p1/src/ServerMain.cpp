#include "net/TcpServer.h"
#include "net/UdpSocket.h"

namespace {

static struct {
  std::string mUserName;
  std::string mPassword;
  bool mIsLoggedIn;
} ACCEPTABLE_USERS[] = {
  { "PikachuA", "abcd", false },
  { "PikachuB", "abcd", false },
  { "PikachuC", "abcd", false },
  { "PikachuD", "abcd", false },
};

static bool verifyUser(const std::string& aUserName, const std::string& aPassword)
{
  for (size_t i = 0; i < sizeof(ACCEPTABLE_USERS)/sizeof(ACCEPTABLE_USERS[0]); i++) {
    auto u = ACCEPTABLE_USERS[i];
    if (aUserName == u.mUserName && aPassword == u.mPassword) {
      // User found.
      return true;
    }
    if (aUserName == u.mUserName && aPassword != u.mPassword) {
      // Invalid password.
      return false;
    }
  }
  // Invalid user name.
  return false;
}

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
    // Verify user log in.
    std::string userName;
    std::string password;
    if (!aClientSocket->receiveLine(userName) ||
        !aClientSocket->receiveLine(password) ||
        !verifyUser(userName, password)) {
      error("Invalid user '%s' logged in", userName.c_str());
      aClientSocket->close();
      return;
    }

    trace("Verify success!");

    // Would be run onn off-main thread.
    std::string line;
    while (aClientSocket->receiveLine(line)) {
      trace("Received %s", line.c_str());
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
