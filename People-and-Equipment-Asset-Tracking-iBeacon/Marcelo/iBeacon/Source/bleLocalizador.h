#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringSerial.h>
#include <stdlib.h>
#include "distance_rssi.h"
#include <mysql/mysql.h>
#include <time.h>
#define WAITING_TIME 1000
#define SAMPLE 5

double median(int n, double x[]);
int countSubstr(char subStr[10],char strMsg[1000]);
char* getStr(char* buff);
int receiveString(char* msgString);
void  insertDatabase(char* msgString,int qtdDevice);

