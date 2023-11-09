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

#include "tesseract/baseapi.h"
#include "leptonica/allheaders.h"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"


int oddRand(int min, int max){
        srand(time(NULL));
        int rand_value = rand() % max + min;
        if(rand_value%2 == 0){
                rand_value += 1;
                return rand_value;
        }
        else{
                return rand_value;
        }

}

int randomWindowSize(cv::Mat a){
        return oddRand((a.rows/2),(a.rows));
}

int randomThreshValue(cv::Mat a){
        int total_val = 0;
        for(int j = 1; j < a.rows; j++){
                for(int i = 1; i < a.cols; i++){
                        total_val +=  static_cast<int>(a.data[i*j]);
                }
        }
        int mean_val = total_val/(a.rows*a.cols);
        int thresh_value = oddRand(mean_val/48,mean_val/12);
        return thresh_value;
}


struct MultipleLists{
        std::vector<int> train_splitted;
        std::vector<int> test_splitted;
        };

MultipleLists splitTrainTest(int min, int max, float ratio, std::vector<int> file_list){
        srand(time(0));
        MultipleLists splitted_list;
        std::random_shuffle(file_list.begin(),file_list.end());
        std::vector<int>::iterator iter = file_list.begin();

        if(!file_list.empty()){
                std::cout << "File list is valid" << std::endl;
        }
        else{
                std::cout << "File list is empty" << std::endl;
        }

        for(int i = 0; i < max*ratio; i++){
                splitted_list.train_splitted.push_back(*iter);
                std::advance(iter,1);
        }

        while(iter != file_list.end()){
                splitted_list.test_splitted.push_back(*iter);
                std::advance(iter,1);
        }
        return splitted_list;
}




int main(int argc,char** argv){


            cv::Mat gray,bin;
            int count = 0;
            std::string project_name = argv[1];
            float train_ratio = std::atof(argv[2]);

//Define vector that contain all file in directory

            std::vector<int> file_list;
            std::vector<int>::iterator file_num = file_list.begin();

            std::stringstream image_dir;
            image_dir << "data/" << project_name << "/all/images/";
            std::cout << "image directory path is" << image_dir.str() << std::endl;

            std::stringstream train_image_dir;
            train_image_dir << "data/" << project_name << "/train/images/cropped/";
            std::cout << "image directory path is" << train_image_dir.str() << std::endl;

            std::stringstream train_gray_dir;
            train_gray_dir << "data/" << project_name << "/train/images/cropped_gray/";
            std::cout << "image directory path is" << train_gray_dir.str() << std::endl;

            std::stringstream test_image_dir;
            test_image_dir << "data/" << project_name << "/test/images/";
            std::cout << "image directory path is" << test_image_dir.str() << std::endl;

//Define text directory

            std::stringstream text_dir;
            text_dir << "data/" << project_name << "/all/text/";
            std::cout << "image directory path is" << text_dir.str() << std::endl;

            std::stringstream train_text_dir;
            train_text_dir << "data/" << project_name << "/train/text/";
            std::cout << "image directory path is" << train_text_dir.str() << std::endl;

            std::stringstream training_file_dir;
            training_file_dir << "data/" << project_name << "/train/training_data/" << project_name << "-ground-truth/";
            std::cout << "image directory path is" << training_file_dir.str() << std::endl;

            std::stringstream test_text_dir;
            test_text_dir << "data/" << project_name << "/test/text/";
            std::cout << "image directory path is" << test_text_dir.str() << std::endl;


            DIR *directory = opendir((image_dir.str()).c_str());
            std::cout << "Dir set passed" << std::endl;
            struct  dirent *entry;
            std::cout<< "entry output" << std::endl;
            std::cout << "entry value is" << entry << std::endl;
            std::vector<std::string> file_name_list;


            if((directory != nullptr)){
                        while((entry=readdir(directory)) != nullptr){
                                    std::string dir_file_name = entry -> d_name;
                                    if(dir_file_name != "." && dir_file_name != ".."){
                                    file_name_list.push_back(entry -> d_name);
                                    }
                                    count += 1;
                                    }
                        
                        for(int file_num=0; file_num<(count-2); file_num++){
                                    file_list.push_back(file_num);
                                    }
                        
                        closedir(directory);
                        }

            else{
                        std::cerr << "No directory found" << std::endl;
                        return 1;
                        }
            std::vector<std::string>::iterator file_name_iterator = file_name_list.begin();
            std::cout << "Iterator point at : " << *file_name_iterator << std::endl;

            //-------------------------------------------------------------------------------------------------------------------------//
// Dataset_splitting

            int total_files =count-3;
            MultipleLists splitted_list = splitTrainTest(0,total_files,train_ratio,file_list);

            std::vector<int> train_list = splitted_list.train_splitted;
            std::sort(train_list.begin(),train_list.end());
            std::vector<int>::iterator train_point  = train_list.begin();
            while(train_point  != train_list.end()){
                        std::cout << "Train_num is : " << *train_point  << std::endl;
                        std::advance(train_point ,1);
            }


            std::vector<int> test_list = splitted_list.test_splitted;
            std::sort(test_list.begin(),test_list.end());
            std::vector<int>::iterator test_point = test_list.begin();
            while(test_point  != test_list.end()){
                        std::cout << "Test_num is : " << *test_point  << std::endl;
                        std::advance(test_point ,1);
            }


            //Combining image path with stringstream

            std::string file_base_name =*file_name_iterator;
            size_t prefix_pointer = file_base_name.find("_");
            size_t extension_pointer = file_base_name.find(".");
            std::string name_prefix = file_base_name.substr(0,prefix_pointer);
            std::string file_extension = file_base_name.substr(extension_pointer+1);
            std::cout<< "Name prefix is " << name_prefix << std::endl;
            std::cout<< "File_extension  is " << file_extension << std::endl;



            // train data split

            std::vector<int>::iterator train_iterator = train_list.begin();

            while(train_iterator != train_list.end()){

                        std::stringstream image_path_stream;
                        image_path_stream << image_dir.str() << name_prefix << "_" << *train_iterator << "." << file_extension;
                        std::cout << "src : " << image_path_stream.str() << std::endl;
                        std::ifstream src(image_path_stream.str(), std::ios::binary);

                        std::stringstream train_path_stream;
                        train_path_stream << train_image_dir.str() << name_prefix << "_" << *train_iterator << "." << file_extension;
                        std::cout << "dst : " << train_path_stream.str() << std::endl;
                        std::ofstream dst(train_path_stream.str(), std::ios::binary);

                        dst << src.rdbuf();
                        src.close();
                        dst.close();

                        std::stringstream input_text_file;
                        input_text_file << text_dir.str() << "gt.txt"; 
                        std::ifstream input_txt(input_text_file.str());
                        std::string line;
                        int selected_line_num = *train_iterator;
                        int current_line = 0;
                        while(std::getline(input_txt,line)){
                                    if(current_line == selected_line_num){
                                    std::ostringstream output_file_path;
                                    output_file_path << train_text_dir.str() << name_prefix << "_" << *train_iterator << ".txt";
                                    std::ofstream output_text_file(output_file_path.str());
                                    output_text_file << line;
                                    }
                                    current_line += 1;
                        }

                        std::advance(train_iterator,1);
            }


            //test split
            int test_count = 0;
            std::vector<int>::iterator test_iterator = test_list.begin();
            while(test_iterator != test_list.end()){
                        std::stringstream image_path_stream;
                        image_path_stream << image_dir.str() << name_prefix << "_" << *test_iterator << "." << file_extension;
                        std::cout << "src : " << image_path_stream.str() << std::endl;
                        std::ifstream src(image_path_stream.str(), std::ios::binary);

                        std::stringstream test_path_stream;
                        test_path_stream << test_image_dir.str() << name_prefix << "_" << *test_iterator << "." << file_extension;
                        std::cout << "dst : " << test_path_stream.str() << std::endl;
                        std::ofstream dst(test_path_stream.str(), std::ios::binary);

                        dst << src.rdbuf();
                        src.close();
                        dst.close();

                        std::stringstream input_text_file;
                        input_text_file << text_dir.str() << "gt.txt"; 
                        std::ifstream input_txt(input_text_file .str());
                        std::string line;
                        int selected_line_num = *test_iterator;
                        int current_line = 0;
                        while(std::getline(input_txt,line)){
                                    if(current_line == selected_line_num){
                                    std::ostringstream output_file_path;
                                    output_file_path << test_text_dir.str() << name_prefix << "_" <<*test_iterator << ".txt";
                                    std::ofstream output_text_file(output_file_path.str());
                                    output_text_file << line;
                                    }
                                    current_line += 1;
                        }





                        std::advance(test_iterator,1);
            }



            


            std::cout << "last file num is : " << count-3 << std::endl;


            //-------------------------------------------------------------------------------------------------------------------------//

            //Create gray directory
            for(int i; i<int(5000/count); i++){

            std::vector<int>::iterator training_iterator = train_list.begin();
            int training_count = 0;

                        while(training_iterator != train_list.end()){
                                    // Define input image
                                    std::stringstream input_image_stream;
                                    input_image_stream << train_image_dir.str() << name_prefix << "_" << *training_iterator << "." << file_extension;
                                    std::cout << "Input path is" << input_image_stream.str() << std::endl;

                                    // Define output image name
                                    std::stringstream training_image_path;
                                    training_image_path << training_file_dir.str() << name_prefix << "_" << (i*count)+training_count  << "." << file_extension;
                                    std::cout << "output path is" << training_image_path.str() << std::endl;

                                    cv::Mat src_img = cv::imread(input_image_stream.str());
                                    cv::Mat gray;

                                    cv::cvtColor(src_img, gray, cv::COLOR_BGR2GRAY);
                                    cv::adaptiveThreshold(gray, bin, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, randomWindowSize(gray), randomThreshValue(gray));
                                    cv::imwrite(training_image_path.str(),bin);
                                    

                                    std::stringstream input_text_file;
                                    input_text_file << text_dir.str() << "gt.txt"; 
                                    std::ifstream input_txt(input_text_file.str());
                                    std::string line;
                                    int selected_line_num = *training_iterator;
                                    int current_line = 0;

                                    while(std::getline(input_txt,line)){
                                    if(current_line == selected_line_num){

                                                std::ostringstream training_text_stream;
                                                training_text_stream << training_file_dir.str() << name_prefix << "_" << (i*count)+training_count << ".gt.txt";
                                                std::ofstream output_training_text(training_text_stream.str());
                                                output_training_text << line;
                                    }

                                    current_line += 1;

                                    }

                        std::advance(training_iterator,1);
                        training_count++;

                        }

            }



            std::cout << "data_preparing" << std::endl;
            return 0;       

}