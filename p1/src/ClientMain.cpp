#include "net/TcpSocket.h"
#include "net/UdpSocket.h"
#include "sys/ThreadPool.h"
#include "utl/Timer.h"

static std::string getCurrentLocalTime()
{
  time_t t = time( NULL );
  struct tm tm = *localtime( &t );
  char buffer[1024] = { 0 };
  strftime( buffer, sizeof( buffer ), "%Y-%m%d-%H%M-%S", &tm );
  return buffer;
}

static bool sendCurrentLocalTimeOnce(net::SocketPtr aClientSocket)
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
      printf("Faile to receive line\n");
      continue;
    }
    if (0 != line.find("ACK")) {
      printf("Received non-ACK\n");
      continue;
    }

    // Got ACK. Awesome!
    return true;
  }

  return false;
}

static void sendCurrentLocalTimeForever(net::SocketPtr aClientSocket)
{
  while (sendCurrentLocalTimeOnce(aClientSocket)) {
    sleep(7);
  }
}

int main()
{
  sys::ThreadPool threadPool(2, 10);

  // Fire an tcp sending-time
  net::TcpSocketPtr tcpClient = net::TcpSocket::Create();
  if (!tcpClient->connect("127.0.0.1", 8888)) {
    return -1;
  }
  threadPool.postTask(boost::bind(&sendCurrentLocalTimeForever, tcpClient),
                      "TcpSendTime");

  // Fire an udp sending-time
  net::UdpSocketPtr udpClient = net::UdpSocket::Create();
  udpClient->setSendDestination("127.0.0.1", 9999);
  threadPool.postTask(boost::bind(&sendCurrentLocalTimeForever, udpClient),
                      "UdpSendTime");

  while (getchar());
}
