#include "utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Genre name mapping */
static const char *genreNames[] = {
    "Action", "Adventure", "Animation", "Biography", "Comedy",
    "Crime",  "Drama",     "Family",    "Fantasy",   "History",
    "Horror", "Music",     "Musical",   "Mystery",   "Romance",
    "Sci-Fi", "Sport",     "Thriller",  "War",       "Western"};

/* Get genre name from enum */
const char *getGenreName(Genre genre) {
  if (genre >= GENRE_ACTION && genre <= GENRE_WESTERN) {
    return genreNames[genre];
  }
  return "Unknown";
}

/* Convert string to genre enum (case insensitive) */
Genre getGenreFromString(const char *genreStr) {
  int i;
  char lowerStr[MAX_STRING_LENGTH];
  char lowerGenre[MAX_STRING_LENGTH];

  toLowerString(lowerStr, genreStr);

  for (i = GENRE_ACTION; i <= GENRE_WESTERN; i++) {
    toLowerString(lowerGenre, genreNames[i]);
    if (strcmp(lowerStr, lowerGenre) == 0) {
      return (Genre)i;
    }
  }

  return GENRE_NONE;
}

/* Print list of all available genres */
void printGenreList(void) {
  int i;
  printf("\nAvailable Genres:\n");
  printLine(50);
  for (i = GENRE_ACTION; i <= GENRE_WESTERN; i++) {
    printf("%2d. %-15s", i + 1, genreNames[i]);
    if ((i + 1) % 3 == 0) {
      printf("\n");
    }
  }
  printf("\n");
  printLine(50);
}

/* Trim leading and trailing whitespace */
void trimString(char *str) {
  int i, j = 0, len;
  int start = 0, end;

  len = strlen(str);

  /* Find first non-whitespace */
  while (start < len && isspace((unsigned char)str[start])) {
    start++;
  }

  /* Find last non-whitespace */
  end = len - 1;
  while (end >= start && isspace((unsigned char)str[end])) {
    end--;
  }

  /* Copy trimmed string */
  for (i = start; i <= end; i++) {
    str[j++] = str[i];
  }
  str[j] = '\0';
}

/* Convert string to lowercase */
void toLowerString(char *dest, const char *src) {
  int i = 0;
  while (src[i] != '\0') {
    dest[i] = tolower((unsigned char)src[i]);
    i++;
  }
  dest[i] = '\0';
}

/* Case-insensitive substring search */
int containsSubstring(const char *haystack, const char *needle) {
  char lowerHaystack[MAX_DESCRIPTION_LENGTH];
  char lowerNeedle[MAX_STRING_LENGTH];

  toLowerString(lowerHaystack, haystack);
  toLowerString(lowerNeedle, needle);

  return (strstr(lowerHaystack, lowerNeedle) != NULL);
}

/* Read integer with validation */
int readInteger(const char *prompt, int min, int max) {
  int value;
  int valid = 0;

  while (!valid) {
    printf("%s", prompt);
    if (scanf("%d", &value) == 1) {
      while (getchar() != '\n')
        ; /* Clear input buffer */
      if (value >= min && value <= max) {
        valid = 1;
      } else {
        printf("Error: Value must be between %d and %d.\n", min, max);
      }
    } else {
      while (getchar() != '\n')
        ; /* Clear input buffer */
      printf("Error: Invalid input. Please enter a number.\n");
    }
  }

  return value;
}

/* Read float with validation */
float readFloat(const char *prompt, float min, float max) {
  float value;
  int valid = 0;

  while (!valid) {
    printf("%s", prompt);
    if (scanf("%f", &value) == 1) {
      while (getchar() != '\n')
        ; /* Clear input buffer */
      if (value >= min && value <= max) {
        valid = 1;
      } else {
        printf("Error: Value must be between %.2f and %.2f.\n", min, max);
      }
    } else {
      while (getchar() != '\n')
        ; /* Clear input buffer */
      printf("Error: Invalid input. Please enter a number.\n");
    }
  }

  return value;
}

/* Read string with max length */
void readString(const char *prompt, char *buffer, int maxLength) {
  printf("%s", prompt);
  if (fgets(buffer, maxLength, stdin) != NULL) {
    /* Remove newline if present */
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
      buffer[len - 1] = '\0';
    }
    trimString(buffer);
  }
}

/* Read yes/no confirmation */
int readConfirmation(const char *prompt) {
  char response[10];

  printf("%s (y/n): ", prompt);
  if (fgets(response, sizeof(response), stdin) != NULL) {
    trimString(response);
    toLowerString(response, response);
    return (strcmp(response, "y") == 0 || strcmp(response, "yes") == 0);
  }

  return 0;
}

/* Validation functions */
int isValidYear(int year) {
  return year >= 1888 && year <= 2100; /* Cinema started around 1888 */
}

int isValidDuration(int duration) {
  return duration > 0 && duration <= 600; /* Max 10 hours seems reasonable */
}

int isValidRating(float rating) { return rating >= 0.0f && rating <= 10.0f; }

int isValidRevenue(float revenue) { return revenue >= 0.0f; }

/* Display utility functions */
void clearScreen(void) {
  /* Cross-platform clear screen implementation */
#ifdef _WIN32
  system("cls"); /* Windows CMD/PowerShell */
#else
  printf("\033[2J\033[H"); /* Unix/Linux/macOS ANSI escape codes */
#endif
}

void pauseScreen(void) {
  printf("\nPress ENTER to continue...");
  while (getchar() != '\n')
    ;
}

void printLine(int length) {
  int i;
  for (i = 0; i < length; i++) {
    printf("-");
  }
  printf("\n");
}

void printHeader(const char *title) {
  int titleLen = strlen(title);
  int totalLen = 80;
  int padding = (totalLen - titleLen - 2) / 2;
  int i;

  printf("\n");
  printLine(totalLen);

  for (i = 0; i < padding; i++) {
    printf(" ");
  }
  printf(" %s ", title);
  printf("\n");

  printLine(totalLen);
}
