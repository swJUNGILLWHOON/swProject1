#ifndef SingleList2_h_
#define SingleList2_h_

#include<iostream>
#include<fstream>
#include <ctime>
#include<string>
#include<cstring>
#define INFO "INFO"
#define ERROR "ERROR"
#define WARN "WARN"


using namespace std;

struct Node{
   public :
      string _value;
      string _time;
      string _level;
      Node *next; 
};
class SingleLinked_List{
   private:
      Node *head;
      Node *tail;
      int size = 0;

   public:
      SingleLinked_List() : head(NULL), tail(NULL){}
      ~SingleLinked_List() {}
      

      void AddNode(const char*  value, string time,const char* level){ //add node!
        
         Node *node = new Node;
         size++;
         node ->_level = level;
         node ->_time = time;
         node -> _value = value;
         node -> next = NULL;
         if(head == NULL){
            head = node;
            tail = node;
          
         }

         else {
            tail -> next = node;
            tail = tail->next;
         }
 }
      int PrintList(const char* level){
   Node *node = head;
   cout <<"Log message level is : "<<level<<endl;
   while (node != NULL)
   {
      if(strcmp(node->_level.c_str(), level)==0){
         cout <<"time : "<< node->_time<<"log message : "<<node -> _value <<endl;
         node = node->next;
      }
      else{
         node = node ->next;
      }
      }
   return 0;
      }
     

};
#endif

