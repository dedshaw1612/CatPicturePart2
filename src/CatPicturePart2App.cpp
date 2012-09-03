#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CatPicturePart2App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

  private:
	  //The surface I will be using on which to draw
	Surface* mySurface_;

	//Screen dimensions
	static const int kAppWidth= 800;
	static const int kAppHeight= 600;
	static const int kTextureSize=1024;

	uint8_t* my_blur_pattern_;

	//Filling a rectangle that will change colors
	void drawRectangles(uint8_t* pixels, int x, int y, Color8u c);


};

void CatPicturePart2App::setup()
{
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);

}


void CatPicturePart2App::drawRectangles(uint8_t* pixels, int x, int y, Color8u c)
{
}

void CatPicturePart2App::mouseDown( MouseEvent event )
{
}

void CatPicturePart2App::update()
{
	uint8_t* dataArray = (*mySurface_).getData();
	drawRectangles(dataArray, 800, 600, Color8u(255,0,0));
}

void CatPicturePart2App::draw()
{
	//Draw the surface
	gl::draw(*mySurface_); 
}

CINDER_APP_BASIC( CatPicturePart2App, RendererGl )
