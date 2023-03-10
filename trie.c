#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "trie.h"


int COUNT = 0; // number of elements in trie


int main(int argc, char * argv[])
{
    // driver code
    if (argc != 2 && argc != 3)
    {
        printf("Please use as follows: ./trie [DICTIONARY] [TEXT] or ./trie [DICTIONARY]\n");
        return 1;
    }
    trienode * root = initTrie();
    char * dictionary = argv[1]; 


    bool uploaded = upload(root, dictionary);
    if (uploaded == false)
    {
        printf("Could not upload %s\n", dictionary);
        return 1;
    }

    printTrie(root);

    if (argc == 3)
    {
        char * sampletext = argv[2];
        spell_check(root, sampletext);
    }


    prefixSearchPrompt(root);
    prefixDeletePrompt(root);

    freeTrie(root);
}


// main function for running tests
// int main(int argc, char * argv[]) 
// {
//     if (argc != 2 && argc != 3)
//     {
//         printf("Please use as follows: ./trie [DICTIONARY] [TEXT] or ./trie [DICTIONARY]\n");
//         return 1;
//     }
//     trienode * root = initTrie();
//     char * dictionary = argv[1]; 


//     // TESTS (Please run each separately)

//     //insert_dict_then_delete(root, dictionary);
//     //insert_dict_then_prefixdelete(root, dictionary);
    


// }

trienode * initTrie()                                                  
{
    trienode * root = malloc(sizeof(struct trienode));

    //check whether malloc successful
    if (root == NULL)
    {
        printf("Error: Malloc failed\n");
        return root;
    }

    for (int i = 0; i < N; i++)
    {
        root->next[i] = NULL;
        root->endofword = false;
    }

    return root;

}


// the following code is for input which is made of english characters along with apostrophe


//inserts string of characters into trie
trienode * insertTrie(trienode * root, char * word)
{
    if (root == NULL)
    {
        root = initTrie(); // root will be NULL only when all elements are deleted from a trie, then we need to assign root to insertTrie()
    }

    trienode * temp = root;
    int length = strlen(word);
    if (length > MAXWORDSIZE || length <= 0)
    {
        printf("Length of string should be >= 1 and <= %i\n", MAXWORDSIZE);
        printf("Error string: '%s'\n\n", word);
        return root;
    }

    for (int i = 0; i < length; i++)
    {
        if (isalpha(word[i]) == 0 && word[i] != '\'') // if a char is not alphabetical and not apostrophe
        {
            printf("Word must consist of english alphabets\n"); 
            printf("Error string: '%s'\n\n", word);
            return root;
        }

        word[i] = tolower(word[i]);                 // converts everything to lowercase

        assert((word[i] >= 'a' && word[i] <= 'z') || word[i] == '\'');
        
        int index;
        if (word[i] == '\'') // apostrophe
        {
            index = 26; // apostrophe is mapped to the last index
        }
        else
        {
            index = word[i] - 97;                   // 97 corresponds to 'a' so if word[i] is z (ascii 122) then index will be 122-97 = 25
        }

        if (temp->next[index] == NULL)              // if the letter value does not exist in the trienode
        {
            temp->next[index] = initTrie();         // create new trienode corresponding to the index
            temp = temp->next[index];               // temp points to this new trie node

            if (i == length - 1)                    // if we reached end of a string
            {
                temp->endofword = true;
            }
        }

        else if (temp->next[index]) // if the letter value exists then move on to the next node
        {
            temp = temp->next[index];

            if (i == length - 1)
            {
                temp->endofword = true; // for example if "antique" already there and we are inserting "anti"
            }
        }

    }

    COUNT++;
    return root;

}

// searches for a word in the trie
bool searchTrie(trienode * root, char * word)
{
    trienode * temp = root;
    int length = strlen(word);

    for (int i = 0; i < length; i++)
    {
        if (isalpha(word[i]) == 0 && word[i] != '\'')
        {
            printf("Word must consist of english alphabets\n");
            printf("Error string: '%s'\n", word);
            return false;
        }  

        word[i] = tolower(word[i]);

        assert((word[i] >= 'a' && word[i] <= 'z') || word[i] == '\'');

        int index;
        if (word[i] == '\'') // apostrophe
        {
            index = 26;
        }
        else
        {
            index = word[i] - 97;         
        }


        if (temp->next[index]) // if it exists
        {
            if (i == length - 1 && temp->next[index]->endofword == true)  // if we reached end of input string and the boolean endofword marker is true
            {
                printf("\nYES, the string '%s' exists in the trie\n", word);
                return true;
            }

            else if (i == length - 1 && temp->next[index]->endofword == false) // if we reached end of string and boolean endofword marker is false
            {
                printf("\nNO, the string '%s' does not exist in the trie\n", word);
                return false;
            }

            else if(i != length - 1)                                           // if we haven't reached end of string yet
            {
                temp = temp->next[index];
            }

        }
        else // if the letter value does not exist
        {
            printf("\nNO, the string '%s' does not exist in the trie\n", word);
            return false;
        }


    }

}

bool simple_search_trie(trienode * root, char * word) // just returns true or false based on whether word is in trie or not, without printing anything
{
    trienode * temp = root;
    int length = strlen(word);

    for (int i = 0; i < length; i++)
    {
        if (isalpha(word[i]) == 0 && word[i] != '\'')
        {
            return false;
        }

        word[i] = tolower(word[i]);
        assert((word[i] >= 'a' && word[i] <= 'z') || word[i] == '\'');

        int index;
        if (word[i] == '\'') // apostrophe
        {
            index = 26;
        }
        else
        {
            index = word[i] - 97;                   // 97 corresponds to 'a' so if word[i] is z (ascii 122) then index will be 122-97 = 25
        }

        if (temp->next[index])
        {
            if (i == length - 1 && temp->next[index]->endofword == true)  // if we reached end of input string and the boolean endofword marker is true
            {

                return true;
            }

            else if (i == length - 1 && temp->next[index]->endofword == false) // if we reached end of string and boolean endofword marker is false
            {

                return false;
            }

            else if(i != length - 1)                                           // if we haven't reached end of string yet
            {
                temp = temp->next[index];
            }

        }
        else
        {

            return false; // if the node corresponding to letter does not exist
        }

    }

}

// checks if a trienode has at least one child or not
bool no_children(trienode * root)
{
    for (int i = 0; i < N; i++)
    {
        if (root->next[i] != NULL) // if any one of the children is not NULL then the trienode has at least one child
        {
            return false;
        }
        else
        {
            continue;
        }
    }

    return true;
}


// deletes a word from the trie. Called with trie_level = 0
trienode * delete_from_trie(trienode * root, char * word, int trie_level) 
{
    if (root == NULL)
    {
        printf("Root is NULL\n");
        return root;
    }

    if (trie_level == 0 && !simple_search_trie(root, word)) // searching for the word from the root
    {
        printf("The word '%s' is not in the trie\n", word);
        return root;
    }

    int length = strlen(word);

    if (trie_level == length)               // if we reached last character in given word
    {
        if (root->endofword == true)
        {
            root->endofword = false;        // setting endofword marker to false effectively deletes word from trie
            COUNT--;
        }

        if (no_children(root))              // if the trienode has no children we can just free it and set it to NULL
        {
            free(root);
            root = NULL;
        }

        // if the trienode has children and we reached the end of the word then nothing else to do since the endofword flag has already been set to false

        return root;
    }

    // if we haven't reached the last character in the given word
    int index;
    assert((word[trie_level] >= 'a' && word[trie_level] <= 'z') || word[trie_level] == '\'');

    if (word[trie_level] == '\'') // apostrophe
    {
        index = 26;
    }
    else
    {
        index = word[trie_level] - 97;
    }

    root->next[index] = delete_from_trie(root->next[index], word, trie_level + 1); // recursively delete

    if (no_children(root) && root->endofword == false) // accounting for trienodes whose children have been deleted
    {
        free(root);
        root = NULL;
    }

    return root;

}



bool is_leaf(trienode * root)
{
    if (root->endofword == true)
    {
        return true;
    }
    else
    {
        return false;
    }

}

void print_trie_helper(trienode * root, char buffer[], int level)
{

    if (root == NULL)
    {
        return;
    }

    // if node is a leaf node, it indicates it is the end of some word so null char is added and word is printed

    if (is_leaf(root))
    {
        buffer[level] = '\0';
        printf("%s\n", buffer);
    }
    
    for (int i = 0; i < N; i++)
    {
        // if a child exists then add the corresponding parent character to the buffer
        if (root->next[i])
        {
            if (i == 26)
            {
                buffer[level] = '\'';
            }
            else
            {
                buffer[level] = i + 'a';                                // 'a' corresponds to 97
            }
            print_trie_helper(root->next[i], buffer, level + 1); // calling print function recursively on children
        }
    }
}

// prints trie in lexicographic order (Here alphabetical and with the apostrophe being the last character in the order after 'z')
void printTrie(trienode * root)
{
    if (root == NULL)
    {
        printf("\nRoot of trie is NULL\n");
        return;
    }
    printf("\nTrie: \n");
    char buffer[MAXWORDSIZE+1];
    print_trie_helper(root, buffer, 0); // since we start from level 0 of the trie 
}



// given a string input like "ca", it finds all the words starting with those letters
bool prefix_search(trienode * root, char * word) 
{
    if (root == NULL)
    {
        return false;
    }

    trienode * temp = root;
    int length = strlen(word);

    for (int i = 0; i < length; i++)
    {
        if (isalpha(word[i]) == 0 && word[i] != '\'')
        {
            printf("Prefix must contain all alphabetical english letters\n");
            return false;
        }

        word[i] = tolower(word[i]);

        assert((word[i] >= 'a' && word[i] <= 'z') || word[i] == '\'');

        int index;
        if (word[i] == '\'') // apostrophe
        {
            index = 26;
        }
        else
        {
            index = word[i] - 97;
        }

        if (temp->next[index])
        {
            if (i == length - 1)
            {   
                printf("\nWords starting with prefix '%s' in dictionary: \n", word);
                prefix_print(temp->next[index], word);
                return true;
            }

            else if (i != length - 1)
            {
                temp = temp->next[index];
            }
        }
        else
        {
            printf("\nPrefix '%s' does not exist\n", word);
            return false;
        }

    }

}

// prints words starting with given prefix
void prefix_print(trienode * temp, char * prefix)
{
    int length = strlen(prefix);
    char buffer[MAXWORDSIZE + 1 + length];

    for (int i = 0; i < length; i++)
    {
        buffer[i] = prefix[i];
    }                                       

    // now the buffer has the prefix and can be used to add all the suffixes in the helper function

    helper_prefix_print(temp, length, buffer);
}


void helper_prefix_print(trienode * root, int level, char buffer[])
{
    if (is_leaf(root))
    {
        buffer[level] = '\0';
        printf("%s\n", buffer);
    }
    
    for (int i = 0; i < N; i++)
    {
        if (root->next[i])
        {
            if (i == 26)
            {
                buffer[level] = '\'';
            }
            else
            {
                buffer[level] = i + 'a';                                
            }
            print_trie_helper(root->next[i], buffer, level + 1);
        }
    }

}

// delete all words starting with given prefix
int prefix_delete(trienode * root, char * prefix) 
{
    if (root == NULL)
    {
        return 1;
    }

    trienode * temp = root;
    int length = strlen(prefix);

    for (int i = 0; i < length; i++)
    {
        if (isalpha(prefix[i]) == 0 && prefix[i] != '\'')
        {
            printf("Prefix must contain all alphabetical english letters\n");
            return 2;
        }

        prefix[i] = tolower(prefix[i]);

        assert((prefix[i] >= 'a' && prefix[i] <= 'z') || prefix[i] == '\'');

        int index;
        if (prefix[i] == '\'') // apostrophe
        {
            index = 26;
        }
        else
        {
            index = prefix[i] - 97;
        }

        if (temp->next[index])
        {
            if (i == length - 1)
            {
                freeTrie(temp->next[index]);
                temp->next[index] = NULL;
                
                return 0;
            }

            else if (i != length - 1)
            {
                temp = temp->next[index];
            }
        }

        else
        {
            printf("Prefix '%s' does not exist\n", prefix);
            return 3;
        }

    }

}



// upload the dictionary into the trie word by word
bool upload(trienode * root, char * dictionary) 
{
    if (wrong_file_format(root, dictionary) == 0) // check whether the file extension is .txt
    {
        return false;
    }
    
    int len = strlen(dictionary);
    assert(dictionary[len-1] == 't'); 
    assert(dictionary[len-2] == 'x');
    assert(dictionary[len-3] == 't');
    assert(dictionary[len-4] == '.');

    FILE *dict = fopen(dictionary, "r");

    if (dict == NULL)
    {
        printf("Could not open %s\n", dictionary);
        return false;
    }

    char word_buffer[MAXWORDSIZE+1]; 

    while(fscanf(dict, "%s", word_buffer) != EOF) // scans and puts words in buffer word by word until EOF is reached
    {
        insertTrie(root, word_buffer);
    }

    fclose(dict);
    return true;

}

trienode * freeTrie(trienode * root)
{
    if (root == NULL)
    {
        return root;
    }

    for (int i = 0; i < N; i++)
    {
        if (root->next[i] != NULL)
        {
            freeTrie(root->next[i]); // recursively free children
        }
    }

    free(root);                      // after freeing children, free the root and set it to NULL
    root = NULL;

    return root;
}


// reads input string character by character to save space and checks for validity 
char * input_string(void)                                       
{
    char * buffer = malloc(sizeof(char));
    int c;
    int buffer_size = 0;
    int buffer_cap = 0;

     while ((c = fgetc(stdin)) != '\r' && c != '\n' && c != EOF)
    {
        
        if (buffer_size + 1 > buffer_cap) // dynamically increase buffer size if it is needed
        {
            
            if (buffer_cap < MAXWORDSIZE + 1) // increase buffer cap if possible
            {
                buffer_cap++;
            }
            else
            {
                free(buffer);
                return NULL;
            }

            
            char * temp = realloc(buffer, buffer_cap); // reallocate memory to increase buffer capacity
            if (temp == NULL)
            {
                free(buffer);
                return NULL;
            }
            buffer = temp;
        }

        
        buffer[buffer_size++] = c; // append current char to buffer and then increments buffer size
    }

    // check if no input given
    if (buffer_size == 0 && c == EOF)
    {
        return NULL;
    }

    if (buffer_size == MAXWORDSIZE + 1)
    {
        free(buffer);
        return NULL;
    }

    char * mystring = realloc(buffer, buffer_size + 1);
    if (mystring == NULL)
    {
        free(buffer);
        return NULL;
    }
    mystring[buffer_size] = '\0';
    return mystring;
}

void prefixSearchPrompt(trienode * root)
{
    printf("Enter prefix: ");
    char * prefix = input_string(); 

    if (prefix == NULL)
    {
        printf("Invalid input\n");
        return;
    }
    printf("Prefix: %s\n", prefix);
    bool search = prefix_search(root, prefix);
    free(prefix);

}

void prefixDeletePrompt(trienode * root)
{
    printf("Delete all words starting with given prefix: ");
    char * prefix = input_string(); 

    if (prefix == NULL)
    {
        printf("Invalid input\n");
        return;
    }
    printf("Prefix: %s\n", prefix);
    bool search = prefix_search(root, prefix);

    prefix_delete(root, prefix);
    printf("\nAfter deletion: \n");
    prefix_search(root, prefix);
    printTrie(root);

    free(prefix);

}


void spell_check(trienode * root, char * text)
{
    int len = strlen(text);

    if (wrong_file_format(root, text) == 0)
    {
        return;
    }
    
    assert(text[len-1] == 't'); // check whether the file extension is .txt
    assert(text[len-2] == 'x');
    assert(text[len-3] == 't');
    assert(text[len-4] == '.');

    FILE *samptext = fopen(text, "r");

    if (samptext == NULL)
    {
        printf("\nCouldn't open or find text file to check\n");
        return;
    }

    int index = 0;
    int  wrongspellings = 0;
    int  words = 0;
    char word[MAXWORDSIZE];
    char c;

    printf("\nWords misspelled:\n");

    while (fread(&c, sizeof(char), 1, samptext))
    {
        if (isalpha(c) || (c == '\'' && index > 0)) // apostrophe can't be the first character in correctly spelled word in this case
        {
            // make the word character by character
            word[index] = c;
            index++;

            
            if (index > MAXWORDSIZE-1) // ignore words that are too big
            {
                while (fread(&c, sizeof(char), 1, samptext) && isalpha(c)); // goes to next word
                index = 0; // Prepare for new wordx
            }
        }

        // ignore words with numbers
        else if (isdigit(c))
        {
            while (fread(&c, sizeof(char), 1, samptext) && isalpha(c)); // goes to next word
            index = 0; 
        }

        else if (index > 0) // we have found a whole word
        {
            word[index] = '\0';
            words++;

            // Print word if wrong spelling and increment number of wrong spellings
            if (simple_search_trie(root, word) == false)
            {
                printf("%s\n", word);
                wrongspellings++;
            }
        
            index = 0;
        }
    }

    fclose(samptext);

    printf("\nNumber of words misspelled:    %d\n", wrongspellings);
    printf("Number of words in dictionary: %d\n", COUNT);
    printf("Number of words in the text:   %d\n", words);

    return;

}




// TESTS




int insert_dict_then_delete(trienode * root, char * dictionary)
{   
    bool uploaded = upload(root, dictionary);
    if (uploaded == false)
    {
        printf("Could not upload %s\n", dictionary);
        return 1;
    }
    //printTrie(root);
    freeTrie(root);
    root = NULL;
    printf("\nAfter deletion of trie:\n");
    printTrie(root);
    return 0;
}


int insert_dict_then_prefixdelete(trienode * root, char * dictionary)
{
    bool uploaded = upload(root, dictionary);
    if (uploaded == false)
    {
        printf("Could not upload %s\n", dictionary);
        return 1;
    }
    prefixDeletePrompt(root);
}



int wrong_file_format(trienode * root, char * text)
{
    int length = strlen(text);    
    for (int i = 0; i < length; i++)
    {
        text[i] = tolower(text[i]);
    }

    int c1 = text[length-1];
    int c2 = text[length-2];
    int c3 = text[length-3];
    int c4 = text[length-4];

    if (c1 == 116 && c2 == 120 && c3 == 116 && c4 == 46)
    {
        return 1;
    }

    else 
    {
        printf("\nPlease use a .txt file\n");
        return 0;
    }
    
}

