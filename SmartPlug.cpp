#include "SmartPlug.hpp"
#include <cstdlib>

SmartPlug::SmartPlug(){

}

SmartPlug::SmartPlug(std::vector<char> b_dirIp, std::vector<char> b_dirMac){
    ab_direccionIP = b_dirIp;
    ab_direccionMAC = b_dirMac;
    i_numAleatorio = rand() % (Globales::MAXIMO_ALEATORIO+1);
}

std::vector<char> SmartPlug::dameKeyAES(){
    std::vector<char> v_aux(ab_keyAES);
    return v_aux;
}
std::vector<char> SmartPlug::dameDireccionIP() const{
    std::vector<char> v_aux(ab_direccionIP);
    return v_aux;
}
std::vector<char> SmartPlug::dameDireccionMAC() const{
    std::vector<char> v_aux(ab_direccionMAC);
    return v_aux;
}

// Sobrecarga del operador +
SmartPlug& SmartPlug::operator =(const SmartPlug &sp1)
{
  return *(new SmartPlug(sp1.dameDireccionIP(), sp1.dameDireccionMAC()) );
}
