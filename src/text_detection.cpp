#include <ros/ros.h>
#include <std_msgs/String.h>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <tesseract/baseapi.h>
#include <string.h>
#include <leptonica/allheaders.h>
using namespace std;

bool image_sub = false;
cv::Mat original_img;
cv::Mat gray;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
	try{
		cv_bridge::CvImage cv_img=*cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
		original_img = cv_img.image;
		image_sub = true;
	}
	catch (cv_bridge::Exception& e){
		ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
	}	
}

int main(int argc, char* argv[])
{
	ros::init(argc,argv,"text_detection");
	ros::NodeHandle node_("~");

	string camera_topic_, image_, tessdata_path_, language_;
	int camera_framerate_;
	char path[64], language[64];
	node_.param("camera_topic", camera_topic_, string("/usb_cam/image_raw"));
	node_.param("camera_framerate", camera_framerate_, 30);
	node_.param("image", image_, string("none"));
	node_.param("tessdata_path", tessdata_path_, string("/home/username/tesseract"));
	node_.param("language", language_, string("eng"));

	sprintf(path, "%s", tessdata_path_.c_str());
	sprintf(language, "%s", language_.c_str());
	ros::Rate loop_rate(camera_framerate_);

	image_transport::ImageTransport it(node_);
	image_transport::Subscriber camera_sub = it.subscribe(camera_topic_, 1, imageCallback);
	ros::Publisher text_pub = node_.advertise<std_msgs::String>("text",1);
	std_msgs::String text_msg;

	char *outText;
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
	if (api->Init(tessdata_path_.c_str(), language_.c_str())) {
		fprintf(stderr, "Could not initialize tesseract.\n");
		exit(1);
	}

	if(image_ != "none") {
		cout<<"image : "<<image_<<endl<<endl;
		original_img = cv::imread(image_);
		cv::cvtColor(original_img, gray, CV_RGB2GRAY);
		cv::threshold(gray, gray, 0, 255, cv::THRESH_BINARY|cv::THRESH_OTSU);
		api->SetImage((uchar*)gray.data, gray.size().width, gray.size().height, gray.channels(), gray.step1());
		outText = api->GetUTF8Text();

		cout<<"<text>"<<endl;
		cout<<(string){outText}<<endl;
		cout<<endl;
	}

	while(ros::ok() && image_=="none") {
		if(image_sub) {
			cv::cvtColor(original_img, gray, CV_RGB2GRAY);
			cv::threshold(gray, gray, 0, 255, cv::THRESH_BINARY|cv::THRESH_OTSU);
			api->SetImage((uchar*)gray.data, gray.size().width, gray.size().height, gray.channels(), gray.step1());
			outText = api->GetUTF8Text();

			text_msg.data = (string){outText};
			text_pub.publish(text_msg);
		}
		image_sub = false;
		ros::spinOnce();
		loop_rate.sleep();
	}

	// Destroy used object and release memory
	api->End();
	delete [] outText;

	return 0;
}
