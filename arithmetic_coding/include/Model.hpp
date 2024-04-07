#ifndef __MODEL_HPP_INCLUDED__
#define __MODEL_HPP_INCLUDED__

#include <map>
#include <vector>
#include <fstream>


/*will not work with adaptative coding!!!*/
class Model
{
private:
    std::vector<std::pair<double, double>> freq_table;

public:
    Model(std::ifstream& input);
    
    Model();
    ~Model();

    uint8_t get_symbol(double d);
    const std::pair<double, double>& get_range(uint8_t symbol);
};


#endif // __MODEL_HPP_INCLUDED__