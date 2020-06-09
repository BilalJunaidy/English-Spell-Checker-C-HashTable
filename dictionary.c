// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

//Declaration of a global variable which is going to count up the number of words read in the dictionary
unsigned int dict_words = 0;

//Declaration of the preprocessor directive to be used below (in the hash function as well as in the load function)
#define Num_of_arrays_in_hash_table 765

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

//vARIABLE AND POINTER INITIALIZATIONS
char *word_dict;
FILE *dict_read;
node *new_node;


//Declaration of the compare_text_to_dict function. This is done below the definition of the structure called node.
bool compare_text_to_dict (const char *word, node *tmp);


// Number of buckets in hash table
const unsigned int N = 765;

// Hash table
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{

    int hash_code_text = hash(word);

    //Check if the ith element of the array (represented by the hash code derived from hash_code_text) is currently pointing to a node or not.
    //If it is not, then it will be pointing to NULL and hence, it clearly means that there was no word in the dictionary which returned hash_code_text when ran through the hash function.

    if(table[hash_code_text] == NULL)
    {
        return false;
    }

    else
    {
    node *tmp;
    tmp = table[hash_code_text];

    //The following while loop condition helps make sure that we traverse through the entirety of the linked list for our selected ith element of the array of linked list.
    while(tmp != NULL)
    {

        if(compare_text_to_dict (word, tmp) == true)
        {
            return true;
        }

        tmp = tmp->next;
    }
    //The following condition returns false if the while loop has been completed and we have stll not been able to find our word from the text in the linked list.
    return false;
    }

}

// Hashes word to a number
unsigned int hash(const char *word)
{

    //My very own hash function
    int sum = 0;

    //the following for loop helps convert the first 3 characters of the word to  upper case. If there are any characters that are not alphabetics, it simply returns the same item without any changes.
    for (int i = 0; i < 3; i++)
    {
        if(isalpha(word[i] == 0))
        {
            sum += (int) word[i];
        }
        else if(isupper(word[i] == 0))
        {
            sum += (toupper(word[i]));
        }

        else if(isupper(word[i] != 0))
        {
            sum += (int) word[i];
        }
    }

    return (sum % Num_of_arrays_in_hash_table);

}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{


    //The following for loop initializes each of the N (N = #_of_arrays_in_hash_table) elements of the table to be pointing to NULL;
    for (int i = 0; i < Num_of_arrays_in_hash_table; i++)
    {
        table[i] = NULL;
    }


    //Open file for reading
    dict_read = fopen(dictionary, "r");

    if(dict_read == NULL)
    {
        printf("no memory for dict_read\n");
        return false;
    }

    //VARIABLE AND POINTER INITIALIZATIONS
    word_dict = malloc((1 + LENGTH) * sizeof(char));

    while(feof(dict_read) == 0)
    {
        fscanf(dict_read, "%s" , word_dict);
        dict_words++;


        new_node = malloc(sizeof(node));

        if(new_node == NULL)
        {
            printf("new_node failed\n");
            return false;
        }

        // Declare the values of the node
        strcpy (new_node->word, word_dict);
        new_node->next = NULL;

        //Equating the hash_code into this new int declared hash_code
        int hash_code_dict = hash(word_dict);


        //The following if statement checks if the ith element (interchangeable with the hash_code) of the array is pointing to NULL or not.
        //if it is point to a NULL, this means that this is the first time this hash_code has been outputted from the hash function.

        if(table[hash_code_dict] == NULL)
        {
            table[hash_code_dict] = new_node;
        }

        else
        {
            new_node->next = table[hash_code_dict];
            table[hash_code_dict] = new_node;
        }
    }

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if(&load)
    {
        return (dict_words - 1);
    }
    else
    {
        return 0;
    }

}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{

    for (int i = 0; i < Num_of_arrays_in_hash_table; i++)
    {

        //The following condition is simply checking if the ith element of the array is pointing towards NULL or not. If its not, then this means that there is at least one element in the array of linked list.
        //If it is indeed pointing to NULL, then nothing needs to be freed and hence we move on to the (i + 1)th element of the array.
        if(table[i] != NULL)
        {

        //Declaration of two temporary node *'s.
        node *cursor = table[i];
        node *tmp;

        while(cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }

        }
    }
       free(word_dict);
       free(new_node);
       fclose(dict_read);
       return true;
}


// Function used to compare the word in our text to the word in our linked list
bool compare_text_to_dict (const char *word, node *tmp)
{
    int w = strlen(word);
    int t = strlen(tmp->word);

    //if the length of both of the strings is different, then obviously, they are not going to be match. Hence if condition is true, it returns a false.
    if (w != t)
    {
        return false;
    }

    else if (strcasecmp (word, tmp->word) == 0)
    {
        return true;
    }

    else
    {
        return false;
    }
}