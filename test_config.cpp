
#include "cfig.h"

int main()
{

    Cfig config("config.ini", Cfig::EQUAL, Cfig::HASH);

    for (const auto& section : config.sections())
        std::cout << section << std::endl;

    return 0;
}
