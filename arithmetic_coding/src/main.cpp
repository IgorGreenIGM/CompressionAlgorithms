#include <random>
#include <iostream>

#include "../include/Encoder.hpp"

using namespace std::literals;

int main(int argc, char* argv[])
{
    if (argc == 2)
    {
        Encoder encoder(argv[1]);
        std::vector<bool> result(encoder.encode());

        encoder.decode("./compressed-"s + argv[1], result);
    }
    else
    {
        std::default_random_engine rnd {};

        std::ofstream out("out.bin", std::ios::binary);
        out.unsetf(std::ios::skipws);
        
        std::random_device device;
        std::mt19937 rng(device());
        std::uniform_int_distribution<std::mt19937::result_type> dist(1, 255);
        
        for (int i = 0; i < 2<<15; ++i)
            out << dist(rng);
    }

    // double integral;
    // double fractional = std::modf(0.14, &integral);
    // std::cout << fractional << std::endl;
    return 0;
}