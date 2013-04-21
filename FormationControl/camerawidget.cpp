#include "camerawidget.h"

#include <QtGui/QPainter>
#include <QtGui/QImage>

static const int CAMERA_SCALE_PPM_DIST = 100;     //constant for camera
static const int CAMERA_DISTANCE_MM    = 2000;    //constant for application
int gCameraScalePPM;

CameraWidget::CameraWidget(QWidget *parent)
   : QWidget(parent)
{
   image = 0;
   //gCameraScalePPM = (int)((float)(CAMERA_DISTANCE_MM * CAMERA_SCALE_PPM_DIST) / 1000.0);
   gCameraScalePPM = 200;
}

CameraWidget::~CameraWidget()
{

}

void CameraWidget::updateImage(QImage* img)
{
   if(img)
   {
      image = img;
      update();
   }
}

void CameraWidget::paintEvent(QPaintEvent* e)
{
   if(image)
   {
      resize(image->size());
      QPainter painter(this);
      painter.drawImage(0, 0, image->rgbSwapped().mirrored());
      painter.end();
   }
}

