/* ==================================================================
Robot Final Project Support: 
linkedlist.h edited by Sophie Robertson | 12/7/2023
=====================================================================

=====================================================================*/

template <class T>
class node {
  public:
    T element;
    node* next;
    node* prev;

    node(T element, node* prev, node* next) : element(element)
    {
      this->next = next;
      this->prev = prev;
    };
};

template <class T> class linkedlist  {
  private:
    int length;
    node<T>* head;
    node<T>* tail;
    node<T>* cu;
  public:
    linkedlist();
    linkedlist(const linkedlist<T>&);
    ~linkedlist();
    T& getCurrent();
    T& first() const;
    T& last() const;
    int getLength();
    void append(T);
    bool isEmpty() { return head == nullptr; }
    bool next();
    bool prev();
    bool goToHead();
    void printList();
    void clear();
    linkedlist& operator = (const linkedlist<T>&);
};

template <class T> linkedlist<T>::linkedlist() {
  length = 0;
  head = nullptr;
  tail = nullptr;
  cu = nullptr;
}

template <class T> linkedlist<T>::linkedlist(const linkedlist<T> & list) {
  length = 0;
  head = nullptr;
  tail = nullptr;
  cu = nullptr;

  node<T> * temp = list.head;

  while(temp != nullptr) {
    append(temp->element);
    temp = temp->next;
  }
}

template <class T> linkedlist<T> & linkedlist<T>::operator=(const linkedlist<T> & list) {
  clear();
  node<T> * temp = list.head;
  while(temp != nullptr) {
    append(temp->element);
    temp = temp->next;
  }
  return *this;
}

template <class T> linkedlist<T>::~linkedlist() {
  clear();
}

template<class T> T& linkedlist<T>::getCurrent() {
  return cu->element;
}

template<class T> T& linkedlist<T>::first() const {
  return head->element;
}

template<class T> T& linkedlist<T>::last() const {
  return tail->element;
}

template<class T> int linkedlist<T>::getLength() {
  return length;
}

template <class T> void linkedlist<T>::append(T element) {
  node<T> * temp = new node<T>(element, tail, nullptr);

  if(length == 0)
    cu = tail = head = temp;
  else {
    tail->next = temp;
    tail = temp;
  }
  length++;
}

template <class T> bool linkedlist<T>::next() {
  if(length == 0) {
    return false;
  }
  if(cu->next == nullptr) {
    return false;
  }
  cu = cu->next;
  return true;
}

template <class T> bool linkedlist<T>::goToHead() {
  cu = head;
  return length != 0;
}

template<class T> bool linkedlist<T>::prev() {
  if(length == 0) { 
    return false; 
  }
  if(cu->prev != nullptr) {
    return false;
  }
  cu = cu->prev;
  return true;
}

template <typename T> void linkedlist<T>::printList() {
  //Serial.println("printing: ");
  node<T> *cu = head;
  int temp = cu->element;
  while (cu->next != nullptr) { //prints until last val
    temp = cu->element;
    Serial.print(temp);
    Serial.print(",");
    cu = cu->next;
  }
  temp = cu->element;
  Serial.print(temp); //last val
  Serial.print("]");
  Serial.print('\n'); //print newline
}

template <class T> void linkedlist<T>::clear() {
  if(length == 0) {
    return;
  }
  node<T> * temp = head;

  while(temp != nullptr) {
    head = head->next;
    delete temp;
    temp = head;
  }
  head = cu = tail = nullptr;
  length = 0;
}
