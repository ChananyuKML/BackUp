#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <string_view>

#include "tesseract/baseapi.h"
#include "leptonica/allheaders.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"

int threshValue(cv::Mat a){
        int total_val = 0;
        for(int j = 1; j < a.rows; j++){
                for(int i = 1; i < a.cols; i++){
                        total_val +=  static_cast<int>(a.data[i*j]);
                }
        }
        int mean_val = total_val/(a.rows*a.cols);
        int thresh_value = mean_val*0.8;
        return thresh_value;
}

int windowSize(cv::Mat gray){
        int window_size = gray.rows/2;
        if(window_size%2 == 0){
                window_size += 1;
                return window_size;
        }
        else{
                return window_size;
        }

}

cv::Mat boundingBox(cv::Mat image, int left, int top, int right, int bottom, int thickness){
        int x = left;
        int y = top;
        int width = right-left;
        int height = bottom - top;
        cv::Scalar color(0,255,0); //Green color
        cv::rectangle(image, cv::Point(top,left), cv::Point(right,bottom), color, thickness, cv::LINE_8 );
        return image;
}




int main(int argc,char** argv){
        std::cout << "Read_Dir passed" << std::endl;
        int count = 0;
        std::string image_dir = argv[1];
        std::string trained_data = argv[2];
        std::cout << "Image Dir is : " <<  image_dir << std::endl;

        std::vector<std::string> file_name_list;

        DIR *directory = opendir((image_dir).c_str());
        struct  dirent *entry;

        if((directory != nullptr)){
                while((entry=readdir(directory)) != nullptr){
                        std::string dir_file_name = entry -> d_name;
                        if(dir_file_name != "." && dir_file_name != ".."){
                                file_name_list.push_back(entry -> d_name);
                                std::cout << entry -> d_name << std::endl;
                                }
                        count += 1;
                        }

                closedir(directory);
        }

        std::cout << "Read_Dir passed" << std::endl;



        /*
        tesseract::TessBaseAPI tess;
        tess.Init(NULL,trained_data.c_str());
        tess.SetVariable("tessedit_pageseg_mode", "6"); // Automatic page segmentation

*/



        std::vector<std::string>::iterator file_iterator = file_name_list.begin();
        std::string output_dir= "data/keyence_4/test/postprocess/";
        while (file_iterator != file_name_list.end()){
                std::stringstream image_path;
                image_path << image_dir <<  *file_iterator;

                std::stringstream output_stream;
                output_stream << output_dir << *file_iterator;
                cv::Mat src = cv::imread(image_path.str(),cv::IMREAD_GRAYSCALE);
                cv::Mat dst,bin_dst;
                cv::adaptiveThreshold(src, dst, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, windowSize(src), threshValue(src));
                cv::threshold(src,bin_dst,threshValue(src),255,cv::THRESH_BINARY);

                cv::imwrite(output_stream.str(),bin_dst);
                /*
                tess.Recognize(0);
                std::string text = tess.GetUTF8Text();
                std::cout << text << std::endl;

                tesseract::ResultIterator* iterator = tess.GetIterator();
                if(iterator != nullptr){
                        std::cout << image_path.str() << std::endl;
                        while (iterator -> Next(tesseract::RIL_SYMBOL)){
                                std::string symbol = iterator-> GetUTF8Text(tesseract::RIL_SYMBOL);
                                int left, top, right, bottom;
                                iterator -> BoundingBox(tesseract::RIL_SYMBOL, &left, &top, &right, &bottom);
                                std::cout << "Character :: " << symbol << std::endl;
                                std::cout << "Bounding box : " << " " << left << " " << top  << " " << right  << " " << bottom << std::endl;
                        }
                }
        */
/*
                std::stringstream  output_file_stream;
                std::string file_name = *file_iterator;
                size_t file_base_name = file_name.find(".");
                output_file_stream << image_dir << file_name.substr(0,file_base_name);
                std::string text = tess.GetUTF8Text();


                std::ofstream csvFile(output_file_stream.str());
                if(csvFile.is_open()){
                        csvFile << text;
                        csvFile.close();
                }
                */
                std::advance(file_iterator,1);
        }
        
        return 0;
}