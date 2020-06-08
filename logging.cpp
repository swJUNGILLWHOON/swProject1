#include <ctime>
#include<iostream>
#include<string>
#include "Log.h"
#include<fstream>
#include<cstdio>
using namespace std;
int main(){

        logging log_;
        FILE *f= fopen("LIST.txt","a");
        log_.log_init("public_key.txt", f);
        log_.log("9line, info log",INFO);
        log_.log("10line, warning log", WARN);
        log_.log("11line, error log", ERROR);
        log_.log("12line, error log", ERROR);
        fclose(f);
        return 0;

}
