
#include <vector>

/*
 * Your implementation of the container bag<Val>.
 * Write both the declaration and definition of the
 * container in this file.
 */

using namespace std;

struct trie_exception {
    trie_exception(string const& str) : m_str(str) {}

    string what() const { return m_str; }

private:
    std::string m_str;
};

template <typename T>
struct trie;

template <typename T>
struct bag;

template <typename T>
struct bag<trie<T>> {

    //Contructors and deconstructor
    
    bag() {
        nodes = vector<trie<T>*>();
    }

    bag(const bag& other, trie<T> * parent) {
        for(int i = 0; i < other.size(); i++){

            const trie<T> * oldNode = other.get(i);
            trie<T> * newNode = new trie<T>(*oldNode);

            newNode->set_parent(parent);

            nodes.push_back(newNode);
        }
    }

    ~bag() {
        for(trie<T>* node : nodes){
            delete node;
        }
        nodes.clear();
    }

    bag& assignFrom(const bag& other, trie<T> * parent) {
        if (this == &other) {
            return *this;
        }

        // Clear existing nodes
        for (trie<T>* node : nodes) {
            delete node;
        }
        nodes.clear();

        // Copy nodes from other bag
        for (int i = 0; i < other.size(); i++) {
            const trie<T>* oldNode = other.get(i);
            trie<T>* newNode = new trie<T>(*oldNode);
            newNode->set_parent(parent);
            nodes.push_back(newNode);
        }

        return *this;
    }

    //Methods
    void add(trie<T> * node) {


        if(nodes.size() == 0){
            nodes.push_back(node);
            return;
        }

        

        for(int i = 0; i < nodes.size(); i++){

            if(*(*node).get_label() > *(*nodes[i]).get_label() ){
                if(i == nodes.size() - 1){
                    nodes.push_back(node);
                    return;
                }

                if((*node).get_label() < (*nodes[i+1]).get_label()){
                    nodes.insert(nodes.begin() + i + 1, node);
                    return;
                }
            } else {
                nodes.insert(nodes.begin() + i, node);
                return;
            }
        }
    }

    bool hasLabel(const T* label) {
        for(int i = 0; i < nodes.size(); i++){

            if(*((*nodes[i]).get_label()) == *label){
                return true;
            }
        }

        return false;
    }

    trie<T> * get(int i) const {

        if(i < 0 || i >= nodes.size()){
            return nullptr;
        }

        return nodes[i];
    }


    int size() const {
        return nodes.size();
    }

private:

    //Data
    vector<trie<T>*> nodes;

};