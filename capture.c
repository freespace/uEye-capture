#include <stdio.h>
#include <stdlib.h>

#include <uEye.h>

void chk(int st) {
  if (st != IS_SUCCESS) {
    fprintf(stderr, "API Error: %d\n", st);
    exit(st);
  }
}

HIDS cam_connect(int *widthout, int*heightout) {
  fprintf(stderr, "Connecting...");
  HIDS hCam = 0;

  chk(is_InitCamera(&hCam, NULL));

  fprintf(stderr, "connected to camera: %d\n", hCam);

  SENSORINFO sensinfo;
  chk(is_GetSensorInfo(hCam, &sensinfo));

  fprintf(stderr, "\tSensor: %s\n"
                  "\tResolution: %ux%u\n"
                  "\tPixel Size: %.2f\n",
                  sensinfo.strSensorName,
                  sensinfo.nMaxWidth,
                  sensinfo.nMaxHeight,
                  sensinfo.wPixelSize/100.0);

  chk(is_GetSensorInfo(hCam, &sensinfo));
  return hCam;
}

int main(void) {
  int width, height;
  HIDS hCam = cam_connect(&width, &height);
  //chk(is_FreezeVideo(hCam, IS_WAIT));

  chk(is_ExitCamera(hCam));
  fprintf(stderr, "Disconnected from camera %d\n", hCam);

  return 0;
}
