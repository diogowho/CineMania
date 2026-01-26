#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "movie.h"
#include "display.h"
#include "fileio.h"
#include "utils.h"

/* Function prototypes for menu handlers */
void showMainMenu(void);
void handleListMovies(MovieDatabase* db);
void handleSearchMovies(MovieDatabase* db);
void handleViewMovie(MovieDatabase* db);
void handleAddMovie(MovieDatabase* db);
void handleEditMovie(MovieDatabase* db);
void handleDeleteMovie(MovieDatabase* db);
void handleClearMovies(MovieDatabase* db);
void handleImportMovies(MovieDatabase* db);
void handleExportMovies(MovieDatabase* db);

int main(void) {
    MovieDatabase db;
    int choice;
    int running = 1;
    
    /* Initialise database */
    initDatabase(&db);
    
    printHeader("CineMania - Movie Management System");
    printf("Welcome to CineMania!\n");
    pauseScreen();
    
    /* Main program loop */
    while (running) {
        clearScreen();
        showMainMenu();
        
        choice = readInteger("\nEnter your choice: ", 0, 9);
        printf("\n");
        
        switch (choice) {
            case 1:
                handleListMovies(&db);
                break;
                
            case 2:
                handleSearchMovies(&db);
                break;
                
            case 3:
                handleViewMovie(&db);
                break;
                
            case 4:
                handleAddMovie(&db);
                break;
                
            case 5:
                handleEditMovie(&db);
                break;
                
            case 6:
                handleDeleteMovie(&db);
                break;
                
            case 7:
                handleClearMovies(&db);
                break;
                
            case 8:
                handleImportMovies(&db);
                break;
                
            case 9:
                handleExportMovies(&db);
                break;
                
            case 0:
                if (readConfirmation("Are you sure you want to exit?")) {
                    printf("Thank you for using CineMania!\n");
                    running = 0;
                }
                break;
                
            default:
                printf("Invalid choice. Please try again.\n");
                pauseScreen();
                break;
        }
    }
    
    return 0;
}

/* Display main menu */
void showMainMenu(void) {
    printHeader("Main Menu");
    printf("1. List all movies\n");
    printf("2. Search movies\n");
    printf("3. View movie details\n");
    printf("4. Add new movie\n");
    printf("5. Edit movie\n");
    printf("6. Delete movie\n");
    printf("7. Clear all movies\n");
    printf("8. Import movies from CSV file\n");
    printf("9. Export movies to CSV file\n");
    printf("0. Exit\n");
    printLine(80);
}

/* Menu option 1: List all movies */
void handleListMovies(MovieDatabase* db) {
    clearScreen();
    
    if (db->count == 0) {
        printf("No movies in database.\n");
        pauseScreen();
        return;
    }
    
    listAllMovies(db);
    pauseScreen();
}

/* Menu option 2: Search movies */
void handleSearchMovies(MovieDatabase* db) {
    int searchType;
    char searchTerm[MAX_STRING_LENGTH];
    int results[MAX_MOVIES];
    int resultCount = 0;
    Genre genre;
    int genreChoice;
    
    clearScreen();
    
    if (db->count == 0) {
        printf("No movies in database.\n");
        pauseScreen();
        return;
    }
    
    printHeader("Search Movies");
    printf("Search by:\n");
    printf("1. Title (substring)\n");
    printf("2. Genre\n");
    printf("3. Director\n");
    printf("4. Actor\n");
    printf("0. Cancel\n");
    printLine(50);
    
    searchType = readInteger("Choice: ", 0, 4);
    
    if (searchType == 0) {
        return;
    }
    
    printf("\n");
    
    switch (searchType) {
        case 1: /* Search by title */
            readString("Enter title (or part of it): ", searchTerm, MAX_STRING_LENGTH);
            if (strlen(searchTerm) == 0) {
                printf("Search term cannot be empty.\n");
                pauseScreen();
                return;
            }
            resultCount = searchByTitle(db, searchTerm, results, MAX_MOVIES);
            break;
            
        case 2: /* Search by genre */
            printGenreList();
            genreChoice = readInteger("\nSelect genre (1-20): ", 1, 20);
            genre = (Genre)(genreChoice - 1);
            resultCount = searchByGenre(db, genre, results, MAX_MOVIES);
            break;
            
        case 3: /* Search by director */
            readString("Enter director name: ", searchTerm, MAX_STRING_LENGTH);
            if (strlen(searchTerm) == 0) {
                printf("Director name cannot be empty.\n");
                pauseScreen();
                return;
            }
            resultCount = searchByDirector(db, searchTerm, results, MAX_MOVIES);
            break;
            
        case 4: /* Search by actor */
            readString("Enter actor name: ", searchTerm, MAX_STRING_LENGTH);
            if (strlen(searchTerm) == 0) {
                printf("Actor name cannot be empty.\n");
                pauseScreen();
                return;
            }
            resultCount = searchByActor(db, searchTerm, results, MAX_MOVIES);
            break;
            
        default:
            printf("Invalid choice.\n");
            pauseScreen();
            return;
    }
    
    printf("\n");
    
    if (resultCount > 0) {
        displaySearchResults(db, results, resultCount);
    } else {
        printf("No movies found.\n");
    }
    
    pauseScreen();
}

/* Menu option 3: View movie details */
void handleViewMovie(MovieDatabase* db) {
    int code;
    int index;
    
    clearScreen();
    
    if (db->count == 0) {
        printf("No movies in database.\n");
        pauseScreen();
        return;
    }
    
    code = readInteger("Enter movie code: ", 1, 999999);
    
    index = findMovieByCode(db, code);
    
    if (index == -1) {
        printf("\nMovie with code %d not found.\n", code);
    } else {
        printf("\n");
        displayMovieDetails(&db->movies[index]);
    }
    
    pauseScreen();
}

/* Menu option 4: Add new movie */
void handleAddMovie(MovieDatabase* db) {
    clearScreen();
    
    if (db->count >= MAX_MOVIES) {
        printf("Database is full (maximum %d movies).\n", MAX_MOVIES);
        pauseScreen();
        return;
    }
    
    if (addMovieInteractive(db)) {
        /* Success message already printed by addMovieInteractive */
    }
    
    pauseScreen();
}

/* Menu option 5: Edit movie */
void handleEditMovie(MovieDatabase* db) {
    int code;
    
    clearScreen();
    
    if (db->count == 0) {
        printf("No movies in database.\n");
        pauseScreen();
        return;
    }
    
    code = readInteger("Enter movie code to edit: ", 1, 999999);
    
    printf("\n");
    if (editMovie(db, code)) {
        /* Success message already printed by editMovie */
    }
    
    pauseScreen();
}

/* Menu option 6: Delete movie */
void handleDeleteMovie(MovieDatabase* db) {
    int code;
    int index;
    
    clearScreen();
    
    if (db->count == 0) {
        printf("No movies in database.\n");
        pauseScreen();
        return;
    }
    
    code = readInteger("Enter movie code to delete: ", 1, 999999);
    
    index = findMovieByCode(db, code);
    
    if (index == -1) {
        printf("\nMovie with code %d not found.\n", code);
    } else {
        printf("\nMovie: %s\n", db->movies[index].title);
        if (readConfirmation("Are you sure you want to delete this movie?")) {
            deleteMovie(db, code);
        } else {
            printf("Deletion cancelled.\n");
        }
    }
    
    pauseScreen();
}

/* Menu option 7: Clear all movies */
void handleClearMovies(MovieDatabase* db) {
    clearScreen();
    
    if (db->count == 0) {
        printf("Database is already empty.\n");
        pauseScreen();
        return;
    }
    
    printf("Current number of movies: %d\n\n", db->count);
    
    if (readConfirmation("Are you sure you want to clear all movies?")) {
        clearAllMovies(db);
    } else {
        printf("Operation cancelled.\n");
    }
    
    pauseScreen();
}

/* Menu option 8: Import movies from CSV */
void handleImportMovies(MovieDatabase* db) {
    char filename[MAX_STRING_LENGTH];
    
    clearScreen();
    printHeader("Import Movies");
    
    printf("Current number of movies: %d\n", db->count);
    printf("Available space: %d\n\n", MAX_MOVIES - db->count);
    
    readString("Enter CSV filename (or path): ", filename, MAX_STRING_LENGTH);
    
    if (strlen(filename) == 0) {
        printf("Filename cannot be empty.\n");
        pauseScreen();
        return;
    }
    
    printf("\n");
    importMoviesFromCSV(db, filename);
    
    pauseScreen();
}

/* Menu option 9: Export movies to CSV */
void handleExportMovies(MovieDatabase* db) {
    char filename[MAX_STRING_LENGTH];
    
    clearScreen();
    printHeader("Export Movies");
    
    if (db->count == 0) {
        printf("No movies to export.\n");
        pauseScreen();
        return;
    }
    
    printf("Number of movies to export: %d\n\n", db->count);
    
    readString("Enter CSV filename (or path): ", filename, MAX_STRING_LENGTH);
    
    if (strlen(filename) == 0) {
        printf("Filename cannot be empty.\n");
        pauseScreen();
        return;
    }
    
    printf("\n");
    exportMoviesToCSV(db, filename);
    
    pauseScreen();
}
