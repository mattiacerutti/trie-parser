#include "../include/trie.hpp"  // It is forbidden to include other libraries!
#include <fstream>
/* Here below, your implementation of trie. */
template <typename T>
void trie<T>::set_weight(double w) {

    /* TODO: Implement leaf check: if the node IS NOT a leaf (meaning it has some children) should throw an error*/

    if(this->m_c.size() > 0){
        throw parser_exception("Cannot set weight for a non-leaf node");
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
    
        this->m_l = l;
        return;
    } 
    throw parser_exception("Root node should not have a label. If you're trying to set a label for a child node, please call set_parent first");

    
}

template <typename T>
T const* trie<T>::get_label() const {

    // Return the label only if node IS NOT the root (meaning it has a parent)
    if(this->m_p != nullptr){
        return this->m_l;
    }

    return nullptr;

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

    // Should have a valid label before being added
    if(c.get_label() == nullptr){
        throw parser_exception("Child node should have a valid label before being added");
    }

    // Should have a valid parent before being added
    if(c.get_parent() == nullptr){
        throw parser_exception("Child node should have a valid parent before being added");
    }

    // Check if there are any siblings with the same label
    bag<trie<T>> siblings = this->m_c;
    if(siblings.hasLabel(c.get_label())){
        throw parser_exception("Invalid input: a node with the same label already exists");
    }


    // If current node has a weight, we need to remove it
    if(this->m_w != 0.0){
        this->m_w = 0.0;
    }

    trie<T>* p_child = new trie<T>(c);
    p_child->set_parent(this);

    this->m_c.add(p_child);
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

    bag<trie<T>>* p_bag = new bag<trie<T>>();

    this->m_c = *p_bag;
}

template <typename T>
trie<T>::trie(double w){
    this->m_p = nullptr;
    this->m_l = nullptr;
    this->m_w = w;

    //Initialize bag
    bag<trie<T>>* p_bag = new bag<trie<T>>();

    this->m_c = *p_bag;
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

    // Children are copied
    bag<trie<T>>* p_bag = new bag<trie<T>>(other.get_children(), this);

    this->m_c = *p_bag;
}

void cleanString(string& str){

    int firstChar = str.find_first_not_of(" \t\n");

    int lastChar = str.find_last_not_of(" \t\n");

    if(firstChar == string::npos || lastChar == string::npos){
        return;
    }

    string newStr = str.substr(firstChar, lastChar - firstChar + 1);

    //If the new string still has spaces, tabs or newlines, it means that the keyword is not well formatted
    if(newStr.find(' ') != string::npos || newStr.find('\t') != string::npos || newStr.find('\n') != string::npos){
        throw parser_exception("Invalid input: keywords can't have spaces, tabs or newlines in the middle of the string");
    }

    //Remove spaces only in the edges
    str.erase(remove(str.begin(), str.end(), ' '), str.end());

    //Remove newlines
    str.erase(remove(str.begin(), str.end(), '\n'), str.end());

    //Remove tabs
    str.erase(remove(str.begin(), str.end(), '\t'), str.end());
}

string getNextChunk(istream& is){
    string chunk;
    getline(is, chunk, ' ');

    cleanString(chunk);

    while(chunk == "" || chunk == "\n" || chunk == "\t"){
        getline(is, chunk, ' ');
    }

    return chunk;
}

char getNextChar(istream& is){
    char nextChar;
    is>>nextChar;

    while(nextChar == ' ' || nextChar == '\n' || nextChar == '\t'){
         is>>nextChar;
    }

    return nextChar;
}

void C(istream& is){
    string firstChunk = getNextChunk(is);

    if(firstChunk == "children"){
        // We have a label and a weight
    } else {
        throw parser_exception("Invalid input: expected children but got something else (this usually happens when a node has another property other than weight and label)");
    }
}

template <typename T>
void B(istream& is, bool& shouldEspectLeaf, trie<T>& currentTrie){
    string firstChunk = getNextChunk(is);

    if(firstChunk == "children"){
        // We have only a label

    } else{
        // We have a label and a weight

        //Check if weight is double
        try{
            currentTrie.set_weight(stod(firstChunk));
        } catch(invalid_argument e){
            throw parser_exception("Invalid input: weight should be a double");
        }

        shouldEspectLeaf = true;
    
        C(is);

    }
}
template <typename T>
void S(istream& is, trie<T>& currentTrie, trie<T>& parentTrie);

template <typename T>
void S(istream& is, trie<T>& currentTrie);

template <typename T>
void R(istream& is, bool shouldEspectLeaf, trie<T>& currentTrie){

    char nextChar = getNextChar(is);
    if(nextChar == '}'){
        // We have a leaf

        if(!shouldEspectLeaf){
            throw parser_exception("Invalid input: a leaf node must have a weight");
        }

        is.unget();
    } else {
        // We have a node

        if(shouldEspectLeaf){
            throw parser_exception("Invalid input: a non-leaf node must not have a weight");
        }

        //Go back one character
        is.unget();

        // Build the child
        
        auto it = currentTrie.root();
        using ValueType = typename decltype(it)::value_type;


        trie<ValueType>* p_child = new trie<ValueType>();
        trie<ValueType> child = *p_child;
        child.set_parent(&currentTrie);
        S(is, child, currentTrie);

        // Add the new child to the parent trie we passed as an argument
        currentTrie.add_child(child);
    }
}

template <typename T>
void S(istream& is, trie<T>& currentTrie, trie<T>& parentTrie){


    string firstChunk = getNextChunk(is);

    bool shouldEspectLeaf = false;

    if(firstChunk == "children"){
        // We are in the root

        throw parser_exception("Invalid input: non-root nodes should have a label");


    } else {
        // We are in a child and firstChunk is a label 

        if(firstChunk.find('}') != string::npos || firstChunk.find('{') != string::npos || firstChunk.find('=') != string::npos || firstChunk.find(',') != string::npos){
            throw parser_exception("Invalid input: got unexpected character in the beginning of the string");
        }

        using trieType = decay_t<T>;

        try{
            if constexpr (is_same_v<trieType, string>){
                currentTrie.set_label(new trieType(firstChunk));
            } else if constexpr (is_same_v<trieType, char>) {

                if(firstChunk.size() != 1){
                    throw parser_exception("Invalid input: char label can only have one character");
                }

                currentTrie.set_label(new trieType(firstChunk[0]));
            } else if constexpr (is_integral_v<trieType>){

                if(firstChunk.find('.') != string::npos){
                    throw parser_exception("Invalid input: label type is an integer, but got a floating point number");
                }

                trieType value = stoi(firstChunk);

                currentTrie.set_label(new trieType(value));
            }else if constexpr (is_floating_point_v<trieType>){

                trieType value = stod(firstChunk);

                currentTrie.set_label(new trieType(value));
            }
            
            else {
                throw parser_exception("Invalid input: label type not supported");
            }
        } catch(invalid_argument e){
            throw parser_exception("Error while parsing label");
        }
        

        B(is, shouldEspectLeaf, currentTrie);
    }


    /* Here we correctly set label and weight */


    // Check if there is "="
    char equalChar = getNextChar(is);
    if(equalChar != '='){
        throw parser_exception("Invalid input: expected = but got something else");
    }

    // Check if there is "{"
    char firstChar = getNextChar(is);
    if(firstChar != '{'){
        throw parser_exception("Invalid input: expected { but got something else");
    }

    R(is, shouldEspectLeaf, currentTrie);


    // Check if there is "}"
    char secondChar = getNextChar(is);
    if(secondChar != '}'){
        throw parser_exception("Invalid input: expected } but got something else");
    }



    /* Here we correctly inserted all children */


    // Check if there is ","
    char thirdChar = getNextChar(is);
    if(thirdChar == ','){

        // We have a sibling

        auto it = currentTrie.root();
        using ValueType = typename decltype(it)::value_type;

        trie<ValueType>* p_sibling = new trie<ValueType>();
        trie<ValueType> sibling = *p_sibling;

        sibling.set_parent(&parentTrie);

        S(is, sibling, parentTrie);

        // Add the new sibling to the parent trie we passed as an argument
        parentTrie.add_child(sibling);

    } else {
        // We need to go back one character, because we already read the next pharentesis
        is.unget();
    }
    
}

template <typename T>
void S(istream& is, trie<T>& currentTrie){
    string firstChunk = getNextChunk(is);

    bool shouldEspectLeaf = false;

    if(firstChunk == "children"){
        // We are in the root


    } else {
        

        throw parser_exception("Invalid input: root node should start with children keyword");

    }


    /* Here we correctly set label and weight */


    // Check if there is "="
    char equalChar = getNextChar(is);
    if(equalChar != '='){
        throw parser_exception("Invalid input: expected = but got something else");
    }

    // Check if there is "{"
    char firstChar = getNextChar(is);
    if(firstChar != '{'){
        throw parser_exception("Invalid input: expected { but got something else");
    }

    R(is, shouldEspectLeaf, currentTrie);

    // Check if there is "}"
    char secondChar = getNextChar(is);
    if(secondChar != '}'){
        throw parser_exception("Invalid input: expected } but got something else");
    }



    /* Here we correctly inserted all children */


    // Check if there is ","
    char thirdChar = getNextChar(is);
    if(thirdChar == ','){
        // We have a sibling

        throw parser_exception("Invalid input: root node should not have siblings");


    } else {
        // We need to go back one character, because we already read the next pharentesis
        is.unget();
    }


}





template <typename T>
istream& operator>>(std::istream& stream, trie<T>& trie){

    //TODO: Add label type checking = T

    //If file is empty
    if(stream.peek() == EOF){
        throw parser_exception("File is empty");
        return stream;
    }
    
    S(stream, trie);
    cout<<"Parsing successful"<<endl;


    return stream;
}

template <typename T>
trie<T>::node_iterator::node_iterator(trie<T>* ptr) : m_ptr(ptr) {}

template <typename T>
typename trie<T>::node_iterator trie<T>::root() {
    //FIXME: NOT CORRECT
    return node_iterator(this);
}

int main() {
    try{
        // trie<int> t;

        // trie<int> child1;
        // child1.set_parent(&t);
        // child1.set_weight(5);
        // child1.set_label(new int(2));
        // t.add_child(child1);

        // // trie<int> child2;
        // // child2.set_parent(&t);
        // // child2.set_weight(6.0);
        // // child2.set_label(new int(3));
        // // t.add_child(child2);

        // trie<int> t2 = trie<int>(t);

        // cout<<"Children1 size is: "<<t.get_children().size()<<endl;
        // cout<<"Children2 size is: "<<t2.get_children().size()<<endl;

        trie<int> t3;
        
        //FIXME: ONLY FOR TESTING REASONS
        ifstream file("../test.txt");
        file>>t3;

        // cin>>t3;

        cout<<"Size of root: "<<t3.get_children().size()<<endl;
        cout<<"Size of child:"<<t3.get_children().getNode(0)->get_children().size()<<endl;
        
        return 0;
    }  catch (parser_exception e){
        throw runtime_error(e.what());
        return 1;
    }
     catch (trie_exception e){
        throw runtime_error(e.what());
        return 1;
    }


}