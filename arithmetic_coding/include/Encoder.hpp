#ifndef __ENCODER_H_INCLUDED__
#define __ENCODER_H_INCLUDED__

#include <fstream>
#include <iostream>

#include "Model.hpp"

class Encoder
{
private:
    Model model;
    std::size_t symbols_cpt;
    std::ifstream input_stream;

public:
    ~Encoder();
    Encoder(const std::string& file_path);

    std::vector<bool> encode();
    void decode(const std::string& file_path, const std::vector<bool>& message);
};


#endif // __ENCODER_H_INCLUDED__