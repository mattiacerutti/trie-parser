#include "../src/trie.cpp"
#include <iostream>
#include <cassert>
#include <fstream>
using namespace std;

template <typename T>
void parsing_should_throw_exception(string filePath, T type){
    trie<T> t;
    std::ifstream file((string)std::filesystem::current_path() + "/tools/datasets/" + filePath);
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
    std::ifstream file((string)std::filesystem::current_path() + "/tools/datasets/" + filePath);
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

int main() {
    test_parsing_validation();
    return 0;
}