#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/photo.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/stitching.hpp>
#include <vector>
#include <string>
#include <iostream>

using namespace cv;
using namespace std;
bool try_use_gpu = true;
Stitcher::Mode mode = Stitcher::SCANS;
string result_name = "result.jpg";

int find(const char* name){
    const char* filename = name;

    Mat src = imread(samples::findFile(filename), IMREAD_COLOR);
    //detailEnhance(src, src, 150, 1);

    if (src.empty()) {
        printf(" Error opening image\n");
        printf(" Program Arguments: [image_name -- default %s] \n", filename);
        return EXIT_FAILURE;
    }

    Mat gray;
    cvtColor(src, gray, COLOR_BGR2GRAY);
    medianBlur(gray, gray, 1);
    vector<Vec3f> circles;
    HoughCircles(gray, circles, HOUGH_GRADIENT, 1,
        gray.rows / 32,
        100, 30, 10, 60);
    //imshow("gray", src);
    for (size_t i = 0; i < circles.size(); i++){
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);

        circle(src, center, 1, Scalar(100, 0, 255), 1, LINE_AA);

        int radius = c[2];
        circle(src, center, radius, Scalar(100, 0, 255), 2, LINE_AA);
        cout << "Center: " << c[0] << " " << c[1] << endl;
        cout << "Radius: " << c[2] << endl;
    }
    resize(src, src, Size(src.cols, src.rows));
    imshow("detected circles", src);
    cout << "Coins num: " << circles.size() <<endl;
    waitKey();
    return EXIT_SUCCESS;
}

int counter = 0;
void extract_frames(const string& videoFilePath, vector<Mat>& frames){
	try {
		VideoCapture cap(videoFilePath);
        if (!cap.isOpened()) {
            cout << "Can not open Video file";
        }
		for (int frameNum = 0; frameNum < cap.get(CAP_PROP_FRAME_COUNT); frameNum++){
			Mat frame;
			cap >> frame;
            if (frameNum % 30 == 0) {
                frames.push_back(frame);
                counter++;
            }
		}
	}
	catch (cv::Exception& e){
		cerr << e.msg << endl;
		exit(1);
	}
}

void save_frames(vector<Mat>& frames, const string& outputDir){
	vector<int> compression_params;
	compression_params.push_back(IMWRITE_JPEG_QUALITY);
	compression_params.push_back(100);
	int frameNumber = 0;
	for(vector<Mat>::iterator frame = frames.begin(); frame != frames.end(); frame++){
		frameNumber++;
		string filePath = outputDir + to_string(static_cast<long long>(frameNumber)) + ".jpg";
		imwrite(filePath, *frame, compression_params);
	}
}

int main(int argc, char* argv[]){
    /*vector<Mat> frames;
    extract_frames("C:\\Users\\Orest\\source\\repos\\LabAI4\\coins3.mp4", frames);

    save_frames(frames, "C:\\Users\\Orest\\source\\repos\\LabAI4\\");

    Mat pano;
    Ptr<Stitcher> stitcher = Stitcher::create(mode);
    Stitcher::Status status = stitcher->stitch(frames, pano);
    if (status != Stitcher::OK){
        cout << "Can't stitch images, error code = " << int(status) << endl;
        return EXIT_FAILURE;
    }
    imwrite(result_name, pano);
    cout << "stitching completed successfully\n" << result_name << " saved!";

    pano = NULL;
    stitcher = NULL;*/

    find("result.jpg");
    return EXIT_SUCCESS;
}

