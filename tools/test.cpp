#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../src/trie.cpp"

using namespace std;

#define WORKSPACE_PATH "/Users/clucch/Uni/Projects/trie-parser"

template <typename T>
trie<T> load_trie(string filePath) {
   trie<T> t;
   string fullPath = (string)WORKSPACE_PATH + "/tools/datasets/" + filePath;
   std::ifstream file(fullPath);
   file >> t;
   file.close();
   return t;
}

template <typename T>
void parsing_should_throw_exception(string filePath, T type) {
   try {
      trie<T> t = load_trie<T>(filePath);
      assert(false);
   } catch (const parser_exception& e) {
      assert(true);
   }
}

template <typename T>
void parsing_should_complete(string filePath, T type) {
   try {
      trie<T> t = load_trie<T>(filePath);
      assert(true);
   } catch (const parser_exception& e) {
      cout << e.what() << endl;
      assert(false);
   }
}

void test_parsing_validation() {
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

void test_set_label() {
   try {
      /* Correct label, we are in a child */
      trie<char> t;
      trie<char> t2;

      t2.set_parent(&t);
      t2.set_label(new char('b'));

   } catch (const parser_exception& e) {
      assert(false);
   }

   try {
      /* Incorrect label, we are in root node */
      trie<char> t;
      t.set_label(new char('a'));

      assert(false);

   } catch (const parser_exception& e) {
      assert(true);
   }
}

void test_set_weight() {
   try {
      /* Correct weight */
      trie<char> t;
      t.set_weight(1);
      assert(t.get_weight() == 1);

   } catch (const parser_exception& e) {
      assert(false);
   }
}

void test_set_parent() {
   try {
      /* Correct parent */
      trie<char> t;
      trie<char> t2;
      t.set_parent(&t2);
      assert(t.get_parent() == &t2);

   } catch (const parser_exception& e) {
      assert(false);
   }
}

void test_getters_setters() {
   test_set_parent();
   test_set_weight();
   test_set_label();
}

void test_bag_iterator() {
   try {
      trie<char> t = load_trie<char>("trie_char1.tr");
      bag<trie<char>>::child_iterator it = t.get_children().begin();

      // Check first element - arrow operator
      assert(*it->get_label() == 'a');

      // Check first element - dereference operator
      assert(*(*it).get_label() == 'a');

      // Check if incrementing works
      it++;
      assert(*it->get_label() == 'b');

      // Check if .end() works
      it++;
      assert(it == t.get_children().end());

      // Check if ++it works
      it = t.get_children().begin();
      assert(*((++it)->get_label()) == 'b');

      // Check if it++ works
      it = t.get_children().begin();
      assert(*((it++)->get_label()) == 'a');
      assert(*((it)->get_label()) == 'b');

      // Check if sub-iterator works
      it = t.get_children().begin();
      bag<trie<char>>::child_iterator second_it = it->get_children().begin();
      assert(second_it == it->get_children().end());

      it++;
      second_it = it->get_children().begin();
      assert(*second_it->get_label() == 'b');

      second_it++;
      assert(*second_it->get_label() == 'c');

      second_it->set_label(new char('d'));
      assert(*second_it->get_label() == 'd');

      second_it++;
      assert(second_it == nullptr);
   } catch (const parser_exception& e) {
      cout << e.what() << endl;
      assert(false);
   }
}

void test_const_bag_iterator() {
   try {
      const trie<char> t = load_trie<char>("trie_char1.tr");
      bag<trie<char>>::const_child_iterator it = t.get_children().begin();

      // Check first element - arrow operator
      assert(*it->get_label() == 'a');

      // Check first element - dereference operator
      assert(*(*it).get_label() == 'a');

      // Check if incrementing works
      it++;
      assert(*it->get_label() == 'b');

      // Check if .end() works
      it++;
      assert(it == t.get_children().end());

      // Check if ++it works
      it = t.get_children().begin();
      assert(*((++it)->get_label()) == 'b');

      // Check if it++ works
      it = t.get_children().begin();
      assert(*((it++)->get_label()) == 'a');
      assert(*((it)->get_label()) == 'b');

      // Check if sub-iterator works
      it = t.get_children().begin();
      bag<trie<char>>::const_child_iterator second_it = it->get_children().begin();
      assert(second_it == it->get_children().end());

      it++;
      second_it = it->get_children().begin();
      assert(*second_it->get_label() == 'b');

      second_it++;
      assert(*second_it->get_label() == 'c');

      second_it++;
      assert(second_it == nullptr);
   } catch (const parser_exception& e) {
      cout << e.what() << endl;
      assert(false);
   }
}

int main() {
   test_parsing_validation();
   test_getters_setters();
   test_bag_iterator();
   test_const_bag_iterator();
   return 0;
}