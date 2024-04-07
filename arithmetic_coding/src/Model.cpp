#include <map>
#include <vector>
#include <numeric>
#include <fstream>
#include <iostream>
#include <algorithm>


#include "../include/Model.hpp"


Model::Model()
{
}


Model::Model(std::ifstream& input)
{
    input.seekg(0, std::ios::beg); // go at file begining

    for (int i = 0; i < 256; ++i)
        this->freq_table.emplace_back(std::make_pair<double, double>(.0, .0));

    uint8_t val;
    std::size_t count = 0;
    while (input >> val)
    {
        this->freq_table[val].second += 1;
        ++count;
    }
    
    if (count != 0)
    {
        double prev_ub = 0.;
        for (int i = 0; i < 256; ++i)
        {   
            if (this->freq_table[i].second == .0)
                continue;

            this->freq_table[i].first = prev_ub;
            this->freq_table[i].second = prev_ub + this->freq_table[i].second/count;
            prev_ub = this->freq_table[i].second;
        }
    }
    else
        throw std::invalid_argument(" there is no symbol in source file \n");

    input.clear();
    input.seekg(0, std::ios::beg); // re-placing the cursor at its previous position
}


Model::~Model()
{
}


const std::pair<double, double>& Model::get_range(uint8_t symbol)
{
    return this->freq_table[symbol];
}


uint8_t Model::get_symbol(double d)
{   
    auto it = std::find_if(this->freq_table.begin(), this->freq_table.end(), [&](const std::pair<double, double>& v){ return v.first <= d and d < v.second; });

    if (it == this->freq_table.end())
        throw std::invalid_argument(" cannot find range for decoding, invalid input detected : " + std::to_string(d));
    else
        return std::distance(this->freq_table.begin(), it);
}