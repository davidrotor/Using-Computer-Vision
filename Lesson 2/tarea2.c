
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/highgui/highgui_c.h"
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>



IplImage* histograma(IplImage* im){
	uchar index;
	IplImage* hist = cvCreateImage(cvSize(255, 257), 8, 1);
	IplImage* img=cvCloneImage(im);
	for( int y=0; y< hist->height; y++ ) { 
        //uchar* ptr = (uchar*) ( grayscale->imageData + y * grayscale->widthStep ); 
        for( int x=0; x<hist->width; x++ ) { 
            
            hist->imageData[index*hist->widthStep+x]=0;
        }
    }
	for( int y=0; y< img->height; y++ ) { 
        //uchar* ptr = (uchar*) ( grayscale->imageData + y * grayscale->widthStep ); 
        for( int x=0; x<img->width; x++ ) { 
            index= img->imageData[y*img->widthStep+x];
            hist->imageData[index*hist->widthStep+x]++;
        }
    }
    return hist;
}
IplImage* binary(IplImage* im, int thresh){
	IplImage* img=cvCloneImage(im);
	for( int y=0; y< img->height; y++ ) { 
        for( int x=0; x<img->width; x++ ) { 
            if( (int)img->imageData[y*img->widthStep+x]>=thresh)
            {
            	img->imageData[y*img->widthStep+x]=0;
            }
            else{
            	img->imageData[y*img->widthStep+x]=255;
            }
            
        }
    }
    return img;
}
int autothresh(IplImage* im){
	IplImage* img=cvCloneImage(im);
	int index=0;
	int index2=0;
	int average1;
	int average2;
	int thresh;
	for( int y=0; y< img->height; y++ ) { 
        for( int x=0; x<img->width/2; x++ ) { 
             index=index+ img->imageData[y*img->widthStep+x];
            
        }
    }

    average1=(int)index/(img->height*img->width/2);
     printf("%d\n",average1);
    
    for( int y=0; y< img->height; y++ ) { 
        for( int x=img->width/2; x<img->width; x++ ) { 
             index2=index2+ img->imageData[y*img->widthStep+x];
            
        }
    }
    average2=(int)index2/(img->height*img->width/2);
    printf("%d\n",average2);
    
   
    
    return (int)(average1+average2)/2;

}

int main(int argc, char * argv[])
{
	IplImage* img = cvLoadImage("baboon.jpg",1);  //Load image file bgr
	IplImage* brg = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);//brg    
	IplImage* gbr = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);//verde por azul
	IplImage* r = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);    //r
	IplImage* g = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);    //g
	IplImage* b = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);    //b
	IplImage* sumrgb = cvCreateImage(cvGetSize(img), IPL_DEPTH_16U, 1);    //b
	IplImage* sumgb = cvCreateImage(cvGetSize(img), IPL_DEPTH_16U, 1);
	IplImage* grayscale = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 1);
	IplImage* reflejo = cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);//reflejo
	cvSplit(img, b, g, r, NULL);       //OpenCV likes it in BGR format
	cvEqualizeHist( b, b );    //equalise r
	cvEqualizeHist( g, g );    //equalise g
	cvEqualizeHist( r, r );    //equalise b
	//OPERACION DE GRAYSCALE
	for( int y=0; y< grayscale->height; y++ ) { 
        for( int x=0; x<grayscale->width; x++ ) { 
            uchar filtro=(((uchar) img->imageData[y*img->widthStep+3*x+2]+img->imageData[y*img->widthStep+3*x+1]+img->imageData[y*img->widthStep+3*x+0]))/3;
            grayscale->imageData[y*grayscale->widthStep+x]=(uchar)filtro; //Set red to max (BGR format)
        }
    }
     //Histogramas
    cvShowImage("Histograma rojo",histograma(r));
	cvShowImage("Histograma verde",histograma(g));
	cvShowImage("Histograma azul",histograma(b));
	cvShowImage("Histograma grayscale",histograma(grayscale));

	
	//threshold   
	
	cvMerge(b, r, g, NULL, brg);  //merge all separate channels together to output image rgb
	cvMerge(g, b, r, NULL, gbr);
	
	cvShowImage("Red Channel", r); //Show output image in output window
	cvShowImage("green Channel", g);	
	cvShowImage("Blue Channel", b);

	cvShowImage("Rojo por verde", brg);
	cvShowImage("Verde por Azul", gbr);

	cvShowImage("Filtro Grayscale", grayscale);
	cvNamedWindow("Binaria rojo", 1);
	cvShowImage("Original RBG", img);
	
		//Binarias
	cvShowImage("Binaria rojo autothreshold",binary(r,80));
	cvShowImage("Binaria azul autothreshold",binary(b,80));
	cvShowImage("Binaria verde autothreshold",binary(g,80));
	cvShowImage("Binaria grayscale autothreshold",binary(grayscale, 80));
	

	
	cvShowImage("Binaria rojo autothreshold",binary(r,autothresh(r)));
	cvShowImage("Binaria azul autothreshold",binary(b,autothresh(g)));
	cvShowImage("Binaria verde autothreshold",binary(g,autothresh(b)));
	cvShowImage("Binaria grayscale autothreshold",binary(grayscale, autothresh(grayscale)));

	

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
