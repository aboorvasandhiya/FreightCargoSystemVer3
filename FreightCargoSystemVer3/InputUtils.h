#pragma once 

#include <iostream> 
#include <string>   
#include <limits>  
#include <regex>    

int getValidChoice(int min, int max);
bool isValidTime(const std::string& time);
bool isValidGroupSize(int groupSize);

