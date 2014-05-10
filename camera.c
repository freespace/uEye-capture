#include "camera.h"

static void chk(int st) {
  if (st != IS_SUCCESS) {
    fprintf(stderr, "API Error: %d\n", st);
    exit(st);
  }
}

int cam_connect(Camera *camera) {
  fprintf(stderr, "Connecting...");
  HIDS hCam = 0;

  chk(is_InitCamera(&hCam, NULL));

  fprintf(stderr, "connected to camera: %d\n", hCam);
  camera->hCam = hCam;

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

  camera->width = sensinfo.nMaxWidth;
  camera->height = sensinfo.nMaxHeight;

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
        camera->hCam, 
        camera->width,
        camera->height,
        3*8,      // bits per pixel
        &imagemem,
        &imagememid));
  
  chk(is_SetImageMem(hCam, imagemem, imagememid));

  camera->imagemem = imagemem;
  camera->imagememid = (unsigned int)imagememid;

  camera->status = CameraStatusConnected;
  return 0;
}

void cam_disconnect(Camera *camera) {
  if (camera->status == CameraStatusConnected) {
    chk(is_FreeImageMem(camera->hCam, 
                        camera->imagemem,
                        camera->imagememid));

    chk(is_ExitCamera(camera->hCam));
    fprintf(stderr, "Disconnected from camera %d\n", camera->hCam);
  }

  camera->status = CameraStatusDisconnected;
}

wchar_t *cam_capture(Camera *camera, wchar_t *path) {
  fprintf(stderr, "Capturing image\n");
  chk(is_FreezeVideo(camera->hCam, IS_WAIT));

  if (path == NULL) path = L"snapshot.bmp";

  fprintf(stderr, "Saving image to %ls\n", path);
  IMAGE_FILE_PARAMS fparams;
  fparams.pwchFileName = path;
  fparams.nFileType = IS_IMG_BMP;
  fparams.ppcImageMem = &camera->imagemem;
  fparams.pnImageID = &camera->imagememid;

  chk(is_ImageFile(
        camera->hCam,
        IS_IMAGE_FILE_CMD_SAVE,
        &fparams,
        sizeof(fparams)));
  return path;
}
