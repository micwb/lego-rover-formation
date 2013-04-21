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

#include "portVideoQt.h"

void portVideoQt::saveBuffer(unsigned char* buffer,int size) {
	char fileName[32];
	sprintf(fileName,"frame%ld_%dx%d.raw",framenumber_, width_, height_);
	FILE*  imagefile=fopen(fileName, "w");
	fwrite((const char *)buffer, 1,  size, imagefile);
	fclose(imagefile);
}


// the principal program sequence
void portVideoQt::run() {

	if( !setupCamera() )
		return;
	
	if( !setupWindow() ) {
		teardownCamera();
		return;
	}
	
	allocateBuffers();
	initFrameProcessors();

	bool success = camera_->startCamera();

	if( success ){

		//SDL_EnableKeyRepeat(200, 10);

		//printf( "display:  n - none\n" );
		//printf( "          s - source\n" );
		//printf( "          t - target\n" );		
		//printf( "          f - fps\n" );
		//printf( "settings: c - camera\n" );
		//printf( "commands: b - save BMP frame\n" );
		//printf( "          r - save RAW frame\n" );
		//printf( "          p - pause\n" );
		//printf( "          ESC - quit\n" );
		//printf( "\n" );

		for (frame = processorList.begin(); frame!=processorList.end(); frame++)
			(*frame)->printOptions();
	
      //CREATE QT THREAD
		cameraThread = new CameraThread(this);
      cameraThread->start();
		
		mainLoop();

		camera_->stopCamera();

	}else{
      //QMessageBox::critical(0, "Critical", "could not start camera\n");
	}	

	teardownCamera();
	teardownWindow();
	
	freeBuffers();
	
}

// does what its name suggests
void portVideoQt::mainLoop()
{
	unsigned char *cameraReadBuffer = NULL;

	while(running_) {
		
		process_events();

		// do nothing if paused
		if (pause_){
			msleep(10);
			continue;
		}

		// loop until we get access to a frame
		cameraReadBuffer = ringBuffer->getNextBufferToRead();
		while (cameraReadBuffer==NULL) {
			msleep(30);
			cameraReadBuffer = ringBuffer->getNextBufferToRead();
			if(!running_) goto emergencyexit; // escape on quit
		}
		
		// try again if we can get a more recent fram
		do {
			memcpy(sourceBuffer_,cameraReadBuffer,ringBuffer->size());
			ringBuffer->readFinished();
			
			cameraReadBuffer = ringBuffer->getNextBufferToRead();
		} while( cameraReadBuffer != NULL );
		
		// do the actual image processing job
		for (frame = processorList.begin(); frame!=processorList.end(); frame++)
			(*frame)->process(sourceBuffer_,destBuffer_);
		
		// update display
		switch( displayMode_ ) {
			case NO_DISPLAY:
				break;
			case SOURCE_DISPLAY: {
            if(cw)
               cw->updateImage(sourceImage_);
				//SDL_BlitSurface(sourceImage_, NULL, window_, NULL);
				//SDL_Flip(window_);
				//SDL_UpdateRect( window_, 0, 0, width_, height_ );
				break;
			}			
			case DEST_DISPLAY: {
            if(cw)
               cw->updateImage(destImage_);
				//SDL_BlitSurface(destImage_, NULL, window_, NULL);
				//SDL_Flip(window_);
				//SDL_UpdateRect( window_, 0, 0, width_, height_ );
				break;
			}
		}
		
		//if (fpson_) fpsCount();
	}
	emergencyexit: 
	
	// do the actual image processing job
	for (frame = processorList.begin(); frame!=processorList.end(); frame++)
		(*frame)->finish();

	return;
}

void portVideoQt::fpsCount() {

#if 0
	frames_++;

	time_t currentTime;
	time(&currentTime);
	long diffTime = (long)( currentTime - lastTime_ );
	
	if (diffTime >= 2) {
		int fps = (int)( frames_ / diffTime );
		static char caption[24] = "";
		sprintf(caption,"%s - %d FPS",appName,fps);
		SDL_WM_SetCaption( caption, NULL );
		frames_ = 0;
		
		lastTime_ = (long)currentTime;
    }
#endif
}

bool portVideoQt::setupWindow() {
#if 0
	// initialize SDL
	if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		printf("SDL could not be initialized: %s\n", SDL_GetError());
		//SDL_Quit();
		return false;
	}

	#ifndef __APPLE__
	SDL_WM_SetIcon(SDL_LoadBMP("icon.bmp"), NULL);
	#endif
	
	window_ = SDL_SetVideoMode(width_, height_, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
	if ( window_ == NULL ) {
		printf("Could not open window: %s\n", SDL_GetError());
		SDL_Quit();
		return false;
	}

	static char caption[24] = "";
	sprintf(caption,"%s - ?? FPS",appName);
	SDL_WM_SetCaption(caption, NULL);
#endif
	return true;
}

void portVideoQt::teardownWindow()
{
	exit();
   qApp->quit();
}

void portVideoQt::process_events()
{
#if 0
    SDL_Event event;

    while( SDL_PollEvent( &event ) ) {

        switch( event.type ) {
			case SDL_KEYDOWN:
				//printf("%d\n",event.key.keysym.sym);
				if( event.key.keysym.sym == SDLK_n ){
					displayMode_ = NO_DISPLAY;
					// turn the display black
					for (int i=0;i<width_*height_*bytesPerDestPixel_;i++)
						destBuffer_[i]=0;
					SDL_BlitSurface(destImage_, NULL, window_, NULL);
					SDL_UpdateRect( window_, 0, 0, width_, height_ );
				} else if( event.key.keysym.sym == SDLK_s ){
					displayMode_ = SOURCE_DISPLAY;
				} else if( event.key.keysym.sym == SDLK_t ){
					displayMode_ = DEST_DISPLAY;
				} else if( event.key.keysym.sym == SDLK_c ){
					camera_->showSettingsDialog(); 
				} else if( event.key.keysym.sym == SDLK_f ){
					if (fpson_) {
						fpson_=false;
						SDL_WM_SetCaption(appName, NULL );
					}
					else {
						// reset the fps counter
						frames_ = 0;
						time_t currentTime;
						time(&currentTime);
						lastTime_ = (long)currentTime;
						fpson_=true;
						static char caption[24] = "";
						sprintf(caption,"%s - ?? FPS",appName);
						SDL_WM_SetCaption(caption, NULL);
					} 
				} else if( event.key.keysym.sym == SDLK_p ){
					if (pause_) {
						pause_=false;
						// reset the fps counter
						frames_ = 0;
						time_t currentTime;
						time(&currentTime);
						lastTime_ = (long)currentTime;
						static char caption[24] = "";
						if (fpson_) sprintf(caption,"%s - ?? FPS",appName);
						else sprintf(caption,"%s",appName);
						SDL_WM_SetCaption(caption, NULL);
						
					} else {
						pause_=true;
						static char caption[24] = "";
						sprintf(caption,"%s - paused",appName);
						SDL_WM_SetCaption(caption, NULL);
						// turn the display black
						for (int i=0;i<width_*height_*bytesPerDestPixel_;i++)
							destBuffer_[i]=0;
						SDL_BlitSurface(destImage_, NULL, window_, NULL);
						SDL_UpdateRect( window_, 0, 0, width_, height_ );
					}
				} else if( event.key.keysym.sym == SDLK_b ){
					char fileName[32];
					sprintf(fileName,"frame%li.bmp",framenumber_);
					SDL_SaveBMP(sourceImage_, fileName);
				} else if( event.key.keysym.sym == SDLK_r ){
					saveBuffer(sourceBuffer_, width_ * height_ * bytesPerSourcePixel_);
				} else if( event.key.keysym.sym == SDLK_ESCAPE ){
					running_=false;
				} else {
					for (frame = processorList.begin(); frame!=processorList.end(); frame++)
						(*frame)->toggleFlag(event.key.keysym.sym);
				}
				break;
			case SDL_QUIT:
				running_ = false;
				break;
        }
    }
#endif
}

bool portVideoQt::setupCamera() {

	camera_ = cameraTool::findCamera();	
	if (camera_ == NULL) return false;
	
	bool colour = false;
	if (sourceDepth_==24) colour = true;
	bool success = camera_->initCamera(width_, height_, colour);
	
	if(success) {
		width_ = camera_->getWidth();
		height_ = camera_->getHeight();
		fps_ = camera_->getFps();
					
		//printf("camera: %s\n",camera_->getName());
		//printf("format: %dx%d, %dfps\n\n",width_,height_,fps_);
		return true;
	} else {
      //QMessageBox::critical(0, "Critical", "could not initialize camera\n");
		teardownCamera();
		return false;
	}
}

void portVideoQt::teardownCamera()
{
	camera_->closeCamera();
	delete camera_;
}

void portVideoQt::allocateBuffers()
{
	bytesPerSourcePixel_ = sourceDepth_/8;	
	bytesPerDestPixel_ = destDepth_/8;
	sourceBuffer_ = new unsigned char[width_*height_*bytesPerSourcePixel_];
	destBuffer_ = new unsigned char[width_*height_*bytesPerDestPixel_];
	cameraBuffer_ = NULL;
		
	//sourceImage_ = SDL_CreateRGBSurfaceFrom(sourceBuffer_, width_, height_, sourceDepth_, width_*bytesPerSourcePixel_, 0 , 0, 0, 0);
	//if (sourceDepth_==8)
	//	SDL_SetPalette(sourceImage_, SDL_LOGPAL|SDL_PHYSPAL, palette_, 0, 256 );
   sourceImage_ = new QImage(sourceBuffer_, width_, height_, QImage::Format_RGB888);

	//destImage_ = SDL_CreateRGBSurfaceFrom(destBuffer_, width_, height_, destDepth_, width_*bytesPerDestPixel_, 0 , 0, 0, 0);
	//if (destDepth_==8)
	//	SDL_SetPalette(destImage_, SDL_LOGPAL|SDL_PHYSPAL, palette_, 0, 256 );
		
   destImage_ = new QImage(sourceBuffer_, width_, height_, QImage::Format_RGB888);
	//SDL_DisplayFormat(sourceImage_);
	//SDL_DisplayFormat(destImage_);
	
	ringBuffer = new RingBuffer(width_*height_*bytesPerSourcePixel_);
}

void portVideoQt::freeBuffers()
{
	delete sourceImage_;
	delete destImage_;
	delete [] sourceBuffer_;
	delete [] destBuffer_;
	
	delete ringBuffer;
}

void portVideoQt::addFrameProcessor(FrameProcessor *fp) {

	processorList.push_back(fp);
}


void portVideoQt::removeFrameProcessor(FrameProcessor *fp) {
	frame = std::find( processorList.begin(), processorList.end(), fp );
	if( frame != processorList.end() )
		processorList.erase( frame );

}

void portVideoQt::setDisplayMode(DisplayMode mode) {
	displayMode_ = mode;
}

void portVideoQt::initFrameProcessors() {
	for (frame = processorList.begin(); frame!=processorList.end(); ) {
		bool success = (*frame)->init(width_ , height_, bytesPerSourcePixel_, bytesPerDestPixel_);
		if(!success) {	
			processorList.erase( frame );
			//printf("removed frame processor\n");
		} else frame++;
	}
}

portVideoQt::portVideoQt(char* name, bool srcColour, bool destColour, CameraWidget* cameraWidget)
	: frames_( 0 )
	, width_( WIDTH )
	, height_( HEIGHT )
	, displayMode_( SOURCE_DISPLAY )
{
	time((time_t*)&lastTime_);
	running_ = true;
	fpson_ = true;
	pause_ = false;
	
	framenumber_=0;
	
	appName = name;
	sourceDepth_ = (srcColour?24:8);
	destDepth_   = (destColour?24:8);
		
	//for(int i=0;i<256;i++){
	//	palette_[i].r=i;
	//	palette_[i].g=i;
	//	palette_[i].b=i;
	//}

   cw = cameraWidget;
}

portVideoQt::portVideoQt()
	: frames_( 0 )
	, width_( WIDTH )
	, height_( HEIGHT )
	, displayMode_( SOURCE_DISPLAY )
{
	time((time_t*)&lastTime_);
	running_ = true;
	fpson_ = true;
	pause_ = false;
	
	framenumber_=0;
	
	appName = "Video";
	sourceDepth_ = 8;
	destDepth_   = 8;
		
	//for(int i=0;i<256;i++){
	//	palette_[i].r=i;
		//palette_[i].g=i;
		//palette_[i].b=i;
	//}
   cw = 0;
}


