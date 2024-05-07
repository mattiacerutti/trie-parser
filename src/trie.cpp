#include "trie.hpp"  // It is forbidden to include other libraries!

/* Here below, your implementation of trie. */
template <typename T>
void trie<T>::set_weight(double w) {

    /* TODO: Implement leaf check: if the node IS NOT a leaf (meaning it has some children) should throw an error*/


    this->m_w = w;
}

template <typename T>
double trie<T>::get_weight() const {
    return this->m_w;
}

template <typename T>
void trie<T>::set_label(const T* l) {

    // Set the label only if node IS NOT the root (meaning it has a parent)
    if(this->m_p){

        /* TODO: Implement label check on parent children: label should be assigned on this node only if there is not another arc from a parent to one of the siblings with the same label*/

        this->m_l = l;
    }

    
}

template <typename T>
T const* trie<T>::get_label() const {

    // Return the label only if node IS NOT the root (meaning it has a parent)
    if(this->m_p){
        return this->m_l;
    }
    //TODO: Throw error

}

template <typename T>
void trie<T>::set_parent(const trie<T>* p){   
    this->m_p = p;
}

template <typename T>
const trie<T>* trie<T>::get_parent() const {
    return this->m_p;
}

template <typename T>
void trie<T>::add_child(trie<T> const& c){

    /* C should have a valid label before being added*/

    if(c.get_label() == nullptr){
        //FIXME: Throw error? Idk
    }

    this->m_c.add(c);
}

template <typename T>
bag<trie<T>> const& trie<T>::get_children() const{
    return this->m_c;
}

template <typename T>
trie<T>::trie(){
    this->m_p = nullptr;
    this->m_l = nullptr;
    this->m_w = 0.0;
}

template <typename T>
trie<T>::trie(double w){
    this->m_p = nullptr;
    this->m_l = nullptr;
    this->m_w = w;
}


