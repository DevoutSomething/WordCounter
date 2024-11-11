#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LENGTH 50   
#define MAX_WORD_COUNT 1000  
#define MAX_PATH_LENGTH 255    

/**
 * @brief Structure representing a word and its frequency count.
 */
struct Word { 
    char word[MAX_WORD_LENGTH];
    int count;
};

/**
 * @brief Structure representing a dynamic list of words and their frequencies.
 */
struct WordList {
    struct Word* words;      
    int size;
    int capacity;
};

/**
 * @brief Removes punctuation from a word and converts it to lowercase.
 *
 * @param[in,out] word The word to be cleaned. Modified in place.
 */
void cleanWord(char *word);

/**
 * @brief Finds the index of a word in an array of Word structs.
 *
 * @param[in] words Array of Word structs.
 * @param[in] size Number of elements in the words array.
 * @param[in] word The word to search for.
 * @return Index of the word if found; -1 otherwise.
 */
int find_word(struct Word words[], int size, char *word);

/**
 * @brief Creates a new WordList with the specified initial capacity.
 *
 * Allocates memory for a WordList struct and its dynamic array of Word structs.
 *
 * @param[in] capacity Initial capacity of the word list.
 * @return Pointer to the newly created WordList, or NULL if allocation fails.
 */
struct WordList* createWordList(int capacity);

/**
 * @brief Frees the memory associated with a WordList.
 *
 * @param[in] list Pointer to the WordList to free.
 */
void freeWordList(struct WordList* list);

/**
 * @brief Adds a word to the list or increments its count if already present.
 *
 * If the word already exists in the list, its count is incremented.
 * If the word is new, it is added to the list. The list resizes if capacity is reached.
 *
 * @param[in,out] list Pointer to the WordList to update.
 * @param[in] word The word to add or increment.
 */
void addOrIncrementWord(struct WordList* list, char* word);


int main() {
    char filePath[MAX_PATH_LENGTH];

    printf("Please enter a valid file relative file path: ");
    if(fgets(filePath, sizeof(filePath), stdin)){
        filePath[strcspn(filePath, "\n")] = '\0';
    }

    struct WordList *return_list = createWordList(MAX_WORD_COUNT);
    if (return_list == NULL) {
        printf("Failed to create word list.\n");
        return 1;
    }

    FILE *pF = fopen(filePath, "r");
    if(pF == NULL){
        printf("Unable to open file!\n");
        freeWordList(return_list);
        return 1;
    }

    char buffer[MAX_WORD_LENGTH];
    while(fscanf(pF, "%s", buffer) == 1){
        cleanWord(buffer);
        if(strlen(buffer) == 0) continue;
        addOrIncrementWord(return_list, buffer);
    }
    fclose(pF);

    printf("Word Frequencies:\n");
    for (int i = 0; i < return_list->size; i++) {
        printf("%s: %d\n", return_list->words[i].word, return_list->words[i].count);
    }

    freeWordList(return_list);
    return 0;
}

void cleanWord(char *word){
    for(int i = 0; word[i]; i++){
        word[i] = tolower(word[i]);
        if(ispunct(word[i])){
            word[i] = '\0';  // Terminate string at punctuation
        }
    }
}

int find_word(struct Word words[], int size, char *word) {
    for(int i = 0; i < size; i++){
        if(strcmp(words[i].word, word) == 0){
            return i; 
        }
    }
    // Word was not found
    return -1;  
}

struct WordList* createWordList(int capacity) {
    struct WordList* list = (struct WordList*) malloc(sizeof(struct WordList));
    if(list == NULL) {
        return NULL;
    }
    list->words = (struct Word*) malloc(capacity * sizeof(struct Word));
    if(list->words == NULL) {
        free(list);
        return NULL;
    }
    list->size = 0;
    list->capacity = capacity;
    return list;  
}

void freeWordList(struct WordList* list) {
    free(list->words);
    free(list);
}

void addOrIncrementWord(struct WordList* list, char* word) {
    if(list->words == NULL) {
        printf("No words were found.\n");
        return;
    } else {
        int index = find_word(list->words, list->size, word);
        if(index != -1) {
            list->words[index].count++;
            printf("Incremented count for word: %s\n", word);
        } else {
            if (list->size == list->capacity) {
                list->capacity *= 2;
                list->words = (struct Word*) realloc(list->words, list->capacity * sizeof(struct Word));
            }
            strcpy(list->words[list->size].word, word);
            list->words[list->size].count = 1;
            list->size++;
            printf("Added a new word: %s\n", word);
        }
    }
}
