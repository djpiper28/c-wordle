#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ansi_colour.h"

#define WORD_FILE "./words.txt"
#define BUFFER_LENGTH 6

char **read_words(int * length)
{
  int count = 1;
  char **output = malloc(sizeof * output); // array of strings
  char buffer[BUFFER_LENGTH]; // Eeach line of the file
  buffer[BUFFER_LENGTH - 1] = 0;
  
  FILE *f = fopen(WORD_FILE, "r");
  if (f == NULL) {
    printf("Cannot read words file.\n");
    exit(1);
  }

  int index = 0; // how far into a word we are
  for (int c;c = fgetc(f), c != EOF;) {
    if (index < BUFFER_LENGTH - 1) {
      buffer[index] = c;
    }

    if (c == '\n') {
      buffer[index] = 0;
      index = 0;

      output = realloc(output, count * sizeof * output);
      if (output == NULL) {
        printf("Cannot assign more memory for output.\n");
        exit(1);
      }

      output[count - 1] = malloc(BUFFER_LENGTH * sizeof ** output);
      strcpy(output[count - 1], buffer);
      count++;
    }

    index++;
  }

  fclose(f);
  *length = --count;
  return output;
}

char *get_word(char **words, int count)
{
  int t = time(NULL) / (60 * 60 * 24);
  srand(t);
  return words[rand() % count];
}

char *read_word()
{
  char *output = malloc(BUFFER_LENGTH * sizeof * output);
  while (1) {
    puts("Enter a word:");
    printf("> ");
    for (int c, i = 0; c = fgetc(stdin), c != EOF && i < BUFFER_LENGTH; i++) {
      if (c == '\n') {
        if (i != BUFFER_LENGTH - 1) {
          break;
        } else {
          output[BUFFER_LENGTH - 1] = 0;
          return output;
        }
      } else {
        if (c >= 'A' && c < 'a') {
          c = 'a' + c - 'A';
        }
        output[i] = c;
      }
    }
      
    puts("Expected 5 letters try again.");
  }

  exit(1);
  return NULL;
}

void print_word_help(char *word, char *guess)
{
  int seen_before[26];
  memset(seen_before, 0, sizeof(seen_before));

  // Look ahead for exact matches
  for (int i = 0; word[i] != 0 && guess[i] != 0; i++) {
    if (word[i] == guess[i]) {
      seen_before[guess[i] - 'a']++;
    }
  }

  for (int i = 0; word[i] != 0 && guess[i] != 0; i++) {
    printf(ANSI_BACKGROUND_WHITE);
    // Print exact matches
    if (word[i] == guess[i]) {
      printf(ANSI_BACKGROUND_GREEN);
    } else {
      int letter_seen = 0;
      for (int j = 0; word[j] != 0; j++) {
        if (word[j] == guess[i]) {
          letter_seen++;
          if (letter_seen > seen_before[guess[i] - 'a']++) {
            printf(ANSI_BACKGROUND_YELLOW);
            break;
          }
        }
      }
    }

    printf(ANSI_BLACK " %c " ANSI_RESET, guess[i]);
  }
  puts("");
}

int has_won(char *word, char *guess)
{
  return strcmp(word, guess) == 0;
}

int main(int argc, char **argv)
{
  int count = 0;
  char **words = read_words(&count);
  char *word = get_word(words, count);

  int guesses = 0;
  int won = 0;
  while (!won) {
    guesses++;
    char *w_in = read_word();
    print_word_help(word, w_in);    
    won = has_won(word, w_in);
    free(w_in);
  }

  printf("You won in %d guesses\n", guesses);

  for (int i = 0; i < count ; i++) {
    free(words[i]);
  }
  free(words);
}

