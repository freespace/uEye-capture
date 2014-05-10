#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

int main (int argc, char **argv) {
  assert (argc == 4);
  char *host = argv[1];
  char *port = argv[2];
  char *msg = argv[3];

  void *ctx = zmq_ctx_new();
  void *requester = zmq_socket(ctx, ZMQ_REQ);
  char buf[512];

  assert(strlen(host) + strlen(port) < 500);

  sprintf(buf, "tcp://%s:%s", host, port);
  int st = zmq_connect(requester, buf);
  
  assert (st == 0);

  zmq_send(requester, msg, strlen(msg), 0);
  zmq_recv(requester, buf, sizeof buf, 0);

  zmq_close(requester);
  zmq_ctx_destroy(ctx);

  return 0;
}


