#include <improc_drawer_config.hpp>

#include <iostream>

int main()
{
    std::cout   << "improc_drawer Version: " 
                << IMPROC_DRAWER_VERSION_MAJOR 
                << "."
                << IMPROC_DRAWER_VERSION_MINOR
                << "."
                << IMPROC_DRAWER_VERSION_PATCH
                << std::endl;
    return 0;
}