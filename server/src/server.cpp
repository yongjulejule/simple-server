/**
 * @file server.c
 * @author yongjule (lyjshow200@gmail.com)
 * @brief simple server
 * @version 0.1
 * @date 2022-03-23
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <arpa/inet.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <unistd.h>

#include <fstream>
#include <iostream>

#define BUFFER_SIZE 2048

bool isNumbers(std::string str) {
  return !str.empty() &&
         std::find_if(str.begin(), str.end(), [](char c) -> bool {
           return !std::isdigit(c);
         }) == str.end();
}

int main(int argc, char **argv) {
  if (argc != 2 || !isNumbers(argv[1])) {
    std::cerr << "Usage: " << argv[0] << " <port>\n";
    exit(EXIT_FAILURE);
  }

  // create a socket (TCP, IPv4). check /etc/protocols
  int socketFd = socket(AF_INET, SOCK_STREAM, 0);
  if (socketFd == -1) {
    std::cerr << "socket() failed: " << std::strerror(errno) << "\n";
    exit(EXIT_FAILURE);
  }

  sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(atoi(argv[1]));
  serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

  // bind the socket to the IP address and port number
  if (bind(socketFd, reinterpret_cast<sockaddr *>(&serverAddr),
           sizeof(serverAddr)) == -1) {
    std::cerr << "bind() failed: " << std::strerror(errno) << "\n";
    exit(EXIT_FAILURE);
  }

  // mark the socket as a passive socket (대기상태)
  if (listen(socketFd, 42) == -1) {
    std::cerr << "listen() failed: " << std::strerror(errno) << "\n";
    exit(EXIT_FAILURE);
  }

  sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);

  // extract request from client and fill info to clientAddr. Finally return new
  // socket fd to communicate with client
  int connectionFd = accept(socketFd, reinterpret_cast<sockaddr *>(&clientAddr),
                            &clientAddrLen);
  if (connectionFd == -1) {
    std::cerr << "accept() failed: " << std::strerror(errno) << "\n";
    exit(EXIT_FAILURE);
  }

  std::string buffer;
  while (std::getline(std::cin, buffer))
    ;

  write(connectionFd, buffer.c_str(), buffer.size());
  write(1, "Hello, Server!\n", 15);

  close(connectionFd);
  close(socketFd);

  return EXIT_SUCCESS;
}
