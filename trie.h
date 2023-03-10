#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>

#define N 27 // N-ary trie here 27 since english alphabets + apostrophe being accounted for
#define MAXWORDSIZE 45 // maximum size of word that will be included in the trie (excluding null char)


struct trienode
{
    struct trienode * next[N];
    bool endofword;
};

typedef struct trienode trienode;

trienode * initTrie();
trienode * insertTrie(trienode * root, char * word);
bool searchTrie(trienode * root, char * word);
bool simple_search_trie(trienode * root, char * word);
bool no_children(trienode * root);
trienode * delete_from_trie(trienode * root, char * word, int trie_level);
bool is_leaf(trienode * root);
void print_trie_helper(trienode * root, char buffer[], int level);
void printTrie(trienode * root);
bool prefix_search(trienode * root, char * word);
void prefix_print(trienode * temp, char * prefix);
void helper_prefix_print(trienode * root, int level, char buffer[]);
int prefix_delete(trienode * root, char * prefix);
void prefixSearchPrompt(trienode * root);
void prefixDeletePrompt(trienode * root);
bool upload(trienode * root, char * dictionary);
char * input_string(void);
void spell_check(trienode * root, char * text);
trienode * freeTrie(trienode * root);


void weirdInputs(trienode * root);

int insert_dict_then_delete(trienode * root, char * dictionary);

int insert_dict_then_prefixdelete(trienode * root, char * dictionary);


int wrong_file_format(trienode * root, char * text);


#endif
