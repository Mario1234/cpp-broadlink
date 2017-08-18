#include <string>
#include "Descubrimiento.hpp"
#include <time.h>//para saber la hora y fecha de hoy
#include <iostream> //salida por consola

Descubrimiento::Descubrimiento(){

}

std::string Descubrimiento::descubreDispositivos(SocketUDP socketU){
    Globales::dameDireccionIPlocal();
    imprimeMensaje(Globales::baG_direccionIPlocal);
    if(Globales::sG_direccionIPlocal.compare("IP local no encontrada") == 0){
			return Globales::sG_direccionIPlocal;
    }
    //asociamos un puerto al azar al socket de los datagramas
    std::string prefiAux = Globales::saG_direccionIPlocal[0]+"."+Globales::saG_direccionIPlocal[1]+"."+Globales::saG_direccionIPlocal[2];
    //std::string s_res = socketU.creaSocketUDP(prefiAux+".255", true);
    std::string s_res = socketU.creaSocketUDP("255.255.255.255", true);
    if(s_res.compare("creado socket con exito")!=0){
        return s_res;
    }

	std::vector<char> ba_mensaje(48);
	for(uint8_t i=0;i<ba_mensaje.size();i++){
        ba_mensaje[i]=0x00;
    }
	time_t t_hoy;
	struct tm * infoHoy;
	TIME_ZONE_INFORMATION TimeZoneInfo;
	time(&t_hoy);
    infoHoy=localtime ( &t_hoy );

    GetTimeZoneInformation( &TimeZoneInfo );
    //TimeZone tz = TimeZone.getTimeZone("Europe/Spain");
    //long desplazamiento = tz.getOffset(hoy.getTime());
    int i_zonaHoraria = -( float(TimeZoneInfo.Bias) / 60 );
    if(i_zonaHoraria<0){
        ba_mensaje[8] = (0x0ff + i_zonaHoraria - 1)&0x0ff;
        ba_mensaje[9] =  0x0ff;
        ba_mensaje[10] = 0x0ff;
        ba_mensaje[11] = 0x0ff;
    }
    else{
        ba_mensaje[8] = i_zonaHoraria & 0x0ff;
        ba_mensaje[9] = 0x00;
        ba_mensaje[10] = 0x00;
        ba_mensaje[11] = 0x00;
    }
    int i_anio = infoHoy->tm_year + 1900;
    ba_mensaje[12] = i_anio & 0x0ff;
    ba_mensaje[13] = (i_anio >> 8)&0x0ff;
    ba_mensaje[14] = (infoHoy->tm_min)&0x0ff;
    ba_mensaje[15] = (infoHoy->tm_hour)&0x0ff;
    int anio2Dig = i_anio%100;
    ba_mensaje[16] = anio2Dig &0x0ff;
    ba_mensaje[17] =  infoHoy->tm_wday &0x0ff;
    ba_mensaje[18] =  infoHoy->tm_mday &0x0ff;
    ba_mensaje[19] =  (infoHoy->tm_mon + 1)&0x0ff;
    ba_mensaje[24] =  Globales::baG_direccionIPlocal[0] &0x0ff;
    ba_mensaje[25] =  Globales::baG_direccionIPlocal[1] &0x0ff;
    ba_mensaje[26] =  Globales::baG_direccionIPlocal[2] &0x0ff;
    ba_mensaje[27] =  Globales::baG_direccionIPlocal[3] &0x0ff;
    ba_mensaje[28] = Globales::iG_puertoServidorLocal & 0x0ff;
    ba_mensaje[29] = (Globales::iG_puertoServidorLocal >> 8)&0x0ff;
    ba_mensaje[38] = 6;
    uint16_t checksum = 0xbeaf;
    for(uint8_t i=0;i<ba_mensaje.size();i++){
        checksum = checksum + (ba_mensaje[i] & 0x0ff);
    }
    checksum &= 0xffff;
    ba_mensaje[32] = checksum & 0x0ff;
    ba_mensaje[33] = (checksum >> 8)& 0x0ff;

    imprimeMensaje(ba_mensaje);


    s_res = socketU.enviaDatagrama((char*)&ba_mensaje[0], 48);
    if(s_res.compare("enviado datagrama con exito")!=0){
        return s_res;
    }
    int i_res = socketU.recibeDatagrama(10000);//10s
    if(i_res>0){
            std::vector<char> ba_datosRecibidos = socketU.dameDatosRecibidos();
            std::vector<char> ba_dirIPpaqueteRecibido = socketU.dameDirIPpaqueteRecibido();
            int i_longDatos = ba_datosRecibidos.size();
            if(i_longDatos>63){
                std::vector<char> direccionMac(6);
                direccionMac[0]=ba_datosRecibidos[58];
                direccionMac[1]=ba_datosRecibidos[59];
                direccionMac[2]=ba_datosRecibidos[60];
                direccionMac[3]=ba_datosRecibidos[61];
                direccionMac[4]=ba_datosRecibidos[62];
                direccionMac[5]=ba_datosRecibidos[63];
                if(esBroadLink(direccionMac)){
                    SmartPlug disp = SmartPlug(ba_dirIPpaqueteRecibido,direccionMac);
                    listaDispositivos[i_numDisp]=disp;
                    i_numDisp++;
                }
            }
    }
    else{
        return "fallo al recibir"+Globales::itoa(i_res,4);
    }
    return "recibida respuesta a paquete hello";
}

bool Descubrimiento::esBroadLink(std::vector<char> mac){
    return (mac[0]==0xB4 && mac[1]==0x43 && mac[2]==0x0D);
}

std::string Descubrimiento::listaDatosDispositivos(){
    std::string s_res;
    for(int i=0;i<i_numDisp;i++){
        SmartPlug disp = listaDispositivos[i];
        s_res="Disp "+Globales::itoa(i,2)+":\n";
        std::vector<char> ba_ip = disp.dameDireccionIP();
        std::vector<char> ba_mac = disp.dameDireccionMAC();

        s_res=+"IP: "+Globales::dameStringDeArrayNumeros(ba_ip)+"\n";
        s_res=+"MAC: "+Globales::representacionHexadecimalStringDeArrayByte(ba_mac)+"\n";
    }
    return s_res;
}

void Descubrimiento::imprimeMensaje(std::vector<char> ba_mens){
    std::string salida = Globales::representacionHexadecimalStringDeArrayByte(ba_mens);
    std::cout << salida << std::endl;
}
