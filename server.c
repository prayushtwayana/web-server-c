#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
  char buffer[BUFSIZ];
  int server_fd, client_fd, ret;
  uint16_t port = 6969;
  struct sockaddr_in server_addr, client_addr;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("ERROR creating a socket");
    exit(1);
  }

  memset(&server_addr, 0, sizeof server_addr);

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr);

  ret = bind(server_fd, (struct sockaddr *) &server_addr, sizeof server_addr);
  if (ret < 0) {
    perror("ERROR binding the socket");
    exit(1);
  }

  ret = listen(server_fd, 5);
  if (ret < 0) {
    perror("ERROR listening for connections");
    exit(1);
  }

  socklen_t client_addr_len = sizeof(client_addr);
  client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
  if (ret < 0) {
    perror("ERROR accepting connections");
    exit(1);
  }

  bzero(&buffer, BUFSIZ);
  ret = read(client_fd, buffer, BUFSIZ - 1);
  if (ret < 0) {
    perror("ERROR reading from socket");
    exit(1);
  }
  char ip_str[13];
  printf("%s", buffer);
  /*printf("client ip: %s\tclient port:%u\tmessage: %s\n", inet_ntop(AF_INET, client_addr.sin_addr.s_addr, ip_str, sizeof ip_str), htons(client_addr.sin_port), buffer);*/

  close(client_fd);
  close(server_fd);

  return 0;
}
