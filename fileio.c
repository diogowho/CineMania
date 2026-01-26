#include "fileio.h"
#include "movie.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Replace comma with dot for decimal parsing (CSV uses comma, C uses dot) */
void replaceCommaWithDot(char *str) {
  int i;
  if (str == NULL) {
    return;
  }

  for (i = 0; str[i] != '\0'; i++) {
    if (str[i] == ',') {
      str[i] = '.';
    }
  }
}

/* Replace dot with comma for CSV export */
void replaceDotWithComma(char *str) {
  int i;
  if (str == NULL) {
    return;
  }

  for (i = 0; str[i] != '\0'; i++) {
    if (str[i] == '.') {
      str[i] = ',';
    }
  }
}

/* Parse a single CSV field, handling quotes and semicolon delimiters */
char *parseCSVField(char **linePtr, char *buffer, int bufferSize) {
  char *line = *linePtr;
  int i = 0;

  if (line == NULL || buffer == NULL) {
    return NULL;
  }

  /* Skip leading whitespace */
  while (*line == ' ' || *line == '\t') {
    line++;
  }

  /* Check if field starts with quote */
  if (*line == '"') {
    line++; /* Skip opening quote */

    /* Read until closing quote */
    while (*line != '\0' && i < bufferSize - 1) {
      if (*line == '"') {
        /* Check for escaped quote (double quote) */
        if (*(line + 1) == '"') {
          buffer[i++] = '"';
          line += 2;
        } else {
          /* End of quoted field */
          line++;
          break;
        }
      } else {
        buffer[i++] = *line++;
      }
    }

    /* Skip to semicolon or end of line */
    while (*line != '\0' && *line != ';') {
      line++;
    }
  } else {
    /* Unquoted field - read until semicolon or end of line */
    while (*line != '\0' && *line != ';' && i < bufferSize - 1) {
      buffer[i++] = *line++;
    }
  }

  buffer[i] = '\0';
  trimString(buffer);

  /* Move past the semicolon if present */
  if (*line == ';') {
    line++;
  }

  *linePtr = line;
  return buffer;
}

/* Import movies from CSV file */
int importMoviesFromCSV(MovieDatabase *db, const char *filename) {
  FILE *file;
  char line[4096];
  char field[MAX_DESCRIPTION_LENGTH];
  char *linePtr;
  Movie movie;
  int importedCount = 0;
  int duplicateCount = 0;
  int lineNumber = 0;
  int i;
  char *token;
  char genreBuffer[MAX_STRING_LENGTH];
  char actorBuffer[MAX_DESCRIPTION_LENGTH];
  Genre genre;

  if (db == NULL || filename == NULL) {
    printf("Error: Invalid parameters.\n");
    return 0;
  }

  file = fopen(filename, "r");
  if (file == NULL) {
    printf("Error: Could not open file '%s'.\n", filename);
    return 0;
  }

  printf("Importing movies from '%s'...\n", filename);

  /* Skip header line */
  if (fgets(line, sizeof(line), file) == NULL) {
    printf("Error: File is empty.\n");
    fclose(file);
    return 0;
  }
  lineNumber++;

  /* Read each line */
  while (fgets(line, sizeof(line), file) != NULL) {
    lineNumber++;

    /* Skip empty lines */
    if (strlen(line) <= 1) {
      continue;
    }

    linePtr = line;

    /* Initialise movie structure */
    movie.genreCount = 0;
    movie.actorCount = 0;
    for (i = 0; i < MAX_GENRES_PER_MOVIE; i++) {
      movie.genres[i] = GENRE_NONE;
    }
    for (i = 0; i < MAX_ACTORS_PER_MOVIE; i++) {
      movie.actors[i][0] = '\0';
    }

    /* Parse code */
    parseCSVField(&linePtr, field, sizeof(field));
    movie.code = atoi(field);

    /* Check for duplicate */
    if (movieCodeExists(db, movie.code)) {
      duplicateCount++;
      continue;
    }

    /* Parse title */
    parseCSVField(&linePtr, movie.title, MAX_STRING_LENGTH);

    /* Parse genres */
    parseCSVField(&linePtr, genreBuffer, MAX_STRING_LENGTH);
    token = strtok(genreBuffer, ",");
    while (token != NULL && movie.genreCount < MAX_GENRES_PER_MOVIE) {
      trimString(token);
      genre = getGenreFromString(token);
      if (genre != GENRE_NONE) {
        movie.genres[movie.genreCount++] = genre;
      }
      token = strtok(NULL, ",");
    }

    /* Parse description */
    parseCSVField(&linePtr, movie.description, MAX_DESCRIPTION_LENGTH);

    /* Parse director */
    parseCSVField(&linePtr, movie.director, MAX_STRING_LENGTH);

    /* Parse actors */
    parseCSVField(&linePtr, actorBuffer, MAX_DESCRIPTION_LENGTH);
    token = strtok(actorBuffer, ",");
    while (token != NULL && movie.actorCount < MAX_ACTORS_PER_MOVIE) {
      trimString(token);
      if (strlen(token) > 0) {
        strncpy(movie.actors[movie.actorCount], token,
                MAX_ACTOR_NAME_LENGTH - 1);
        movie.actors[movie.actorCount][MAX_ACTOR_NAME_LENGTH - 1] = '\0';
        movie.actorCount++;
      }
      token = strtok(NULL, ",");
    }

    /* Parse year */
    parseCSVField(&linePtr, field, sizeof(field));
    movie.year = atoi(field);

    /* Parse duration */
    parseCSVField(&linePtr, field, sizeof(field));
    movie.duration = atoi(field);

    /* Parse rating (replace comma with dot for decimal) */
    parseCSVField(&linePtr, field, sizeof(field));
    replaceCommaWithDot(field);
    movie.rating = atof(field);

    /* Parse favorite */
    parseCSVField(&linePtr, field, sizeof(field));
    movie.favorite = atoi(field);

    /* Parse revenue (replace comma with dot for decimal) */
    parseCSVField(&linePtr, field, sizeof(field));
    replaceCommaWithDot(field);
    movie.revenue = atof(field);

    /* Validate and add movie */
    if (validateMovieData(&movie)) {
      if (addMovie(db, &movie)) {
        importedCount++;
      }
    } else {
      printf("Warning: Invalid data on line %d, skipped.\n", lineNumber);
    }
  }

  fclose(file);

  printf("\nImport complete:\n");
  printf("- %d movies imported successfully\n", importedCount);
  if (duplicateCount > 0) {
    printf("- %d duplicate movies skipped\n", duplicateCount);
  }

  return importedCount;
}

/* Export movies to CSV file */
int exportMoviesToCSV(const MovieDatabase *db, const char *filename) {
  FILE *file;
  FILE *testFile;
  int i, j;
  char ratingStr[20];
  char revenueStr[20];

  if (db == NULL || filename == NULL) {
    printf("Error: Invalid parameters.\n");
    return 0;
  }

  if (db->count == 0) {
    printf("Error: No movies to export.\n");
    return 0;
  }

  /* Check if file already exists - portable across all platforms */
  testFile = fopen(filename, "r");
  if (testFile != NULL) {
    fclose(testFile);
    printf("Error: File '%s' already exists. Export cancelled.\n", filename);
    return 0;
  }

  file = fopen(filename, "w");
  if (file == NULL) {
    printf("Error: Could not create file '%s'.\n", filename);
    return 0;
  }

  printf("Exporting %d movies to '%s'...\n", db->count, filename);

  /* Write header */
  fprintf(file, "code;title;genres;description;director;actors;year;duration;"
                "rating;favorite;revenue\n");

  /* Write each movie */
  for (i = 0; i < db->count; i++) {
    const Movie *m = &db->movies[i];

    /* Code */
    fprintf(file, "%d;", m->code);

    /* Title */
    fprintf(file, "%s;", m->title);

    /* Genres (comma-separated) */
    for (j = 0; j < m->genreCount; j++) {
      if (j > 0) {
        fprintf(file, ", ");
      }
      fprintf(file, "%s", getGenreName(m->genres[j]));
    }
    fprintf(file, ";");

    /* Description (with proper quote escaping) */
    if (strchr(m->description, ';') != NULL ||
        strchr(m->description, '"') != NULL) {
      /* Field contains special characters, quote it */
      fprintf(file, "\"");
      for (j = 0; m->description[j] != '\0'; j++) {
        if (m->description[j] == '"') {
          fprintf(file, "\"\""); /* Escape quote */
        } else {
          fprintf(file, "%c", m->description[j]);
        }
      }
      fprintf(file, "\"");
    } else {
      fprintf(file, "%s", m->description);
    }
    fprintf(file, ";");

    /* Director */
    fprintf(file, "%s;", m->director);

    /* Actors (comma-separated) */
    for (j = 0; j < m->actorCount; j++) {
      if (j > 0) {
        fprintf(file, ", ");
      }
      fprintf(file, "%s", m->actors[j]);
    }
    fprintf(file, ";");

    /* Year */
    fprintf(file, "%d;", m->year);

    /* Duration */
    fprintf(file, "%d;", m->duration);

    /* Rating (with comma as decimal separator) */
    snprintf(ratingStr, sizeof(ratingStr), "%.1f", m->rating);
    replaceDotWithComma(ratingStr);
    fprintf(file, "%s;", ratingStr);

    /* Favorite */
    fprintf(file, "%d;", m->favorite);

    /* Revenue (with comma as decimal separator) */
    snprintf(revenueStr, sizeof(revenueStr), "%.2f", m->revenue);
    replaceDotWithComma(revenueStr);
    fprintf(file, "%s\n", revenueStr);
  }

  fclose(file);

  printf("Export complete: %d movies exported to '%s'.\n", db->count, filename);
  return 1;
}
