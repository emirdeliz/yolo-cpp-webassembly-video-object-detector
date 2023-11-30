// Include Libraries.
#include <opencv2/opencv.hpp>
#include <fstream>
#include <cstring>
#include <string>
#include <iostream>
#include "emscripten.h"

// Namespaces.
using namespace cv;
using namespace std;
using namespace cv::dnn;

// Constants.
const float INPUT_WIDTH = 640.0;
const float INPUT_HEIGHT = 640.0;
const float SCORE_THRESHOLD = 0.5;
const float NMS_THRESHOLD = 0.45;
const float CONFIDENCE_THRESHOLD = 0.45;

char *convert_string_to_char_array(string s)
{
	char *cstr = new char[s.length() + 1];
	strcpy(cstr, s.c_str());
	return cstr;
}

char *convert_int_to_char_array(int i)
{
	string s = to_string(i);
	return convert_string_to_char_array(s);
}

vector<Mat> pre_process(Mat &input_image)
{
	// Convert to blob.
	Net net = readNet("yolov5s.onnx");
	net.setPreferableBackend(DNN_BACKEND_OPENCV);
	
	Mat blob;
	blobFromImage(input_image, blob, 1. / 255., Size(INPUT_WIDTH, INPUT_HEIGHT), Scalar(), true, false);

	net.setInput(blob);

	// Forward propagate.
	vector<Mat> outputs;
	net.forward(outputs, net.getUnconnectedOutLayersNames());

	return outputs;
}

char * post_process(Mat &input_image)
{
	// Load model.
	vector<Mat> outputs = pre_process(input_image);

	// Load class list.
	vector<string> class_name;
	ifstream ifs("coco.names");
	string line;

	while (getline(ifs, line))
	{
		class_name.push_back(line);
	}

	// // Initialize vectors to hold respective outputs while unwrapping detections.
	// vector<int> class_ids;
	// vector<float> confidences;
	// vector<Rect> boxes;

	// // Resizing factor.
	// float x_factor = input_image.cols / INPUT_WIDTH;
	// float y_factor = input_image.rows / INPUT_HEIGHT;

	// float *data = (float *)outputs[0].data;

	// const int dimensions = 85;
	// const int rows = 25200;
	
	// // Iterate through 25200 detections.
	// for (int i = 0; i < rows; ++i)
	// {
	// 	float confidence = data[4];
	// 	// Discard bad detections and continue.
	// 	if (confidence >= CONFIDENCE_THRESHOLD)
	// 	{
	// 		float *classes_scores = data + 5;
	// 		// Create a 1x85 Mat and store class scores of 80 classes.
	// 		Mat scores(1, class_name.size(), CV_32FC1, classes_scores);
	// 		// Perform minMaxLoc and acquire index of best class score.
	// 		Point class_id;
	// 		double max_class_score;
	// 		minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
	// 		// Continue if the class score is above the threshold.
	// 		if (max_class_score > SCORE_THRESHOLD)
	// 		{
	// 			// Store class ID and confidence in the pre-defined respective vectors.

	// 			confidences.push_back(confidence);
	// 			class_ids.push_back(class_id.x);

	// 			// Center.
	// 			float cx = data[0];
	// 			float cy = data[1];
	// 			// Box dimension.
	// 			float w = data[2];
	// 			float h = data[3];
	// 			// Bounding box coordinates.
	// 			int left = int((cx - 0.5 * w) * x_factor);
	// 			int top = int((cy - 0.5 * h) * y_factor);
	// 			int width = int(w * x_factor);
	// 			int height = int(h * y_factor);
	// 			// Store good detections in the boxes vector.
	// 			boxes.push_back(Rect(left, top, width, height));
	// 		}
	// 	}
	// 	// Jump to the next column.
	// 	data += 85;
	// }

	// // Perform Non Maximum Suppression and draw predictions.
	// vector<int> indices;
	// NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);

	char *result = new char[10];
	// for (int i = 0; i < indices.size(); i++)
	// {
	// 	int idx = indices[i];
	// 	Rect box = boxes[idx];

	// 	int left = box.x;
	// 	int top = box.y;
	// 	int width = box.width;
	// 	int height = box.height;

	// 	// Get the label for the class name and its confidence.
	// 	string label = format("%.2f", confidences[idx]);
	// 	label = class_name[class_ids[idx]] + ":" + label;
	// 	// Draw class labels.

	// 	char *resultItem = new char[5];
	// 	resultItem[0] = *convert_string_to_char_array(label);
	// 	resultItem[1] = *convert_int_to_char_array(left);
	// 	resultItem[1] = *convert_int_to_char_array(top);
	// 	resultItem[1] = *convert_int_to_char_array(width);
	// 	resultItem[1] = *convert_int_to_char_array(height);
	// 	result[i] = *resultItem;
	// };

	return result;
}

#ifdef __cplusplus
extern "C"
{
#endif
	char * EMSCRIPTEN_KEEPALIVE check_image(
			unsigned char *img_data, int img_width, int img_height,
			unsigned char *templ_data, int templ_width, int templ_height)
	{
		Mat img(img_height, img_width, CV_8UC4, img_data);

		char *result = post_process(img);
		return result;
	}
#ifdef __cplusplus
}
#endif