/**
 * This program when ran without arguments will open the camera and capture
 * a single frame into snapshot.bmp.
 *
 * When ran with a single argument, it will covert that argument into an
 * integer, and then open an zeromq socket on that port, listening on all
 * interfaces. This is called network mode.
 *
 * In network mode, when it receives a string, which is not expected to be
 * null terminated, it will interpret that string as a file path. It will then
 * take a snapshot, and save it to that path.
 *
 * The program will exit network mode when !EXIT is received.
 */
#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>

#include <zmq.h>

#include <assert.h>

#include "camera.h"

int main(int argc, char **argv) {
  Camera camera;
  cam_connect(&camera);
  cam_capture(&camera, NULL);
  cam_disconnect(&camera);

  return 0;
}
