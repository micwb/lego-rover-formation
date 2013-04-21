#ifndef CAMERAWIDGET_H
#define CAMERAWIDGET_H

#include <QtWidgets/QWidget>
#include <QtGui/QImage>
#include <QtGui/QPaintEvent>

class CameraWidget : public QWidget
{
   Q_OBJECT

public:
    CameraWidget(QWidget *parent);
    ~CameraWidget();

    void updateImage(QImage* img);

protected:
   void paintEvent(QPaintEvent* e);

private:
   QImage* image;
};

#endif // CAMERAWIDGET_H
