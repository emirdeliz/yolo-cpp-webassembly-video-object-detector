#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <vector>
#include <cstdlib>
#include "emscripten.h"

// Define struct to represent a bounding box
struct BoundingBox
{
	float x, y, width, height;
	int class_label;
};

// Function to perform object detection using YOLO
std::vector<BoundingBox> perform_yolo_detection(const cv::Mat &image)
{
	std::vector<BoundingBox> detected_objects;
	
	// Load YOLO model configuration and weights
	cv::dnn::Net net = cv::dnn::readNet("yolov3.weights", "yolov3.cfg", "darknet");

	// Specify target device (CPU or GPU)
	net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
	net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU); // or DNN_TARGET_OPENCL for GPU

	// Preprocess image (resize, normalize, etc.)
	cv::Mat blob = cv::dnn::blobFromImage(
		image, 1.0 / 255.0,
		cv::Size(416, 416),
		cv::Scalar(0, 0, 0),
		true,
		false
	);

	// Set input blob
	net.setInput(blob);

	// Forward pass
	cv::Mat detectionMat = net.forward();

	// Process detection results
	// Iterate through detectionMat and extract bounding boxes, confidence scores, and class labels
	// Placeholder code: generate dummy bounding boxes for demonstration
	BoundingBox bbox1;
	bbox1.x = 0.1f;
	bbox1.y = 0.2f;
	bbox1.width = 0.3f;
	bbox1.height = 0.4f;
	bbox1.class_label = 0;

	BoundingBox bbox2;
	bbox2.x = 0.5f;
	bbox2.y = 0.6f;
	bbox2.width = 0.2f;
	bbox2.height = 0.3f;
	bbox2.class_label = 1;

	detected_objects.push_back(bbox1);
	detected_objects.push_back(bbox2);
	
	return detected_objects;
}

// Define the main function to be called from JavaScript
extern "C"
{
	int EMSCRIPTEN_KEEPALIVE detectObjects(const unsigned char *imageData, int width, int height, float *boundingBoxes)
	{
		// Convert image data to OpenCV Mat
		cv::Mat image(height, width, CV_8UC3, const_cast<unsigned char *>(imageData));

		// Perform object detection
		std::vector<BoundingBox> detected_objects = perform_yolo_detection(image);

		// Copy detection results to the provided buffer
		int num_boxes = detected_objects.size();
		for (int i = 0; i < num_boxes; ++i)
		{
			boundingBoxes[i * 5] = detected_objects[i].x;
			boundingBoxes[i * 5 + 1] = detected_objects[i].y;
			boundingBoxes[i * 5 + 2] = detected_objects[i].width;
			boundingBoxes[i * 5 + 3] = detected_objects[i].height;
			boundingBoxes[i * 5 + 4] = detected_objects[i].class_label;
		}

		return num_boxes;
	}
}