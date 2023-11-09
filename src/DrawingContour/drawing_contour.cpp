#include <iostream>
#include <string>
#include <fstream>
#include <vector>


#include "tesseract/baseapi.h"
#include "leptonica/allheaders.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "../../../include/unitext/cvUniText.hpp"


//Function to get threshold value for adaptive threshold
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

int threshValue(cv::Mat a){
        int total_val = 0;
        for(int j = 1; j < a.rows; j++){
                for(int i = 1; i < a.cols; i++){
                        total_val +=  static_cast<int>(a.data[i*j]);
                }
        }
        int mean_val = total_val/(a.rows*a.cols);
        int thresh_value = mean_val/8;
        return thresh_value;
}

//Function to get kernel size for adaptive threshold
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

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

//Function to detect contour
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//

std::vector<cv::Rect> getBoungingBox(cv::Mat img){
        cv::Mat gray,edge,otsu,ada,dila,con,erode;
        cv::cvtColor(img,gray,cv::COLOR_BGR2GRAY);
        cv::Canny(gray,edge,10,255);

        cv::adaptiveThreshold(gray, ada, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, windowSize(gray), threshValue(gray));

        cv::Mat rect_ker = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5,5));

        cv::erode(otsu,erode,rect_ker);
        std::vector<std::vector<cv::Point>> contours;

        cv::findContours(erode, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

        std::vector<std::vector<cv::Point>>::iterator contours_iterator = contours.begin();

        std::vector<cv::Rect> detected_contours;
        while(contours_iterator != contours.end()) {
                cv::Rect rect = cv::boundingRect(*contours_iterator);
                detected_contours.push_back(rect);
                std::advance(contours_iterator,1);
        }

        return detected_contours;

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//


int main(int argc, char** argv){

//Define Variables

        std::string file_path = argv[1];
        std::string psm = argv[3];

        //tesseract-OCR init
        tesseract::TessBaseAPI tess;
        tess.Init(NULL, argv[2]);//Specify traineddata that will be used for read
        tess.SetVariable("tessedit_pageseg_mode",psm.c_str());

        //WhiteListst for Keyence_2
        //tess.SetVariable("tessedit_char_whitelist", "結束番号品種丸鋼寸法明細鋼番製造年月成分");
        //WhiteList for Keyence_4
        //tess.SetVariable("tessedit_char_whitelist", "規格寸法長さ本数/質量本鋼番結束番号");

//Define OpenCV image
        cv::Mat Blur;
        cv::Mat image = cv::imread(file_path);
        cv::Mat draw_image = cv::imread(file_path);
        cv::GaussianBlur(image,Blur,cv::Size(15,15),0);

//Define the font and size of text that will be written in the image
        uni_text::UniText uniText("utils/fonts/static/NotoSansJP-Regular.ttf",12);

//Color of bounding boxes
        cv::Scalar box_color(0,255,0);



//Manually identify boxes
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
    
        std::vector<cv::Rect> BoxList;

//Keyence_1 Boxes

        // cv::Rect box1(589,1744,665,187);
        // cv::Rect box2(594,1920,330,154);
        // cv::Rect box3(583,2068,358,160);
        // cv::Rect box4(561,2228,918,160);
        // cv::Rect box5(550,2382,688,192);

        // BoxList.push_back(box1);
        // BoxList.push_back(box2);
        // BoxList.push_back(box3);
        // BoxList.push_back(box4);
        // BoxList.push_back(box5);


//Keyence_2 Boxes
/*
        cv::Rect box1(874,1132,163,80);
        cv::Rect box3(894,1299,169,68);
        cv::Rect box5(916,1453,145,66);
        cv::Rect box7(933,1604,152,74);
        cv::Rect box9(982,2636,147,78);
        cv::Rect box11(969,2781,146,75);
        cv::Rect box13(962,2927,139,70);
        cv::Rect box15(956,3069,145,80);

        BoxList.push_back(box1);
        BoxList.push_back(box3);
        BoxList.push_back(box5);
        BoxList.push_back(box7);
        BoxList.push_back(box9);
        BoxList.push_back(box11);
        BoxList.push_back(box13);
        BoxList.push_back(box15);

*/
//Keyence_3 Boxes

        // cv::Rect box1(909,1047,318,126);
        // cv::Rect box2(900,1287,357,153);
        // cv::Rect box3(894,1545,357,159);
        // cv::Rect box4(897,1812,345,150);
        // cv::Rect box5(888,2058,380,153);
        // cv::Rect box6(885,2334,345,117);
        // cv::Rect box7(879,2610,357,144);
        // cv::Rect box8(1331,1263,337,202);

        // BoxList.push_back(box1);
        // BoxList.push_back(box2);
        // BoxList.push_back(box3);
        // BoxList.push_back(box4);
        // BoxList.push_back(box5);
        // BoxList.push_back(box6);
        // BoxList.push_back(box7);
        // BoxList.push_back(box8);

//Keyence_4 Boxes

        // cv::Rect box1(1161,896,257,94);
        // cv::Rect box2(1151,1083,267,98);
        // cv::Rect box3(1127,1271,280,100);
        // cv::Rect box4(1074,1464,341,99);
        // cv::Rect box5(1095,1655,293,99);
        // cv::Rect box6(1010,1862,441,99);
        // cv::Rect box7(1815,1479,203,143);
        
        // BoxList.push_back(box1);
        // BoxList.push_back(box2);
        // BoxList.push_back(box3);
        // BoxList.push_back(box4);
        // BoxList.push_back(box5);
        // BoxList.push_back(box6);
        // BoxList.push_back(box7);
        
//OPM Boxes
        // cv::Rect box1(120,163,80,55);
        // cv::Rect box2(627,29,171,43);
        // cv::Rect box3(792,72,255,68);
        // cv::Rect box4(824,299,191,49);
        // cv::Rect box5(463,587,71,23);
        // cv::Rect box6(469,614,131,31);
        // cv::Rect box7(636,443,112,30);
        // cv::Rect box8(637,478,183,33);
        // cv::Rect box9(641,508,200,32);
        // cv::Rect box10(725,569,134,47);
        // cv::Rect box11(869,638,163,24);

        // BoxList.push_back(box1);
        // BoxList.push_back(box2);
        // BoxList.push_back(box3);
        // BoxList.push_back(box4);
        // BoxList.push_back(box5);
        // BoxList.push_back(box6);
        // BoxList.push_back(box7);
        // BoxList.push_back(box8);
        // BoxList.push_back(box9);
        // BoxList.push_back(box10);
        // BoxList.push_back(box11);


//OPM Boxes
        cv::Rect box1(213,65,170,41);
        cv::Rect box2(214,112,168,40);
        cv::Rect box3(218,156,165,45);
        cv::Rect box4(413,62,36,38);
        cv::Rect box5(413,108,38,37);
        cv::Rect box6(411,153,39,36);

        BoxList.push_back(box1);
        BoxList.push_back(box2);
        BoxList.push_back(box3);
        BoxList.push_back(box4);
        BoxList.push_back(box5);
        BoxList.push_back(box6);


//Replace Boxlist with contours if using text detection algorithm
        //std::vector<cv::Rect> AutoContours = getBoungingBox(image);

        //std::vector<cv::Rect>::iterator contour_iterator = auto_contours.begin();

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//        
//Drawing all bounding box in the list (BoxList/AutoContours) 
        std::vector<cv::Rect>::iterator BoxList_iterator = BoxList.begin();

        while(BoxList_iterator != BoxList.end()){
                cv::Rect Box = *BoxList_iterator;
                cv::rectangle(draw_image,Box,box_color,2);
                cv::Mat croppedBox = draw_image(Box);
                cv::cvtColor(croppedBox, croppedBox, cv::COLOR_BGR2GRAY);
                cv::adaptiveThreshold(croppedBox, croppedBox, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, windowSize(croppedBox), threshValue(croppedBox));
                tess.SetImage(croppedBox.data, croppedBox.cols, croppedBox.rows, 1, croppedBox.cols);
                tess.Recognize(0);
                char* ocrResult_c = tess.GetUTF8Text();
                tesseract::ResultIterator* ri = tess.GetIterator();
                tesseract::PageIteratorLevel level = tesseract::RIL_SYMBOL;

                const char* word = ri->GetUTF8Text(level);
                float confidence = ri->Confidence(level);
                if(ri != 0)
                if(word != 0){
                        std::cout<< word << " : " << confidence <<std::endl;
                }
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Write text on the image
                std::string ocrResult = tess.GetUTF8Text();
                std::string modified_ocrResult = ocrResult.substr(0, ocrResult.length() - 1);
                std::stringstream output_w_conf;
                output_w_conf << modified_ocrResult << " : "<< confidence;
                std::cout << modified_ocrResult << std::endl;
                cv::Rect rect = uniText.PutText(draw_image, output_w_conf.str(), cv::Point(Box.x,Box.y), cv::Scalar(255,0,0), false);
                std::advance(BoxList_iterator,1);
        }

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------//
//Define output file
        size_t dir_range = file_path.find_last_of("/");
        std::string file_dir = file_path.substr(0,dir_range+1);
        std::stringstream output_con_stream;
        output_con_stream << file_dir << "tess_result.jpg";
        
        cv::imwrite(output_con_stream.str(),draw_image);


        return 0;
}