#include "../include/trie.hpp"  // It is forbidden to include other libraries!
#include <fstream>
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
        throw parser_exception("Invalid input: expected children but got something else");
    }
}


void B(istream& is, bool& shouldEspectLeaf){
    string firstChunk = getNextChunk(is);

    if(firstChunk == "children"){
        // We have only a label
    } else{
        // We have a label and a weight

        //Check if weight is double
        try{
            stod(firstChunk);
        } catch(invalid_argument e){
            throw parser_exception("Invalid input: weight should be a double");
        }

        shouldEspectLeaf = true;
    
        C(is);
    }
}

void S(istream& is, bool isFirstNode = false);

void T(istream& is, bool shouldEspectLeaf){

    char nextChar = getNextChar(is);
    if(nextChar == '}'){

        if(!shouldEspectLeaf){
            throw parser_exception("Invalid input: a leaf node must have a weight");
        }

        // We have a leaf
        is.unget();
    } else {
        // We have a node

        if(shouldEspectLeaf){
            throw parser_exception("Invalid input: a non-leaf node must not have a weight");
        }

        //Go back one character
        is.unget();
        S(is);
    }
}


void S(istream& is, bool isFirstNode){
    string firstChunk = getNextChunk(is);

    bool shouldEspectLeaf = false;

    if(firstChunk == "children"){

        if(!isFirstNode){
            throw parser_exception("Invalid input: all non-root nodes should have a label");
        }

        // We are in the root
    } else {

        if(isFirstNode){
            throw parser_exception("Invalid input: root node should start with children keyword");
        }


        // We are in a child

        if(firstChunk.find('}') != string::npos || firstChunk.find('{') != string::npos || firstChunk.find('=') != string::npos || firstChunk.find(',') != string::npos){
            throw parser_exception("Invalid input: got unexpected character in the beginning of the string");
        }
        B(is, shouldEspectLeaf);
    }

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


    T(is, shouldEspectLeaf);

    // Check if there is "}"
    char secondChar = getNextChar(is);
    if(secondChar != '}'){
        throw parser_exception("Invalid input: expected } but got something else");
    }

    // Check if there is ","
    char thirdChar = getNextChar(is);
    if(thirdChar == ','){
        S(is);
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
    
    S(stream, true);
    cout<<"Parsing successful"<<endl;


    return stream;
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

        // cout<<"Children1 size is: "<<t.get_children().size()<<endl;
        // cout<<"Children2 size is: "<<t2.get_children().size()<<endl;

        trie<int> t3;

        //FIXME: ONLY FOR TESTING REASONS
        ifstream file("../test.txt");
        file>>t3;
        
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