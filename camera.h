#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include <uEye.h>

typedef struct {
  HIDS hCam;
  unsigned int width;
  unsigned int height;
  char *imagemem;
  unsigned int imagememid;
} Camera;

/**
 * Connects an uEye camera. On success camera will contain valid values.
 */
int cam_connect(Camera *camera);

/**
 * Counter part to cam_connect, call this to properly close the camera
 * and release assoicated memory buffers
 */ 
void cam_disconnect(Camera *camera);

/**
 * Causes a snapshot to be take an written to path as a BMP file. If path
 * is NULL, then the snapshot is written to "snapshot.bmp"
 */
wchar_t *cam_capture(Camera *camera, wchar_t *path);

#endif
