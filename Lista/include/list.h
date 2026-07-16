#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include <initializer_list>

template<typename T>
class Node{
    public:
        Node(T data) : data(data), next(nullptr) {}
        void setProx(Node* p){this->next = p;};
        Node<T>* getProx(){return this->next;};
        T getData(){return this->data;}
        T data;
        Node<T>* next;
};

template<typename T>
class List{
    public:
        List() : head(nullptr), tail(nullptr), size(0) {}
        List(std::initializer_list<T> data): head(nullptr), tail(nullptr), size(0) {
            for(auto it = data.begin(); it != data.end(); it++) push_back(*it);
        }
        
        void push_back(T data){
            Node<T> *newnode = (Node<T>*)malloc(sizeof(Node<T>));
            newnode->data = data;
            newnode->next = nullptr;
            if (tail) tail->next = newnode;
            tail = newnode;
            if (!head) head = newnode;
            size++;
        }
        void pop_back(){
            if(!tail) return;
            Node<T> *temp = tail;
            tail = tail->next;
            free(temp);
            size--;
            if(!tail) head = nullptr;
        }
        T front(){return head->data;}
        T back(){return tail->data;}

        T get(int index)
        {
            if(index < 0 || index > size) return T();
            Node<T>* temp = head;
            for(int i{0};i<index;i++) temp = temp->next;
            return temp->data;
        }
        
        int get_size(){return size;}
        bool is_empty(){return size == 0;}
        
        void clear() {            
            while(head)
            {
                Node<T>* t = head;
                head = t->next;
                free(t);
            }
            size = 0;
        }

    private:
        Node<T>* head;
        Node<T>* tail;
        int size;
};
#endif // LIST_H