#include "Grid.h"
#include "Node.h"
#include <string>
#include <vector>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <iostream>


void Grid::updateStart(int xs, int ys) {
    // Если уже есть start — удаляем старый объект
    if (start != nullptr) {
        delete start;
    }
    start = new Node(xs, ys);
}

void Grid::updateEnd(int xe, int ye) {
    // Если уже есть end — удаляем старый объект
    if (end != nullptr) {
        delete end;
    }
    end = new Node(xe, ye);
}


void Grid::loadFromBmp(const std::string FileName){ 
  cv::Mat image = cv::imread(FileName, cv::IMREAD_COLOR);

  if (image.empty()){
    std::cout << "Ошибка" << std::endl;
    return;
  }

  bool startFound, endFound = false;

  for (int i =0; i < image.rows; i++){
    for (int j = 0; j < image.cols; j++){
      cv::Vec3b pixel = image.at<cv::Vec3b>(i, j);
      uchar blue = pixel[0];
      uchar green = pixel[1];
      uchar red = pixel[2];
      if (blue < 10 && green < 10 && red > 245){ //Красный
        endFound = true;
        updateEnd(i, j); 
      }
      else if(blue < 10 && green > 245 && red > 245){ //Желтый
        startFound = true;
        updateStart(i, j);
      }
      if(startFound && endFound){
        break;
      }
    }
    if(startFound && endFound){
      break;
    }
  }

  cv::Mat gray_image;
  cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
  cv::Mat binary_image;
  cv::threshold(gray_image, binary_image, 128, 255, cv::THRESH_BINARY);

  this -> width = binary_image.cols;
  this -> height = binary_image.rows;
  
  // Изменение размера вектора препятствий
  this -> obstacle.resize(this -> height);
  for (int i = 0; i < this -> height; i++){
    this -> obstacle[i].resize(this -> width);
  }
  
  // Заполнение вектора препятствий на основе бинаризованного изображения
  for (int i = 0; i < this->height; ++i){
    for(int j = 0; j < this -> width; j++){
        this->obstacle[i][j] = (binary_image.at<uchar>(i, j) == 0);
    }
  }
};

Grid::Grid(const std::string& FileName){
  loadFromBmp(FileName);
}
Grid::~Grid(){
  delete start;
  delete end;
}

bool Grid::isObstacle(int x, int y) const{
  if (obstacle[x][y] == 0)
    return true;
  else
    return false;
}

int Grid::getWidth() const{
  return width;
}

int Grid::getHeight() const{
  return height;
}
