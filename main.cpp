#include "opencv2/opencv.hpp"
#include "BackgroundSubtract.h"
#include "Detector.h"

#include <opencv2/highgui/highgui_c.h>
#include "Ctracker.h"
#include <iostream>
#include <vector>

//------------------------------------------------------------------------
// Mouse callbacks
//------------------------------------------------------------------------
void mv_MouseCallback(int event, int x, int y, int /*flags*/, void* param)
{
	if (event == cv::EVENT_MOUSEMOVE)
	{
		cv::Point2f* p = (cv::Point2f*)param;
		if (p)
		{
			p->x = static_cast<float>(x);
			p->y = static_cast<float>(y);
		}
	}
}

// ----------------------------------------------------------------------
// set to 0 for Bugs tracking example
// set to 1 for mouse tracking example
// ----------------------------------------------------------------------
 #define ExampleNum 0

int main(int ac, char** av)
{
	std::string inFile("..\\..\\data\\TrackingBugs.mp4");
	if (ac > 1)
	{
		inFile = av[1];
	}

#if ExampleNum
	cv::Scalar Colors[] = { cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255), cv::Scalar(255, 255, 0), cv::Scalar(0, 255, 255), cv::Scalar(255, 0, 255), cv::Scalar(255, 127, 255), cv::Scalar(127, 0, 255), cv::Scalar(127, 0, 127) };
	cv::VideoCapture capture(inFile);
	if (!capture.isOpened())
	{
		return 1;
	}
	cv::namedWindow("Video");
	cv::Mat frame;
	cv::Mat gray;

	CTracker tracker(0.2f, 0.1f, 60.0f, 5, 10);

	capture >> frame;
	cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
	CDetector* detector = new CDetector(gray);
	int k = 0;

	double freq = cv::getTickFrequency();

	int64 allTime = 0;

	while (k != 27)
	{
		capture >> frame;
		if (frame.empty())
		{
			//capture.set(cv::CAP_PROP_POS_FRAMES, 0);
			//continue;
			break;
		}
		cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

		int64 t1 = cv::getTickCount();

		const std::vector<Point_t>& centers = detector->Detect(gray);

		tracker.Update(centers);

		int64 t2 = cv::getTickCount();

		allTime += t2 - t1;

		for (int i = 0; i < centers.size(); i++)
		{
			cv::circle(frame, centers[i], 3, cv::Scalar(0, 255, 0), 1, CV_AA);
		}

		std::cout << tracker.tracks.size() << std::endl;

		for (int i = 0; i < tracker.tracks.size(); i++)
		{
			if (tracker.tracks[i]->trace.size()>1)
			{
				for (int j = 0; j < tracker.tracks[i]->trace.size() - 1; j++)
				{
					cv::line(frame, tracker.tracks[i]->trace[j], tracker.tracks[i]->trace[j + 1], Colors[tracker.tracks[i]->track_id % 9], 2, CV_AA);
				}
			}
		}

		cv::imshow("Video", frame);

		k = cv::waitKey(20);
	}

	std::cout << "work time = " << (allTime / freq) << std::endl;

	delete detector;


	cv::destroyAllWindows();
	return 0;
#else

	int k = 0;
	cv::Scalar Colors[] = { cv::Scalar(255, 0, 0), cv::Scalar(0, 255, 0), cv::Scalar(0, 0, 255), cv::Scalar(255, 255, 0), cv::Scalar(0, 255, 255), cv::Scalar(255, 255, 255) };
	cv::namedWindow("Video");
	cv::Mat frame = cv::Mat(800, 800, CV_8UC3);

	cv::VideoWriter vw = cv::VideoWriter("output.mpeg", CV_FOURCC('P', 'I', 'M', '1'), 20, frame.size());

	// Set mouse callback
	cv::Point2f pointXY;
	cv::setMouseCallback("Video", mv_MouseCallback, (void*)&pointXY);

	CTracker tracker(0.3f, 0.5f, 60.0f, 25, 25);
	track_t alpha = 0;
	cv::RNG rng;
	while (k != 27)
	{
		frame = cv::Scalar::all(0);

		// Noise addition (measurements/detections simulation )
		float Xmeasured = pointXY.x + static_cast<float>(rng.gaussian(2.0));
		float Ymeasured = pointXY.y + static_cast<float>(rng.gaussian(2.0));

		// Append circulating around mouse cv::Points (frequently intersecting)
		std::vector<Point_t> pts;
		pts.push_back(Point_t(Xmeasured + 100.0f*sin(-alpha), Ymeasured + 100.0f*cos(-alpha)));
		pts.push_back(Point_t(Xmeasured + 100.0f*sin(alpha), Ymeasured + 100.0f*cos(alpha)));
		pts.push_back(Point_t(Xmeasured + 100.0f*sin(alpha / 2.0f), Ymeasured + 100.0f*cos(alpha / 2.0f)));
		pts.push_back(Point_t(Xmeasured + 100.0f*sin(alpha / 3.0f), Ymeasured + 100.0f*cos(alpha / 1.0f)));
		alpha += 0.05f;


		for (size_t i = 0; i < pts.size(); i++)
		{
			cv::circle(frame, pts[i], 3, cv::Scalar(0, 255, 0), 1, CV_AA);
		}

		tracker.Update(pts);

		std::cout << tracker.tracks.size() << std::endl;

		for (size_t i = 0; i < tracker.tracks.size(); i++)
		{
			const auto& tracks = tracker.tracks[i];

			if (tracks->trace.size()>1)
			{
				for (size_t j = 0; j < tracks->trace.size() - 1; j++)
				{
					cv::line(frame, tracks->trace[j], tracks->trace[j + 1], Colors[i % 6], 2, CV_AA);
				}
			}
		}

		cv::imshow("Video", frame);
		vw << frame;

		k = cv::waitKey(10);
	}

	vw.release();
	cv::destroyAllWindows();
	return 0;

#endif
}
