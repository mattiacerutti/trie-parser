#include "../include/trie.hpp"  // It is forbidden to include other libraries!

/* Here below, your implementation of trie. */
template <typename T>
void trie<T>::set_weight(double w) {

    /* TODO: Implement leaf check: if the node IS NOT a leaf (meaning it has some children) should throw an error*/

    if(this->m_c.size() > 0){
        throw parser_exception("Node is not a leaf");
    }


    this->m_w = w;
}

template <typename T>
double trie<T>::get_weight() const {

    return this->m_w;
}

template <typename T>
void trie<T>::set_label(T* l) {

    // Set the label only if node IS NOT the root (meaning it has a parent)
    if(this->m_p){

        /* TODO: Implement label check on parent children: label should be assigned on this node only if there is not another arc from a parent to one of the siblings with the same label*/
        /* This label check WON'T be tested, but is good practice to implement it anyway*/

        bag<trie<T>> parentChildren = this->m_p->get_children();

        if(parentChildren.hasLabel(l)){
            throw parser_exception("Label already exists in parent children");
        }
    
        this->m_l = l;
        return;
    } 
    throw parser_exception("Root node should not have a label");

    
}

template <typename T>
T const* trie<T>::get_label() const {

    // Return the label only if node IS NOT the root (meaning it has a parent)
    if(this->m_p != nullptr){
        return this->m_l;
    }

    return nullptr;
    //TODO: Throw error

}

template <typename T>
void trie<T>::set_parent(trie<T>* p){   
    this->m_p = p;
}

template <typename T>
const trie<T>* trie<T>::get_parent() const {
    return this->m_p;
}

template <typename T>
void trie<T>::add_child(trie<T> const& c){

    /* C should have a valid label before being added */

    if(c.get_label() == nullptr){
        //FIXME: Throw error? Idk
        throw parser_exception("Child node should have a valid label before being added");
    }
    this->m_c.add(&c);
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

    //Initialize bag
    this->m_c = bag<trie<T>>();
}

template <typename T>
trie<T>::trie(double w){
    this->m_p = nullptr;
    this->m_l = nullptr;
    this->m_w = w;

    //Initialize bag
    this->m_c = bag<trie<T>>(this);
}

template <typename T>
trie<T>::trie(trie<T> const& other){ 

    if(other.get_label() == nullptr){
        this->m_l = nullptr;
    } else {
        this->m_l = new T(*other.get_label());
    }

    this->m_w = other.get_weight();

    // Parent is not copied
    this->m_p = nullptr;

    this->m_c = bag<trie<T>>(other.get_children(), this);
}



int main() {
    try{
        trie<int> t;

        trie<int> child1;
        child1.set_parent(&t);
        child1.set_weight(5);
        child1.set_label(new int(2));
        t.add_child(child1);

        // trie<int> child2;
        // child2.set_parent(&t);
        // child2.set_weight(6.0);
        // child2.set_label(new int(3));
        // t.add_child(child2);

        trie<int> t2 = trie<int>(t);





        cout<<"Children1 size is: "<<t.get_children().size()<<endl;
        cout<<"Children2 size is: "<<t2.get_children().size()<<endl;

        // trie<int> retrievedNode = t.get_children().getNode(0);
        
        // cout<<"Weight of first children is: "<<retrievedNode.get_weight()<<endl;
        
        return 0;
    }  catch (parser_exception* e){
        throw runtime_error(e->what());
        return 1;
    }
     catch (trie_exception* e){
        throw runtime_error(e->what());
        return 1;
    }


}