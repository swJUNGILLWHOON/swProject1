#include <ctime>
#include <iostream>
#include <cstring>
#include "Olog.h"

int main(){
        Ologging log_;
        std::cout << "select level : (INFO), (WARN), (ERROR)"<<std::endl;
        char level[10];
        std::cin >> level;
        char *filename="LIST.txt";
        log_.Olog_init("encrypt_private_key.txt", filename, level);
        log_.Olog();
//      log_.ViewLog(level);
        return 0;

}
