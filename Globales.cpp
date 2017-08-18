#define WINVER 0x0600
#include <windows.h>
#include <stdio.h>
#include <stdlib.h> // atoi

#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include "Globales.hpp"

#define DEFAULT_ADAPTER_ADDRESSES_SIZE 15360

//#pragma comment(lib, "ws2_32.lib")
//#pragma comment(lib, "iphlpapi.lib")

std::string Globales::sG_direccionIPlocal = "192.168.43.111";
std::vector<std::string> Globales::saG_direccionIPlocal = {"192","168","43","111"};
std::vector<char> Globales::baG_direccionIPlocal = {(char)192,(char)168,43,111};//por defecto para pruebas
int Globales::iG_puertoServidorLocal = 0;
std::string Globales::PREFIJO_LAN = "192.168.";
std::string Globales::NOMBRE_ADAPTADOR_WIFI = "Wi-Fi";

Globales::Globales(){

}

std::vector<std::string> Globales::damePartesDireccionIPdeString(std::string s_dirIp){
    std::vector<std::string> sa_direccionIP(4);
    int i_posicionPunto=0;
    for(int i=0;i<4;i++){
        i_posicionPunto=s_dirIp.find(".");
        sa_direccionIP[i] = s_dirIp.substr(0, i_posicionPunto);
        s_dirIp=s_dirIp.substr(i_posicionPunto+1,(s_dirIp.length()-1));
    }
    return sa_direccionIP;
}

std::vector<char> Globales::dameDireccionIPdeString(std::string s_dirIp){
    std::vector<char> ba_direccionIP(4);
    std::vector<std::string> sa_direccionIP = damePartesDireccionIPdeString(s_dirIp);
    for(int i=0;i<4;i++){
        char ca_aux[3];
        strncpy(ca_aux, sa_direccionIP[i].c_str(), sizeof(ca_aux));
        ba_direccionIP[i]=atoi(ca_aux);
    }
    return ba_direccionIP;
}

std::string Globales::dameDireccionIPlocal(){
    DWORD doblepalabra = DEFAULT_ADAPTER_ADDRESSES_SIZE;
    PIP_ADAPTER_ADDRESSES direccionesAdaptador, aa;
    PIP_ADAPTER_UNICAST_ADDRESS direccionIP;
    int i_res = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &doblepalabra);
    if(i_res!= ERROR_BUFFER_OVERFLOW){
        return "error recogiendo direcciones de adaptador, desbordamiento";
    }
    direccionesAdaptador = (PIP_ADAPTER_ADDRESSES)malloc(DEFAULT_ADAPTER_ADDRESSES_SIZE);
    if (direccionesAdaptador == NULL){
        return "reserva de memoria fallida";
    }
    i_res = GetAdaptersAddresses(AF_INET, GAA_FLAG_INCLUDE_PREFIX, NULL, direccionesAdaptador, &doblepalabra);
    if(i_res!= ERROR_SUCCESS){
        free(direccionesAdaptador);
        return "error recogiendo direcciones de adaptador";
    }

    for (aa = direccionesAdaptador; aa != NULL; aa = aa->Next) {
        char ca_nombreAdaptador[BUFSIZ];
        memset(ca_nombreAdaptador, 0, BUFSIZ);
        WideCharToMultiByte(CP_ACP, 0, aa->FriendlyName, wcslen(aa->FriendlyName), ca_nombreAdaptador, BUFSIZ, NULL, NULL);
        std::string s_nombreAdaptador(ca_nombreAdaptador);
        std::size_t si_posicionEncontrada = s_nombreAdaptador.find(Globales::NOMBRE_ADAPTADOR_WIFI);//busca solo el adaptador de WiFi
        if(si_posicionEncontrada!=std::string::npos){
            for (direccionIP = aa->FirstUnicastAddress; direccionIP != NULL; direccionIP = direccionIP->Next) {
                char ca_direccionIP[BUFSIZ];
                memset(ca_direccionIP, 0, BUFSIZ);
                i_res = getnameinfo(direccionIP->Address.lpSockaddr,
                                        direccionIP->Address.iSockaddrLength,
                                        ca_direccionIP,
                                        sizeof(ca_direccionIP),
                                        NULL, 0,NI_NUMERICHOST);
                std::string s_direccionIP(ca_direccionIP, BUFSIZ);
                si_posicionEncontrada = s_direccionIP.find(Globales::PREFIJO_LAN);
                if(si_posicionEncontrada!=std::string::npos){
                    Globales::sG_direccionIPlocal = s_direccionIP;
                    Globales::saG_direccionIPlocal = Globales::damePartesDireccionIPdeString(Globales::sG_direccionIPlocal);
                    Globales::baG_direccionIPlocal = Globales::dameDireccionIPdeString(Globales::sG_direccionIPlocal);
                }
            }
        }
	}


    free(direccionesAdaptador);

    return sG_direccionIPlocal;
}

std::string Globales::dameStringDeArrayNumeros(std::vector<char> ab_arr){
    std::string res="";
    for(auto num: ab_arr){
        res+=Globales::itoa(num,2)+".";
    }
    return res;
}

std::string Globales::representacionHexadecimalStringDeByte(char b) {
    // Returns hex String representation of byte b
    const char hexDigit[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    char b1 = hexDigit[(b >> 4) & 0x0f];
    char b2 = hexDigit[b & 0x0f];
    char ac_repre[2]={b1, b2};
    std::string s_repre(ac_repre,2);
    return s_repre;
}

std::string Globales::representacionHexadecimalStringDeArrayByte(std::vector<char> ab_arr){
    std::string res("");
    for(auto b: ab_arr){
        res+=representacionHexadecimalStringDeByte(b)+":";
    }
    return res;
}

std::string Globales::itoa(int i, int maximoNumCaracteres){
    char ca_aux[maximoNumCaracteres];
    snprintf(ca_aux, sizeof(ca_aux), "%d", i);
    std::string s_aux(ca_aux, maximoNumCaracteres);
    return s_aux;
}
