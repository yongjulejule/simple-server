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

#include <iostream>

#define BUFFER_SIZE 2048

bool isNumbers(std::string str) {
  return !str.empty() &&
         std::find_if(str.begin(), str.end(), [](char c) -> bool {
           return !std::isdigit(c);
         }) == str.end();
}

bool isIPv4(char *ip) {
  struct sockaddr_in sa;
  return inet_pton(AF_INET, ip, &(sa.sin_addr)) != 0;
}

int main(int argc, char **argv) {
  if (argc != 3 || !isNumbers(argv[2]) || !isIPv4(argv[1])) {
    std::cerr << "Usage: ./server <IPv4> <port>\n";
    return EXIT_FAILURE;
  }

  int clientFd;
  char buffer[BUFFER_SIZE];
  memset(buffer, 0, BUFFER_SIZE);

  clientFd = socket(AF_INET, SOCK_STREAM, 0);
  if (clientFd == -1) {
    std::cerr << "socket() failed: " << strerror(errno) << "\n";
    return EXIT_FAILURE;
  }

  struct sockaddr_in serverAddr;
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = inet_addr(argv[1]);
  serverAddr.sin_port = htons(atoi(argv[2]));

  if (connect(clientFd, reinterpret_cast<sockaddr *>(&serverAddr),
              sizeof(serverAddr)) == -1) {
    std::cerr << "connect() failed: " << strerror(errno) << "\n";
    return EXIT_FAILURE;
  }

  if (read(clientFd, buffer, BUFFER_SIZE) == -1) {
    std::cerr << "read() failed: " << strerror(errno) << "\n";
    return EXIT_FAILURE;
  }
  std::cout << buffer;

  close(clientFd);

  return EXIT_SUCCESS;
}