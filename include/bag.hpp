
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
        nodes = vector<const trie<T>*>();
    }

    bag(const bag& other, trie<T> * parent) {
        for(int i = 0; i < other.size(); i++){

            const trie<T> * oldNode = other.getNode(i);
            trie<T> * newNode = new trie<T>(*oldNode);

            newNode->set_parent(parent);

            nodes.push_back(newNode);
        }
    }

    ~bag() {
        nodes.clear();
    }

    //Methods
    void add(const trie<T> * node) {


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

    const trie<T> * getNode(int i) const {

        if(i < 0 || i >= nodes.size()){
            throw new trie_exception("Node with index " + to_string(i) + " does not exist.");
        }

        return nodes[i];
    }

    int size() const {
        return nodes.size();
    }

private:

    //Data
    vector<const trie<T>*> nodes;

};