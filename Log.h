#ifndef LOG_H
#define LOG_H
#include<iostream>
#include<fstream>
#include <ctime>
#include<string>
#include<cstring>
#include <fstream>
#include "rsatest4.h"
#define INFO "INFO"
#define ERROR "ERROR"
#define WARN "WARN"


// time ---------------------------------------

const std::string currentDateTime(){
        time_t now = time(0);
        struct tm tstruct;
        char buf[100];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
   return buf;
}

class logging{
   private:
      const char* _mes;
      const char* _level;
      std::string _time;
      FILE *_file;
      Crypto crypto;

   public:
      void log_init(char *pub_key, FILE *f);
      void log(const char* mes, const char* level);
   };
using namespace std;

void logging::log_init(char *pub_key,FILE *f){
   crypto.set_key(pub_key);
   _file = f;
}


void logging::log(const char* mes, const char * level){
   
   _time = currentDateTime();
   _mes = mes;
   _level = level;  
 
   unsigned char *emes = crypto.encrypt((char*)_mes);


char _rtime[20];
strcpy(_rtime,_time.c_str());
std::ofstream writeFile("LIST.txt", fstream::out | fstream::app);


if(writeFile.is_open()){
   writeFile.write(_level,strlen(_level));
   writeFile.write(",",1);
   writeFile.write(_rtime,strlen(_rtime));
   writeFile.write(",",1);   
   writeFile.write((char *)emes,1024);
   writeFile.write("end_log\n",8);
}
writeFile.close();



}

#endif
