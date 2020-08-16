#ifndef NODO_H_INCLUDED
#define NODO_H_INCLUDED

#include <map>

class Nodo
{
public:
    std::string Palabra;
    map<int, Nodo *> Sig;
    Nodo(string P = "")
    {
        Palabra = P;
    }
};

#endif // NODO_H_INCLUDED
