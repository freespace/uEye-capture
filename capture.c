#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

#include <zmq.h>
#if ZMQ_VERSION_MAJOR < 3
# error ZMQ version 3 or higher is required
#endif

#include <assert.h>

#include "camera.h"

Camera camera;
void cleanup(void) {
  cam_disconnect(&camera);
}

int main(int argc, char **argv) {
  cam_connect(&camera);
  
  atexit(cleanup);

  if (argc == 1) {
    cam_capture(&camera, NULL);
  } else {
    // we were given a port numbers, so go into network mode
    // and listen on that port
    char *endptr = NULL;
    long port = strtol(argv[1], &endptr, 10);
    assert(endptr != NULL && *endptr == '\0');

    void *ctx = zmq_ctx_new();
    void *responder = zmq_socket(ctx, ZMQ_REP);
    char bindstr[128] = {'\0'};
    snprintf(bindstr, (sizeof bindstr)-1, "tcp://*:%ld", port);

    int st = zmq_bind(responder, bindstr);
    if (st != 0) {
      printf("Could not listen on %s\n", bindstr);
      abort();
    }

    fprintf(stderr, "Listening on %s\n", bindstr);

    char buf[512];
    const char *bufp;

    wchar_t wbuf[(sizeof buf)];
    char *okp = "ok";
    char *cmd_exit = "@exit";
    char *cmd_status = "@status";

    for (;;) {
      memset(buf, '\0', sizeof buf);
      bufp = buf;

      zmq_recv(responder, buf, (sizeof buf)-1, 0);
      fprintf(stderr, "Got: %s\n", buf);

      if (strncmp(buf, cmd_exit, strlen(cmd_exit)) == 0) {
        // exit command
        
        // do this to allow the client to exit naturally
        zmq_send(responder, okp, strlen(okp), 0);
        break;
      } else if (strncmp, cmd_status, strlen(cmd_status) == 0) {
        // status command

        // or now the status command is only used to determine if we are
        // in this loop
        zmq_send(responder, okp, strlen(okp), 0);
      } else 
        // assume we got a filepath, take a snap shot
        mbsrtowcs(wbuf, &bufp, (sizeof buf), NULL);

        cam_capture(&camera, wbuf);

        zmq_send(responder, okp, strlen(okp), 0);
      } 
    }

    zmq_close(responder);
    zmq_ctx_destroy(ctx);
  }


  cleanup();
  return 0;
}
