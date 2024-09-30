#ifndef ERROR_SQUARECELL_H_INCLUDED
#define ERROR_SQUARECELL_H_INCLUDED
  
#include <string>

namespace error_squarecell
{
    // violation du domaine autorise d'un indice [0, g_max-1]
    std::string print_index(unsigned index, unsigned max);

    // violation du domaine couvert [0, g_max-1]
    // par la combinaison de l'origine et du cote d'un carre
    std::string print_outside(unsigned index, unsigned side, unsigned max);
}




#endif // ERROR_SQUARECELL_H_INCLUDED
 
