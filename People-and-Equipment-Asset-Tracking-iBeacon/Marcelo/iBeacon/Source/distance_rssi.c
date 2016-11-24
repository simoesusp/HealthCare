#include "distance_rssi.h"
#include <math.h>

/*
 *  @desc   Estima a distancia entre Bluetooth e Scan Bluetooth por regressao exponencial
 *  @param  rssi: Valor de RSSI em dB; measPower: Valor de Transmissao de Energia(dB) usado para calculo de Distancia
 *  @return Valor Estimado
 */

double getDistance_RSSI(double rssi, double measPower) {

    double ratio = rssi*1.0/measPower;
    if (ratio < 1.0) {
        return pow(ratio,10);
    }
    else {
        double accuracy =  (0.89976)*pow(ratio,7.7095) + 0.111;    
        return accuracy;
    }
}
