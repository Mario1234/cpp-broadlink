#ifndef _DESCUBRIMIENTO_H_
#define _DESCUBRIMIENTO_H_

#include <vector>
#include "SmartPlug.hpp"
#include "SocketUDP.hpp"
#include "Globales.hpp"

class Descubrimiento {
    public:
        Descubrimiento();
        std::string descubreDispositivos(SocketUDP socketU);
        std::string listaDatosDispositivos();
    private:
        SmartPlug listaDispositivos[10];
        int i_numDisp=0;
        bool esBroadLink(std::vector<char> mac);
        void imprimeMensaje(std::vector<char> mens);
};
#endif
