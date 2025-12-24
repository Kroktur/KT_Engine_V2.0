#pragma once
#include <random>
#include <vector>
#include "PrimaryTypeTrait.h"
//true random generation
//need improvement with seed management
namespace KT {
    class Random
    {
    public:
        //floating types random
        template<typename type> requires is_floating_type_v<type>
        type getRandomNumber(type min, type max)
        {
            std::uniform_real_distribution<type> dis(min, max);
            return  static_cast<type>(dis(m_gen));
        }
        // integral types random
        template<typename type> requires is_integral_type_v<type>
        type getRandomNumber(type min, type max)
        {
            std::uniform_int_distribution<type> dis(min, max);
            return static_cast<type>(dis(m_gen));
        }
        // range number for all types 
        template<typename type>
        std::vector<type> getRandomNumberRange(type min, type max, const size_t& number)
        {
            std::vector<type> result;
            for (std::make_signed_t<size_t> i = 0; i < number; ++i)
            {
                result.push_back(getRandomNumber<type>(min, max));
            }
            return result;
        }
        Random() : m_gen((m_rd()))
        {
        }
    private:

        std::random_device m_rd;
        std::mt19937 m_gen;
    };
}
