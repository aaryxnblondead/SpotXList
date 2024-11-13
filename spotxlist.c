// Remove GTK dependencies since they're causing issues
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct Song {
    char title[100];
    char artist[100];
    char mood[50];
    int duration;
} Song;

typedef struct Node {
    Song song;
    struct Node* next;
    struct Node* prev;
} Node;

typedef struct Playlist {
    char name[100];
    Node* songs;
    struct Playlist* next;
} Playlist;

Node* head = NULL;
Node* currentTrack = NULL;
Playlist* playlists = NULL;

Node* createNode(char* title, char* artist, char* mood, int duration);
void addSong(char* title, char* artist, char* mood, int duration);
void displayAllSongs(void);
void playSong(char* title);
void editSong(char* oldTitle, char* newTitle, char* newArtist, char* newMood);
void deleteSong(char* title);
void nextSong(void);
void previousSong(void);
void createMoodPlaylist(char* mood);
void displayMenu(void);
void clearScreen(void);

Node* createNode(char* title, char* artist, char* mood, int duration) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    strcpy(newNode->song.title, title);
    strcpy(newNode->song.artist, artist);
    strcpy(newNode->song.mood, mood);
    newNode->song.duration = duration;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

void addSong(char* title, char* artist, char* mood, int duration) {
    Node* newNode = createNode(title, artist, mood, duration);
    
    if (head == NULL) {
        head = newNode;
        head->next = head;
        head->prev = head;
    } else {
        Node* last = head->prev;
        last->next = newNode;
        newNode->prev = last;
        newNode->next = head;
        head->prev = newNode;
    }
}

void displayAllSongs() {
    if (head == NULL) {
        printf("\n||  No songs in playlist              ||\n");
        return;
    }

    Node* temp = head;
    int count = 1;
    printf("\n ========= Your Library =========\n");
    do {
        printf("%d. %s - %s (%s) [%d:%.2d]\n", 
            count++, 
            temp->song.title, 
            temp->song.artist, 
            temp->song.mood,
            temp->song.duration / 60,
            temp->song.duration % 60);
        temp = temp->next;
    } while (temp != head);
}

void playSong(char* title) {
    if (head == NULL) {
        printf("\n| Playlist is empty                 |\n");
        return;
    }

    Node* temp = head;
    do {
        if (strcmp(temp->song.title, title) == 0) {
            currentTrack = temp;
            printf("\nNow Playing: %s - %s\n", temp->song.title, temp->song.artist);
            printf("Mood: %s | Duration: %d:%.2d\n", 
                temp->song.mood,
                temp->song.duration / 60,
                temp->song.duration % 60);
            return;
        }
        temp = temp->next;
    } while (temp != head);
    
    printf("\n||  Song not found                    ||\n");
}

void createMoodPlaylist(char* mood) {
    if (head == NULL) {
        printf("\n||  No songs available                ||\n");
        return;
    }

    printf("\n======== %s Mood Playlist ========\n", mood);
    Node* temp = head;
    int count = 1;
    do {
        if (strcmp(temp->song.mood, mood) == 0) {
            printf("%d. %s - %s [%d:%.2d]\n",
                count++,
                temp->song.title,
                temp->song.artist,
                temp->song.duration / 60,
                temp->song.duration % 60);
        }
        temp = temp->next;
    } while (temp != head);
}

void displayMenu() {
    clearScreen();
    printf("\n================================\n");
    printf("           SPOTX                \n");
    printf("================================\n");
    printf("1. Add Song\n");
    printf("2. Edit Song\n");
    printf("3. Delete Song\n");
    printf("4. Play Song\n");
    printf("5. Display Library\n");
    printf("6. Next Track\n");
    printf("7. Previous Track\n");
    printf("8. Create Mood Playlist\n");
    printf("9. Exit\n");
    printf("================================\n");

    if (currentTrack != NULL) {
        printf("\nNow Playing: %s - %s\n", 
            currentTrack->song.title, 
            currentTrack->song.artist);
    }
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void nextSong() {
    if (currentTrack != NULL) {
        currentTrack = currentTrack->next;
        printf("\nNow Playing: %s - %s\n", 
            currentTrack->song.title, 
            currentTrack->song.artist);
    }
}

void previousSong() {
    if (currentTrack != NULL) {
        currentTrack = currentTrack->prev;
        printf("\nNow Playing: %s - %s\n", 
            currentTrack->song.title, 
            currentTrack->song.artist);
    }
}

void editSong(char* oldTitle, char* newTitle, char* newArtist, char* newMood) {
    if (head == NULL) {
        printf("\n||  No songs in playlist              ||\n");
        return;
    }

    Node* temp = head;
    do {
        if (strcmp(temp->song.title, oldTitle) == 0) {
            strcpy(temp->song.title, newTitle);
            strcpy(temp->song.artist, newArtist);
            strcpy(temp->song.mood, newMood);
            printf("\n||  Song updated successfully         ||\n");
            return;
        }
        temp = temp->next;
    } while (temp != head);
    
    printf("\n||  Song not found                    ||\n");
}

void deleteSong(char* title) {
    if (head == NULL) {
        printf("\n||  No songs in playlist              ||\n");
        return;
    }

    Node* temp = head;
    do {
        if (strcmp(temp->song.title, title) == 0) {
            if (temp->next == temp) {
                head = NULL;
            } else {
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                if (temp == head) {
                    head = temp->next;
                }
            }
            if (currentTrack == temp) {
                currentTrack = temp->next;
            }
            free(temp);
            printf("\n||  Song deleted successfully         ||\n");
            return;
        }
        temp = temp->next;
    } while (temp != head);
    
    printf("\n||  Song not found                    ||\n");
}

int main() {
    int choice;
    char title[100], artist[100], mood[50];
    int duration;
    char oldTitle[100], newTitle[100], newArtist[100], newMood[50];

    while (1) {
        displayMenu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear input buffer

        switch (choice) {
            case 1:
                printf("Enter song title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;
                printf("Enter artist name: ");
                fgets(artist, sizeof(artist), stdin);
                artist[strcspn(artist, "\n")] = 0;
                printf("Enter mood: ");
                fgets(mood, sizeof(mood), stdin);
                mood[strcspn(mood, "\n")] = 0;
                printf("Enter duration (in seconds): ");
                scanf("%d", &duration);
                addSong(title, artist, mood, duration);
                break;
            case 2:
                printf("Enter song title to edit: ");
                fgets(oldTitle, sizeof(oldTitle), stdin);
                oldTitle[strcspn(oldTitle, "\n")] = 0;
                printf("Enter new title: ");
                fgets(newTitle, sizeof(newTitle), stdin);
                newTitle[strcspn(newTitle, "\n")] = 0;
                printf("Enter new artist: ");
                fgets(newArtist, sizeof(newArtist), stdin);
                newArtist[strcspn(newArtist, "\n")] = 0;
                printf("Enter new mood: ");
                fgets(newMood, sizeof(newMood), stdin);
                newMood[strcspn(newMood, "\n")] = 0;
                editSong(oldTitle, newTitle, newArtist, newMood);
                break;
            case 3:
                printf("Enter song title to delete: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;
                deleteSong(title);
                break;
            case 4:
                printf("Enter song title to play: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0;
                playSong(title);
                break;
            case 5:
                displayAllSongs();
                break;
            case 6:
                nextSong();
                break;
            case 7:
                previousSong();
                break;
            case 8:
                printf("Enter mood to create playlist: ");
                fgets(mood, sizeof(mood), stdin);
                mood[strcspn(mood, "\n")] = 0;
                createMoodPlaylist(mood);
                break;
            case 9:
                printf("\nThank you for using SpotX!\n");
                return 0;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
        printf("\nPress any key to continue...");
        getch();
    }
    return 0;
}

// Add these new function declarations at the top with the others
char* getCurrentTrackInfo(void);
int getSongCount(void);
void getSongInfo(int index, char* buffer);

// Add these new functions to the file
char* getCurrentTrackInfo() {
    static char info[256];
    if (currentTrack != NULL) {
        snprintf(info, sizeof(info), "%s - %s", 
                currentTrack->song.title, 
                currentTrack->song.artist);
        return info;
    }
    return "No track playing";
}

int getSongCount() {
    if (head == NULL) return 0;
    
    int count = 0;
    Node* temp = head;
    do {
        count++;
        temp = temp->next;
    } while (temp != head);
    return count;
}

void getSongInfo(int index, char* buffer) {
    if (head == NULL || buffer == NULL) {
        strcpy(buffer, "");
        return;
    }
    
    Node* temp = head;
    int current = 0;
    
    do {
        if (current == index) {
            snprintf(buffer, 256, "%s - %s (%s) [%d:%.2d]", 
                    temp->song.title, 
                    temp->song.artist, 
                    temp->song.mood,
                    temp->song.duration / 60,
                    temp->song.duration % 60);
            return;
        }
        current++;
        temp = temp->next;
    } while (temp != head && current <= index);
    
    strcpy(buffer, "");
}
