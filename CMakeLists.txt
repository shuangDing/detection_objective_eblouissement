#CMake La version exigée
cmake_minimum_required (VERSION 2.8)

#Information du projet
project (detection_angle_cmake)

#Parcourir tous les fichier du repertoire actuel
#aux_source_directory(. DIR_SRCS)

#L'ajout de header de OpenCV
find_package(OpenCV REQUIRED)

#L'objet engendré
add_executable(detection_contours decomposition_canal.cpp YCBCr.cpp haar_cascade.cpp)

#Linker librairie de OpenCV sur le programme principal
target_link_libraries(detection_contours ${OpenCV_LIBS})
