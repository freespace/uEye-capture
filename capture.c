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

  if (widthout) *widthout = sensinfo.nMaxWidth;
  if (heightout) *heightout = sensinfo.nMaxHeight;

  return hCam;
}

int main(void) {
  int width, height;
  HIDS hCam = cam_connect(&width, &height);

  fprintf(stderr, "Setting color mode\n");
  // use 8 bits per channel, with 3 channels. Need BGR order for correct
  // colour display when saving to BMP
  chk(is_SetColorMode(hCam, IS_CM_BGR8_PACKED));

  // set a low exposure
  double exposure_ms = 1000;
  chk(is_Exposure(
        hCam,
        IS_EXPOSURE_CMD_SET_EXPOSURE,
        &exposure_ms,
        sizeof(exposure_ms)));


  fprintf(stderr, "Allocating memory for images\n");
  char *imagemem = NULL;
  int imagememid;

  chk(is_AllocImageMem(
        hCam, 
        width,
        height,
        3*8,      // bits per pixel
        &imagemem,
        &imagememid));
  
  chk(is_SetImageMem(hCam, imagemem, imagememid));

  fprintf(stderr, "Capturing image\n");
  chk(is_FreezeVideo(hCam, IS_WAIT));

  wchar_t *outfileame = L"snapshot.bmp";

  fprintf(stderr, "Saving image to %ls\n", outfileame);
  IMAGE_FILE_PARAMS fparams;
  fparams.pwchFileName = outfileame;
  fparams.nFileType = IS_IMG_BMP;
  fparams.ppcImageMem = &imagemem;
  fparams.pnImageID = (unsigned int*)&imagememid;

  chk(is_ImageFile(
        hCam,
        IS_IMAGE_FILE_CMD_SAVE,
        &fparams,
        sizeof(fparams)));

  chk(is_FreeImageMem(hCam, imagemem, imagememid));

  chk(is_ExitCamera(hCam));
  fprintf(stderr, "Disconnected from camera %d\n", hCam);

  return 0;
}
