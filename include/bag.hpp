
#include <vector>

/*
 * Your implementation of the container bag<Val>.
 * Write both the declaration and definition of the
 * container in this file.
 */

template <typename T>
struct bag {

    //Contructors and deconstructor
    
    bag() {
        nodes = vector<T>();
    }

    ~bag() {
        nodes.clear();
    }

    //Methods
    void add(T node) {
        for(int i = 0; i < nodes.size(); i++){

            T currentNode = nodes[i];

            if(node.get_label() > currentNode.get_label() ){
                if(i == nodes.size() - 1){
                    nodes.push_back(node);
                    return;
                }

                if(node.get_label() < nodes[i+1].get_label()){
                    nodes.insert(nodes.begin() + i + 1, node);
                    return;
                }
            } else {
                nodes.insert(nodes.begin() + i, node);
                return;
            }
        }
    }




    private:

    //Data
    vector<T> nodes;

};