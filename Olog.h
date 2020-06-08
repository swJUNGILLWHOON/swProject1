#ifndef OLOG_H
#define OLOG_H
#include<iostream>
#include<fstream>
#include <ctime>
#include<string>
#include<cstring>
#include <fstream>
#include <string>
#include "SingleList2.h"
#include "rsatest4.h"
#define INFO "INFO"
#define ERROR "ERROR"
#define WARN "WARN"

class Ologging{
   private : 
      std::string _mes;
      std::string _level;
      std::string _time;
      char *_file;
      SingleLinked_List list;
      char *__level;
      
   
   public :
      Crypto crypto;
      void Olog_init(char *pri_key, char *file, char *level);
      void Olog();
      void ViewLog(const char *level);


};
using namespace std;
void Ologging::Olog_init(char *pri_key, char *file, char *level){
      crypto.set_key("public_key.txt");
   crypto.pset_key("encrypt_private_key.txt");
   _file = file;
   __level = level;
}


void Ologging::Olog(){
   ifstream logfile(_file);

   std::size_t pos=0, pos2=0;
   std::string str2="";
   cout << "open file"<<endl;   
   if(logfile.is_open()){
      logfile.seekg(0,std::ios::end);
      int size = logfile.tellg();
      str2.resize(size);
      logfile.seekg(0,std::ios::beg);
      logfile.read(&str2[0],size);
      
      logfile >> str2;
      logfile.close();
   }
logfile.open(_file);
   unsigned char _encrypt[1024] = {};
   if( logfile )
        {
                
                logfile.read( (char*)( &_encrypt[0] ), 1024 ) ;
                size_t counted = logfile.gcount();
                cout << counted << endl;
        }
   
unsigned char *emes = crypto.encrypt((char*)"abc"); 



printf("decrypt start============================\n\n\n\n\n");
    
   while(1){
      if(pos >= str2.size()){
                        break;
                }
      pos2 =str2.find(",",pos);
      _level = str2.substr(pos,pos2-pos);

 
      pos = pos2+1;
      pos2 = str2.find(",",pos);
      _time = str2.substr(pos,pos2-pos);
      pos = pos2+1;
      pos2 = str2.find("end_log\n",pos);
      _mes = str2.substr(pos,pos2-pos);
      pos = str2.find("\n",pos2)+1;


      if(_level == __level){
      cout<< "level : " <<_level <<endl<<"time : " <<_time <<endl<<"msg : "<<_mes<<endl<<endl;

      cout<<"ONE decrypt finished!!! pos : "<<pos<<" total length : "<<str2.size()<<endl;
      unsigned char *dmes = crypto.decrypt((unsigned char*)_mes.c_str());
 
      list.AddNode((const char*)dmes, _time, _level.c_str());
      }
                
      }
   }


void  Ologging::ViewLog(const char *level){
   list.PrintList(level);
   
}


#endif
