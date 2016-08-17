#include "net/TcpSocket.h"
#include "net/UdpSocket.h"
#include "sys/ThreadPool.h"

static std::string getCurrentLocalTime()
{
  time_t t = time( NULL );
  struct tm tm = *localtime( &t );
  char buffer[1024] = { 0 };
  strftime( buffer, sizeof( buffer ), "%Y-%m%d-%H%M-%S", &tm );
  return buffer;
}

class TestClient : public utl::Logger {
public:
  TestClient(const std::string& aSocketType,
             const std::string& aUserName,
             const std::string& aPassword)
    : Logger("TestClient")
    , mThreadPool(2, 10)
    , mSocketType(aSocketType)
    , mUserName(aUserName + "\n")
    , mPassword(aPassword + "\n")
  {
  }

  bool run()
  {
    if ("tcp" == mSocketType) {
      net::TcpSocketPtr tcpClient = net::TcpSocket::Create();
      if (!tcpClient->connect("127.0.0.1", 8888)) {
        error("Failed to connect to local.");
        return false;
      }

      if (!tcpClient->send(mUserName.c_str(), mUserName.length()) ||
          !tcpClient->send(mPassword.c_str(), mPassword.length())) {
        error("Failed to send user name and password to server.");
        return false;
      }

      mThreadPool.postTask(boost::bind(&TestClient::sendCurrentLocalTimeForever, this, tcpClient),
                          "TcpSendTime");
      return true;
    }

    if ("udp" == mSocketType) {
      // Fire an udp sending-time
      net::UdpSocketPtr udpClient = net::UdpSocket::Create();
      udpClient->setSendDestination("127.0.0.1", 9999);
      mThreadPool.postTask(boost::bind(&TestClient::sendCurrentLocalTimeForever, this, udpClient),
                          "UdpSendTime");

      return true;
    }

    return false;
  }

private:
  bool sendCurrentLocalTimeOnce(net::SocketPtr aClientSocket)
  {
    int retryRemain = 5;

    std::string currentTime = getCurrentLocalTime() + "\n";
    while (retryRemain) {
      retryRemain--;

      if (!aClientSocket->send(currentTime.c_str(), currentTime.length())) {
        // Unable to send!
        return false;
      }

      const int TIMEOUT_MS = 2000; // 2 seconds.
      std::string line;

      if (!aClientSocket->receiveLine(line, TIMEOUT_MS)) {
        error("Faile to receive line\n");
        continue;
      }
      if (0 != line.find("ACK")) {
        error("Received non-ACK\n");
        continue;
      }

      // Got ACK. Awesome!
      return true;
    }

    return false;
  }

  void sendCurrentLocalTimeForever(net::SocketPtr aClientSocket)
  {
    while (sendCurrentLocalTimeOnce(aClientSocket)) {
      sleep(7);
    }
  }

  sys::ThreadPool mThreadPool;
  std::string mSocketType;
  std::string mUserName;
  std::string mPassword;
};

int main(int argc, char* argv[])
{
  if (argc < 4) {
    perror("Please pass in the socket type, username and password\n");
    perror("For example: TestClient tcp thisIsUsername thisIsPassword\n");
    return -1;
  }

  std::string socketType = argv[1];
  std::string userName = argv[2];
  std::string password = argv[3];

  if (socketType != "tcp" && socketType != "udp") {
    perror("Invalid socket type");
    return -1;
  }

  TestClient client(socketType, userName, password);
  if (!client.run()) {
    return -1;
  }
  while (getchar());
}
