#!/bin/bash
#region set Default_Variable

# language that use for training tesseract
LANG=jpn_vert

# tesseract-oce page segmentation mode
PSM=8

# directory that contain .traineddata file that use for train (only support tessdata-best)
TESSDATA_DIR="tessdata"

# learning rate that use for train
LEARNING_RATE=0.0001

# max iteration for tesseract training
LIMIT_ITER=200000

# target error rate to stop training 
TARGET_ERR=0.0001

# tesseract out
OUTPUT_FORMAT=txt

# set INVERT_IMAGE to TRUE for white color text
INVERT_IMAGE=FALSE

# ratio between train-data and test-data
TRAIN_RATIO=1

# required library for tesseract to install
TARGET_LIBRARY=all

# tesseract directory that contaon .traineddata file for use
TESSERACT_DATA_DIRECTORY="/usr/share/tesseract-ocr/5/tessdata"
#endregion

#region Define Argument
MODE=$1
shift
FIRST_ARGS=$1
shift;
while getopts ":r:d:p:i:l:t:v:c:e:o:s:" args; do
    case "$args" in
        r)
            LEARNING_RATE=${OPTARG}
            ;;
        d)
            TRAINED_DATA=${OPTARG}
            ;;
        p)
            PSM=${OPTARG}
            ;;
        i)
            LIMIT_ITER=${OPTARG}
            ;;
        l)
            LANG=${OPTARG}
            ;;
        t)
            TRAINED_RATIO=${OPTARG}
            ;;
        v)
            INVERT_IMAGE=${OPTARG}
            ;;
        c)
            READ_COLOUR=${OPTARG}
            ;;
        e)
            TARGET_ERR=${OPTARG}
            ;;
        o)
            OUTPUT_FORMAT=${OPTARG}
            ;;
    esac
done
#endregion



function training() {
    TESS_DIR="/usr/share/tesseract-ocr/5/tessdata"
    TESSDATA_DIR="utils/tessdata"
    MODEL_NAME=$FIRST_ARGS
    TRAINING_DIR=$MODEL_NAME'-ground-truth'
    echo $ITER
    export TESSDATA_PREFIX=${TESSDATA_DIR}
    make training MODEL_NAME=$MODEL_NAME TESSDATA=$TESSDATA_DIR START_MODEL=$LANG LEARNING_RATE=$LEARNING_RATE MAX_ITERATIONS=$LIMIT_ITER PSM=$PSM TARGET_ERROR_RATE=$TARGET_ERR
}

function checkout() {
    MODEL_NAME=$FIRST_ARGS
    TRAINED_DATA_DIR=data/${MODEL_NAME}/train/training_data/${MODEL_NAME}.traineddata
    TESS_DIR=${TESSERACT_DATA_DIRECTORY}
    sudo cp ${TRAINED_DATA_DIR} $TESS_DIR
}

function testing() {
    MODEL_NAME=$FIRST_ARGS
    USING_DATA=${TRAINED_DATA:-$MODEL_NAME}
    DIR="data/${MODEL_NAME}/test/images"
    export TESSDATA_PREFIX="/usr/share/tesseract-ocr/5/tessdata"
    cd $DIR
    for file in $(ls *.tif *.tiff *.jpg *.jpeg *.png);
    do
        tesseract --dpi 300 $file ${file%.*}$TRAINED_DATA "-l" $USING_DATA "--psm" "6" ${OUTPUT_FORMAT}
    done

}

function reading() {
    IMAGE_DIR=$FIRST_ARGS
    echo $IMAGE_DIR
    echo $MODEL_NAME
    ./build/src/DrawingContour/DrawingContour $IMAGE_DIR $LANG $PSM $INVERT_IMAGE
}

function install() {
    echo $MODE
    function pylib_install(){
        sudo apt-get install python3-pip
        pip install -r requirement.txt
    }
    
    function tess_install()
    {
        sudo add-apt-repository ppa:alex-p/tesseract-ocr5
        sudo apt update
        sudo apt install tesseract-ocr
    }

    function lep_install()
    {
        make leptonica-tesseract
    }

    function get_langdata()
    {   
        make tesseract-langdata
        sudo apt-get install tesseract-ocr-${LANG}
    }

    case $FIRST_ARGS in
    
        "")
            tess_install
            lep_install
            get_langdata
            pylib_install
            ;;
        tesseract)
            tess_install
            ;;
        leptonica)
            lep_install
            ;;
        langdata)
            get_langdata
            ;;
        pylib)
            pylib_install
            ;;
        *)
            help
            ;;
    esac
}

function setup_environment() {
    NAME=$FIRST_ARGS
    mkdir build/
    mkdir data/$NAME
    mkdir data/$NAME/all
    mkdir data/$NAME/all/images
    mkdir data/$NAME/all/text
    mkdir data/$NAME/train
    mkdir data/$NAME/train/images
    mkdir data/$NAME/train/training_data
    mkdir data/$NAME/train/training_data/$NAME-ground-truth
    mkdir data/$NAME/train/text
    mkdir data/$NAME/test
    mkdir data/$NAME/test/images
    mkdir data/$NAME/test/text
    mkdir data/$NAME/train/images/cropped
    mkdir data/$NAME/train/images/cropped_gray
    mkdir data/$NAME/train/images/cropped_bw
}

function preprocess() {
    echo $MODE
    NAME=$FIRST_ARGS
    ./build/src/DataPreparing/DataPreparing ${NAME} ${TRAIN_RATIO}
}

function help() {
        echo ""
	    echo "  MODE"
    	echo ""
    	echo "    install           Install required libraries"
    	echo "    add_project       Setup the environment of each project"
        echo "    data_split        Split dataset into train and test"
	    echo "    preprocess        Preparing dataset and export to ground-truth directory"
    	echo "    train             Start training Tesseract-OCR"
    	echo "    export_model      Export the trained data allow Tesseract to use"
    	echo "    test              Test the performance of traineddata by reading test data"
        echo ""
        echo ""
        echo "  Syntax"
    	echo ""
    	echo "    install           ./tess_train.sh install [TARGET_LIBRARY]"
        echo ""
    	echo "    add_project       ./tess_train.sh add_project [PROJECT_NAME]"
        echo ""
        echo "    data_split        ./tess_train.sh [PROJECT_NAME] [TRAIN_RATIO]"
        echo ""
	    echo "    preprocess        ./tess_train.sh preprocess [PROJECT_NAME]"
        echo ""
    	echo "    train             ./tess_train.sh train [PROJECT_NAME] [OPTIONAL_ARGUMENT]"
        echo ""
    	echo "    export_model      ./tess_train.sh checked [PROJECT_NAME] [TESSERACT_DATA_DIRECTORY]"
        echo ""
    	echo "    test              ./tess_train.sh test [PROJECT_NAME] [OPTIONAL_ARGUMENT]"
        echo ""
    	echo "    read              ./tess_train.sh read [IMAGE_DIRECTORY] [TRAINED_DATA]"
        echo ""
        echo ""
    	echo "  Variables"
    	echo ""
    	echo "    PROJECT_NAME       Name of the project, this is the same one that have added in add_project: ${TRAIN_RATIO}"
    	echo "    TRAIN_RATIO        Ratio of dataset that will be splitted to train dirctory. Default: ${TRAIN_RATIO}"
    	echo "    TESSERACT_DATA_DIRECTORY       Data directory tesseract that contained .traineddata files Default: ${TESSERACT_DATA_DIRECTORY}"
        echo "    TRAINED_DATA       .traineddata file tha use for tesseract reading Ex. using "example_123.traineddata" "TRAINED_DATA = example_123""
        echo ""
        echo ""
        echo "  Optional Argument Variables "
        echo ""
    	echo "    TARGET_LIBRARY     Name of the library that is going to be installed Default: ${TARGET_LIBRARY}"
        echo "      key:             -           "
        echo "      option:          "
        echo "      - all            install all library"
        echo "      - tesseract      Install Tesseract-OCR"
        echo "      - leptonica      Install Leptonica"
        echo "      - langdata       Get language alphabets data"
        echo "      - pylib          Install python library"
        echo ""
    	echo "    PSM                Page segmentation mode of tesseract Default: ${PSM}"
        echo "      key:             p           "
        echo "      type:          "
        echo "      - int            1-13"
        echo ""
    	echo "    OUTPUT_FORMAT      Format of tesseract extracted text output. Default: ${OUTPUT_FORMAT}"
        echo "      key:             o           "
        echo "      option:          "
        echo "      - txt            output as text file"
        echo "      - tsv            output as .tsv file"
        echo ""
    	echo "    READ_COLOUR        Type of image to read. Default: ${READ_COLOUR}"
        echo "      key:             c          "
        echo "      option:          "
        echo "      - bw             turn image to binary before read"
        echo "      - gray           turn image to gray-scale before read"
        echo "      - none           do not apply any image processing before read"
        echo ""
    	echo "    INVERT_IMAGE       Name of the model to continue from. Default: '${INVERT_IMAGE}'"
        echo "      key:             v         "
        echo "      option:          "
        echo "      - True           Invert image color for train"
        echo "      - False          Non-inverted image for train"
        echo ""
    	echo "    LEARNING_RATE      Learning rate that use for train. Default: '${LEARNING_RATE}'"
        echo "      key:             r           "
        echo "      type:          "
        echo "      - float          >= 0"
        echo ""
    	echo "    LIMIT_ITER         Max training iteration. Default: ${LIMIT_ITER}"
        echo "      key:             i          "
        echo "      option:          "
        echo "      - int            >= 0"
        echo ""
    	echo "    LANGUAGE           Language that use for train. Default: ${LANG}"
        echo "      key:             l           "
        echo "      type:          "
        echo "      - str            tesseract language key"
}

case "$MODE" in
    install)
        install
        exit;;

    train)
        training
        exit;;

    test)
        testing
        exit;;

    export_model)
        checkout
        exit;;

    preprocess)
        preprocess
        exit;;

    read)
        reading
        exit;;

    help)
        help
        exit;;
    
    add_project)
        setup_environment
        exit;;

    *)
        echo Please input correct MODE keyword
        help 
        exit;;

esac 