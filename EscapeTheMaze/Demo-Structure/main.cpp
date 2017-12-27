// Resources Used
// http://alleg.sourceforge.net/docs.html
// http://wiki.allegro.cc/index.php?title=Allegro_5_Tutorial
// http://alleg.sourceforge.net/a5docs/5.0.2/index.html#api
// http://msdn.microsoft.com/en-us/library/ty9hx077.aspx
// http://www.google.com
// http://www.1001freefonts.com/Pirulen.php
// http://wiki.allegro.cc/index.php?title=Collision_detection

/* These are include files. Think of them like the Yellow Pages
for your code. Each *.h file holds a list of functions that
do things. In this code file, we can use those functions to
accomplish things. For example, string.h contains the function
strcat(), which stands for "string concatenation," and allows
you to put two strings together.
*/
#include <iostream>
#include <stdio.h>
#include <string>
#include <math.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
//#include <allegro5/allegro_audio.h>
//#include <allegro5/allegro_acodec.h>
#include <list>
#include <Windows.h>

using namespace std;

/* The const keyword stands for 'constant,' which means
the value of the const never changes. */

/* Determines display refresh rate */
const float FPS = 60;

/* Sets the window size. If you want to make the game window larger,
* this is where you do it. */
const int SCREEN_W = 640;
const int SCREEN_H = 480;

/* Sets the size of the moving square. */
const int BOUNCER_SIZE = 20;

/* Creates an enumerated list of constants for
keyboard interaction. */
enum KEYBOARD_KEYS 
{
	KEY_UP, 
	KEY_DOWN, 
	KEY_LEFT, 
	KEY_RIGHT,
	KEY_W,
	KEY_S,
	KEY_A,
	KEY_D
};

/* This struct (structure) packs all the things we need
* to draw a sprite on the screen. An example of a sprite
* is the player character, or perhaps a moving object such
* as an asteroid. */
struct sprite
{
	/* The X and Y position (measured from the top left corner
	* of the screen) of the sprite. */
	int xPosition;
	int yPosition;

	/* The current X and Y velocity of the sprite. The units
	* are in pixels/frame-rate. For example, if the framerate
	* is set to 60, then an xVelocity of 1 would be 
	* 1 pixel/(1/60 second). */
	float xVelocity;
	float yVelocity;

	/* This list holds all of the different images we use
	* for the sprite. Normally, we would only use one, but
	* sometimes we want to create animations, in which case we
	* would use two or more. */
	std::list<ALLEGRO_BITMAP*> spriteBitmapList;

	/* We also have to keep track of which bitmap is currently
	* in use, so we can cycle between them effectively. This
	* variable lets us do that. */
	int currentlyActiveSpriteBitmap;
};

std::string InitializeAllegro()
{
	/* **********Initializing library, objects and addons ************* */
	/* These init calls start and set up the Allegro subsystems so we can
	use them. They generally don't need to be changed, though you might 
	have to add subsystems if you get really fanc-ay. */

	if(al_init() == false) 
	{
		return "failed to initialize allegro!\n";
	}

	al_init_font_addon();

	if(al_init_ttf_addon() == false)
	{	
		return "failed to initialize ttf addon!\n";
	}

	if(al_install_mouse() == false) 
	{
		return "failed to initialize the mouse!\n";
	}

	if(!al_init_image_addon())
	{
		return"failed to init image addon!\n";
	}

	if(!al_init_primitives_addon())
	{
		return"failed to init primitives addon!\n";
	}

	if(!al_install_keyboard()) 
	{
		return "failed to initialize the keyboard!\n";
	}

	/* If we got through all these functions, we were successful,
	* and we should tell the world. */
	return "Allegro game library sucessfully initialized.\n";
}

/* Main Loop. This is where your code starts executing when you run it.
Ignore the argc and argv parameters -- they help you manage initial input
from the user in a command-line application, but serve no use here. */
int main(int argc, char **argv)
{
	/* Object Declarations. These objects are necessary to make the
	game run properly. Each one does something useful.*/
	ALLEGRO_DISPLAY *display = NULL;				/* This is the display object. Think of it as the window you see when the game starts. */
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;		/* The event queue contains a record of all events (keyboard presses, timers going off, etc.) */
	ALLEGRO_TIMER *timer = NULL;					/* This is a timer we can use to keep track of time, or make things happen later. */

	/* This event object tracks the types of events that happen
	while the event queue is running in the while loop. */
	ALLEGRO_EVENT ev;

	/* Game sprites: these are the things you see on screen. */
	ALLEGRO_BITMAP *bouncer = NULL;
	ALLEGRO_BITMAP *bouncer2 = NULL;
	ALLEGRO_BITMAP *spotlightA1 = NULL;
	ALLEGRO_BITMAP *spotlightB1 = NULL;
	ALLEGRO_BITMAP *spotlightC1 = NULL;
	ALLEGRO_BITMAP *spotlightD1 = NULL;
	ALLEGRO_BITMAP *spotlightE1 = NULL;
	ALLEGRO_BITMAP *spotlightA2 = NULL;
	ALLEGRO_BITMAP *spotlightB2 = NULL;
	ALLEGRO_BITMAP *spotlightC2 = NULL;
	ALLEGRO_BITMAP *spotlightD2 = NULL;
	ALLEGRO_BITMAP *spotlightE2 = NULL;
	ALLEGRO_BITMAP *spotlightF2 = NULL;
	ALLEGRO_BITMAP *background1 = NULL;
	ALLEGRO_BITMAP *background2 = NULL;
	ALLEGRO_BITMAP *background3 = NULL;
	ALLEGRO_BITMAP *door1 = NULL;
	ALLEGRO_BITMAP *door2 = NULL;


	/* ************** Variable Declarations ******************* */

	//Starting Level
	int lvl = 3;

	//declares the variable of the colour that the bouncer is on
	ALLEGRO_COLOR pathColorL;
	ALLEGRO_COLOR pathColorR;
	ALLEGRO_COLOR pathColorT;
	ALLEGRO_COLOR pathColorB;
	ALLEGRO_COLOR pathColorL2;
	ALLEGRO_COLOR pathColorR2;
	ALLEGRO_COLOR pathColorT2;
	ALLEGRO_COLOR pathColorB2;

	//death counter
	int numDeaths = 0;

	//timer counter
	int counter = 0;
	bool timerOnOff = true;

	// Stores the location of the square on the screen.
	float bouncer_x = 35;
	float bouncer_y = 3;
	float bouncer2_x = 35;
	float bouncer2_y = 28;

	float spotA1_y = 391;
	float spotA1_x = 300;
	bool spotA1Dir = true;
	float spotB1_y = 391;
	float spotB1_x = 180;
	bool spotB1Dir = true;
	float spotC1_y = 90;
	float spotC1_x = 180;
	bool spotC1Dir = true;
	float spotD1_y = 195;
	float spotD1_x = 268;
	bool spotD1Dir = true;
	float spotE1_y = 290;
	float spotE1_x = 470;
	bool spotE1Dir = true;
	float spotA2_y = 278;
	float spotA2_x = 160;
	bool spotA2Dir = true;
	float spotB2_y = 210;
	float spotB2_x = 110;
	bool spotB2Dir = true;
	float spotC2_y = 339;
	float spotC2_x = 180;
	bool spotC2Dir = true;
	float spotD2_y = 400;
	float spotD2_x = 70;
	bool spotD2Dir = true;
	float spotE2_y = 436;
	float spotE2_x = 340;
	bool spotE2Dir = true;
	float spotF2_y = 140;
	float spotF2_x = 400;
	bool spotF2Dir = true;

	bool doorA = true;
	bool doorB = true;

	/* These variables can be used to scroll a background image.
	Select one and hit ctrl-F to find out where they get used! */
	int background1_coord_x = 0;
	int background1_coord_y = 0;
	int background2_coord_x = 0;
	int background2_coord_y = 0;
	int background3_coord_x = 0;
	int background3_coord_y = 0;

	int background1_height = 0;
	int background1_width = 0;
	int background2_height = 0;
	int background2_width = 0;
	int background3_height = 0;
	int background3_width = 0;

	/* Helps identify the keys that are pressed. */
	bool key[8] = { false, false, false, false, false, false, false, false };
	/* Signals when the display should be redrawn. */
	bool redraw = true;
	/* Signals when the program should exit. */
	bool doexit = false;

	/* Strings for putting text on the screen. */
	/* TODO FIX Change these to strings. */
	char* str1 = new char[30];
	char* str2 = new char[30];
	char* str3 = new char[30];

	//Avoid the spotlights label;
	char* str4 = new char[30];

	// Strings used to make the end label
	char* end_str1 = new char[30];
	char* end_str2 = new char[30];
	char* end_str3 = new char[30];

	/* We have to initialize a bunch of Allegro sub-systems. To
	* see what this function does, right click on it and click
	* "Go to Definition" (or select it and press F12.) */
	string initStatus = InitializeAllegro();
	/* Let's print the status out to the console. */
	cout << initStatus;
	/* If the returned status isn't success, then we end the program. */
	if( initStatus != "Allegro game library sucessfully initialized.\n" )
	{
		return -1;
	}

	/* Load the game font. Font from Larabie Free Fonts. */
	ALLEGRO_FONT *font = al_load_ttf_font("arialbd.ttf",30,0 ); 


	/* **************** Create game objects. **************** */
	/* Here we make all of the objects we're going to use in the game.
	These can include timers, displays, and bitmaps that show graphics
	on screen. */

	/* Set up a timer. This timer ticks at a frequency of 1/FPS, which
	in this case is 1/60s. */
	timer = al_create_timer(1.0 / FPS);
	/* Oh no! The timer creation failed. If this happens, kill the whole
	program (al_create_timer returns NULL if it is unsuccessful.) Note
	that a lot of other functions do this, so this type of check is
	very useful. */
	if(timer == NULL) 
	{
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}

	/* Create the game display. This object is the game window in which
	you play the game. */
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(display == NULL) 
	{
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	/* Creates the event queue and loads it. */
	event_queue = al_create_event_queue();
	if(event_queue == NULL) 
	{
		fprintf(stderr, "failed to create event_queue!\n");
		return -1;
	}

	/* Register the types of events that are going to get added to the
	event queue as they happen (we're going to ignore some types of events
	because they're not relevant. */
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());



	/* ******************* Create Visible Game Objects. ************** */
	/* The parts above are all necessary, and you won't change them much
	for a given game. The parts below are objects meant to be seen and
	interacted with, and will change often.
	*/

	/* Create a bitmap that will hold the image of the bouncing ball. 
	Each object that you show on screen will have to be put on a bitmap
	so that you can draw it to screen. */

	background1 = al_load_bitmap("1.png");
	background2 = al_load_bitmap("2.png");
	background3 = al_load_bitmap("3.png");

	bouncer = al_create_bitmap(	BOUNCER_SIZE, BOUNCER_SIZE );
	bouncer = al_load_bitmap("bouncerFigure.png");
	bouncer2 = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE );
	bouncer2 = al_load_bitmap("bouncerFigure2.png");

	spotlightA1 = al_load_bitmap("spotlight.png");
	spotlightB1 = al_load_bitmap("spotlight.png");
	spotlightC1 = al_load_bitmap("spotlight.png");
	spotlightD1 = al_load_bitmap("spotlight.png");
	spotlightE1 = al_load_bitmap("spotlight.png");
	spotlightA2 = al_load_bitmap("spotlight.png");
	spotlightB2 = al_load_bitmap("spotlight.png");
	spotlightC2 = al_load_bitmap("spotlight.png");
	spotlightD2 = al_load_bitmap("spotlight.png");
	spotlightE2 = al_load_bitmap("spotlight.png");
	spotlightF2 = al_load_bitmap("spotlight.png");

	door1 = al_load_bitmap("door1.png");
	door2 = al_load_bitmap("door2.png");


	if(bouncer == NULL) 
	{
		fprintf(stderr, "failed to create bouncer bitmap!\n");
		return -1;
	}
	if(bouncer2 == NULL) 
	{
		fprintf(stderr, "failed to create bouncer bitmap!\n");
		return -1;
	}

	/* Get the size of the parent bitmap that we'll use to make the background out of. */
	background1_height = al_get_bitmap_height(background1);
	background1_width  = al_get_bitmap_width(background1);
	background2_height = al_get_bitmap_height(background2);
	background2_width  = al_get_bitmap_width(background2);
	background3_height = al_get_bitmap_height(background3);
	background3_width = al_get_bitmap_height(background3);

	/* Set the background so that it simply takes a window-sized chunk of the background
	image from the top left corner of the screen (in computer game convention, the top
	left pixel is the [0,0] pixel. The positive x and y axes of the display matrix point
	to the right and down, respectively.

	Increasing these numbers would shift the part of the background image used as the
	background. Increasing y would push the part down; increasing x pushes the part to
	the right. 
	*/
	background1_coord_x = 0;
	background1_coord_y = 0;
	background2_coord_x = 0;
	background2_coord_y = 0;
	background3_coord_x = 0;
	background3_coord_y = 0;

	/* This command allows us to use a portion of the parent bitmap without being
	inefficient (no new copies of the bitmap are created.) The 'background' object
	is set to an image that is cut out of the original 'background' object. */

	background1 = al_create_sub_bitmap( background1,			/* The source bitmap. */
		background1_coord_x,			/* The x coordinate to start copying at. */
		background1_coord_y,			/* The y coordinate to start copying at. */
		background1_coord_x+SCREEN_W,	/* The x coordinate to stop copying at. */
		background1_coord_y+SCREEN_H	/* The y coordinate to stop copying at. */
		);

	background2 = al_create_sub_bitmap( background2,			/* The source bitmap. */
		background2_coord_x,			/* The x coordinate to start copying at. */
		background2_coord_y,			/* The y coordinate to start copying at. */
		background2_coord_x+SCREEN_W,	/* The x coordinate to stop copying at. */
		background2_coord_y+SCREEN_H	/* The y coordinate to stop copying at. */
		);

	background3 = al_create_sub_bitmap( background3,			/* The source bitmap. */
		background3_coord_x,			/* The x coordinate to start copying at. */
		background3_coord_y,			/* The y coordinate to start copying at. */
		background3_coord_x+SCREEN_W,	/* The x coordinate to stop copying at. */
		background3_coord_y+SCREEN_H	/* The y coordinate to stop copying at. */
		);


	/* Causes bitmap functions to adjust bouncer instead of display (Default was display). */
	al_set_target_bitmap(bouncer);
	al_set_target_bitmap(bouncer2);

	/* Causes bitmap functions to adjust display once more. */
	al_set_target_bitmap(al_get_backbuffer(display));

	/* Sets up the background (in this case, it makes it pure black.) Note that
	this is different from drawing a background image, which is why the game
	background is a starfield and not black. */
	al_clear_to_color(al_map_rgb(0,0,0));

	/* The display is double-buffered, which means that there are two memories to store 
	the content of the screen. This memory is a 2D matrix that keeps track of each
	pixel's color value.

	When the first is being displayed, we can draw on the other. Once we're done drawing,
	we can "flip" the two, putting the new one on the display. We can then overwrite
	the contents of the old one with new stuff and flip again. 

	al_flip_display() flips the two screen buffers, showing the one we just finished
	drawing to. */
	al_flip_display();
	/* Start the timer. */
	al_start_timer(timer);

	/* This is the game while loop. It loops infinitely until the doexit variable is flipped
	to true, at which time the loop ends and the game is destroyed. Each iteration of this
	loop corresponds to one displayed frame on the screen.*/

	/*music = al_load_sample("music.mp3");
	al_play_sample(music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);*/

	while(!doexit)
	{	

		/* This function waits for something to happen. This could include an event on
		any of the following sources: 
		display
		timer
		keyboard
		mouse
		This means the function does not return until one of these things happens.
		Think of it like a pause button. Once an event is detected, the code 
		continues execution from here.
		*/


		al_wait_for_event(event_queue, &ev);

		/*	This if statement executes if the timer event was fired. Since the 
		timer goes off 60 times per second, this route is taken that often.

		For every timer event:
		The box will move if the user presses a direction arrow 
		(up, down, left, right.)
		the circle will move around the screen at constant velocity and 
		bounce off the walls.
		*/
		/* We can make bitmaps out of images by loading them. In this case, we're
		creating the background bitmap from the image PIA01320_small.png. */

		if(ev.type == ALLEGRO_EVENT_TIMER) 
		{


			/* Here we update the square. */
			/* Make sure the UP key is pressed, and the box is still on the
			screen (we don't want to move the box off the screen.) */
			if (lvl == 1) {
				pathColorR =  al_get_pixel(background1, bouncer_x + 20, bouncer_y + 10);
				pathColorB =  al_get_pixel(background1, bouncer_x + 10, bouncer_y + 20);
				pathColorT =  al_get_pixel(background1, bouncer_x + 10, bouncer_y);
				pathColorL =  al_get_pixel(background1, bouncer_x, bouncer_y + 10);
				pathColorR2 =  al_get_pixel(background1, bouncer2_x + 20, bouncer2_y + 10);
				pathColorB2 =  al_get_pixel(background1, bouncer2_x + 10, bouncer2_y + 20);
				pathColorT2 =  al_get_pixel(background1, bouncer2_x + 10, bouncer2_y);
				pathColorL2 =  al_get_pixel(background1, bouncer2_x, bouncer2_y + 10);
			}

			else if (lvl == 2) {
				pathColorR =  al_get_pixel(background2, bouncer_x + 20, bouncer_y + 10);
				pathColorB =  al_get_pixel(background2, bouncer_x + 10, bouncer_y + 20);
				pathColorT =  al_get_pixel(background2, bouncer_x + 10, bouncer_y);
				pathColorL =  al_get_pixel(background2, bouncer_x, bouncer_y + 10);
				pathColorR2 =  al_get_pixel(background2, bouncer2_x + 20, bouncer2_y + 10);
				pathColorB2 =  al_get_pixel(background2, bouncer2_x + 10, bouncer2_y + 20);
				pathColorT2 =  al_get_pixel(background2, bouncer2_x + 10, bouncer2_y);
				pathColorL2 =  al_get_pixel(background2, bouncer2_x, bouncer2_y + 10);
			}

			else if (lvl == 3) {
				pathColorR =  al_get_pixel(background3, bouncer_x + 20, bouncer_y + 10);
				pathColorB =  al_get_pixel(background3, bouncer_x + 10, bouncer_y + 20);
				pathColorT =  al_get_pixel(background3, bouncer_x + 10, bouncer_y);
				pathColorL =  al_get_pixel(background3, bouncer_x, bouncer_y + 10);
				pathColorR2 =  al_get_pixel(background3, bouncer2_x + 20, bouncer2_y + 10);
				pathColorB2 =  al_get_pixel(background3, bouncer2_x + 10, bouncer2_y + 20);
				pathColorT2 =  al_get_pixel(background3, bouncer2_x + 10, bouncer2_y);
				pathColorL2 =  al_get_pixel(background3, bouncer2_x, bouncer2_y + 10);
			}

			if(key[KEY_UP] && bouncer_y >= 1.5) 
			{
				/* Move the box upwards. */
				bouncer_y -= 1.5;
				if (pathColorT.r == 0) {
					bouncer_y += 1.5;
				}
			}
			
			if(key[KEY_W] && bouncer2_y >= 1.5) 
			{
				/* Move the box upwards. */
				bouncer2_y -= 1.5;
				if (pathColorT2.r == 0) {
					bouncer2_y += 1.5;
				}
			}

			if(key[KEY_DOWN] && bouncer_y <= SCREEN_H - BOUNCER_SIZE - 1.5) {
				bouncer_y += 1.5;
				if (pathColorB.r == 0) {
					bouncer_y -= 1.5;
				}
			}

			if(key[KEY_S] && bouncer2_y <= SCREEN_H - BOUNCER_SIZE - 1.5) {
				bouncer2_y += 1.5;
				if (pathColorB2.r == 0) {
					bouncer2_y -= 1.5;
				}
			}

			if(key[KEY_LEFT] && bouncer_x >= 1.5) {
				bouncer_x -= 1.5;
				if (pathColorL.r == 0) {
					bouncer_x += 1.5;
				}
			}

			if(key[KEY_A] && bouncer2_x >= 1.5) {
				bouncer2_x -= 1.5;
				if (pathColorL2.r == 0) {
					bouncer2_x += 1.5;
				}
			}

			if(key[KEY_RIGHT] && bouncer_x <= SCREEN_W - BOUNCER_SIZE - 1.5) {
				bouncer_x += 1.5;
				if (pathColorR.r == 0) {
					bouncer_x -= 1.5;
				}
			}

			if(key[KEY_D] && bouncer2_x <= SCREEN_W - BOUNCER_SIZE - 1.5) {
				bouncer2_x += 1.5;
				if (pathColorR2.r == 0) {
					bouncer2_x -= 1.5;
				}
			}

			//Spotlight Movement
			if (spotA1Dir == true && spotA1_x >= 240)
			{spotA1_x -= 4.0;}

			else if (spotA1Dir == false && spotA1_x <= 400)
			{spotA1_x += 4.0;}

			else if (spotA1_x < 240)
			{spotA1Dir = false;}

			else
			{spotA1Dir= true;}

			//////////////////////////////////////////////////////

			if (spotB1Dir == true && spotB1_y >= 90)
			{spotB1_y -= 2.5;}

			else if (spotB1Dir == false && spotB1_y <= 391)
			{spotB1_y += 2.5;}

			else if (spotB1_y < 90)
			{spotB1Dir = false;}

			else
			{spotB1Dir= true;}

			///////////////////////////////////

			if (spotC1Dir == true && spotC1_x >= 180)
			{spotC1_x -= 2.5;}

			else if (spotC1Dir == false && spotC1_x <= 470)
			{spotC1_x += 2.5;}

			else if (spotC1_x < 180)
			{spotC1Dir = false;}

			else
			{spotC1Dir= true;}

			///////////////////////////////////

			if (spotD1Dir == true && spotD1_x >= 268)
			{spotD1_x -= 7;}

			else if (spotD1Dir == false && spotD1_x <= 470)
			{spotD1_x += 7;}

			else if (spotD1_x < 268)
			{spotD1Dir = false;}

			else
			{spotD1Dir= true;}

			///////////////////////////////////

			if (spotE1Dir == true && spotE1_x >= 268)
			{spotE1_x -= 6.5;}

			else if (spotE1Dir == false && spotE1_x <= 470)
			{spotE1_x += 6.5;}

			else if (spotE1_x < 268)
			{spotE1Dir = false;}

			else
			{spotE1Dir= true;}

			///////////////////////////////////

			if (spotA2Dir == true && spotA2_x >= 160)
			{spotA2_x -= 3.0;}

			else if (spotA2Dir == false && spotA2_x <= 350)
			{spotA2_x += 3.0;}

			else if (spotA2_x < 160)
			{spotA2Dir = false;}

			else
			{spotA2Dir= true;}

			///////////////////////////////////

			if (spotB2Dir == true && spotB2_y >= 210)
			{spotB2_y -= 5.0;}

			else if (spotB2Dir == false && spotB2_y <= 339)
			{spotB2_y += 5.0;}

			else if (spotB2_y < 210)
			{spotB2Dir = false;}

			else
			{spotB2Dir= true;}

			///////////////////////////////////

			if (spotC2Dir == true && spotC2_y >= 210)
			{spotC2_y -= 5.0;}

			else if (spotC2Dir == false && spotC2_y <= 339)
			{spotC2_y += 5.0;}

			else if (spotC2_y < 210)
			{spotC2Dir = false;}

			else
			{spotC2Dir= true;}

			///////////////////////////////////

			if (spotD2Dir == true && spotD2_x >= 70)
			{spotD2_x -= 3.5;}

			else if (spotD2Dir == false && spotD2_x <= 340)
			{spotD2_x += 3.5;}

			else if (spotD2_x < 70)
			{spotD2Dir = false;}

			else
			{spotD2Dir= true;}

			///////////////////////////////////

			if (spotE2Dir == true && spotE2_x >= 70)
			{spotE2_x -= 3.5;}

			else if (spotE2Dir == false && spotE2_x <= 340)
			{spotE2_x += 3.5;}

			else if (spotE2_x < 70)
			{spotE2Dir = false;}

			else
			{spotE2Dir= true;}

			///////////////////////////////////

			if (spotF2Dir == true && spotF2_x >= 400)
			{spotF2_x -= 4.5;}

			else if (spotF2Dir == false && spotF2_x <= 580)
			{spotF2_x += 4.5;}

			else if (spotF2_x < 410)
			{spotF2Dir = false;}

			else
			{spotF2Dir= true;}

			//Hit Detection for Spotlight
			if (lvl == 1){

			if (spotA1_y < bouncer_y && bouncer_y < spotA1_y + 20 && spotA1_x + 20 > bouncer_x && bouncer_x > spotA1_x || spotA1_y < bouncer_y + 20 && bouncer_y + 20 < spotA1_y + 20 && spotA1_x + 20 > bouncer_x && bouncer_x > spotA1_x || spotA1_y < bouncer_y + 20 && bouncer_y < spotA1_y + 20 && spotA1_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotA1_x || spotA1_y < bouncer_y && bouncer_y < spotA1_y + 20 && spotA1_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotA1_x || spotA1_y < bouncer2_y && bouncer2_y < spotA1_y + 20 && spotA1_x + 20 > bouncer2_x && bouncer2_x > spotA1_x || spotA1_y < bouncer2_y + 20 && bouncer2_y + 20 < spotA1_y + 20 && spotA1_x + 20 > bouncer2_x && bouncer2_x > spotA1_x || spotA1_y < bouncer2_y + 20 && bouncer2_y < spotA1_y + 20 && spotA1_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotA1_x || spotA1_y < bouncer2_y && bouncer2_y < spotA1_y + 20 && spotA1_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotA1_x)
			{
				bouncer_x = 35;
				bouncer_y = 3;
				bouncer2_x = 35;
				bouncer2_y = 28;
				numDeaths += 1;
				doorA = true;
			}

			if (spotB1_y < bouncer_y && bouncer_y < spotB1_y + 20 && spotB1_x + 20 > bouncer_x && bouncer_x > spotB1_x || spotB1_y < bouncer_y + 20 && bouncer_y + 20 < spotB1_y + 20 && spotB1_x + 20 > bouncer_x && bouncer_x > spotB1_x || spotB1_y < bouncer_y + 20 && bouncer_y < spotB1_y + 20 && spotB1_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotB1_x || spotB1_y < bouncer_y && bouncer_y < spotB1_y + 20 && spotB1_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotB1_x || spotB1_y < bouncer2_y && bouncer2_y < spotB1_y + 20 && spotB1_x + 20 > bouncer2_x && bouncer2_x > spotB1_x || spotB1_y < bouncer2_y + 20 && bouncer2_y + 20 < spotB1_y + 20 && spotB1_x + 20 > bouncer2_x && bouncer2_x > spotB1_x || spotB1_y < bouncer2_y + 20 && bouncer2_y < spotB1_y + 20 && spotB1_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotB1_x || spotB1_y < bouncer2_y && bouncer2_y < spotB1_y + 20 && spotB1_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotB1_x)
			{
				bouncer_x = 35;
				bouncer_y = 3;
				bouncer2_x = 35;
				bouncer2_y = 28;
				numDeaths += 1;
				doorA = true;
			}

			if (spotC1_y < bouncer_y && bouncer_y < spotC1_y + 20 && spotC1_x + 20 > bouncer_x && bouncer_x > spotC1_x || spotC1_y < bouncer_y + 20 && bouncer_y + 20 < spotC1_y + 20 && spotC1_x + 20 > bouncer_x && bouncer_x > spotC1_x || spotC1_y < bouncer_y + 20 && bouncer_y < spotC1_y + 20 && spotC1_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotC1_x || spotC1_y < bouncer_y && bouncer_y < spotC1_y + 20 && spotC1_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotC1_x || spotC1_y < bouncer2_y && bouncer2_y < spotC1_y + 20 && spotC1_x + 20 > bouncer2_x && bouncer2_x > spotC1_x || spotC1_y < bouncer2_y + 20 && bouncer2_y + 20 < spotC1_y + 20 && spotC1_x + 20 > bouncer2_x && bouncer2_x > spotC1_x || spotC1_y < bouncer2_y + 20 && bouncer2_y < spotC1_y + 20 && spotC1_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotC1_x || spotC1_y < bouncer2_y && bouncer2_y < spotC1_y + 20 && spotC1_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotC1_x) 
			{
				bouncer_x = 35;
				bouncer_y = 3;
				bouncer2_x = 35;
				bouncer2_y = 28;
				numDeaths += 1;
				doorA = true;
			}

			if (spotD1_y < bouncer_y && bouncer_y < spotD1_y + 20 && spotD1_x + 20 > bouncer_x && bouncer_x > spotD1_x || spotD1_y < bouncer_y + 20 && bouncer_y + 20 < spotD1_y + 20 && spotD1_x + 20 > bouncer_x && bouncer_x > spotD1_x || spotD1_y < bouncer_y + 20 && bouncer_y < spotD1_y + 20 && spotD1_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotD1_x || spotD1_y < bouncer_y && bouncer_y < spotD1_y + 20 && spotD1_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotD1_x || spotD1_y < bouncer2_y && bouncer2_y < spotD1_y + 20 && spotD1_x + 20 > bouncer2_x && bouncer2_x > spotD1_x || spotD1_y < bouncer2_y + 20 && bouncer2_y + 20 < spotD1_y + 20 && spotD1_x + 20 > bouncer2_x && bouncer2_x > spotD1_x || spotD1_y < bouncer2_y + 20 && bouncer2_y < spotD1_y + 20 && spotD1_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotD1_x || spotD1_y < bouncer2_y && bouncer2_y < spotD1_y + 20 && spotD1_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotD1_x) 
			{
				bouncer_x = 35;
				bouncer_y = 3;
				bouncer2_x = 35;
				bouncer2_y = 28;
				numDeaths += 1;
				doorA = true;
			}

			if (spotE1_y < bouncer_y && bouncer_y < spotE1_y + 20 && spotE1_x + 20 > bouncer_x && bouncer_x > spotE1_x || spotE1_y < bouncer_y + 20 && bouncer_y + 20 < spotE1_y + 20 && spotE1_x + 20 > bouncer_x && bouncer_x > spotE1_x || spotE1_y < bouncer_y + 20 && bouncer_y < spotE1_y + 20 && spotE1_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotE1_x || spotE1_y < bouncer_y && bouncer_y < spotE1_y + 20 && spotE1_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotE1_x || spotE1_y < bouncer2_y && bouncer2_y < spotE1_y + 20 && spotE1_x + 20 > bouncer2_x && bouncer2_x > spotE1_x || spotE1_y < bouncer2_y + 20 && bouncer2_y + 20 < spotE1_y + 20 && spotE1_x + 20 > bouncer2_x && bouncer2_x > spotE1_x || spotE1_y < bouncer2_y + 20 && bouncer2_y < spotE1_y + 20 && spotE1_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotE1_x || spotE1_y < bouncer2_y && bouncer2_y < spotE1_y + 20 && spotE1_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotE1_x) 
			{
				bouncer_x = 35;
				bouncer_y = 3;
				bouncer2_x = 35;
				bouncer2_y = 28;
				numDeaths += 1;
				doorA = true;
			}
			}

			if (lvl == 2){

			if (spotA2_y < bouncer_y && bouncer_y < spotA2_y + 20 && spotA2_x + 20 > bouncer_x && bouncer_x > spotA2_x || spotA2_y < bouncer_y + 20 && bouncer_y + 20 < spotA2_y + 20 && spotA2_x + 20 > bouncer_x && bouncer_x > spotA2_x || spotA2_y < bouncer_y + 20 && bouncer_y < spotA2_y + 20 && spotA2_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotA2_x || spotA2_y < bouncer_y && bouncer_y < spotA2_y + 20 && spotA2_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotA2_x || spotA2_y < bouncer2_y && bouncer2_y < spotA2_y + 20 && spotA2_x + 20 > bouncer2_x && bouncer2_x > spotA2_x || spotA2_y < bouncer2_y + 20 && bouncer2_y + 20 < spotA2_y + 20 && spotA2_x + 20 > bouncer2_x && bouncer2_x > spotA2_x || spotA2_y < bouncer2_y + 20 && bouncer2_y < spotA2_y + 20 && spotA2_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotA2_x || spotA2_y < bouncer2_y && bouncer2_y < spotA2_y + 20 && spotA2_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotA2_x) 
			{
				bouncer_x = 35;
				bouncer_y = 3;
				bouncer2_x = 35;
				bouncer2_y = 28;
				numDeaths += 1;
				doorB = true;
			}

			if (spotB2_y < bouncer_y && bouncer_y < spotB2_y + 20 && spotB2_x + 20 > bouncer_x && bouncer_x > spotB2_x || spotB2_y < bouncer_y + 20 && bouncer_y + 20 < spotB2_y + 20 && spotB2_x + 20 > bouncer_x && bouncer_x > spotB2_x || spotB2_y < bouncer_y + 20 && bouncer_y < spotB2_y + 20 && spotB2_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotB2_x || spotB2_y < bouncer_y && bouncer_y < spotB2_y + 20 && spotB2_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotB2_x || spotB2_y < bouncer2_y && bouncer2_y < spotB2_y + 20 && spotB2_x + 20 > bouncer2_x && bouncer2_x > spotB2_x || spotB2_y < bouncer2_y + 20 && bouncer2_y + 20 < spotB2_y + 20 && spotB2_x + 20 > bouncer2_x && bouncer2_x > spotB2_x || spotB2_y < bouncer2_y + 20 && bouncer2_y < spotB2_y + 20 && spotB2_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotB2_x || spotB2_y < bouncer2_y && bouncer2_y < spotB2_y + 20 && spotB2_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotB2_x) 
			{
				bouncer_x = 35;
				bouncer_y = 3;
				bouncer2_x = 35;
				bouncer2_y = 28;
				numDeaths += 1;
				doorB = true;
			}

			if (spotC2_y < bouncer_y && bouncer_y < spotC2_y + 20 && spotC2_x + 20 > bouncer_x && bouncer_x > spotC2_x || spotC2_y < bouncer_y + 20 && bouncer_y + 20 < spotC2_y + 20 && spotC2_x + 20 > bouncer_x && bouncer_x > spotC2_x || spotC2_y < bouncer_y + 20 && bouncer_y < spotC2_y + 20 && spotC2_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotC2_x || spotC2_y < bouncer_y && bouncer_y < spotC2_y + 20 && spotC2_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotC2_x || spotC2_y < bouncer2_y && bouncer2_y < spotC2_y + 20 && spotC2_x + 20 > bouncer2_x && bouncer2_x > spotC2_x || spotC2_y < bouncer2_y + 20 && bouncer2_y + 20 < spotC2_y + 20 && spotC2_x + 20 > bouncer2_x && bouncer2_x > spotC2_x || spotC2_y < bouncer2_y + 20 && bouncer2_y < spotC2_y + 20 && spotC2_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotC2_x || spotC2_y < bouncer2_y && bouncer2_y < spotC2_y + 20 && spotC2_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotC2_x) 
			{
				bouncer_x = 35;
				bouncer_y = 3;
				bouncer2_x = 35;
				bouncer2_y = 28;
				numDeaths += 1;
				doorB = true;
			}

			if (spotD2_y < bouncer_y && bouncer_y < spotD2_y + 20 && spotD2_x + 20 > bouncer_x && bouncer_x > spotD2_x || spotD2_y < bouncer_y + 20 && bouncer_y + 20 < spotD2_y + 20 && spotD2_x + 20 > bouncer_x && bouncer_x > spotD2_x || spotD2_y < bouncer_y + 20 && bouncer_y < spotD2_y + 20 && spotD2_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotD2_x || spotD2_y < bouncer_y && bouncer_y < spotD2_y + 20 && spotD2_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotD2_x || spotD2_y < bouncer2_y && bouncer2_y < spotD2_y + 20 && spotD2_x + 20 > bouncer2_x && bouncer2_x > spotD2_x || spotD2_y < bouncer2_y + 20 && bouncer2_y + 20 < spotD2_y + 20 && spotD2_x + 20 > bouncer2_x && bouncer2_x > spotD2_x || spotD2_y < bouncer2_y + 20 && bouncer2_y < spotD2_y + 20 && spotD2_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotD2_x || spotD2_y < bouncer2_y && bouncer2_y < spotD2_y + 20 && spotD2_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotD2_x) 
			{
				bouncer_x = 35;
				bouncer_y = 3;
				bouncer2_x = 35;
				bouncer2_y = 28;
				numDeaths += 1;
				doorB = true;
			}

			if (spotE2_y < bouncer_y && bouncer_y < spotE2_y + 20 && spotE2_x + 20 > bouncer_x && bouncer_x > spotE2_x || spotE2_y < bouncer_y + 20 && bouncer_y + 20 < spotE2_y + 20 && spotE2_x + 20 > bouncer_x && bouncer_x > spotE2_x || spotE2_y < bouncer_y + 20 && bouncer_y < spotE2_y + 20 && spotE2_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotE2_x || spotE2_y < bouncer_y && bouncer_y < spotE2_y + 20 && spotE2_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotE2_x || spotE2_y < bouncer2_y && bouncer2_y < spotE2_y + 20 && spotE2_x + 20 > bouncer2_x && bouncer2_x > spotE2_x || spotE2_y < bouncer2_y + 20 && bouncer2_y + 20 < spotE2_y + 20 && spotE2_x + 20 > bouncer2_x && bouncer2_x > spotE2_x || spotE2_y < bouncer2_y + 20 && bouncer2_y < spotE2_y + 20 && spotE2_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotE2_x || spotE2_y < bouncer2_y && bouncer2_y < spotE2_y + 20 && spotE2_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotE2_x) 
			{
				bouncer_x = 35;
				bouncer_y = 3;
				bouncer2_x = 35;
				bouncer2_y = 28;
				numDeaths += 1;
				doorB = true;
			}

			if (spotF2_y < bouncer_y && bouncer_y < spotF2_y + 20 && spotF2_x + 20 > bouncer_x && bouncer_x > spotF2_x || spotF2_y < bouncer_y + 20 && bouncer_y + 20 < spotF2_y + 20 && spotF2_x + 20 > bouncer_x && bouncer_x > spotF2_x || spotF2_y < bouncer_y + 20 && bouncer_y < spotF2_y + 20 && spotF2_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotF2_x || spotF2_y < bouncer_y && bouncer_y < spotF2_y + 20 && spotF2_x + 20 > bouncer_x + 20 && bouncer_x + 20 > spotF2_x || spotF2_y < bouncer2_y && bouncer2_y < spotF2_y + 20 && spotF2_x + 20 > bouncer2_x && bouncer2_x > spotF2_x || spotF2_y < bouncer2_y + 20 && bouncer2_y + 20 < spotF2_y + 20 && spotF2_x + 20 > bouncer2_x && bouncer2_x > spotF2_x || spotF2_y < bouncer2_y + 20 && bouncer2_y < spotF2_y + 20 && spotF2_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotF2_x || spotF2_y < bouncer2_y && bouncer2_y < spotF2_y + 20 && spotF2_x + 20 > bouncer2_x + 20 && bouncer2_x + 20 > spotF2_x) 
			{
				bouncer_x = 35;
				bouncer_y = 3;
				bouncer2_x = 35;
				bouncer2_y = 28;
				numDeaths += 1;
				doorB = true;
			}
			}


			// Door Check for Level 1
			if (doorA == true && lvl == 1 && bouncer_x + 20 >= 358 && bouncer_x + 20 <= 385 && bouncer_y > 0 && bouncer_y < 50)
			{
				bouncer_x -= 2;
			}

			if (doorA == true && lvl == 1 && bouncer2_x + 20 >= 358 && bouncer2_x + 20 <= 385 && bouncer2_y > 0 && bouncer2_y < 50)
			{
				bouncer2_x -= 2;
			}

			if (doorA == true && lvl == 1 && bouncer_x < 424 && bouncer_x > 385 && bouncer_y > 0 && bouncer_y < 50)
			{
				bouncer_x += 2;
			}

			if (doorA == true && lvl == 1 && bouncer2_x < 424 && bouncer2_x > 385 && bouncer2_y > 0 && bouncer2_y < 50)
			{
				bouncer2_x += 2;
			}

			// Door Check for Level 2
			if (doorB == true && lvl == 2 && bouncer_y <= 273 && bouncer_y >= 253 && bouncer_x > 369 && bouncer_x < 472)
			{
				bouncer_y += 2;
			}

			if (doorB == true && lvl == 2 && bouncer2_y <= 273 && bouncer2_y >= 253 && bouncer2_x > 369 && bouncer2_x < 472)
			{
				bouncer2_y += 2;
			}

			// Green Button Detection for Level 1
			if (lvl == 1 && bouncer_y + 20 > 422 && bouncer_x > 314 && bouncer_x < 345)
			{
				doorA = false;
			}
			else if (lvl == 1 && bouncer2_y + 20 > 422 && bouncer2_x > 314 && bouncer2_x < 345)
			{
				doorA = false;
			}
			else if (lvl == 1 && bouncer_y + 10 > 183 && bouncer_y + 10 < 194 && bouncer_x + 10 > 589 && bouncer_x + 10 < 604)
			{
				doorA = false;
			}
			else if (lvl == 1 && bouncer2_y + 10 > 183 && bouncer2_y + 10 < 194 && bouncer2_x + 10 > 589 && bouncer2_x + 10 < 604)
			{
				doorA = false;
			}
			else 
			{
				doorA = true;
			}

			// Green Button Detection for Level 2
			if (lvl == 2 && bouncer_y + 10 > 276 && bouncer_y + 10 < 294 && bouncer_x + 10 > 37 && bouncer_x + 10 < 56 && bouncer2_y + 10 > 423 && bouncer2_y + 10 < 441 && bouncer2_x > 29 && bouncer2_x < 54)
			{
				doorB = false;
			}
			else if (lvl == 2 && bouncer2_y + 10 > 276 && bouncer2_y + 10 < 294 && bouncer2_x + 10 > 37 && bouncer2_x + 10 < 56 && bouncer_y + 10 > 423 && bouncer_y + 10 < 441 && bouncer_x > 29 && bouncer_x < 54)
			{
				doorB = false;
			}


			/* Since we've now updated the positions of the items onscreen, we should redraw.
			Setting redraw to true tells the program we want to do this. */
			redraw = true;
		}

		/* If you hit the window close button (X button, top right corner of
		the window,) the program will exit. */
		else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) 
		{
			break;
		}

		/*	To handle key presses, this loop and the next keeps track of 
		which buttons are pressed.

		The tracking happens in the key[4] array. For each key, the
		corresponding key[] element is true if the key is down, and 
		false if it is up.

		This else if block sets the key[] array to true if it sees 
		any keys that are pressed down.
		*/
		else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) 
		{
			switch(ev.keyboard.keycode) 
			{
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;
			case ALLEGRO_KEY_W:
				key[KEY_W] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;
			case ALLEGRO_KEY_S:
				key[KEY_S] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;
			case ALLEGRO_KEY_A:
				key[KEY_A] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;
			case ALLEGRO_KEY_D:
				key[KEY_D] = true;
				break;
			}
		}

		/* This else if block sets the corresponding element in the
		key[] array to false if that keyboard key is released. */
		else if(ev.type == ALLEGRO_EVENT_KEY_UP) 
		{
			switch(ev.keyboard.keycode) 
			{
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				break;
			case ALLEGRO_KEY_W:
				key[KEY_W] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				break;
			case ALLEGRO_KEY_S:
				key[KEY_S] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = false;
				break;
			case ALLEGRO_KEY_A:
				key[KEY_A] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
				break;
			case ALLEGRO_KEY_D:
				key[KEY_D] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}

		// changing levels
		if (bouncer_x < 620 && bouncer_x > 580 && bouncer_y < 460 && bouncer_y > 420 && bouncer2_x < 620 && bouncer2_x > 580 && bouncer2_y < 460 && bouncer2_y > 420)
		{
			bouncer_x = 35;
			bouncer_y = 3;
			bouncer2_x = 35;
			bouncer2_y = 28;
			lvl += 1;
			if (lvl == 4)
			{
				Sleep(1000);
				lvl = 1;
			}
		}

		/* Here, we refresh the display. We only do this if the position of
		any of the objects onscreen was changed. 
		*/
		string deaths = "Deaths: " + to_string(numDeaths);
		const char* charDeaths = deaths.c_str();


		if( (redraw == true) && (al_is_event_queue_empty(event_queue) == true) ) 
		{
			/* Make sure we don't redraw twice for the same new content. */
			redraw = false;

			/* Sets the whole display to black. */
			al_clear_to_color(al_map_rgb(0,0,0));

			if (lvl == 1) {
				al_draw_bitmap( background1,	/* What to draw (the background.) */
					0.0,							/* The x coordinate on the screen to draw the background to. */
					0.0,							/* The y coordinate on the screen to draw the background to. */
					0
					);
			}

			else if (lvl == 2) {
				al_draw_bitmap( background2,	/* What to draw (the background.) */
					0.0,							/* The x coordinate on the screen to draw the background to. */
					0.0,							/* The y coordinate on the screen to draw the background to. */
					0
					);
			}

			else if (lvl == 3) {
				al_draw_bitmap( background3,	/* What to draw (the background.) */
					0.0,							/* The x coordinate on the screen to draw the background to. */
					0.0,							/* The y coordinate on the screen to draw the background to. */
					0
					);
			}


			/* Brings in a background to the display. */
			if (lvl == 1) {

				/* Brings in the door to the display. */
				if (doorA == true) {
					al_draw_bitmap( door1,
						358.0,
						14.0,
						0
						);
				}

				/* This copies the label to the string str1 */
				strcpy(str1, "TBD! LEVEL 1");
				strcpy(str4, charDeaths);

				al_draw_text( font,
					al_map_rgb(0,255,179),
					640/2,
					(480 - (480/12)),
					ALLEGRO_ALIGN_CENTRE,
					str4
					);

				/* this draws text onto the screen that tells you the distance */
				al_draw_text( font, 
					al_map_rgb(0,255,179),		/* White text. */
					640/2,						/* Middle of the screen (width.) */
					(480/125),					/* Fraction from the top of the screen. */
					ALLEGRO_ALIGN_CENTRE,		/* Centre aligned text. */
					str1						/* Contains "Distance: XXX" */
					);

				/* This copies the label to the string str1 */
				strcpy(end_str1, "END");
				/* this draws text onto the screen that tells you the distance */
				al_draw_text( font, 
					al_map_rgb(255,0,0),			/* White text. */
					600,							/* Middle of the screen (width.) */
					440,						/* Fraction from the top of the screen. */
					ALLEGRO_ALIGN_CENTRE,			/* Centre aligned text. */
					end_str1						/* Contains "Distance: XXX" */
					);

			}

			else if (lvl == 2) {

				if (doorB == true) {
					al_draw_bitmap( door2,
						370.0,
						241.0,
						0
						);
				}

				/* This copies the label to the string str2 */
				strcpy(str2, "TBD! LEVEL 2");
				strcpy(str4, charDeaths);

				al_draw_text( font,
					al_map_rgb(0,255,179),
					640/2,
					(480 - (480/12)),
					ALLEGRO_ALIGN_CENTRE,
					str4
					);

				/* this draws text onto the screen that tells you the distance */
				al_draw_text( font, 
					al_map_rgb(0,255,179),		/* White text. */
					640/2,						/* Middle of the screen (width.) */
					(480/125),					/* Fraction from the top of the screen. */
					ALLEGRO_ALIGN_CENTRE,		/* Centre aligned text. */
					str2						/* Contains "Distance: XXX" */
					);

				/* This copies the label to the string str1 */
				strcpy(end_str2, "END");
				/* this draws text onto the screen that tells you the distance */
				al_draw_text( font, 
					al_map_rgb(255,0,0),			/* White text. */
					600,							/* Middle of the screen (width.) */
					440,						/* Fraction from the top of the screen. */
					ALLEGRO_ALIGN_CENTRE,			/* Centre aligned text. */
					end_str2						/* Contains "Distance: XXX" */
					);

			}

			else if (lvl == 3) {

				/* This copies the label to the string str3 */
				strcpy(str3, "TBD! LEVEL 3");
				strcpy(str4, charDeaths);
				al_draw_text( font,
					al_map_rgb(0,255,179),
					640/2,
					(480 - (480/12)),
					ALLEGRO_ALIGN_CENTRE,
					str4
					);
				/* this draws text onto the screen that tells you the distance */
				al_draw_text( font, 
					al_map_rgb(0,255,179),		/* White text. */
					640/2,						/* Middle of the screen (width.) */
					(480/125),					/* Fraction from the top of the screen. */
					ALLEGRO_ALIGN_CENTRE,		/* Centre aligned text. */
					str3						/* Contains "Distance: XXX" */
					);

				/* This copies the label to the string str1 */
				strcpy(end_str3, "END");
				/* this draws text onto the screen that tells you the distance */
				al_draw_text( font, 
					al_map_rgb(255,0,0),			/* White text. */
					600,							/* Middle of the screen (width.) */
					440,						/* Fraction from the top of the screen. */
					ALLEGRO_ALIGN_CENTRE,			/* Centre aligned text. */
					end_str3
					);

			}

			if (lvl <= 3 && lvl > 0)
			{
				/* This draws the bouncer bitmap. */
				al_draw_bitmap( bouncer,/* Bitmap to draw. */
					bouncer_x,			/* X coordinate to draw it at. */
					bouncer_y,			/* Y coordinate to draw it at. */
					0					/* No flags. */
					);

				al_draw_bitmap( bouncer2,/* Bitmap to draw. */
					bouncer2_x,			/* X coordinate to draw it at. */
					bouncer2_y,			/* Y coordinate to draw it at. */
					0					/* No flags. */
					);

				//Draws the spotlights
				if (lvl == 1)
				{
					al_draw_bitmap(spotlightA1, spotA1_x, spotA1_y, 0);
					al_draw_bitmap(spotlightB1, spotB1_x, spotB1_y, 0);
					al_draw_bitmap(spotlightC1, spotC1_x, spotC1_y, 0);
					al_draw_bitmap(spotlightD1, spotD1_x, spotD1_y, 0);
					al_draw_bitmap(spotlightE1, spotE1_x, spotE1_y, 0);
				}

				if (lvl == 2)
				{
					al_draw_bitmap(spotlightA2, spotA2_x, spotA2_y, 0);
					al_draw_bitmap(spotlightB2, spotB2_x, spotB2_y, 0);
					al_draw_bitmap(spotlightC2, spotC2_x, spotC2_y, 0);
					al_draw_bitmap(spotlightD2, spotD2_x, spotD2_y, 0);
					al_draw_bitmap(spotlightD2, spotE2_x, spotE2_y, 0);
					al_draw_bitmap(spotlightD2, spotF2_x, spotF2_y, 0);
				}
			}

			/* The display is double-buffered, which means that there are two screens. When
			the first is being displayed, we can draw on the other. Once we're done drawing,
			we can "flip" the two, putting the new one on the display. We can then overwrite
			the contents of the old one with new stuff and flip again. 

			al_flip_display() flips the two screen buffers, showing the one we just finished
			drawing to. */
			al_flip_display();

		}
	}

	/* If the while loop is exited, then the program is finished. In that case,
	our program comes down here to get to the end of the main function. Before
	we finish, we have to clean up some of the things we created so we don't
	leave a mess behind after the program dies. */

	/* Shutdown Addons */
	al_shutdown_primitives_addon();
	al_shutdown_image_addon();

	/* Clean up all the created objects. */
	al_destroy_bitmap(background1);
	al_destroy_bitmap(background2);
	al_destroy_bitmap(background3);
	al_destroy_bitmap(bouncer);
	al_destroy_bitmap(bouncer2);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}