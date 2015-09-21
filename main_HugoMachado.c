/**
 *  @file IUPGL_image.c Simple example of use of OpenGL and IUP to for image processing (Paint) programs.
 * 
 *  Creates a dialog with a tool bar, a canvas and a msg label bar.
 *  It is a simple WIMP program that paints a image into a IUP canvas using OpenGL.
 *  It shows how to handle mouse events and messages and a simple image process function.
 *
 *  Compile this program with:
 *  iups.c and iups.h     <- layer to simply (reduce the number of calls) of the Iup interface
 *  image.c  and image.h  <- simple ADT (Abtract Data Type) for a digital image 
 *
 *  Button images must be available in:
 *  "img/file_open.bmp"
 *  "img/blue_flag.bmp"
 *  "img/file_save.bmp"
 *  "img/green_flag.bmp" 
 *
 *  Link this program with: 
 *  iup.lib;iupgl.lib;comctl32.lib;opengl32.lib;glu32.lib;
 *
 *  You can find iup libs (iup.lib; iupgl.lib;) at:
 *  http://www.tecgraf.puc-rio.br/iup/
 *  the others lib (comctl32.lib;opengl32.lib;glu32.lib;) are inluded in the Visual Studio 
 *
 *  Do not forget to put the path for the include and lib in the project file.
 *  Here we use: c:\tec\inc and c:\tec\lib
 * 
 *  Ignore: libcmt.lib;
 *
 *  Last modification:  Marcelo Gattass, 15ago2013. 
 *
 **/
/*- Include lib interfaces: ANSI C, IUP and OpenGL ------*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "iups_HugoMachado.h"

#include <GL/gl.h>     /* OpenGL functions*/
#include <GL/glu.h>    /* OpenGL utilitary functions*/
#include "image_HugoMachado.h"     /* interface of the image processing functions */

Image *backup = NULL;
/*- Program context variables(declared as globals to facilitate the comunication in the callback model)-----------*/
struct global_context {
	Ihandle* dialog;                    /* dialog (main window) */
	Ihandle* canvas;                    /* canvas handle */
	Ihandle* msgbar;                    /* message bar  handle */
	Image*   image;                     /* image to be painted in the canvas*/
	int      width,height;              /* width and height of the canvas  */
} gc = { NULL, NULL, NULL, NULL, 640, 480 };

/*------------------------------------------*/
/* IUP Callbacks                            */
/*------------------------------------------*/

/* function called when the canvas is exposed in the screen.  self is the iuphandle of the canvas */
int repaint_cb(Ihandle *self) 
{
	int x,y;
	IupGLMakeCurrent(self);                /* all OpenGL primitives will be directed to this canvas */
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);  /* dark grey color in the RGBA mode A is the alpha channel (ignore) */
	glClear(GL_COLOR_BUFFER_BIT);          /* clear the color buffer */

	if (gc.image!=NULL)
	{
		int h = imgGetHeight(gc.image);
		int w = imgGetWidth(gc.image);
		/* assing to each pixel of the canvas the color of the corresponding pixel in the image */
		glBegin(GL_POINTS);
		for (y=0;y<h;y++) {
			for (x=0;x<w;x++) {
				float r,g,b;
				imgGetPixel3f(gc.image,x,y,&r,&g,&b); /* gets the RGB value the pixel (x,y) */
				glColor3f(r,g,b);        /* define a current color in OpenGL */
				glVertex2i(x,y);         /* paint the pixel */
			}

		}
		glEnd();
	}
	IupGLSwapBuffers(self);  /* change the back buffer with the front buffer */
	return IUP_DEFAULT; /* returns the control to the main loop */
}


/* function called in the event of changes in the width or in the height of the canvas */
int resize_cb(Ihandle *self, int new_width, int new_height)
{
	IupGLMakeCurrent(self);  /* Make the canvas current in OpenGL */

	/* define the entire canvas as the viewport  */
	glViewport(0,0,new_width,new_height);

	/* transformation applied to each vertex */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();           /* identity, i. e. no transformation */

	/* projection transformation (orthographic in the xy plane) */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D (0.0, (GLsizei)(new_width), 0.0, (GLsizei)(new_height));  /* window of interest [0,w]x[0,h] */

	/* update canvas size and repaint */
	gc.width=new_width;
	gc.height=new_height;
	repaint_cb(gc.canvas);

	return IUP_DEFAULT; /* return to the IUP main loop */
}

static char* strip_path_from_filename(char* file_name) {
	char* p=file_name;
	while (*p!='\0') p++; /* go to the end of string */     
	while ( (p>file_name) && (*p!='\\')) p--;  /* return up to '\' */
	return (*p=='\\')?p+1:p;
}

int load_cb(void)
{
	char* file_name = IupSelectFile("File Selection","*.bmp","Load a BMP file");
	if (!file_name) {
		IupSetfAttribute(gc.msgbar, "TITLE", "Selection failed");
		return IUP_DEFAULT;
	}

	if (gc.image!=NULL) imgDestroy(gc.image);

	gc.image=imgReadBMP(file_name);  /* loads the image */
	if (gc.image!=NULL) {
		gc.width = imgGetWidth(gc.image);
		gc.height = imgGetHeight(gc.image);
		backup = imgCopy(gc.image);
		IupCanvasResize(gc.canvas,gc.dialog,gc.width,gc.height);
		repaint_cb(gc.canvas);
		IupSetfAttribute(gc.msgbar, "TITLE", "%s", strip_path_from_filename(file_name));
	} else
		IupSetfAttribute(gc.msgbar, "TITLE", "Can't open %s", strip_path_from_filename(file_name));


	return IUP_DEFAULT;
}

int binary_cb(void)
{
	Image* tmp = gc.image;
	if (tmp != NULL){
		IupSetfAttribute(gc.msgbar, "TITLE", "Grey scale image ...");
		gc.image = imgGrey(tmp);
		repaint_cb(gc.canvas);   /* repaint canvas */
		imgDestroy(tmp);
		tmp = gc.image;
		gc.image = imgBinary(tmp);
		repaint_cb(gc.canvas);   /* repaint canvas */
		imgDestroy(tmp);
		IupSetfAttribute(gc.msgbar, "TITLE", "Grey scale image ...");
	}
	return IUP_DEFAULT;
}

int exit_cb(void)
{
	printf("Function to free memory and do finalizations...\n");
	return IUP_CLOSE;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//The following functions were modified

int erode_cb(void)
{
	if(gc.image == NULL){
		return IUP_DEFAULT;
	}

	Image* tmp = gc.image;
	IupSetfAttribute(gc.msgbar, "TITLE", "Erode call back");
	gc.image = imgEroded(tmp);
	repaint_cb(gc.canvas);   /* repaint canvas */
	imgDestroy(tmp);
	IupSetfAttribute(gc.msgbar, "TITLE", "Erode call back");
	return IUP_DEFAULT;
}

int dilate_cb(void)
{
	if(gc.image == NULL){
		return IUP_DEFAULT;
	}

	Image* tmp = gc.image;
	IupSetfAttribute(gc.msgbar, "TITLE", "Dilate call back");
	gc.image = imgDilated(tmp);
	repaint_cb(gc.canvas);   /* repaint canvas */
	imgDestroy(tmp);
	IupSetfAttribute(gc.msgbar, "TITLE", "Dilate call back");
	return IUP_DEFAULT;
}

int count_cb(void)
{
	int n = 0;
	if(gc.image == NULL){
		return IUP_DEFAULT;
	}
	n = count(gc.image);
	IupMessagef("Result", "%d items", n);
	IupSetfAttribute(gc.msgbar, "TITLE", "Count call back");
	repaint_cb(gc.canvas);   /* repaint canvas */
	return IUP_DEFAULT;
}

int save_cb(void)
{
	IupSetfAttribute(gc.msgbar, "TITLE", "Save call back");
	char* file_name = IupSelectFileSave("Save","*.bmp","Save a BMP file");
	if (!file_name) {
		IupSetfAttribute(gc.msgbar, "TITLE", "Save failed");
		return IUP_DEFAULT;
	}

	imgWriteBMP(file_name, gc.image);

	IupSetfAttribute(gc.msgbar, "TITLE", "Count call back");
	return IUP_DEFAULT;
}

int gauss_cb(void)
{
	if(gc.image == NULL){
		return IUP_DEFAULT;
	}
	Image* tmp = imgCopy(gc.image);
	IupSetfAttribute(gc.msgbar, "TITLE", "Gauss call back");
	imgGauss(gc.image, tmp);
	repaint_cb(gc.canvas);   /* repaint canvas */
	imgDestroy(tmp);
	IupSetfAttribute(gc.msgbar, "TITLE", "Gauss call back");
	return IUP_DEFAULT;
}

int median_cb(void)
{
	if(gc.image == NULL){
		return IUP_DEFAULT;
	}
	IupSetfAttribute(gc.msgbar, "TITLE", "Median call back");
	imgMedian(gc.image);
	repaint_cb(gc.canvas);   /* repaint canvas */
	IupSetfAttribute(gc.msgbar, "TITLE", "Median call back");
	return IUP_DEFAULT;
}

int edge_cb(void)
{
	if(gc.image == NULL){
		return IUP_DEFAULT;
	}
	Image* tmp = imgCopy(gc.image);
	IupSetfAttribute(gc.msgbar, "TITLE", "Edge call back");
	gc.image = imgEdges(tmp);
	repaint_cb(gc.canvas);   /* repaint canvas */
	imgDestroy(tmp);
	IupSetfAttribute(gc.msgbar, "TITLE", "Edge call back");
	return IUP_DEFAULT;
}

int reset_cb(void)
{
	IupSetfAttribute(gc.msgbar, "TITLE", "Reset callback");
	if(backup!=NULL){
		gc.image = imgCopy(backup);
	}
	repaint_cb(gc.canvas);   /* repaint canvas */
	IupSetfAttribute(gc.msgbar, "TITLE", "Reset callback");
	return IUP_DEFAULT;
}


//End: Modified functions


/*-------------------------------------------------------------------------*/
/* Incializa o programa.                                                   */
/*-------------------------------------------------------------------------*/

Ihandle* InitToolbar(void)
{
	Ihandle* toolbar;

	/* Create four buttons */
	Ihandle* load=IupSButton("img/file_open.bmp","load a BMP image file",(Icallback) load_cb);
	Ihandle* binary=IupSButton("img/binary.bmp","convert RGB to binary image callback function",(Icallback)binary_cb);
	Ihandle* erode = IupSButton("img/erode.bmp", "Write your tip here...", (Icallback)erode_cb);
	Ihandle* dilate = IupSButton("img/dilate.bmp", "Write your tip here...", (Icallback)dilate_cb);
	Ihandle* count = IupSButton("img/count.bmp", "Write your tip here...", (Icallback)count_cb);
	Ihandle* save = IupSButton("img/file_save.bmp", "save image in a BMP file", (Icallback)save_cb);
	Ihandle* reset = IupSButton("reset_Hugo.bmp", "reset image", (Icallback)reset_cb);
	Ihandle* gauss = IupSButton("gauss_Hugo.bmp", "Gauss ", (Icallback)gauss_cb);
	Ihandle* median = IupSButton("median_Hugo.bmp", "Median", (Icallback)median_cb);
	Ihandle* edge = IupSButton("edge_Hugo.bmp", "Edge", (Icallback)edge_cb);

	toolbar=IupHbox(load,save,binary,erode,dilate,count, reset, gauss, median, edge, NULL);

	return toolbar;
}

Ihandle* InitDialog(void)
{
	Ihandle* dialog;   /* dialog containing the canvas */
	Ihandle* content;  /* dialog content */

	Ihandle* toolbar=InitToolbar();                                                 /* buttons tool bar */
	gc.canvas = IupSGLCanvas(640,480,(Icallback) repaint_cb, (Icallback) resize_cb);   /* canvas to paint with OpenGL */
	gc.msgbar = IupSMessageBar("Message...",20);     /* a msg bar */

	content = IupVbox(toolbar,gc.canvas,gc.msgbar,NULL);

	/* create the dialog and set its attributes */
	dialog = IupSDialog("Paint with IUP_OpenGL",content, (Icallback) exit_cb, "NO");

	return dialog;
}

/*-----------------------*/
/* Main function.        */
/*-----------------------*/
int main(int argc, char* argv[]) 
{
	IupSOpen(&argc, &argv);                      /* opens the IUP lib */

	gc.dialog = InitDialog();                      /* local function to create a dialog with buttons and canvas */
	IupShowXY(gc.dialog, IUP_CENTER, IUP_CENTER);  /* shows dialog in the center of screen */

	IupMainLoop();                        /* handle the program control to the IUP lib until a return IUP_CLOSE */

	IupClose();
	/* closes the IUP lib */

	return IUP_NOERROR;
}
