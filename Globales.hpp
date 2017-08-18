#ifndef _GLOBALES_H_
#define _GLOBALES_H_

#include <string>
#include <vector>

class Globales{
    public:
        Globales();
        static std::string dameDireccionIPlocal();
        static std::string dameStringDeArrayNumeros(std::vector<char> ab_arr);
        static std::string representacionHexadecimalStringDeArrayByte(std::vector<char> ab_arr);
        static std::vector<char> dameDireccionIPdeString(std::string s_dirIp);
        static std::string itoa(int i, int maximoNumCaracteres);
        static constexpr char ivAES[16] = {0x56, 0x2e, 0x17, (char)0x99, 0x6d, 0x09, 0x3d, 0x28, (char)0xdd, (char)0xb3, (char)0xba, 0x69, 0x5a, 0x2e, 0x6f, 0x58};
        static constexpr int MAXIMO_ALEATORIO = 0xffff;
        static constexpr int PUERTO_DISPOSITIVO = 80;
        static std::string sG_direccionIPlocal;
        static std::vector<std::string> saG_direccionIPlocal;
        static std::vector<char> baG_direccionIPlocal;
        static int iG_puertoServidorLocal;
    private:
        static std::vector<std::string> damePartesDireccionIPdeString(std::string s_dirIp);
        static std::string representacionHexadecimalStringDeByte(char b);
        static std::string PREFIJO_LAN;
        static std::string NOMBRE_ADAPTADOR_WIFI;

};

#endif
