#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "boost/date_time/posix_time/posix_time.hpp"
#include "Resources.h"
 
//My goal for this assignment is to incorporate my part 1 assignment into this one
//By doing so, I wish to have animated all of the pixels on the screen and have them change
//colors if they are within certain bounds. 

//As I run my program now, all I get is a black screen.

using namespace ci;
using namespace ci::app;
using namespace std;

class CatPicturePart2App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepareSettings (Settings* settings);

  private:

	 float red, green, blue;
	 ///These signs will change the direction of the color change (+/-)
	 int redSign, greenSign, blueSign;

	  //The surface I will be using on which to draw
	Surface* mySurface_;

	//Struct for rectangle gradient
	struct rect_grad{
		int line;

	};

	//Screen dimensions
	static const int kAppWidth= 800;
	static const int kAppHeight= 600;
	static const int kTextureSize= 1024;

	//Struct for moving circle
	struct circle_moving{
		int x;
	};

	//Made for smoother color changes
	struct circle_signs{
		int redSign;
		int greenSign;
		int blueSign;
	};
	//Going to use this to change my circle colors
	struct circle_colors{
		int r;
		int g;
		int b;
	};

	uint8_t* my_blur_pattern_;

	//Filling a rectangle that will change colors
	void drawRectangles(uint8_t* pixels, int x1, int y1, int x2, int y2);

	void drawBackground(uint8_t* pixels, int x1, int y1, int r);

	void drawLine(uint8_t* pixels, int x1, int y1);

	void drawCircles(uint8_t* pixels, int center_x, int center_y, int radius);

};

void CatPicturePart2App::prepareSettings(Settings* settings)
{
	(*settings).setWindowSize(kAppWidth,kAppHeight);
	(*settings).setResizable(true);
}

//So far, I haven't figured out what changed my background, but
//this really made it look goofy
void CatPicturePart2App::drawBackground(uint8_t* pixels, int x, int y, int r)
{
	//Using these stucts causes the color changes in my background
	circle_signs c;
	c.redSign = 1;
	c.greenSign = 1;
	c.blueSign = 1;

	circle_colors t;
	t.r = 150;
	t.g = 100;
	t.b = 50;

	//Declare new variables
	int centerx = x;
	int centery = y;
	int radius = r;

	//Checking boundaries
	if (centerx < 0) return;
	if (centery < 0) return;
	if (radius <= 0) return;	

	///I liked what you did in class, but I wanted my color transitions to be smoother
	///So I made the colors bounce between the values of 0.0 - 1.0
	if (red > 255 || red < 0) {
		redSign = redSign*-1;
	}
	if (green > 255 || green < 0) {
		greenSign = greenSign*-1;
	}
	if (blue > 255 || blue < 0) {
		blueSign = blueSign*-1;
	}

	///Regardless of whether _Sign is (+/-) we will add it to the current values
	red = red + (redSign);
	green = green + (greenSign);
	blue = blue + (blueSign);

	for (int i = 0; i < 3*kTextureSize*kTextureSize; i+= 3){
		//something is wrong with this as it was originally intended to be my circle method...
		int distance = (((i-x)*(i-x))+((i-y)*(i-y)));
		if (distance <= r){
			pixels[i] = red;
			pixels[i+1] = green;
			pixels[i+2] = blue;
		}
	}
}

void CatPicturePart2App::drawRectangles(uint8_t* pixels, int x1, int y1, int x2, int y2)
{

	//call the struct made above
	rect_grad grad;
	grad.line = 0;

	//--------------------------------------------------
	//Copied from Dr. Brinkman
	//If x1 is less than x2, take x1, else take x2
	int startx = (x1 < x2) ? x1 : x2;
	int endx = (x1 < x2) ? x2 : x1;
	//Same thing for the y values
	int starty = (y1 < y2) ? y1 : y2;
	int endy = (y1 < y2) ? y2 : y1;
	//--------------------------------------------------

	
	//checking the boundaries
	if(endx < 0) return; 
	if(endy < 0) return; 
	if(startx >= kAppWidth) return;
	if(starty >= kAppHeight) return;
	//Making sure the rectangle drawn isn't outside the viewing box
	if(endx >= kAppWidth) endx = kAppWidth-1;
	if(endy >= kAppHeight) endy = kAppHeight-1;
	
	//making the rectangle
	for ( int y=starty; y <= endy; y++){
		grad.line++;
		for ( int x = startx; x <= endx; x++) {
			if (x >= startx & x <= endx & y >= starty & y <= endy){
			//Set the Red, Green, Blue values for each pixel
				pixels [3* (x+y*kTextureSize)]=(250*(grad.line*x/endx));			
				pixels [3* (x+y*kTextureSize)+1]=(50*(grad.line*x/endx));			
				pixels [3* (x+y*kTextureSize)+2]=(75*(grad.line*x/endx));			
			}
		}		
	}
}

void CatPicturePart2App::setup()
{
	mySurface_ = new Surface(kTextureSize,kTextureSize,false);

	//Set the initial values of the colors and color sign values
	red = 250;
	green = 50;
	blue = 75;
	redSign = 1;
	greenSign = 1;
	blueSign = 1;

}

void CatPicturePart2App::drawCircles(uint8_t* pixels, int centerX, int centerY, int radius)
{

	//Taken from Dr. Brinkmans Circle Funtion, except for the modulus portion of it

	for ( int y=centerY-radius; y <= centerY+radius; y++){		
		for ( int x = centerX-radius; x <= centerX+radius; x++) {	
			int dist = (int)sqrt((double)((x-centerX)*(x-centerX) + (y-centerY)*(y-centerY)));
			if (dist <= radius) {
				//I'm using the values I have stored above to be able to change the circle's color
				pixels[3*(x + y*kTextureSize)] = green;
				pixels[(3*(x + y*kTextureSize))+1] = blue;
				pixels[(3*(x + y*kTextureSize))+2] = red;
			}
		}
	}
}

void CatPicturePart2App::mouseDown( MouseEvent event )
{
	//Try and switch the direction of change for the background and the circle
	//I'm getting an error that I can't figure out:
	//---------------------------------------------------------
	//The variable 'c' is being used without being initialized.
	//---------------------------------------------------------

	/*
	circle_signs c;
	c.redSign*= -1;
	c.greenSign = 1;
	c.blueSign = 1;
	*/

}

void CatPicturePart2App::update()
{
	//Gather the info we need in an array
	uint8_t* dataArray = (*mySurface_).getData();

	//Call background
	drawBackground(dataArray, 100, 200, 10);

	//Call rectangle
	drawRectangles(dataArray, 250, 250, 600, 600);

	//Call circle
	drawCircles(dataArray, 400, 100, 100);
}

void CatPicturePart2App::draw()
{
	//Clear the surface
	//gl::clear(Color(red, green, blue));
	//Draw the surface

	//I'm struggling writing this so that my pixels change in the surface, and not when I clear the surface
	gl::draw(*mySurface_); 
}

CINDER_APP_BASIC( CatPicturePart2App, RendererGl )
