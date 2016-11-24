#include "bleLocalizador.h"

int fd;
int flag1=0,flag2=0;
double distCache1[5];
double distCache2[5];


/*
 *   @desc Esta funcao calcula a mediana de um vetor com tamanho n
 *   @param n: tamanho do vetor; x: vetor x que contem os valores
 *   @return Mediana calculada
 */

double median(int n, double x[]) {
    double temp;
    int i, j;
    // Loops tem a funcao de ordenar de forma crescente o vetor
    for(i=0; i<n-1; i++) {
        for(j=i+1; j<n; j++) {
            if(x[j] < x[i]) {
                // Troca de Posicao
                temp = x[i];
                x[i] = x[j];
                x[j] = temp;
            }
        }
    }

    if(n%2==0) {
        // Para numero par de elementos realizar media entre dois elementos
        return((x[n/2] + x[n/2 - 1]) / 2.0);
    } else {
        // Para numero impar retornar o numero mediano
        return x[n/2];
    }
}

/*
 *  @desc Contar quantos Devices Bluetooths foram encontrados pela String mensagem
 *  @param subStr: SubString para identificar modulos; strMsg: String principal da Mensagem
 *  @return Contagem realizada
 */

int countSubstr(char subStr[10],char strMsg[1000])
{
    int count;
    char *p=strMsg;

    // Contando Devices
    while (p = strstr(p, subStr))
    {
        count++;
        p++;
    }

    return count;   
}

/*
 *  @desc Receber String que device Bluetooth esta enviando pela porta serial
 *  @param buff: Ponteiro de char que recebera a String da mensagem  
 *  @return Retorna a mesma string
 */

char* getStr(char* buff)
{
    int i = 0;
    int j = 0;

    serialFlush(fd);

    // Loop para espera de dados
    while(!serialDataAvail(fd) && i<WAITING_TIME)
    {
        i++;
        delay(10);
        // Tempo limite foi atingido sem dados na porta serial
        if (i> WAITING_TIME)
        {
            printf("Sem dados no serial\n");
            buff = "erro";
            return(buff);
        }
    }
    // Porta Serial possui dados que serao enviados a String
    while(serialDataAvail(fd)>0)
    {
        delay(10);
        buff[j] = serialGetchar(fd);
        j++;
    }
    buff[j] = 0;
    // printf("\n%s\n", buff); //debug

    return(buff);
}

/*
 *  @desc Interface com funcoes de Porta Serial e Bluetooth, preparacao de dados para inserir no Database
 *  @param msgString Passagem de ponteiro para recepcao de dados da porta Serial
 *  @return Contagem de Devices na Mensagem
 */

int receiveString(char* msgString)
{

    if ((fd = serialOpen ("/dev/ttyAMA0", 9600)) < 0)
    {
        fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno)) ;
        return 1 ;
    }
    fflush(stdout);
    serialPuts (fd,"AT+DISI?");
    getStr(msgString);
    delay(10);
    getStr(msgString);
    //printf("\nTamanho total: %d\n",strlen(msgString));
    serialFlush(fd);
    return countSubstr("OK+DISC:",msgString);
}

/*
 *  @desc   Preparacao de dados e Insercao em Database
 *  @param  msgString: String encontrada em porta Serial; qtdDevice: Quantidade de devices encontrado em mensagem
 *  @return Void
 */

void  insertDatabase(char* msgString,int qtdDevice)
{
    int  rssi[qtdDevice], measPower[qtdDevice];
    char macAdr[qtdDevice][15];
    char rssiString[qtdDevice][6];
    char *p; 
    char  measPowerString[qtdDevice][5];
    int cnt;
    double dist[qtdDevice];

    // Estruturacao de mensagem String em variaveis
    for(cnt=0;cnt<qtdDevice;cnt++)
    {
        p=msgString;
        p=p+58+(cnt*78);
        strncpy(measPowerString[cnt],p,2);
        measPowerString[cnt][2]='\0';
        p=p+3;
        strncpy(macAdr[cnt],p,12);
        macAdr[cnt][12]='\0';
        p=p+14;
        strncpy(rssiString[cnt],p,4);
        rssiString[cnt][3]='\0';
    }

    //for(cnt=0;cnt<qtdDevice;cnt++)
    //printf("\nMAC: %s Meas.Power: %s RSSI: %s",macAdr[cnt],measPowerString[cnt],rssiString[cnt]);

    // Conversao de char para Int/Float
    for(cnt=0;cnt<qtdDevice;cnt++)
    {
        measPower[cnt]=strtol(measPowerString[cnt],&p,16);
        rssi[cnt]=atoi(rssiString[cnt])*-1;
        dist[cnt]=getDistance_RSSI(rssi[cnt],(measPower[cnt]-256));
    }

    for(cnt=0;cnt<qtdDevice;cnt++)
        printf("\nMAC: %s Meas.Power: %d RSSI: %d Distancia: %lf",macAdr[cnt],measPower[cnt],rssi[cnt],dist[cnt]);

    printf("\n");

    // Realizacao de cache de informacao para o calculo de mediana
    for(cnt=0;cnt<qtdDevice;cnt++)
        if(strcmp("88C25532CF72",macAdr[cnt])==0)
        {
            distCache1[flag1]=dist[cnt];
            flag1++;
        }
        else if(strcmp("0CF3EE031CB2",macAdr[cnt])==0)
        {
            distCache2[flag2]=dist[cnt];
            flag2++;
        }

    // Quando cache estiver pronto realizar a insercao de dados
    if(flag1==SAMPLE || flag2==SAMPLE)
    {
        MYSQL con;
        mysql_init(&con);

        if(mysql_real_connect(&con,"192.168.0.25","root","admin","LOCBLE",0,NULL,0)) {
            //printf("\nConectado com sucesso!\n");
        } 
        else
        {
            printf("\nConexao falhou!\n");
        }

        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        char datetime[30];
        char test[30];

        // Buscando o tempo do sistema para insercao de dados na Database
        sprintf(datetime,"%d",tm.tm_year+1900);
        strcat(datetime,"-");
        sprintf(test,"%d",(tm.tm_mon+1));
        strcat(datetime,test);
        sprintf(test,"%d",tm.tm_mday);
        strcat(datetime,"-");
        strcat(datetime,test);
        sprintf(test,"%d",tm.tm_hour);
        strcat(datetime," ");
        strcat(datetime,test);
        sprintf(test,"%d",tm.tm_min);
        strcat(datetime,":");
        strcat(datetime,test);
        sprintf(test,"%d",tm.tm_sec);
        strcat(datetime,":");
        strcat(datetime,test);

        char deviceid='1';
        char query[300];

        // Insercao na Database
        if(flag1==SAMPLE)
            sprintf(query,"INSERT INTO LOG VALUES (\"%s\",%c,\"%s\",%f)",datetime,deviceid,"88C25532CF72",median(SAMPLE,distCache1));

        if(flag2==SAMPLE)
            sprintf(query,"INSERT INTO LOG VALUES (\"%s\",%c,\"%s\",%f)",datetime,deviceid,"0CF3EE031CB2",median(SAMPLE,distCache2));

        if(mysql_query(&con,query))
        {
            printf("\nErro! %s\n",mysql_error(&con));
        }

        flag1=0;
        flag2=0;
        mysql_close(&con);
        memset(macAdr,0,qtdDevice);
        printf("\n");
    }
}

