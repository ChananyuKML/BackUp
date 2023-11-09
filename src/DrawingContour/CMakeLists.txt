cmake_minimum_required(VERSION 3.16.3)
project(DrawingContour)
add_executable(${PROJECT_NAME} drawing_contour.cpp)

#---------------------------------------------------------------------------------------
find_package(OpenCV REQUIRED)
if(OpenCV_FOUND)
            message("Found OpenCV module at : " ${OpenCV_INCLUDE_DIRS})
            #message("Linking : "${OpenCV_LIBS})
            list(APPEND EXTRA_LIBS ${OpenCV_LIBRARIES})
            list(APPEND EXTRA_INCLUDES ${OpenCV_INCLUDE_DIRS})
else()
            message("OpenCV package not found")
endif()

#------------------------------------------------------------------------------------

find_package(Tesseract REQUIRED)
if(Tesseract_FOUND)
            message("Found Tesseract module at : " ${Tesseract_INCLUDE_DIRS})
            #message("Linking : "${Tesseract_LIBS})
            list(APPEND EXTRA_LIBS ${Tesseract_LIBRARIES})
            list(APPEND EXTRA_INCLUDES ${Tesseract_INCLUDE_DIRS})
else()
            message("Tesseract package not found")
endif()

#------------------------------------------------------------------------------------

find_package(Leptonica REQUIRED)
if(Leptonica_FOUND)
            message("Found Leptonica module at : " ${Leptonica_INCLUDE_DIRS})
            #message("Linking : " ${Leotonica_LIBS})
            list(APPEND EXTRA_LIBS ${Leptonica_LIBRARIES})
            list(APPEND EXTRA_INCLUDES ${Leptonica_INCLUDE_DIRS})
else()
            message("Leptonica package not found")
endif()
#---------------------------------------------------------------------------------------
message(${EXTRA_INCLUDES})
target_include_directories(${PROJECT_NAME}
            PUBLIC ${PROJECT_BINARY_DIR}
            PUBLIC ${EXTRA_INCLUDES}
            PUBLIC ${PROJECT_BINARY_DIR}
)
message(${PROJECT_BINARY_DIR})

target_link_directories(${PROJECT_NAME}
            PRIVATE ${PROJECT_BINARY_DIR}/include/unitext/install
            )

target_link_libraries(${PROJECT_NAME} 
            cvunitext
            ${EXTRA_LIBS}
)