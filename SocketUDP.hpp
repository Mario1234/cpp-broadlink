#ifndef _SOCKETUDP_H_
#define _SOCKETUDP_H_

#include <string>
#include <vector>
#include "Globales.hpp"

#include <winsock2.h>
//#pragma comment(lib,"ws2_32.lib") //Winsock Library

class SocketUDP {
    public:
        SocketUDP();
        std::string iniciaWinsock();
        std::string creaSocketUDP(std::string dirIpDestino, bool broadcast);
        std::string enviaDatagrama(const char *b_datagrama, int longitud);
        int recibeDatagrama(int timeout);
        std::vector<char> dameDatosRecibidos();
        std::vector<char> dameDirIPpaqueteRecibido();
        int cierraSocket();
    private:
        std::vector<char> ba_dirIPpaqueteRecibido;
        struct sockaddr_in st_infoLectorServidor;//direccion y puerto de nuestro servidor, para escuchar UDP
        struct sockaddr_in st_infoEscritorServidor;//direccion y puerto de nuestro servidor, para enviar
        SOCKET socketServidor;
        WSADATA wsaData;
        char ba_bufferRecepcion[256] = {};
        std::string s_dirIPpaqueteRecibido;
        const int c_banderaTrue = 1;
        int inet_pton(int af, const char *src, void *dst);
        const int INET6_ADDRSTRLEN = 46;
};
#endif
