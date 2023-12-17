#include "SaveCSV.h"

//by now the file is open all the program- what to change maybe
extern std::ofstream DBoutput(R"(C:\projectSources\output.csv)", ofstream::app);

void SaveCSV::start() {
	vector<string>title = initTitle();
	writeRowToCSV(title);
}

//per instance- per video - write the titles of the data to the file
vector<string> SaveCSV::initTitle() {
	vector<string> row;
	row = { "width", "height", "top", "left", "time", "AvgR", "AvgG", "AvgB" };
	return row;
}

//per detection - this function is called by the PostProcess function after each detecting
//it gets the information about the position of the detection in the box object, the current frame's time, and of course, the current frame
//it calls to functions who collects more some info about the avgs of each channel of the detection area, keep ell the data and write all to the csv file
void SaveCSV::SaveDetectionDataToCSV(cv::Mat& image, Rect originalBox, double time) {
	//y+h - top + height
	//x+w - left + width
	Rect box = NormalizeBox(originalBox, image);
	Avgs avgs = calcAvgs(image, box);
	vector<string> row = initRow(box, to_string(time), to_string(avgs.AvgR), to_string(avgs.AvgG), to_string(avgs.AvgB));
	writeRowToCSV(row);
}

Rect SaveCSV::NormalizeBox(const Rect& originalBox, cv::Mat& image) {
	Rect box = originalBox;
	if (box.x < 0)
		box.x = 0;
	if (box.y < 0)
		box.y = 0;
	if (box.height < 0)
		box.height = 0;
	if (box.width < 0)
		box.width = 0;
	if (box.x + box.width > image.cols)
		box.width = image.cols - box.x;
	if (box.y + box.height > image.rows)
		box.height = image.rows - box.y;
	return box;
}

//per detetction - calculate avgs of each channel of the 'sub image'
Avgs SaveCSV::calcAvgs(cv::Mat& image, Rect box) {
	Avgs avgs;
	int sumR = 0, sumG = 0, sumB = 0;
	for (int i = box.y; i < box.y + box.height; i++) {
		for (int j = box.x; j < box.x + box.width; j++) {
			sumB += image.at<Vec3b>(i, j)[RChannel];
			sumG += image.at<Vec3b>(i, j)[GChannel];
			sumR += image.at<Vec3b>(i, j)[BChannel];
		}
	}
	int area = (box.height) * (box.width);
	avgs.AvgB = sumB / area;
	avgs.AvgG = sumG / area;
	avgs.AvgR = sumR / area;
	return avgs;
}

//per detetction - keeping the data in a vector to write to the file
vector<string> SaveCSV::initRow(Rect box, string  time, string AvgR, string  AvgG, string AvgB) {
	vector<string> row;
	row = { to_string(box.width), to_string(box.height), to_string(box.y),
	to_string(box.x), time , AvgR, AvgG, AvgB };
	return row;
}

//per detection - a row with its data is being written to the file
void SaveCSV::writeRowToCSV(const vector<string>& row) {
	for (size_t i = 0; i < row.size(); i++) {
		DBoutput << row[i];
		if (i < row.size() - 1) {
			DBoutput << ",";
		}
	}
	DBoutput << "\n";
}

void SaveCSV::close()
{
	DBoutput.close();
}

//int main() {
//	SaveCSV csv;
//	csv.start();//per video/ per frame that goes to detection- but must once in the begining
//	//csv.SaveDetectionDataToCSV(...)//per detection
//	csv.close();//per video/ per frame that goes to detection
//	//when to close
//	return 1;
//}
