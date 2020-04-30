// 20132927 정재명
#pragma once 

#include <iostream>
#include <cstdlib>
using namespace std;

template <typename Type>
class node {
    public:
        Type data;
        node* next;
        node* prev;
};
template <typename Type>
class d_linked_list{
    protected: 
        node<Type> *head; 
        node<Type> *tail;  
        int length;
    public:
        d_linked_list(): head(nullptr), tail(nullptr), length(0) {}
        virtual ~d_linked_list(){
            delete_list();
        }
        void push_front(Type data){ 
            node<Type>* newNode = new node<Type>();
            newNode->data = data;
            newNode->prev = nullptr;
            if(!head){
                newNode->next = nullptr;
                head = newNode;
                tail = newNode;    
            } else { 
                head->prev = newNode;
                newNode->next = head;
                head = newNode;
            }
            length++;
        }
        void push_back(Type data){
            node<Type>* newNode = new node<Type>();
            newNode->data = data;
            newNode->next = nullptr;
            if(!tail) {
                newNode->prev = nullptr;
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            }
            length++;
        }
        void insert_node_at(int idx, Type data){
            if(idx >= length) return; 
            else if( idx  == 0 ) { 
                push_front(data);
                return;
            } 
            else if( idx == length -1 ){ 
                push_back(data);
                return;
            }
            node<Type>* idxNode = head; 
            for(int i = 1 ; i<= idx ; i++){
                idxNode = idxNode->next;
            }
            node<Type>* newNode = new node<Type>();
            newNode->data = data;

            newNode->prev = idxNode->prev;
            idxNode->prev->next = newNode;

            newNode->next = idxNode;
            idxNode->prev = newNode;

            length++;
        }   
        Type pop_front(){
            if(head == nullptr) {
                cout << "비어있습니다." << endl;
                exit(0);
            }
            node<Type> *popNode = head;
            Type item = popNode->data;
            head = popNode->next; 
            if(head == nullptr) tail = head;
            else head->prev = nullptr;
            delete popNode;
            length--;
            return item;
        }
        Type pop_back(){
            if(tail == nullptr) {
                cout << "비어있습니다." << endl;
                exit(0);
            }
            node<Type> *popNode = tail;
            Type item = popNode->data;
            tail = popNode->prev;
            if(tail == nullptr) head = tail;
            else tail->next = nullptr;
            delete popNode;
            length--;
            return item;
        }
        Type pop_node_at(int idx){
            if(idx >= length) {
                cout << "유요한 index가 아닙니다." << endl;
                exit(0); 
            }
            else if( idx  == 0) { 
                return pop_front();
            } 
            else if( idx == length -1 ){      
                return pop_back();
            }

            node<Type>* idxNode = head;
            for(int i = 1 ; i<= idx ; i++){
                idxNode = idxNode->next;
            }
            idxNode->prev->next = idxNode->next;
            idxNode->next->prev = idxNode->prev;
            Type item = idxNode->data;
            delete idxNode;
            length--;
            return item;
        }

        void delete_list(){  
            node<Type> *temp;
            while(head){ 
                temp = head->next; 
                delete head;
                head = temp;
            }
            tail = head;
        }


        Type front() { 
            if(!head) {
                cout << "비어있습니다." << endl;
                exit(0);
            }
            return (*head).data;
        }
        Type back() { 
            if(!tail) {
                cout << "비어있습니다." << endl;
                exit(0);
            }    
            return (*tail).data; 
        }
        int size() { return length; }
};

template <typename Type>
class dll_deque : public d_linked_list<Type> {
    public:
        dll_deque() {} 
        ~dll_deque() {
            this->delete_list();
        } 
        void addFront(Type data) {
            this->push_front(data);
        }
        void addRear(Type data){
            this->push_back(data);
        }

        Type deleteFront() {
            if(this->head == nullptr){
                cout << "비어있습니다." << endl;
                exit(0);
            }
            return this->pop_front();
        }
        Type deleteRear() {
            if(this->head == nullptr){
                cout << "비어있습니다." << endl;
                exit(0);
            }   
            return this->pop_back();
        }

        Type getFront() { 
            if(this->head == nullptr){
                cout << "비어있습니다." << endl;
                exit(0);
            }  
            return (*this->head).data;
        }
        Type getRear() { 
            if(this->head == nullptr){
                cout << "비어있습니다." << endl;
                exit(0);
            }  
            return (*this->tail).data; 
        }
};
