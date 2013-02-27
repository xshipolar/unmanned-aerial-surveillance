#include <opencv2/opencv.hpp>
#include <tld_utils.h>
#include <iostream>
#include <sstream>
#include <TLD.h>
#include <stdio.h>
#include <quad.h>
#include <time.h>

using namespace cv;
using namespace std;

//Global variables
Rect box;
bool drawing_box = false;
bool gotBB = false;
bool tl = true;
bool rep = false;
bool fromfile=false;
string video;
int lot=0;
bool stopVideo=false;
time_t t1;
int X1,Y1,X2,Y2,x01,y01,x02,y02;
double vert;
double hori;
double moti;
const double v=12.8;
const double h=15.6;
const double l=100.2;



void readBB(char* file){
  ifstream bb_file (file);
  string line;
  getline(bb_file,line);
  istringstream linestream(line);
  string x1,y1,x2,y2;
  getline (linestream,x1, ',');
  getline (linestream,y1, ',');
  getline (linestream,x2, ',');
  getline (linestream,y2, ',');
  int x = atoi(x1.c_str());// = (int)file["bb_x"];
  int y = atoi(y1.c_str());// = (int)file["bb_y"];
  int w = atoi(x2.c_str())-x;// = (int)file["bb_w"];
  int h = atoi(y2.c_str())-y;// = (int)file["bb_h"];
  box = Rect(x,y,w,h);
}
//bounding box mouse callback
void mouseHandler(int event, int x, int y, int flags, void *param){
  switch( event ){
  case CV_EVENT_MOUSEMOVE:
    if (drawing_box){
        box.width = x-box.x;
        box.height = y-box.y;
    }
    break;
  case CV_EVENT_LBUTTONDOWN:
    drawing_box = true;
    box = Rect( x, y, 0, 0 );
    break;
  case CV_EVENT_LBUTTONUP:
    drawing_box = false;
    if( box.width < 0 ){
        box.x += box.width;
        box.width *= -1;
    }
    if( box.height < 0 ){
        box.y += box.height;
        box.height *= -1;
    }
	X1=box.x;
	Y1=box.y;
	X2=box.x+box.width;
	Y2=box.y+box.height;
    gotBB = true;
	stopVideo=true;
    break;
  }
}

void print_help(char** argv){
  //printf("use:\n     %s -p /path/parameters.yml\n",argv[0]);
  //printf("-s    source video\n-b        bounding box file\n-tl  track and learn\n-r     repeat\n");
}

void read_options(int argc, char** argv,VideoCapture& capture,FileStorage &fs){
  for (int i=0;i<argc;i++){
      if (strcmp(argv[i],"-b")==0){
          if (argc>i){
              readBB(argv[i+1]);
              gotBB = true;
          }
          else
            print_help(argv);
      }
      if (strcmp(argv[i],"-s")==0){
          if (argc>i){
              video = string(argv[i+1]);
              capture.open(video);
              fromfile = true;
          }
          else
            print_help(argv);

      }
      if (strcmp(argv[i],"-p")==0){
          if (argc>i){
              fs.open(argv[i+1], FileStorage::READ);
          }
          else
            print_help(argv);
      }
      if (strcmp(argv[i],"-no_tl")==0){
          tl = false;
      }
      if (strcmp(argv[i],"-r")==0){
          rep = true;
      }
  }
}

int main(int argc, char * argv[]){
  VideoCapture capture;
  capture.open(0);
  FileStorage fs;
  //Read options
  read_options(argc,argv,capture,fs);
  //Init camera
  if (!capture.isOpened())
  {
	cout << "capture device failed to open!" << endl;
    return 1;
  }
  //Register mouse callback to draw the bounding box
  cvNamedWindow("TLD",CV_WINDOW_AUTOSIZE);
  cvSetMouseCallback( "TLD", mouseHandler, NULL );
  //TLD framework
  TLD tld;
  //Read parameters file
  tld.read(fs.getFirstTopLevelNode());
  Mat frame;
  Mat last_gray;
  Mat first;
  if (fromfile){
      capture >> frame;
      cvtColor(frame, last_gray, CV_RGB2GRAY);
      frame.copyTo(first);
  }else{
      capture.set(CV_CAP_PROP_FRAME_WIDTH,340);
      capture.set(CV_CAP_PROP_FRAME_HEIGHT,240);
  }
  ///Initialization
GETBOUNDINGBOX:
  while(!gotBB)
  {
    if (!fromfile){
      capture >> frame;
    }
    else
      first.copyTo(frame);
    cvtColor(frame, last_gray, CV_RGB2GRAY);
    drawBox(frame,box);
	imshow("TLD", frame);
	
    if (cvWaitKey(33) == 'q')
	    return 0;
  }
  if (min(box.width,box.height)<(int)fs.getFirstTopLevelNode()["min_win"]){
      cout << "Bounding box too small, try again." << endl;
      gotBB = false;
      goto GETBOUNDINGBOX;
  }
  //Remove callback
  cvSetMouseCallback( "TLD", NULL, NULL );
  //printf("Initial Bounding Box = x:%d y:%d h:%d w:%d\n",box.x,box.y,box.width,box.height);
  //Output file
  FILE  *bb_file = fopen("bounding_boxes.txt","w");
  //TLD initialization
  tld.init(last_gray,box,bb_file);

  ///Run-time
  Mat current_gray;
  BoundingBox pbox;
  vector<Point2f> pts1;
  vector<Point2f> pts2;
  bool status=true;
  int frames = 1;
  int detections = 1;
  time_t t1=time(0);
REPEAT:
	while(capture.read(frame)){
    //get frame
    cvtColor(frame, current_gray, CV_RGB2GRAY);
    //Process Frame
    tld.processFrame(last_gray,current_gray,pts1,pts2,pbox,status,tl,bb_file);
    //Draw Points
    if (status){
      drawPoints(frame,pts1);
      drawPoints(frame,pts2,Scalar(0,255,0));
	    guidance(pbox.x,pbox.y,pbox.x+pbox.width,pbox.y+pbox.height);
      drawBox(frame,pbox);
      detections++;
    }
	time_t t2=time(0);
	lot++;
	if(difftime(t2,t1)>1)
	{
		double fr=(double)lot/(double)difftime(t2,t1);
		//cout << "Frame Rate : "<< fr <<"\n";
		t1=t2;
		lot=0;
	}
	//Display
    imshow("TLD", frame);
    //swap points and images
    swap(last_gray,current_gray);
    pts1.clear();
    pts2.clear();
    frames++;
    //printf("Detection rate: %d/%d\n",detections,frames);
    if (cvWaitKey(33) == 'q')
      break;
  }
  if (rep){
    rep = false;
    tl = false;
    fclose(bb_file);
    bb_file = fopen("final_detector.txt","w");
    //capture.set(CV_CAP_PROP_POS_AVI_RATIO,0);
    capture.release();
    capture.open(video);
    goto REPEAT;
  }
  fclose(bb_file);
  return 0;
}
void guidance(int x_1,int y_1,int x_2,int y_2)
{
  arr1={x_1,y_1,x_2,y_2};	
	int area2=((x_2-x_1)*(y_2-y_1));
	int area1=((X2-X1)*(Y2-Y1));
	if(area2>area1)
	{
		moti=-l*area2;
		moti/=area1;
	}
	else if(area2<area1)
	{
		moti=l*area1;
		moti/=area2;
	}
	else
	moti=0;
	double c2_x=(x_2+x_1)/2;
	double c2_y=(y_2+y_1)/2;
	double c1_x=(X2+X1)/2;
	double c1_y=(Y2+Y1)/2;
	if(c2_x>c1_x)
	vert=v*(c2_x-c1_x);
	else if(c2_x<c1_x)
	vert=-v*(c2_x-c1_x);
	else
	vert=0;
	if(c2_y>c1_y)
	hori=h*(c2_y-c1_y);
	else if(c2_y<c1_y)
	hori=-h*(c2_y-c1_y);
	else
	hori=0;
	cout<<hori<<"\t"<<vert<<"\t"<<moti<<"\n";
  arr={hori,vert,moti};
	X2=x_2;
	Y2=y_2;
	X1=x_1;
	Y1=y_1;
}
double* get_guidance()
{
	arr={hori,vert,moti};
	return arr;
}	
int* tracked_points()
{
	arr1={x01,y01,x02,y02};
	return arr1;
}
