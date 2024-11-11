#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_WORD_LENGTH 50
#define MAX_WORD_COUNT 1000

struct Word{ 
    char word[MAX_WORD_LENGTH];
    int count;
};

struct WordList{
    struct Word* words;
    int size;
    int capacity;
};

void cleanWord(char *word);
int find_word(struct Word words[], int size, char *word);
struct WordList* createWordList(int capacity);
void freeWordList(struct WordList* list);
void addOrIncrementWord(struct WordList* list, char* word);


int main() {
    struct WordList *return_list= createWordList(MAX_WORD_COUNT);
    FILE *pF = fopen("words.txt", "r");

    if(pF == NULL){
        printf("Unable to open file!\n");
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
            word[i] = '\0';
        }
    }
}

int find_word(struct Word words[], int size, char *word) {
    for(int i = 0; i < size; i++){
        if(strcmp(words[i].word, word) == 0){
            return i;
        }
    }
    //word was not found.
    return -1;

}

WordList* createWordList(int capacity){
    struct WordList* list = (struct WordList*) malloc(sizeof(struct WordList));
    if(list == NULL){
        return NULL;
    }
    list->words = (struct Word*) malloc(capacity * sizeof(struct Word));
    if(list->words == NULL){
        free(list);
        return NULL;
    }
    list->size = 0;
    list->capacity = capacity;
    return list;  
}

void freeWordList(WordList* list){
    free(list->words);
    free(list);
}

void addOrIncrementWord(WordList* list, char* word){
    if(list->words == NULL){
        printf("No words were found. \n");
        return;
    }else{
        int index = find_word(list->words, list->size, word);
        if(index != -1){
            list->words[index].count++;
            printf("Incremented count for word: %s\n", word);
        }else{
            if (list->size == list->capacity) {
                list->capacity *= 2;
                list->words = (struct Word*) realloc(list->words, list->capacity * sizeof(struct Word));
            }
            strcpy(list->words[list->size].word, word);
            list->words[list->size].count = 1;
            list->size++;
            printf("Added a new word %s\n", word);
        }
    }
    
}
