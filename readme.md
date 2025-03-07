
# MY Custom OCR

In this repository, I have provided all of the script that necessary for train Tesseract-OCR to extract different front. So, please follow the instruction step by step to avoid confusion and make the training process flowing smoothly. `tess_train.sh` is the script that call others scripts to work together, almost every task can be done using this script.

## Common syntax

```bash
    ./tess_train.sh [MODE] [FIRST_ARGS] [SCOND_ARGS] [OPTIONAL_CONFIG_ARGUMENT]
```

## These are options of first argument [MODE]
- install
- add_project
- preprocess
- train
- export_model
- test

|Mode|Description|
|----|-----------|
|install|Install required libraries that will be used in all futher process|
|add_project|Setup the environment that compatible for running scripts|
|data_split|Devide dataset into train and test to evaluate model performance|
|preprocess|apply image processing to normalize and generate more data by using adaptive threshold technique|
|train|Start training Tesseract-OCR by using training data that provided by preprocess script|
|export_model|Export the trained data from the previous step allow Tesseract can call and use the traineddata|
|test|Test the performance of traineddata by let Tesseract read throught dataset that have splitted as test dataset|

# Install
Before start, First thing that need to do is to clone git repository using command

```bash
    git clone https://github.com/thaiteamRyowa/clavi_ocr.git
```

The work space should contain files as image below


### Syntax

```bash
    ./tess_train.sh install [TARGET_LIBRARY]
```

### Example

```bash
    ./tess_train install all    // Install all libraries
```    

### Arguments
|Library Option|Description|
|----|-----------|
|all|install all library|
|tesseract|Install Tesseract-OCR|
|leptonica|Install Leptonica(image processing tool for tesseract)|
|langdata|Get language alphabets data|
|pylib|Install python library|
        


# add_project
After install all required library,every single model need to use `add_project` to create environment that will be call by every script. Since each script was wrote for making the process to most automatically, please check the environment carefully when find an error that script cannot find required file. 

### Syntax

```bash
    ./tess_train.sh add_project [PROJECT_NAME]
```

### Example

```bash
    ./tess_train.sh add_project example_name
```

### Argument
        
|Argument|Key|Default|Description|
|----|-|-|---------|
|PROJECT_NAME|-|example|Name of the project, this is the same one that have added in add_project|

# data_split
After an environment have created by `add_project`,next is to devide the dataset into train and test. But before running the script, all image file need to be contained in the directory `data/[PROJECT_NAME]/all/images` by named them as `[NAME]_[FILE_NUMBER].[FILE_TYPE]` ,`Ex.example_0.jpg`

![Alt text](git_image/img.png)

and put the ground-truth texts in the text file name `gt.txt` in `data/[PROJECT_NAME]/all/text` inke in the picture below

![Alt text](git_image/gt.png)

### Syntax
```bash
    ./tess_train.sh [PROJECT_NAME] [TRAIN_RATIO]
```
### Example
```bash
    ./tess_train.sh example_name 0.75
```
### Argument
        
|Argument|Key|Default|Description|
|----|-|-|---------|
|PROJECT_NAME|-|example|Name of the project, this is the same one that have added in add_project|
|TRAIN_RATIO|-|0.8|Ratio of dataset that will be splitted to train dirctory|

### Description
After run this sctipt image and text files will be created in `train/test` directory,please check image and text ordering carefully by a little misodered will lead to feeding Tesseract with wrong data and critically impact to the model performance
![Alt text](git_image/gt_list.png)
# preprocess
After split the dataset into two, before move to the next step please download neededtraineddata file from this repository https://github.com/tesseract-ocr/tessdata_best and locate the file into `utils/tessdata`directory
(Example. If you want to train new chinese text  style for Tesseract, the needed file will be `chi_sim.traineddata`)
![Alt text](git_image/traineddata.png)

### Syntax

```bash
    ./tess_train.sh preprocess [MODEL_NAME]
```

### Example

```bash
    ./tess_train.sh preprocess example_name
```

|Argument|Key|Default|Description|
|----|-|-|---------|
|PROJECT_NAME|-|example|Name of the project, this is the same one that have added in add_project|

### Description
After run this sctipt black-white image and ground-truth files will be saved in newly created directory `data/[PROJECT_NAME]/train/training_data/[PROJECT_NAME]-ground-truth` which is the directory that will be used for training tesseract. By using `random adaptive threshold` technique each black-white image will be generated with different threshold value, so each of them will be trated as different image.( you can increase the threshold variation by define the range of random value )

# train
After you have run preprocess script, the necessary data will be prepared.Next step is to run the `train` script to let Tesseract adap with new data.
In the training method there is many value that can be defined to adjust the traing method, or you can just leave the option argument blank to use default value that i have pprovided in script.

### Syntax

```bash
        ./tess_train.sh train [PROJECT_NAME] [OPTIONAL_ARGUMENT]
```

### Example

```bash
        ./tess_train.sh train example -p 13 -l eng -r 0.000001 -i 99999 -e 0.0001
```

### Argument
        
|Argument|Key|Default|Description|
|----|-|-|---------|
|PROJECT_NAME|-|example|Name of the project, this is the same one that have added in add_project|
|PSM|-p|8|Page segmentation mode of tesseract, each psm value represent how tesseract readyour image(0-13)|
|LEARNING_RATE|-r|0.0001|Learning rate of training process|
|LIMIT_ITER|-i|10000|When training process reach limit iteration Tesseract will stop training|
|LANGUAGE|-l|jpn|Language of the text in your image|
|TARGET_ERR|-e|0.01|When error from validation in tesseract training is less or equal this value training process will be done|

### Description 
Intraining process everytime that Tesseract meet lowest error rate, it will record that weigh and bias value as a checkpoint when the training process was stopped Tesseract will use the checkpoint that have lowest error rate to write `.traineddata` file 

# export_model
Since Tesseract can call the traineddata only from it's own tessdata directory. To use the new traineddata that have trained you need to export the traineddata into Tesseract tessdata directory using `tess_train` script. I have already set the default path to tesseract directory, if you need to use default value > lust leave `TESSERACT_DATA_DIRECTORY` 'blank'
### Syntax

```bash
        ./tess_train.sh checked [PROJECT_NAME] [TESSERACT_DATA_DIRECTORY]
```

### Example

```bash
        ./tess_train.sh checked example_name
```

### Arguments
|Argument|Description|
|----|-----------|
|PROJECT_NAME|Name of the project, this is the same one that have added in add_project|
|TESSERACT_DATA_DIRECTORY|The directory that tesseract contain it's own `traineddata` file that should be at `$pwd/usr/share/tesseract-ocr/5/tessdata`

    
# test
Test the model performance by let tesseract read all test file that have splitted to test directory. Since you just use the train directort to train Tesseract, these dataset will be unseen data for this model

### Syntax

```bash
        ./tess_train.sh test [PROJECT_NAME] [OPTIONAL_ARGUMENT]
```

### Example

```bash
        ./tess_train.sh checked example_name -p 6 -o tsv
```

### Arguments
|Argument|Key|Default|Description|
|----|-|-|---------|
|PROJECT_NAME|-|example|Name of the project, this is the same one that have added in add_project|
|PSM|-p|8|Page segmentation mode of tesseract, each psm value represent how tesseract readyour image(0-13)|
|OUTPUT_FORMAT|-o|txt|File for mat of the output of Tesseract(For more detailed data recommened to set as tsv)|
|READ_COLOUR|-c|bw|Specify the type of image that will be used for testing(gray/bw/none)|
|INVERT_IMAGE|-v|False|In some case, there is some image that have dark background and light character(for betterperformancr I'm very suggest to set INVERT_IMAGE= True)|


![SWProjectDiagram](https://github.com/user-attachments/assets/a1c14748-8bd7-4168-9f74-5edce146eb19)
