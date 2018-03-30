#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui_c.h"
#include <ctype.h>
#include <stdio.h>
uchar color (uchar *imdata, int color, int ancho, int largo){

}
int main(int argc, char * argv[])
{
	IplImage* img = cvLoadImage("baboon.jpg",1);  //Load image file bgr
	//cvNamedWindow("Original", CV_WINDOW_AUTOSIZE );     //Create input window
	   //Create output window

	
	//printf(cvGetSize(img));
	//Create space for outputs rgb and its separate channels, r, g and b
	IplImage* brg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);//brg    
	IplImage* gbr = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);//verde por azul
	IplImage* r = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);    //r
	IplImage* g = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);    //g
	IplImage* b = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);    //b
	IplImage* sumrgb = cvCreateImage(cvGetSize(img), IPL_DEPTH_16U, 1);    //b
	IplImage* sumgb = cvCreateImage(cvGetSize(img), IPL_DEPTH_16U, 1);
	IplImage* grayscale = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage* reflejo = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);//reflejo
	
	//cvTranspose(img,reflejo);
	cvSplit(img, b, g, r, NULL);       //OpenCV likes it in BGR format

	cvEqualizeHist( b, b );    //equalise r
	cvEqualizeHist( g, g );    //equalise g
	cvEqualizeHist( r, r );    //equalise b
	//
	//OPERACION DE GRAYSCALE
	for( int y=0; y< grayscale->height; y++ ) { 
        //uchar* ptr = (uchar*) ( grayscale->imageData + y * grayscale->widthStep ); 
        for( int x=0; x<grayscale->width; x++ ) { 
            uchar filtro=(((uchar) img->imageData[y*img->widthStep+3*x+2]+img->imageData[y*img->widthStep+3*x+1]+img->imageData[y*img->widthStep+3*x+0]))/3;
            grayscale->imageData[y*grayscale->widthStep+x]=(uchar)filtro; //Set red to max (BGR format)
        }
    }

	cvMerge(b, r, g, NULL, brg);  //merge all separate channels together to output image rgb
	cvMerge(g, b, r, NULL, gbr);
	//cvReleaseImage(&r);
	//cvReleaseImage(&g);
	//cvReleaseImage(&b);
	
	cvShowImage("Red Channel", r); //Show output image in output window
	cvShowImage("green Channel", g);	
	cvShowImage("Blue Channel", b);
	//cvShowImage("Grayscale", grayscale);
	cvShowImage("Rojo por verde", brg);
	cvShowImage("Verde por Azul", gbr);
	//cvShowImage("Reflejo", reflejo);
	cvShowImage("Filtro Grayscale", grayscale);

	cvShowImage("Original RBG", img);
	cvWaitKey(0);
	cvReleaseImage( &img);
	cvReleaseImage( &r);
	cvReleaseImage( &g);
	cvReleaseImage( &b);
	cvReleaseImage( &grayscale);
	cvDestroyWindow( "Trans-in");
	cvDestroyWindow( "Red Channel");
	cvDestroyWindow( "Green Channel");
	cvDestroyWindow( "Blue Channel");
	return 0;
}