#include "common.h"

/* Address Info */
void address() {
  struct addrinfo hints;
  struct addrinfo *info;
  int ret;
  char ipstr[14];

  memset(&hints, 0, sizeof hints);

  hints.ai_protocol = 0;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_family = AF_INET;

  ret = getaddrinfo("127.0.0.1", "621", &hints, &info);
  if (ret != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(ret));
    exit(1);
  }

  // get the first address returned by getaddrinfo()
  struct sockaddr_in *ipv4 = (struct sockaddr_in *)info->ai_addr;
  inet_ntop(AF_INET, &ipv4->sin_addr.s_addr, ipstr, sizeof(ipstr));

  // print IP address of client
  printf("%s\n", ipstr);

  freeaddrinfo(info);
}

/* Configure Server Socket */
void server_init(const int *server_fd, const struct sockaddr_in *server_addr) {
  int ret;

  // bind socket with address properties
  ret = bind(*server_fd, (struct sockaddr *)server_addr, sizeof *server_addr);
  if (ret < 0)
    error("ERROR binding the socket");

  // listen for connections, max 5 at a time
  ret = listen(*server_fd, 1);
  if (ret < 0)
    error("ERROR listening for connections");
}

 /*Communicate with a client connection */
void talk_client(int client_fd) {
  char buf[BUFSIZ];
  int ret;

  // send rules to client
  bzero(buf, BUFSIZ);
  char msg[] = "Press 'Ctrl + ]' to end the conversation\r\n";
  ret = write(client_fd, msg, sizeof(msg));
  if (ret < 0)
    error("client did not get the rules");

  while (1) {
    int pos = 0;

    // read from client
    bzero(buf, BUFSIZ);
    while (pos < BUFSIZ - 1) {
      ret = read(client_fd, &buf[pos], 1);
      if (ret < 0)
        error("ERROR reading from socket");
      if (buf[pos] == '\0') // newline
        break;
      ++pos;
    }

    // exit condition
    if (pos == 2 && buf[0] == '\035') {
      printf("terminating conversation...\n");
      return;
    }

    // output client message
    printf("prayer: %s", buf);

    // send a reply back to the client
    bzero(buf, BUFSIZ);
    strcpy(buf, "hello darkness my old friend\r\n");
    ret = write(client_fd, buf, strlen(buf) + 1);
    if (ret < 0)
      error("ERROR writing to client");
  }

  return;
}

/* Accept connections */
void server_accept(int server_fd) {
  char buf[BUFSIZ];
  int ret, client_fd;
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  // start accepting connections
  client_fd =
      accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
  if (client_fd < 0)
    error("ERROR accepting connections");

  // get stream rather than block
  talk_client(client_fd);

  // close the connection
  shutdown(client_fd, SHUT_RDWR);
}

int main() {
  int server_fd;
  struct sockaddr_in server_addr;

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0)
    error("ERROR creating a socket");

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) <
      0) {
    error("setsockopt(SO_REUSEADDR) failed");
  }

  // need to set to zero for padding and proper init
  memset(&server_addr, 0, sizeof server_addr);

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

  server_init(&server_fd, &server_addr);
  printf("Server listening on %d...\n", PORT);

  server_accept(server_fd);

  shutdown(server_fd, SHUT_RDWR);

  return 0;
}
