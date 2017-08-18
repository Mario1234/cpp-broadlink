#include "SocketUDP.hpp"

#include <stdio.h>
#include <unistd.h>//usleep
#include <sys/time.h>//timeval para el timeout


SocketUDP::SocketUDP(){

}

std::string SocketUDP::iniciaWinsock(){
    int i_res = WSAStartup(MAKEWORD(2,2), &wsaData);
    if(i_res<0){
        return "error al inicializar Winsock"+Globales::itoa(i_res,3);
    }
    return "inicializado Winsock con exito";
}

std::string SocketUDP::creaSocketUDP(std::string dirIpDestino, bool broadcast){
    //crea socket
    socketServidor = INVALID_SOCKET;

    //crea estructuras
    memset(&st_infoLectorServidor, 0, sizeof(st_infoLectorServidor));
    memset(&st_infoEscritorServidor, 0, sizeof(st_infoEscritorServidor));
    st_infoLectorServidor.sin_family = AF_INET;
    st_infoEscritorServidor.sin_family=AF_INET;

    //st_infoLectorServidor.ai_flags = AI_PASSIVE;//seguro???

    //Create a socket
    int i_res=0;
    if((socketServidor = socket(AF_INET , SOCK_DGRAM , 0 )) == INVALID_SOCKET)
    {
        std::string error = Globales::itoa(WSAGetLastError(),6);
        return "no se ha podido crear el socket, error: "+ error;
    }
    st_infoLectorServidor.sin_port=htons( Globales::iG_puertoServidorLocal );
    st_infoLectorServidor.sin_addr.s_addr=INADDR_ANY;////lee UDP de cualquier dir IP
    //st_lectorServidor.sin_addr.s_addr=htonl(INADDR_ANY);//lee UDP de cualquier dir IP

    inet_pton(AF_INET ,dirIpDestino.c_str(), &st_infoEscritorServidor.sin_addr);//envia UDP solo a la dir IP destino

    i_res = setsockopt(socketServidor, SOL_SOCKET, SO_REUSEADDR, (const char*)&c_banderaTrue, sizeof(c_banderaTrue));
    if (i_res < 0){
        closesocket(socketServidor);
        WSACleanup();
        return "error setsockopt reusar dir"+ Globales::itoa(i_res,3);
    }

    if(broadcast){
        i_res = setsockopt(socketServidor, SOL_SOCKET, SO_BROADCAST, (const char*)&c_banderaTrue, sizeof c_banderaTrue);
        if (i_res < 0){
            closesocket(socketServidor);
            WSACleanup();
            return "error setsockopt broadcast"+ Globales::itoa(i_res,3);
        }
    }

    i_res = bind( socketServidor, (struct sockaddr*)&st_infoLectorServidor, sizeof(st_infoLectorServidor));
    if (i_res == SOCKET_ERROR) {
//        freeaddrinfo(st_infoLectorServidor);
        std::string error = Globales::itoa(WSAGetLastError(),6);//10047 es direccion no permitida para protocolo asignado
        closesocket(socketServidor);
        WSACleanup();
        return "error al hacer bind: "+ error;
    }

    struct sockaddr_in st_infoAux;
    int lonDir = sizeof(st_infoAux);
    i_res = getsockname(socketServidor, (struct sockaddr *)&st_infoAux, &lonDir);
    if(i_res != 0){
        return "error al recoger el peurto asignado por bind: "+Globales::itoa(i_res,3);
    }

    Globales::iG_puertoServidorLocal=ntohs(st_infoAux.sin_port);
    st_infoEscritorServidor.sin_port=htons(Globales::PUERTO_DISPOSITIVO);


    return "creado socket con exito";
}

std::string SocketUDP::enviaDatagrama(const char *b_datagrama, int longitud){
    int i_res;

    i_res = sendto(socketServidor, b_datagrama, longitud, 0, (struct sockaddr*) &st_infoEscritorServidor, sizeof st_infoEscritorServidor);
    if (i_res == SOCKET_ERROR){
        std::string error = Globales::itoa(WSAGetLastError(),6);
        return "error al enviar UDP"+error;
    }
    usleep(100000);
    return "enviado datagrama con exito";
}

int SocketUDP::recibeDatagrama(int timeout){
//    freeaddrinfo(st_infoLectorServidor);

    //establece timeout, limite de espera
    DWORD dw_timeout = timeout;
    if (setsockopt(socketServidor, SOL_SOCKET, SO_RCVTIMEO,(const char *)&dw_timeout,sizeof(struct timeval)) < 0) {
         return -1;
    }

    struct sockaddr_in infoConexionDispositivo;
    int len = sizeof(infoConexionDispositivo);
    int i_longitudRecibido = recvfrom(socketServidor, ba_bufferRecepcion, sizeof(ba_bufferRecepcion), 0,
                                  (struct sockaddr *) &infoConexionDispositivo, &len);
    if(i_longitudRecibido == SOCKET_ERROR){
        return -3;
    }
    //inet_ntop(AF_INET, &(infoConexionDispositivo.sin_addr.s_addr), s_dirIPpaqueteRecibido, INET_ADDRSTRLEN);
    s_dirIPpaqueteRecibido=inet_ntoa(infoConexionDispositivo.sin_addr);
    //ntohs(si_other.sin_port);
    ba_dirIPpaqueteRecibido = Globales::dameDireccionIPdeString(s_dirIPpaqueteRecibido);

    closesocket(socketServidor);
    WSACleanup();
    return i_longitudRecibido;
}

std::vector<char> SocketUDP::dameDatosRecibidos(){
    std::vector<char> v_aux(&ba_bufferRecepcion[0], &ba_bufferRecepcion[0] + 256);
    return v_aux;
}

int SocketUDP::cierraSocket(){
    return close(socketServidor);
}

std::vector<char> SocketUDP::dameDirIPpaqueteRecibido(){
    return ba_dirIPpaqueteRecibido;
}

int SocketUDP::inet_pton(int af, const char *src, void *dst)
{
  struct sockaddr_storage ss;
  int size = sizeof(ss);
  char src_copy[INET6_ADDRSTRLEN+1];

  ZeroMemory(&ss, sizeof(ss));
  /* stupid non-const API */
  strncpy (src_copy, src, INET6_ADDRSTRLEN+1);
  src_copy[INET6_ADDRSTRLEN] = 0;

  if (WSAStringToAddress(src_copy, af, NULL, (struct sockaddr *)&ss, &size) == 0) {
    switch(af) {
      case AF_INET:
    *(struct in_addr *)dst = ((struct sockaddr_in *)&ss)->sin_addr;
    return 1;
//      case AF_INET6:
//    *(struct in6_addr *)dst = ((struct sockaddr_in6 *)&ss)->sin6_addr;
    return 1;
    }
  }
  return 0;
}


