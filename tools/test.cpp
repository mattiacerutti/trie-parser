#include "../src/trie.cpp"
#include <iostream>
#include <cassert>
#include <fstream>
using namespace std;

template <typename T>
void parsing_should_throw_exception(string filePath, T type){
    trie<T> t;
    std::ifstream file((string)filesystem::current_path() + "/tools/datasets/" + filePath);
    try{
        file>>t;
        assert(false);
    } catch (const parser_exception& e) {
        assert(true);
    }
    file.close();

}

template <typename T>
void parsing_should_complete(string filePath, T type){
    trie<T> t;
    std::ifstream file((string)filesystem::current_path() + "/tools/datasets/" + filePath);
    try{
        file>>t;
        assert(true);
    } catch (const parser_exception& e) {
                cout<<e.what()<<endl;
        assert(false);
    }
    file.close();
}

void test_parsing_validation(){
    parsing_should_throw_exception("trie_char_error1.tr", char());
    parsing_should_throw_exception("trie_char_error2.tr", char());
    parsing_should_throw_exception("trie_char_error3.tr", char());
    parsing_should_throw_exception("trie_char_error4.tr", char());
    parsing_should_throw_exception("trie_char_error5.tr", char());

    parsing_should_throw_exception("trie_string_error1.tr", string());
    parsing_should_throw_exception("trie_string_error2.tr", string());
    parsing_should_throw_exception("trie_string_error3.tr", string());
    parsing_should_throw_exception("trie_string_error4.tr", string());

    parsing_should_complete("trie_char1.tr", char());
    parsing_should_complete("trie_string.tr", string());
}

void test_set_label(){
    try{
        /* Correct label, we are in a child */
        trie<char> t;
        trie<char> t2;

        t2.set_parent(&t);
        t2.set_label(new char('b'));


    } catch (const parser_exception& e) {
        assert(false);
    }

    try{
        /* Incorrect label, we are in root node */
        trie<char> t;
        t.set_label(new char('a'));



        assert(false);

    } catch (const parser_exception& e) {
        assert(true);
    }
    
}

void test_set_weight(){
    try{
        /* Correct weight */
        trie<char> t;
        t.set_weight(1);
        assert(t.get_weight() == 1);

    } catch (const parser_exception& e) {
        assert(false);
    }
}

void test_set_parent(){
    try{
        /* Correct parent */
        trie<char> t;
        trie<char> t2;
        t.set_parent(&t2);
        assert(t.get_parent() == &t2);

    } catch (const parser_exception& e) {
        assert(false);
    }
}

void test_getters_setters(){
    test_set_parent();
    test_set_weight();
    test_set_label();
}

int main() {
    test_parsing_validation();
    test_getters_setters();
    return 0;
}