#include "movie.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Initialise database to empty state */
void initDatabase(MovieDatabase *db) {
  int i, j;

  if (db == NULL) {
    return;
  }

  db->count = 0;
  db->nextCode = 1;

  /* Initialise all movies to safe defaults */
  for (i = 0; i < MAX_MOVIES; i++) {
    db->movies[i].code = 0;
    db->movies[i].title[0] = '\0';
    db->movies[i].description[0] = '\0';
    db->movies[i].director[0] = '\0';
    db->movies[i].year = 0;
    db->movies[i].duration = 0;
    db->movies[i].rating = 0.0f;
    db->movies[i].favorite = 0;
    db->movies[i].revenue = 0.0f;
    db->movies[i].genreCount = 0;
    db->movies[i].actorCount = 0;

    /* Initialise genre array with GENRE_NONE */
    for (j = 0; j < MAX_GENRES_PER_MOVIE; j++) {
      db->movies[i].genres[j] = GENRE_NONE;
    }

    /* Initialise actor array */
    for (j = 0; j < MAX_ACTORS_PER_MOVIE; j++) {
      db->movies[i].actors[j][0] = '\0';
    }
  }
}

/* Clear all movies from database */
void clearAllMovies(MovieDatabase *db) {
  if (db == NULL) {
    return;
  }

  initDatabase(db);
  printf("All movies cleared successfully.\n");
}

/* Find movie index by code, returns -1 if not found */
int findMovieByCode(const MovieDatabase *db, int code) {
  int i;

  if (db == NULL) {
    return -1;
  }

  for (i = 0; i < db->count; i++) {
    if (db->movies[i].code == code) {
      return i;
    }
  }

  return -1;
}

/* Check if a movie code already exists */
int movieCodeExists(const MovieDatabase *db, int code) {
  return findMovieByCode(db, code) != -1;
}

/* Get next available unique code */
int getNextAvailableCode(const MovieDatabase *db) {
  int code;

  if (db == NULL) {
    return 1;
  }

  code = db->nextCode;

  /* Ensure code is truly unique (in case of imports with higher codes) */
  while (movieCodeExists(db, code)) {
    code++;
  }

  return code;
}

/* Copy movie data from source to destination */
void copyMovie(Movie *dest, const Movie *src) {
  int i;

  if (dest == NULL || src == NULL) {
    return;
  }

  dest->code = src->code;
  strncpy(dest->title, src->title, MAX_STRING_LENGTH - 1);
  dest->title[MAX_STRING_LENGTH - 1] = '\0';

  strncpy(dest->description, src->description, MAX_DESCRIPTION_LENGTH - 1);
  dest->description[MAX_DESCRIPTION_LENGTH - 1] = '\0';

  strncpy(dest->director, src->director, MAX_STRING_LENGTH - 1);
  dest->director[MAX_STRING_LENGTH - 1] = '\0';

  dest->year = src->year;
  dest->duration = src->duration;
  dest->rating = src->rating;
  dest->favorite = src->favorite;
  dest->revenue = src->revenue;

  /* Copy genres */
  dest->genreCount = src->genreCount;
  for (i = 0; i < MAX_GENRES_PER_MOVIE; i++) {
    dest->genres[i] = src->genres[i];
  }

  /* Copy actors */
  dest->actorCount = src->actorCount;
  for (i = 0; i < MAX_ACTORS_PER_MOVIE; i++) {
    strncpy(dest->actors[i], src->actors[i], MAX_ACTOR_NAME_LENGTH - 1);
    dest->actors[i][MAX_ACTOR_NAME_LENGTH - 1] = '\0';
  }
}

/* Validate movie data */
int validateMovieData(const Movie *movie) {
  if (movie == NULL) {
    return 0;
  }

  if (strlen(movie->title) == 0) {
    printf("Error: Movie title cannot be empty.\n");
    return 0;
  }

  if (movie->genreCount <= 0 || movie->genreCount > MAX_GENRES_PER_MOVIE) {
    printf("Error: Movie must have at least one valid genre.\n");
    return 0;
  }

  if (strlen(movie->director) == 0) {
    printf("Error: Director name cannot be empty.\n");
    return 0;
  }

  if (!isValidYear(movie->year)) {
    printf("Error: Invalid year.\n");
    return 0;
  }

  if (!isValidDuration(movie->duration)) {
    printf("Error: Invalid duration.\n");
    return 0;
  }

  if (!isValidRating(movie->rating)) {
    printf("Error: Invalid rating (must be between 0 and 10).\n");
    return 0;
  }

  if (!isValidRevenue(movie->revenue)) {
    printf("Error: Invalid revenue (must be non-negative).\n");
    return 0;
  }

  return 1;
}

/* Add movie to database */
int addMovie(MovieDatabase *db, const Movie *movie) {
  if (db == NULL || movie == NULL) {
    printf("Error: Invalid database or movie.\n");
    return 0;
  }

  if (db->count >= MAX_MOVIES) {
    printf("Error: Database is full (maximum %d movies).\n", MAX_MOVIES);
    return 0;
  }

  if (movieCodeExists(db, movie->code)) {
    printf("Error: Movie with code %d already exists.\n", movie->code);
    return 0;
  }

  if (!validateMovieData(movie)) {
    return 0;
  }

  /* Copy movie to database */
  copyMovie(&db->movies[db->count], movie);
  db->count++;

  /* Update nextCode if necessary */
  if (movie->code >= db->nextCode) {
    db->nextCode = movie->code + 1;
  }

  return 1;
}

/* Interactive movie addition - prompts user for all fields */
int addMovieInteractive(MovieDatabase *db) {
  Movie newMovie;
  char buffer[MAX_DESCRIPTION_LENGTH];
  char genreInput[MAX_STRING_LENGTH];
  char *token;
  Genre genre;
  int i;

  if (db == NULL) {
    return 0;
  }

  if (db->count >= MAX_MOVIES) {
    printf("Error: Database is full (maximum %d movies).\n", MAX_MOVIES);
    return 0;
  }

  printHeader("Add New Movie");

  /* Auto-generate unique code */
  newMovie.code = getNextAvailableCode(db);
  printf("Auto-generated code: %d\n\n", newMovie.code);

  /* Title */
  readString("Title: ", newMovie.title, MAX_STRING_LENGTH);
  if (strlen(newMovie.title) == 0) {
    printf("Error: Title cannot be empty.\n");
    return 0;
  }

  /* Genres - allow retrying on invalid input */
  newMovie.genreCount = 0;
  while (newMovie.genreCount == 0) {
    printf("\n");
    printGenreList();
    printf("\nEnter genres separated by commas (e.g., Action, Drama, Comedy): ");
    if (fgets(genreInput, MAX_STRING_LENGTH, stdin) != NULL) {
      /* Initialise all genre slots to GENRE_NONE */
      for (i = 0; i < MAX_GENRES_PER_MOVIE; i++) {
        newMovie.genres[i] = GENRE_NONE;
      }

      /* Parse comma-separated genres */
      token = strtok(genreInput, ",");
      while (token != NULL && newMovie.genreCount < MAX_GENRES_PER_MOVIE) {
        trimString(token);
        genre = getGenreFromString(token);

        if (genre != GENRE_NONE) {
          newMovie.genres[newMovie.genreCount++] = genre;
        } else {
          printf("Warning: Unknown genre '%s' ignored.\n", token);
        }

        token = strtok(NULL, ",");
      }

      if (newMovie.genreCount == 0) {
        printf("Error: At least one valid genre is required. Please try again.\n");
      }
    }
  }

  /* Description */
  printf("\n");
  readString("Description: ", newMovie.description, MAX_DESCRIPTION_LENGTH);

  /* Director */
  readString("Director: ", newMovie.director, MAX_STRING_LENGTH);
  if (strlen(newMovie.director) == 0) {
    printf("Error: Director cannot be empty.\n");
    return 0;
  }

  /* Actors */
  printf("\nEnter actors separated by commas: ");
  if (fgets(buffer, MAX_DESCRIPTION_LENGTH, stdin) != NULL) {
    newMovie.actorCount = 0;

    /* Initialise all actor slots */
    for (i = 0; i < MAX_ACTORS_PER_MOVIE; i++) {
      newMovie.actors[i][0] = '\0';
    }

    /* Parse comma-separated actors */
    token = strtok(buffer, ",");
    while (token != NULL && newMovie.actorCount < MAX_ACTORS_PER_MOVIE) {
      trimString(token);
      if (strlen(token) > 0) {
        strncpy(newMovie.actors[newMovie.actorCount], token,
                MAX_ACTOR_NAME_LENGTH - 1);
        newMovie.actors[newMovie.actorCount][MAX_ACTOR_NAME_LENGTH - 1] = '\0';
        newMovie.actorCount++;
      }
      token = strtok(NULL, ",");
    }
  }

  /* Year */
  printf("\n");
  newMovie.year = readInteger("Year: ", 1888, 2100);

  /* Duration */
  newMovie.duration = readInteger("Duration (minutes): ", 1, 600);

  /* Rating */
  newMovie.rating = readFloat("Rating (0-10): ", 0.0f, 10.0f);

  /* Favorites */
  newMovie.favorite = readInteger("Favorites count: ", 0, 999999999);

  /* Revenue */
  newMovie.revenue = readFloat("Revenue (millions): ", 0.0f, 999999.0f);

  /* Add movie to database */
  if (addMovie(db, &newMovie)) {
    printf("\nMovie added successfully with code %d!\n", newMovie.code);
    return 1;
  }

  return 0;
}

/* Delete movie by code */
int deleteMovie(MovieDatabase *db, int code) {
  int index, i;

  if (db == NULL) {
    printf("Error: Invalid database.\n");
    return 0;
  }

  index = findMovieByCode(db, code);

  if (index == -1) {
    printf("Error: Movie with code %d not found.\n", code);
    return 0;
  }

  /* Shift all movies after the deleted one */
  for (i = index; i < db->count - 1; i++) {
    copyMovie(&db->movies[i], &db->movies[i + 1]);
  }

  db->count--;

  printf("Movie with code %d deleted successfully.\n", code);
  return 1;
}

/* Edit movie by code - only editable fields as per spec */
int editMovie(MovieDatabase *db, int code) {
  int index, choice;
  Movie *movie;
  char buffer[MAX_STRING_LENGTH];
  char genreInput[MAX_STRING_LENGTH];
  char *token;
  Genre genre;
  int i;

  if (db == NULL) {
    printf("Error: Invalid database.\n");
    return 0;
  }

  index = findMovieByCode(db, code);

  if (index == -1) {
    printf("Error: Movie with code %d not found.\n", code);
    return 0;
  }

  movie = &db->movies[index];

  printHeader("Edit Movie");
  printf("Editing movie: %s (Code: %d)\n\n", movie->title, movie->code);

  /* Note: According to spec, can edit: title, genres, year, duration, rating,
   * favorite, revenue */
  /* Cannot edit: code, description, director, actors */

  printf("What would you like to edit?\n");
  printLine(50);
  printf("1. Title\n");
  printf("2. Genres\n");
  printf("3. Year\n");
  printf("4. Duration\n");
  printf("5. Rating\n");
  printf("6. Favorites count\n");
  printf("7. Revenue\n");
  printf("0. Cancel\n");
  printLine(50);

  choice = readInteger("Choice: ", 0, 7);

  switch (choice) {
  case 1: /* Title */
    readString("New title: ", buffer, MAX_STRING_LENGTH);
    if (strlen(buffer) > 0) {
      strncpy(movie->title, buffer, MAX_STRING_LENGTH - 1);
      movie->title[MAX_STRING_LENGTH - 1] = '\0';
      printf("Title updated successfully.\n");
    }
    break;

  case 2: /* Genres */
    printGenreList();
    printf("\nEnter new genres separated by commas: ");
    if (fgets(genreInput, MAX_STRING_LENGTH, stdin) != NULL) {
      movie->genreCount = 0;

      /* Initialise all genre slots to GENRE_NONE */
      for (i = 0; i < MAX_GENRES_PER_MOVIE; i++) {
        movie->genres[i] = GENRE_NONE;
      }

      /* Parse comma-separated genres */
      token = strtok(genreInput, ",");
      while (token != NULL && movie->genreCount < MAX_GENRES_PER_MOVIE) {
        trimString(token);
        genre = getGenreFromString(token);

        if (genre != GENRE_NONE) {
          movie->genres[movie->genreCount++] = genre;
        } else {
          printf("Warning: Unknown genre '%s' ignored.\n", token);
        }

        token = strtok(NULL, ",");
      }

      if (movie->genreCount > 0) {
        printf("Genres updated successfully.\n");
      } else {
        printf("Error: At least one valid genre is required. Changes not "
               "saved.\n");
        return 0;
      }
    }
    break;

  case 3: /* Year */
    movie->year = readInteger("New year: ", 1888, 2100);
    printf("Year updated successfully.\n");
    break;

  case 4: /* Duration */
    movie->duration = readInteger("New duration (minutes): ", 1, 600);
    printf("Duration updated successfully.\n");
    break;

  case 5: /* Rating */
    movie->rating = readFloat("New rating (0-10): ", 0.0f, 10.0f);
    printf("Rating updated successfully.\n");
    break;

  case 6: /* Favorites */
    movie->favorite = readInteger("New favorites count: ", 0, 999999999);
    printf("Favorites count updated successfully.\n");
    break;

  case 7: /* Revenue */
    movie->revenue = readFloat("New revenue (millions): ", 0.0f, 999999.0f);
    printf("Revenue updated successfully.\n");
    break;

  case 0: /* Cancel */
    printf("Edit cancelled.\n");
    return 0;

  default:
    printf("Invalid choice.\n");
    return 0;
  }

  return 1;
}

/* Search movies by title substring (case insensitive) */
int searchByTitle(const MovieDatabase *db, const char *searchTerm, int *results,
                  int maxResults) {
  int i, count = 0;

  if (db == NULL || searchTerm == NULL || results == NULL) {
    return 0;
  }

  for (i = 0; i < db->count && count < maxResults; i++) {
    if (containsSubstring(db->movies[i].title, searchTerm)) {
      results[count++] = i;
    }
  }

  return count;
}

/* Search movies by genre */
int searchByGenre(const MovieDatabase *db, Genre genre, int *results,
                  int maxResults) {
  int i, j, count = 0;

  if (db == NULL || results == NULL || genre == GENRE_NONE) {
    return 0;
  }

  for (i = 0; i < db->count && count < maxResults; i++) {
    for (j = 0; j < db->movies[i].genreCount; j++) {
      if (db->movies[i].genres[j] == genre) {
        results[count++] = i;
        break;
      }
    }
  }

  return count;
}

/* Search movies by director (case insensitive) */
int searchByDirector(const MovieDatabase *db, const char *director,
                     int *results, int maxResults) {
  int i, count = 0;
  char lowerDirector[MAX_STRING_LENGTH];
  char lowerSearchDirector[MAX_STRING_LENGTH];

  if (db == NULL || director == NULL || results == NULL) {
    return 0;
  }

  toLowerString(lowerSearchDirector, director);

  for (i = 0; i < db->count && count < maxResults; i++) {
    toLowerString(lowerDirector, db->movies[i].director);
    if (strcmp(lowerDirector, lowerSearchDirector) == 0) {
      results[count++] = i;
    }
  }

  return count;
}

/* Search movies by actor (case insensitive) */
int searchByActor(const MovieDatabase *db, const char *actor, int *results,
                  int maxResults) {
  int i, j, count = 0;

  if (db == NULL || actor == NULL || results == NULL) {
    return 0;
  }

  for (i = 0; i < db->count && count < maxResults; i++) {
    for (j = 0; j < db->movies[i].actorCount; j++) {
      if (containsSubstring(db->movies[i].actors[j], actor)) {
        results[count++] = i;
        break;
      }
    }
  }

  return count;
}

/* Comparison function for qsort - ascending order */
static int compareMoviesByCodeAsc(const void *a, const void *b) {
  const Movie *movieA = (const Movie *)a;
  const Movie *movieB = (const Movie *)b;
  return movieA->code - movieB->code;
}

/* Comparison function for qsort - descending order */
static int compareMoviesByCodeDesc(const void *a, const void *b) {
  const Movie *movieA = (const Movie *)a;
  const Movie *movieB = (const Movie *)b;
  return movieB->code - movieA->code;
}

/* Sort movies by code */
void sortMoviesByCode(MovieDatabase *db, SortOrder order) {
  if (db == NULL || db->count == 0) {
    return;
  }

  if (order == SORT_ASCENDING) {
    qsort(db->movies, db->count, sizeof(Movie), compareMoviesByCodeAsc);
  } else {
    qsort(db->movies, db->count, sizeof(Movie), compareMoviesByCodeDesc);
  }
}

/* Helper structure for sorting indices by title */
typedef struct {
  int index;
  const char *title;
} IndexTitlePair;

/* Comparison function for sorting by title */
static int compareByTitle(const void *a, const void *b) {
  const IndexTitlePair *pairA = (const IndexTitlePair *)a;
  const IndexTitlePair *pairB = (const IndexTitlePair *)b;
  char lowerA[MAX_STRING_LENGTH];
  char lowerB[MAX_STRING_LENGTH];

  toLowerString(lowerA, pairA->title);
  toLowerString(lowerB, pairB->title);

  return strcmp(lowerA, lowerB);
}

/* Sort array of indices by movie title (alphabetically) */
void sortMoviesByTitle(int *indices, int count, const MovieDatabase *db) {
  IndexTitlePair *pairs;
  int i;

  if (indices == NULL || db == NULL || count <= 0) {
    return;
  }

  pairs = (IndexTitlePair *)malloc(count * sizeof(IndexTitlePair));
  if (pairs == NULL) {
    printf("Error: Memory allocation failed.\n");
    return;
  }

  /* Create pairs of index and title */
  for (i = 0; i < count; i++) {
    pairs[i].index = indices[i];
    pairs[i].title = db->movies[indices[i]].title;
  }

  /* Sort pairs by title */
  qsort(pairs, count, sizeof(IndexTitlePair), compareByTitle);

  /* Copy sorted indices back */
  for (i = 0; i < count; i++) {
    indices[i] = pairs[i].index;
  }

  free(pairs);
}
