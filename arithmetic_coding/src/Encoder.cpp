#include <limits>


#include "../include/Encoder.hpp"


Encoder::Encoder(const std::string& file_path)
{
    this->input_stream = std::ifstream(file_path, std::ios::binary);
    this->input_stream.unsetf(std::ios::skipws);

    this->symbols_cpt = 0;
    this->model = Model(input_stream);
}


Encoder::~Encoder()
{
    input_stream.close();
}


std::vector<bool> Encoder::encode()
{    
    uint32_t low = 0x0;
    uint32_t high = std::numeric_limits<uint32_t>::max() - 1;
    uint32_t middle = (high >> 1) + 1;

    this->input_stream.seekg(0, std::ios::beg); // go at begining of the file
    
    uint8_t symbol;
    uint32_t pending_bits = 0;
    std::vector<bool> output;
    while (this->input_stream >> symbol)
    {
        uint32_t range = high - low + 1;
        const auto& prob_range = model.get_range(symbol);
        low = low + range * prob_range.first;   
        high = low + range * prob_range.second - 1;
        if (high < middle)
        {
            output.push_back(0);
            for (; pending_bits != 0; --pending_bits)
                output.push_back(1);
        }
        else if (low >= middle)
        {
            output.push_back(1);
            for (;pending_bits != 0; --pending_bits)
                output.push_back(0);
        }
        else if (low >= (middle >> 1) and high < (middle | 0b01000000000000000000000000000000))
        {
            ++pending_bits;
            low -= (middle >> 1);
            high -= (middle >> 1);
        }
        else
            break;              
        
        high <<= 1;
        ++high;
        high &= (std::numeric_limits<uint32_t>::max() - 1);
        
        low <<= 1;
        low &= (std::numeric_limits<uint32_t>::max() - 1);

        ++this->symbols_cpt; 
    }
    
    return output;
}


void Encoder::decode(const std::string& file_path, const std::vector<bool>& message)
{
    std::ofstream output_stream(file_path, std::ios::binary);
    if (not output_stream.is_open())
        throw std::invalid_argument(" error, cannot open ouput stream file with path : " + file_path);
    output_stream.unsetf(std::ios::skipws);

    std::cout << "size is gg : " << message.size() << std::endl;
    uint32_t low = 0x0;
    uint32_t high = std::numeric_limits<uint32_t>::max() - 1;

    uint32_t value = 0;
    std::size_t dec_cpt = 0;
    uint32_t message_bits_cpt = 0;
    for ( ; message_bits_cpt < 32 ; ++message_bits_cpt) 
    {
        value <<= 1;
        value += message[message_bits_cpt];
    }

    while (true)
    {
        uint32_t range = high - low + 1;
        auto v = (value - low + 1) / range;
        std::cout << "value : " << value << " | low : " << low << " | high " << high << "\n";
        uint8_t symbol = model.get_symbol((double)(value - low + 1) / range);
        output_stream << symbol;

        if (++dec_cpt == this->symbols_cpt)
            return;

        const auto& prob_range = model.get_range(symbol);
        low = low + range * prob_range.first;   
        high = low + range * prob_range.second - 1;

        while (true)
        {
            if (high < 0x80000000U);
                // do nothing, bit is a zero
            else if (low >= 0x80000000U)
            {
                value -= 0x80000000U;
                low -= 0x80000000U;
                high -= 0x80000000U;
            }
            else if (low >= 0x40000000 && high < 0xC0000000U)
            {
                value -= 0x40000000;
                low -= 0x40000000;
                high -= 0x40000000;
            }
            else
                break;

            low <<= 1;
            high <<= 1;
            high++;
            value <<= 1;
            value += message[message_bits_cpt];
            ++message_bits_cpt;            
        }
    }
}