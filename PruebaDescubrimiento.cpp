#include "SocketUDP.hpp"
#include "Descubrimiento.hpp"
#include <stdio.h>
#include <iostream> //salida por consola

int main(int argc, char** argv)
{
    SocketUDP socketU = SocketUDP();
    Descubrimiento des = Descubrimiento();

    socketU.iniciaWinsock();
    std::string res = des.descubreDispositivos(socketU);
//    socketU.delete();
    std::cout << res << std::endl;
    res = des.listaDatosDispositivos();
    std::cout << res << std::endl;
    socketU.cierraSocket();
}
