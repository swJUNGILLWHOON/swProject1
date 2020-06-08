#ifndef RSATEST4
#define RSATEST4
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>

//읽어 들이는 바이트 수

#define KEY_LENGTH 2048

#define PUB_EXP 3

using namespace std;

class Crypto {
        public:
                int a;
                char *_pri_key;
                char *_pub_key;
                size_t _pri_len;
                size_t _pub_len;
                int _encrypt_len;
                //unsigned char* _ciphertxt;
      unsigned char _ciphertxt[4096] = {};
      unsigned char _decrypt[4096] = {};                
      char* _err;
                char* _msg;
                void set_key(char* _keyfile1);
                void pset_key(char* _keyfile2);
                int generateKey();
                unsigned char* encrypt(char* msg);
                int decrypt();
                unsigned char* decrypt(unsigned char* ciphertxt);

        private :
                int b;
};

int Crypto::generateKey() {
    //keypair생성!!
   RSA *keypair= RSA_generate_key(KEY_LENGTH, PUB_EXP, NULL, NULL);
    BIO *pri = BIO_new(BIO_s_mem());
    BIO *pub = BIO_new(BIO_s_mem());
    if(keypair==NULL){
        printf("RSA_keygenerate_key error!! \n");
        return -1;
    }
    if( pri ==NULL){
        printf("allocated error!!\n");
        return -1;
    }
    if( pub ==NULL){
        printf("allocated error!!\n");
        return -1;
    }
    //keypair에게 pri와 pub의 존재를 알려주는 역할들!!
    //BIO공간에 keypair에 속한 정보들을 pri와 pub 스트림에 써준당
    //PEM_write_bio_RSAPrivateKey(<BIO *bp>, <RSA *x>, <const EVP_CIPHER *enc>, <unsigned char *kstr>,
    //<int klen>, <pem_password_cb *cb>, <void *u>)

    //PEM_write_bio_RSAPublicKey(<BIO *bp>, <const RSA *x>)
    PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
    PEM_write_bio_RSAPublicKey(pub, keypair);

    _pri_len = BIO_pending(pri);
    _pub_len = BIO_pending(pub);
    _pri_key= (char*)malloc(_pri_len + 1);
    _pub_key= (char*)malloc(_pub_len + 1);
    BIO_read(pri,_pri_key, (int)_pri_len);
    BIO_read(pub,_pub_key, (int)_pub_len);
    _pri_key[_pri_len]='\0';

    _pub_key[_pub_len]='\0';
    // printf("\n%s\n%s\n", pri_key,pub_key);

    int k = 0;
        /*for(int i=0; i<pri_len+1; i++){
pri_key[i] = pri_key[i] ^ password[i];
        }*/

        std::ofstream privatefile("encrypt_private_key.txt");
        if(privatefile.is_open()){
                privatefile.write(_pri_key,_pri_len);
                privatefile.close();
        }

        std::ofstream pubfile("public_key.txt");
        if(pubfile.is_open()){
                pubfile.write(_pub_key,_pub_len);
                pubfile.close();
        }
       // printf("%d", pub_len);
       //return pub_len;

}

void Crypto::set_key(char* _keyfile1){
        char *buffer;
        int size;
        int count;
        FILE *fp = fopen(_keyfile1, "r");
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        buffer = (char*) malloc(size + 1);
        memset(buffer, 0, size+1);
        fseek(fp, 0, SEEK_SET);
        count = fread(buffer, size, 1, fp);

        //printf("%s", buffer);
        _pub_key = buffer;
       // printf("%s", pub_key);        
}

void Crypto::pset_key(char* _keyfile2){
        char *buffer;
        int size;
        int count;
        FILE *fp = fopen(_keyfile2, "r");
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        buffer = (char*) malloc(size + 1);
        memset(buffer, 0, size+1);
        fseek(fp, 0, SEEK_SET);
        count = fread(buffer, size, 1, fp);

        //printf("%s", buffer);
        _pri_key = buffer;
       // printf("%s", pri_key);        
}

unsigned char* Crypto::encrypt(char* msg) {


        _msg = msg;
        BIO *rpub = BIO_new_mem_buf(_pub_key, -1);
        
        BIO_write(rpub, _pub_key, (int)_pub_len);
        RSA *rsa_pubkey = NULL;
        if (!PEM_read_bio_RSAPublicKey(rpub, &rsa_pubkey, NULL, NULL)) {
                printf("PEM_read_bio_RSAPublicKey error\n");
        //goto free_stuff;
        }

        //_ciphertxt= (unsigned char*) malloc(RSA_size(rsa_pubkey));
       
      
        _err= (char *)malloc(130);
       

        _encrypt_len =0;
         //RSA_PKCS1_OAEP_PADDING은 43byte정도 붙는다...
        if((_encrypt_len=RSA_public_encrypt((int)strlen(_msg)+1, (unsigned char*)_msg, _ciphertxt, rsa_pubkey, RSA_PKCS1_OAEP_PADDING)) == -1){
        ERR_load_crypto_strings();
        ERR_error_string(ERR_get_error(), _err);
        //printf(stderr,"Error Encrypting message : %s", _err);
        //printf("RSA_public_encrypt error!!");
   
        return (unsigned char*)-1;
    }
   printf("cipher txt size : %d\n",RSA_size(rsa_pubkey));

        printf("RSA_size(rsa_pubkey = %d, strlen(_msg) = %d, encrypt_len = %d\n", RSA_size(rsa_pubkey), (int)strlen(_msg), _encrypt_len);


        return (unsigned char*)_ciphertxt;
}

unsigned char* Crypto::decrypt(unsigned char* ciphertxt) {
    //_ciphertxt = ciphertxt;

    printf("cihper txt size?? : %d\n",strlen((char*)ciphertxt));
  
    BIO *rpri = BIO_new_mem_buf(_pri_key,-1);
    _err= (char *)malloc(130);
    BIO_write(rpri, _pri_key,(int)_pri_len);

    RSA *rsa_prikey =NULL;

    if(!PEM_read_bio_RSAPrivateKey(rpri, &rsa_prikey, NULL, NULL)){

        printf("PEM_read_bio_RSAPrivateKey error\n");

        return (unsigned char*)-1;

    }
   
    //unsigned char *decrypt =(unsigned char*) malloc(RSA_size(rsa_prikey));

    int decrypt_len=-1;
     
    // RSA_private_decrypt(<#int flen#>, <#const unsigned char *from#>, <#unsigned char *to#>, <#RSA *rsa#>, <#int padding#>)
    if((decrypt_len=RSA_private_decrypt(_encrypt_len, (unsigned char*)ciphertxt, _decrypt, rsa_prikey, RSA_PKCS1_OAEP_PADDING)) == -1){
        std::cout<<"hi...!2"<<std::endl;
        ERR_load_crypto_strings();
   
        ERR_error_string(ERR_get_error(), _err);
        
        fprintf(stderr,"Error Decrypting message : %s\n", _err);

        printf("RSA_public_encrypt error!!\n");

        return (unsigned char*)-1;

    }
    //std::cout<<"hi...!3"<<std::endl;
    printf("RSA_size(rsa_prikey = %d, strlen(msg) = %d, encrypt_len = %d\n", RSA_size(rsa_prikey), (int)strlen(_msg), _encrypt_len);
    printf("decrypt_len = %d\n", decrypt_len);
    printf("decrypt: >>%s<<\n", _decrypt);

    return (unsigned char*)_decrypt;
}

#endif
