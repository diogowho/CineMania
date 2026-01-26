#include "display.h"
#include "movie.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Print table header for movie list */
void printTableHeader(void) {
  printLine(150);
  printf("%-6s | %-40s | %-25s | %-25s | %-6s | %-4s | %-6s | %-8s | %-8s\n",
         "Code", "Title", "Genres", "Director", "Year", "Dur", "Rating",
         "Favorite", "Revenue");
  printLine(150);
}

/* Print a single movie as table row */
void printMovieRow(const Movie *movie) {
  char genresStr[MAX_STRING_LENGTH] = "";
  int i;

  if (movie == NULL) {
    return;
  }

  /* Build genres string */
  for (i = 0; i < movie->genreCount && i < 3; i++) {
    if (i > 0) {
      strcat(genresStr, ", ");
    }
    strncat(genresStr, getGenreName(movie->genres[i]),
            MAX_STRING_LENGTH - strlen(genresStr) - 1);
  }
  if (movie->genreCount > 3) {
    strcat(genresStr, "...");
  }

  /* Truncate title if too long */
  if (strlen(movie->title) > 40) {
    printf(
        "%-6d | %.37s... | %-25s | %-25s | %-6d | %-4d | %6.1f | %8d | %8.2f\n",
        movie->code, movie->title, genresStr, movie->director, movie->year,
        movie->duration, movie->rating, movie->favorite, movie->revenue);
  } else {
    printf("%-6d | %-40s | %-25s | %-25s | %-6d | %-4d | %6.1f | %8d | %8.2f\n",
           movie->code, movie->title, genresStr, movie->director, movie->year,
           movie->duration, movie->rating, movie->favorite, movie->revenue);
  }
}

/* Display movies in table format with optional pagination */
void displayMoviesTable(const MovieDatabase *db, const int *indices, int count,
                        int usePagination) {
  int i;
  int page = 0;
  int totalPages;
  char input[10];

  if (db == NULL || count == 0) {
    printf("No movies to display.\n");
    return;
  }

  if (usePagination) {
    totalPages = (count + LINES_PER_PAGE - 1) / LINES_PER_PAGE;

    while (page * LINES_PER_PAGE < count) {
      clearScreen();
      printHeader("Movie List");
      printf("Page %d of %d (Total movies: %d)\n\n", page + 1, totalPages,
             count);

      printTableHeader();

      /* Display movies for current page */
      for (i = page * LINES_PER_PAGE;
           i < (page + 1) * LINES_PER_PAGE && i < count; i++) {
        if (indices != NULL) {
          printMovieRow(&db->movies[indices[i]]);
        } else {
          printMovieRow(&db->movies[i]);
        }
      }

      printLine(150);

      if ((page + 1) * LINES_PER_PAGE < count) {
        printf("\n[N]ext page | [P]revious page | [Q]uit: ");
        if (fgets(input, sizeof(input), stdin) != NULL) {
          toLowerString(input, input);
          trimString(input);

          if (input[0] == 'n' && page < totalPages - 1) {
            page++;
          } else if (input[0] == 'p' && page > 0) {
            page--;
          } else if (input[0] == 'q') {
            break;
          }
        }
      } else {
        if (page > 0) {
          printf("\n[P]revious page | [Q]uit: ");
          if (fgets(input, sizeof(input), stdin) != NULL) {
            toLowerString(input, input);
            trimString(input);

            if (input[0] == 'p') {
              page--;
            } else {
              break;
            }
          }
        } else {
          pauseScreen();
          break;
        }
      }
    }
  } else {
    /* No pagination - display all */
    printTableHeader();
    for (i = 0; i < count; i++) {
      if (indices != NULL) {
        printMovieRow(&db->movies[indices[i]]);
      } else {
        printMovieRow(&db->movies[i]);
      }
    }
    printLine(150);
  }
}

/* Display detailed information about a single movie */
void displayMovieDetails(const Movie *movie) {
  int i;

  if (movie == NULL) {
    printf("Error: Invalid movie.\n");
    return;
  }

  printHeader("Movie Details");

  printf("Code:        %d\n", movie->code);
  printf("Title:       %s\n", movie->title);

  printf("Genres:      ");
  for (i = 0; i < movie->genreCount; i++) {
    if (i > 0) {
      printf(", ");
    }
    printf("%s", getGenreName(movie->genres[i]));
  }
  printf("\n");

  printf("Description: %s\n", movie->description);
  printf("Director:    %s\n", movie->director);

  printf("Actors:      ");
  for (i = 0; i < movie->actorCount; i++) {
    if (i > 0) {
      printf(", ");
    }
    printf("%s", movie->actors[i]);
  }
  printf("\n");

  printf("Year:        %d\n", movie->year);
  printf("Duration:    %d minutes\n", movie->duration);
  printf("Rating:      %.1f/10\n", movie->rating);
  printf("Favorites:   %d\n", movie->favorite);
  printf("Revenue:     %.2f million\n", movie->revenue);

  printLine(80);
}

/* List all movies with sorting and pagination options */
void listAllMovies(MovieDatabase *db) {
  int sortChoice;
  int paginationChoice;
  SortOrder order;

  if (db == NULL || db->count == 0) {
    printf("No movies in database.\n");
    return;
  }

  printHeader("List All Movies");
  printf("Total movies: %d\n\n", db->count);

  /* Ask for sort order */
  printf("Sort order:\n");
  printf("1. Ascending by code\n");
  printf("2. Descending by code\n");
  sortChoice = readInteger("Choice: ", 1, 2);

  order = (sortChoice == 1) ? SORT_ASCENDING : SORT_DESCENDING;
  sortMoviesByCode(db, order);

  /* Ask for pagination */
  printf("\nUse pagination (%d lines per page)?\n", LINES_PER_PAGE);
  printf("1. Yes\n");
  printf("2. No\n");
  paginationChoice = readInteger("Choice: ", 1, 2);

  printf("\n");
  displayMoviesTable(db, NULL, db->count, paginationChoice == 1);
}

/* Display search results sorted by title */
void displaySearchResults(const MovieDatabase *db, const int *indices,
                          int count) {
  int *sortedIndices;
  int i;

  if (db == NULL || indices == NULL || count == 0) {
    printf("No movies found.\n");
    return;
  }

  /* Create a copy of indices array for sorting */
  sortedIndices = (int *)malloc(count * sizeof(int));
  if (sortedIndices == NULL) {
    printf("Error: Memory allocation failed.\n");
    return;
  }

  for (i = 0; i < count; i++) {
    sortedIndices[i] = indices[i];
  }

  /* Sort by title */
  sortMoviesByTitle(sortedIndices, count, db);

  printHeader("Search Results");
  printf("Found %d movie(s)\n\n", count);

  /* Display results (no pagination for search results) */
  displayMoviesTable(db, sortedIndices, count, 0);

  free(sortedIndices);
}
