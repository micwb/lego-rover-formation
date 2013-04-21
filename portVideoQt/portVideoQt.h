/*  portVideo, a cross platform camera framework
    Copyright (C) 2005 Martin Kaltenbrunner <mkalten@iua.upf.es>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef PORTVIDEOQT_H
#define PORTVIDEOQT_H

#include <vector>
#include <algorithm>

#ifdef WIN32
#include <windows.h>
#else
#include <stdio.h>
#include <stdlib.h>
#endif

#include <QMessageBox>
#include <QtWidgets/QApplication>
#include <QtGui/QImage>
#include <QtCore/QThread>

#include <time.h>

#define WIDTH 640
#define HEIGHT 480

#include "cameraTool.h"
#include "RingBuffer.h"
#include "FrameProcessor.h"
#include "cameraWidget.h"

class CameraThread;

class portVideoQt : public QThread
{
public:
	portVideoQt();
	portVideoQt(char* name, bool srcColour, bool destColour, CameraWidget* cameraWidget);
	~portVideoQt() {};
	
	void run();

	cameraEngine *camera_;
	unsigned char *cameraBuffer_;
	bool running_;
	bool pause_;
	
	RingBuffer *ringBuffer;
	
	void addFrameProcessor(FrameProcessor *fp);
	void removeFrameProcessor(FrameProcessor *fp);
	
	long framenumber_;
	
	enum DisplayMode { NO_DISPLAY, SOURCE_DISPLAY, DEST_DISPLAY };
	void setDisplayMode(DisplayMode mode);

	QSize getSize() { while(!running_); return QSize(width_, height_);};
	int   getFps()  { while(!running_); return fps_;};

protected:
	bool setupWindow();
	void teardownWindow();

	bool setupCamera();
	void teardownCamera();
	
	void initFrameProcessors();
	
	void allocateBuffers();
	void freeBuffers();
	
	void mainLoop();
	
	void process_events();
	void stopAndClose();
	
	void saveBuffer(unsigned char* buffer, int size);
	
	QImage *sourceImage_;
	QImage *destImage_;

	unsigned char* sourceBuffer_;
	unsigned char* destBuffer_;

private:
   CameraWidget* cw;

	long frames_;
  	long lastTime_;

	bool fpson_;
	void fpsCount();

	int width_;
	int height_;
	int fps_;
	int sourceDepth_;
	int destDepth_;
	int bytesPerSourcePixel_;
	int bytesPerDestPixel_;
	
	std::vector<FrameProcessor*> processorList;
	std::vector<FrameProcessor*>::iterator frame;
		
	
	DisplayMode displayMode_;

	CameraThread *cameraThread;
	
	char* appName;
};

class CameraThread : public QThread
{
public:
   CameraThread(portVideoQt* _engine) { engine = _engine;};

protected:
   void run()
   {
		unsigned char *cameraBuffer = NULL;
		unsigned char *cameraWriteBuffer = NULL;
		
		while(engine->running_) {
			if(!engine->pause_) {
				cameraBuffer = engine->camera_->getFrame();
				if (cameraBuffer!=NULL) {
				
					cameraWriteBuffer = engine->ringBuffer->getNextBufferToWrite();
					if (cameraWriteBuffer!=NULL) {
						memcpy(cameraWriteBuffer,cameraBuffer,engine->ringBuffer->size());
						engine->framenumber_++;
						engine->ringBuffer->writeFinished();
					}
					msleep(20);
				} /*else {
					if (!engine->camera_->stillRunning()) {
						printf("camera disconnected\n");
						engine->running_=false;
					} else SDL_Delay(1);
				}*/
			} else msleep(20);
		}
		exit(0);
   }
private:
   portVideoQt* engine;
};

#endif
