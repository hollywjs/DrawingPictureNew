/**
*@fileDrawingPicturesNewApp.cpp
*CSE 274- Fall 2012
*My Solution for HomeWork 1
*
*@author Jessica Hollywood
*@date 2012-09-05
*
*@note This project satisifies goals A.1(rectangle), A.2(circle),
* A.4(gradient), A.5(copy), C(picture), and E.5(animation)
*/

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include <math.h>


using namespace ci;
using namespace ci::app;
using namespace std;

class DrawingPictureNewApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings(Settings* settings);

  private:
	  Surface* mySurface_;
	  uint8_t* pixels;
	  static const int kAppWidth=900;	
	  static const int kAppHeight=700;	
	  static const int kTextureSize=1024;
	  void DrawingPictureNewApp::drawRectangles(uint8_t* pixels, int x, int y, int width, int height, int red, int green, int blue);
	  void DrawingPictureNewApp::drawCircles(uint8_t* pixels, double x, double y, int radius, int red, int green, int blue);
	  void DrawingPictureNewApp::copyRegion(uint8_t* pixels, int x1, int y1, int x2, int y2, int width, int height);
	  void DrawingPictureNewApp::gradient(uint8_t* pixels);
	  void DrawingPictureNewApp::blurScreen(uint8_t* pixels);
	  int xBall;
	  int yBall;
};

/**
*Draws a rectangle at a speceified x and y value, with specified width
*and height by accessing and changing the pixel array directly.
*
*This satisfies A.1 goal(rectangle).
*/
void DrawingPictureNewApp::drawRectangles(uint8_t* pixels, int x, int y, int width, int height, int red, int green, int blue){
	height = y+height;
	width = x+width;
	int index;
	for(y;y<height;y++){
		for(int i=x;i<width;i++){
			index=(i+y*kTextureSize)*3;
			pixels[index]=red;
			pixels[index+1]=green;
			pixels[index+2]=blue;
		}
	}
};

/**
*Draws a circle at a speceified x and y value, with specified radius
*by accessing and changing the pixel array directly.
*
*This satisfies A.2 goal(circle).
*/
void DrawingPictureNewApp::drawCircles(uint8_t* pixels, double x, double y, int radius, int red, int green, int blue)
{
	for(int y1=0;y1<kAppHeight;y1++){
		for(int x1=0;x1<kAppWidth;x1++){
			double distance=pow((x1-x),2);
			distance = distance + pow((y1-y),2);
			distance = sqrt(distance);
			if(distance <= radius){
				pixels[(x1+y1*kTextureSize)*3]=red;
				pixels[(x1+y1*kTextureSize)*3+1]=green;
				pixels[(x1+y1*kTextureSize)*3+2]=blue;
			}
		}
	}
};

/**
*Creates a gradient from top to bottom of the program by accessing 
*and changing the pixel array directly.
*
*This satisfies A.4 goal(gradient).
*/
void DrawingPictureNewApp::gradient(uint8_t* pixels){
	int index, red=255, green=0, blue=255;
	double yPercent;
	int redB, greenB, blueB;
	for(int y=1;y<kAppHeight;y++){
		for(int x=0;x<kAppWidth;x++){
			index=(x+y*kTextureSize)*3;
			redB=(y/3)%red;
			//greenB=(y/3)%green;
			blueB=(y/3)%blue;
			pixels[index]=255-redB;
			pixels[index+1]=0;
			pixels[index+2]=0+blueB;
		}
	}
};

/**
*Copies any rectangular part of the pixel array into a different part of
*the array given specified starting x and y values copy locations for 
*x and y and specified width and height; accesses and changes the pixel
*array directly.
*
*This satisfies A.1 goal(rectangle).
*/
void DrawingPictureNewApp::copyRegion(uint8_t* pixels, int x1, int y1, int x2, int y2, int width, int height){
	height = y1+height;
	width = x1+width; 
	int index;
	int storData=0;
	for(y1;y1<height;y1++&&y2++){
		for(int i=x1;i<width;i++){
			index=(i+y1*kTextureSize)*3;
			storData=((i+x2)+y2*kTextureSize)*3;
			pixels[storData]=pixels[index];
			pixels[storData+1]=pixels[index+1];
			pixels[storData+2]=pixels[index+2];
		}
	}
};

/**
*Creates a blur in the picture by accessing and changing the pixel
*array directly.
*
*This satisfies B.1 goal(blur), I do not believe tis is working
*
*This code was copied and adjusted from Sonodabe's github account
*/
void DrawingPictureNewApp::blurScreen(uint8_t* pixels){
    int sumR, sumG, sumB;
	int index;
    for(int y = 1; y < kAppHeight-1; y++){
       for(int x = 1; x < kAppWidth-1; x++){
           sumR = 0;
           sumG = 0;
           sumB = 0;
			 for(int avgY = -1; avgY<=1; avgY++){
				  for(int avgX = -1; avgX<=1; avgX++){
				    index = 3*(x+y*kAppWidth)*(x+avgX, y+avgY);
				    sumR = sumR+pixels[index];
				    sumG = sumG+pixels[index+1];
					sumB = sumB+pixels[index+2];
				}
			}
			for(y;y<kAppHeight;y++){
				for(x;x<kAppWidth;x++){
					index=(x+y*kTextureSize)*3;
					pixels[index]=sumR/9.0;
					pixels[index+1]=sumG/9.0;
					pixels[index+2]=sumB/9.0;
			}
		}
      }
   }
};

/*
*Thhis sets the window size and makes resizable false
*/
void DrawingPictureNewApp::prepareSettings(Settings* settings){
	(*settings).setWindowSize(kAppWidth,kAppHeight);
	(*settings).setResizable(false);
}

/*
*This setups the origional surface before any animation
*/
void DrawingPictureNewApp::setup()
{
	xBall=200;
	yBall=100;
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);
	pixels = (*mySurface_).getData();
	gradient(pixels);
	drawRectangles(pixels,100,400,650,50,0,0,0);
	drawRectangles(pixels,150,450,50,250,0,0,0);
	copyRegion(pixels,150,450,500,450,50,250);
	drawCircles(pixels,xBall,yBall,100,155,0,255);
}

void DrawingPictureNewApp::mouseDown( MouseEvent event )
{
}

/*
*This animates my picture, the ball bounces on the table
*until it gets to the edge and then falls off.
*
*This satisfies E.5 goal(animation)
*/
void DrawingPictureNewApp::update()
{
	xBall=xBall+10;
	yBall=yBall+10;
	if(xBall>kAppWidth){
		xBall=0;
		blurScreen(pixels);
	}
	if(yBall>305&&xBall<775)
		yBall=200;
	gradient(pixels);
	drawRectangles(pixels,100,400,650,50,0,0,0);
	drawRectangles(pixels,150,450,50,250,0,0,0);
	copyRegion(pixels,150,450,500,450,50,250);
	drawCircles(pixels,xBall,yBall,100,155,0,255);
}

/*
*This draws the surface
*/
void DrawingPictureNewApp::draw()
{
	gl::draw(*mySurface_);
}


CINDER_APP_BASIC( DrawingPictureNewApp, RendererGl )
