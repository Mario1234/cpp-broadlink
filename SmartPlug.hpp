#ifndef _SMARTPLUG_H_
#define _SMARTPLUG_H_

#include <vector>
#include "Globales.hpp"

class SmartPlug {
    public:
        SmartPlug();
        SmartPlug(std::vector<char> b_dirIp, std::vector<char> b_dirMac);
        std::vector<char> dameKeyAES();
        std::vector<char> dameDireccionIP() const;
        std::vector<char> dameDireccionMAC() const;
        SmartPlug& operator =(const SmartPlug &sp1);
    private:
        std::vector<char> ab_keyAES = {0x03, (char)0xc1, 0x69, 0x3d, 0x77, (char)0x93, 0x1d, (char)0x99, 0x2b, 0x45, (char)0xc7, (char)0xb7, (char)0xb1, (char)0xc3, 0x0b, 0x6d};
        std::vector<char> ab_direccionIP;//de este dispositivo
        std::vector<char> ab_direccionMAC;
        int i_numAleatorio;
        std::vector<char> ab_id;//id inicial es 0,0,0,0
};
#endif
