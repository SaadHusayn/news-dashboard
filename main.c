#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>
#include <ctype.h>

#define NUM_THREADS 4
#define BUFFER_SIZE 20 //20 sentences
#define SENTENCE_SIZE 800 //400 characters size in each sentence
#define SPORTS_FILE "sports.txt"
#define NEWS_FILE "news.txt"
#define STOCKS_FILE "stocks.txt"
#define CRYPTO_FILE "crypto.txt"

pthread_mutex_t sports_mutex;
pthread_mutex_t news_mutex;
pthread_mutex_t stocks_mutex;
pthread_mutex_t crypto_mutex;

sem_t semSportsEmpty;
sem_t semSportsFull;
sem_t semNewsEmpty;
sem_t semNewsFull;
sem_t semStocksEmpty;
sem_t semStocksFull;
sem_t semCryptoEmpty;
sem_t semCryptoFull;


char newsBuffer[BUFFER_SIZE][SENTENCE_SIZE];
char sportsBuffer[BUFFER_SIZE][SENTENCE_SIZE];
char cryptoBuffer[BUFFER_SIZE][SENTENCE_SIZE];
char stocksBuffer[BUFFER_SIZE][SENTENCE_SIZE];

//running state of the program
volatile int running = 1;

// in pointers for producers(threads)
int newsIn = 0;
int sportsIn = 0;
int cryptoIn = 0;
int stocksIn = 0;

// out pointers for consumer(dashboard)
int newsOut = 0;
int sportsOut = 0;
int cryptoOut = 0;
int stocksOut = 0;

void *news_thread(void *arg)
{
    FILE *file = fopen(NEWS_FILE, "r");
    
    if(file == NULL){
        perror("Error opening file\n");
        pthread_exit((void *)EXIT_FAILURE);
    }

    char line[SENTENCE_SIZE];

    while(running && fgets(line, sizeof(line), file)){//while program is running and there are remaining data, produce

        sem_wait(&semNewsEmpty);

        //critical section
        pthread_mutex_lock(&news_mutex);

        //produce
        strncpy(newsBuffer[newsIn], line, strlen(line) + 1);
        newsIn = (newsIn + 1)%BUFFER_SIZE;

        pthread_mutex_unlock(&news_mutex);

        sem_post(&semNewsFull);

    }

    fclose(file);

    pthread_exit((void *)EXIT_SUCCESS);
}

void *sports_thread(void *arg)
{
    FILE *file = fopen(SPORTS_FILE, "r");
    
    if(file == NULL){
        perror("Error opening file\n");
        pthread_exit((void *)EXIT_FAILURE);
    }

    char line[SENTENCE_SIZE];

    while(running && fgets(line, sizeof(line), file)){//while program is running and there are remaining data, produce

        sem_wait(&semSportsEmpty);

        //critical section
        pthread_mutex_lock(&sports_mutex);

        //produce
        strncpy(sportsBuffer[sportsIn], line, strlen(line) + 1);
        sportsIn = (sportsIn + 1)%BUFFER_SIZE;

        pthread_mutex_unlock(&sports_mutex);

        sem_post(&semSportsFull);

    }

    fclose(file);

    pthread_exit((void *)EXIT_SUCCESS);
}

void *crypto_thread(void *arg)
{
    FILE *file = fopen(CRYPTO_FILE, "r");
    
    if(file == NULL){
        perror("Error opening file\n");
        pthread_exit((void *)EXIT_FAILURE);
    }

    char line[SENTENCE_SIZE];

    while(running && fgets(line, sizeof(line), file)){//while program is running and there are remaining data, produce

        sem_wait(&semCryptoEmpty);

        //critical section
        pthread_mutex_lock(&crypto_mutex);

        //produce
        strncpy(cryptoBuffer[cryptoIn], line, strlen(line) + 1);
        cryptoIn = (cryptoIn + 1)%BUFFER_SIZE;

        pthread_mutex_unlock(&crypto_mutex);

        sem_post(&semCryptoFull);

    }

    fclose(file);

    pthread_exit((void *)EXIT_SUCCESS);
}

void *stocks_thread(void *arg)
{
    FILE *file = fopen(STOCKS_FILE, "r");
    
    if(file == NULL){
        perror("Error opening file\n");
        pthread_exit((void *)EXIT_FAILURE);
    }

    char line[SENTENCE_SIZE];

    while(running && fgets(line, sizeof(line), file)){//while program is running and there are remaining data, produce

        sem_wait(&semStocksEmpty);

        //critical section
        pthread_mutex_lock(&stocks_mutex);

        //produce
        strncpy(stocksBuffer[stocksIn], line, strlen(line) + 1);
        stocksIn = (stocksIn + 1)%BUFFER_SIZE;

        pthread_mutex_unlock(&stocks_mutex);

        sem_post(&semStocksFull);

    }

    fclose(file);

    pthread_exit((void *)EXIT_SUCCESS);
}


void printBoxedText(const char* text, int width) {
    int len = strlen(text);
    int i = 0;

    while (i < len) {
        printf("| ");

        int line_chars = 0;
        int last_space = -1;
        int j;

        // Find how much we can print
        for (j = 0; j < width - 4 && (i + j) < len; j++) {
            char c = text[i + j];

            if (c == '\n') {
                break;
            }

            if (isspace(c)) {
                last_space = j;  // Store the last space position
            }
        }

        // Adjust line if word broken at a space
        int print_len = j;
        if (j == width - 4 && last_space != -1) {
            print_len = last_space;  // Break at last space
        }

        // Print the line (text for this line)
        for (int k = 0; k < print_len; k++) {
            printf("%c", text[i + k]);
            line_chars++;
        }

        // Fill spaces to the right if the line is shorter than width
        for (int k = line_chars; k < width - 4; k++) {
            printf(" ");
        }

        printf(" |\n");

        // Move i to next part for the next line
        i += print_len;

        // Skip leading spaces or newlines after wrapping
        while (i < len && (isspace(text[i]) || text[i] == '\n')) {
            i++;
        }
    }
}

 void starting() {

    system("clear");
    char* spacing = "                                                                                ";
    char* dash = "--------------------------------------------------------------------------------";

    printf("\n\n\n\n\n");
    printf("%.*s %s", 40, spacing, "Multi-Threaded News Dashboard");
    fflush(stdout); // <--- force to show text immediately

    for(int i = 0; i < 3; i++) {
        printf(".");
        sleep(1);
        fflush(stdout); // <--- also flush after each dot
    }

    sleep(1);
    system("clear");

    printf("%.*s |%.*s|\n", 10, spacing, 70, dash);
    printf("%.*s | %55s %.*s|\n", 10, spacing, "Welcome to the Multi-Threaded News Dashboard", 13, spacing);
    printf("%.*s |%.*s|\n", 10, spacing, 70, dash);


    printf("%.*s |%.*s|\n", 10, spacing, 70, spacing);
    // printf("\n");
    printf("%.*s | %-68s |\n", 10, spacing, "Stay informed with real-time updates from around the world!");
    
    printf("%.*s |%.*s|\n", 10, spacing, 70, spacing);
    printf("%.*s | %-40s %.*s|\n", 10, spacing, "-Latest News Headlines", 28, spacing);
    printf("%.*s | %-40s %.*s|\n", 10, spacing, "-Live Sports Updates", 28, spacing);
    printf("%.*s | %-40s %.*s|\n", 10, spacing, "-Cryptocurrency Market Updates", 28, spacing);
    printf("%.*s | %-40s %.*s|\n", 10, spacing, "-Stock Market Trends", 28, spacing);

    printf("%.*s |%.*s|\n", 10, spacing, 70, spacing);
    printf("%.*s | %-68s |\n", 10, spacing, "Your personalized dashboard, powered by multi-threading, ensures you");
    printf("%.*s | %-70s |\n", 10, spacing, "never miss a moment — everything delivered fast and smoothly.");
    printf("%.*s |%.*s|\n", 10, spacing, 70, dash);

    printf("\nPress Enter to continue...");
    getchar();
    
}

int main()
{   
    srand(time(NULL));

    //initializing mutex, semaphores and threads

    pthread_mutex_init(&sports_mutex, NULL);
    pthread_mutex_init(&news_mutex, NULL);
    pthread_mutex_init(&stocks_mutex, NULL);
    pthread_mutex_init(&crypto_mutex, NULL);
    
    sem_init(&semSportsEmpty, 0, BUFFER_SIZE);
    sem_init(&semSportsFull, 0, 0);
    sem_init(&semNewsEmpty, 0, BUFFER_SIZE);
    sem_init(&semNewsFull, 0, 0);
    sem_init(&semStocksEmpty, 0, BUFFER_SIZE);
    sem_init(&semStocksFull, 0, 0);
    sem_init(&semCryptoEmpty, 0, BUFFER_SIZE);
    sem_init(&semCryptoFull, 0, 0);

    pthread_t newsT, sportsT, cryptoT, stocksT;

    //(DONE) initializing mutex, semaphores and threads


    // creating threads(producers) and assigning thread routines

    pthread_create(&newsT, NULL, news_thread, NULL);
    pthread_create(&sportsT, NULL, sports_thread, NULL);
    pthread_create(&cryptoT, NULL, crypto_thread, NULL);
    pthread_create(&stocksT, NULL, stocks_thread, NULL);

    //(DONE) creating threads(producers) and assigning thread routines


    //main thread (consumer)

    int choice = 5; // initially all news, sports, crypto, and stocks should be updated

    char current_news[SENTENCE_SIZE];
    char current_sports[SENTENCE_SIZE];
    char current_crypto[SENTENCE_SIZE];
    char current_stocks[SENTENCE_SIZE];

    starting();

    char* spacing = "                                                                                ";
    char* dash = "--------------------------------------------------------------------------------";

    do {

        switch(choice){
            case 1://updating news section

                //fetching news data
                sem_wait(&semNewsFull);
                pthread_mutex_lock(&news_mutex); 
                strncpy(current_news, newsBuffer[newsOut], strlen(newsBuffer[newsOut]) + 1);
                newsOut = (newsOut + 1)%BUFFER_SIZE;
                pthread_mutex_unlock(&news_mutex);
                sem_post(&semNewsEmpty);

                break;
            
            case 2://updating sports section

                //fetching sports data
                sem_wait(&semSportsFull);
                pthread_mutex_lock(&sports_mutex); 
                strncpy(current_sports, sportsBuffer[sportsOut], strlen(sportsBuffer[sportsOut]) + 1);
                sportsOut = (sportsOut + 1)%BUFFER_SIZE;
                pthread_mutex_unlock(&sports_mutex);
                sem_post(&semSportsEmpty);

                break;
            
            case 3://updating crypto section

                //fetching crypto data
                sem_wait(&semCryptoFull);
                pthread_mutex_lock(&crypto_mutex); 
                strncpy(current_crypto, cryptoBuffer[cryptoOut], strlen(cryptoBuffer[cryptoOut]) + 1);
                cryptoOut = (cryptoOut + 1)%BUFFER_SIZE;
                pthread_mutex_unlock(&crypto_mutex);
                sem_post(&semCryptoEmpty);

                break;
            
            case 4://updating stocks section

                //fetching stocks data
                sem_wait(&semStocksFull);
                pthread_mutex_lock(&stocks_mutex); 
                strncpy(current_stocks, stocksBuffer[stocksOut], strlen(stocksBuffer[stocksOut]) + 1);
                stocksOut = (stocksOut + 1)%BUFFER_SIZE;
                pthread_mutex_unlock(&stocks_mutex);
                sem_post(&semStocksEmpty);

                break;
            
            case 5://updating all sections

                //fetching sports data
                sem_wait(&semSportsFull);
                pthread_mutex_lock(&sports_mutex); 
                strncpy(current_sports, sportsBuffer[sportsOut], strlen(sportsBuffer[sportsOut]) + 1);
                sportsOut = (sportsOut + 1)%BUFFER_SIZE;
                pthread_mutex_unlock(&sports_mutex);
                sem_post(&semSportsEmpty);

                //fetching news data
                sem_wait(&semNewsFull);
                pthread_mutex_lock(&news_mutex); 
                strncpy(current_news, newsBuffer[newsOut], strlen(newsBuffer[newsOut]) + 1);
                newsOut = (newsOut + 1)%BUFFER_SIZE;
                pthread_mutex_unlock(&news_mutex);
                sem_post(&semNewsEmpty);

                //fetching stocks data
                sem_wait(&semStocksFull);
                pthread_mutex_lock(&stocks_mutex); 
                strncpy(current_stocks, stocksBuffer[stocksOut], strlen(stocksBuffer[stocksOut]) + 1);
                stocksOut = (stocksOut + 1)%BUFFER_SIZE;
                pthread_mutex_unlock(&stocks_mutex);
                sem_post(&semStocksEmpty);

                //fetching crypto data
                sem_wait(&semCryptoFull);
                pthread_mutex_lock(&crypto_mutex); 
                strncpy(current_crypto, cryptoBuffer[cryptoOut], strlen(cryptoBuffer[cryptoOut]) + 1);
                cryptoOut = (cryptoOut + 1)%BUFFER_SIZE;
                pthread_mutex_unlock(&crypto_mutex);
                sem_post(&semCryptoEmpty);

                break;

            case 6: //exit
                break;

            default:
                printf("\nInvalid input!!!\n");
                printf("Press any key to continue: ");
                fflush(stdin);
                getc(stdin);
                getc(stdin);
        }

        //check if user wants to exit the program
        if(choice == 6){
            running = 0; //change the program's running state to 0 so that threads can terminate
            system("clear");
            printf("\n\n\n\n\n");
            printf("%.*s |%.*s|\n", 10, spacing, 70, dash);
            printf("%.*s | %50s %.*s|\n", 10, spacing, "Multi-Threaded News Dashboard", 18, spacing);
            printf("%.*s |%.*s|\n", 10, spacing, 70, dash);
            printf("%.*s %s",10, spacing,"Exiting...");
            for(int i = 0; i < 3; i++) {
                printf(".");
                sleep(1);
                fflush(stdout); // <--- also flush after each dot
            }
            printf("\n");
            pthread_cancel(newsT);
            pthread_cancel(sportsT);
            pthread_cancel(cryptoT);
            pthread_cancel(stocksT);
            break;//break from forever do while loop
        }

        system("clear");

        printf("|%.*s|\n", 80, dash);
        printf("| %40s %.*s|\n", "*DASHBOARD*", 38, spacing);
        printf("|%.*s|\n", 80, dash);

        printf("| %38s %.*s|\n", "--NEWS--", 40, spacing);
        printBoxedText(current_news, 82);
        printf("|%.*s|\n", 80, dash);

        printf("| %39s %.*s|\n", "--SPORTS--", 39, spacing);
        printBoxedText(current_sports, 82);
        printf("|%.*s|\n", 80, dash);

        printf("| %39s %.*s|\n", "--CRYPTO--", 39, spacing);
        printBoxedText(current_crypto, 82);
        printf("|%.*s|\n", 80, dash);

        printf("| %39s %.*s|\n", "--STOCKS--", 39, spacing);
        printBoxedText(current_stocks, 82);
        printf("|%.*s|\n", 80, dash);

        printf("\n\n");

        printf("|%.*s|\n", 40, dash);
        printf("| %20s %.*s|\n", "MENU", 18, spacing);
        printf("|%.*s|\n", 40, dash);

        printf("| %-38s |", "Press 1 to update News");
        printf("\n");
        printf("| %-38s |", "Press 2 to update Sports");
        printf("\n");
        printf("| %-38s |", "Press 3 to update Crypto");
        printf("\n");
        printf("| %-38s |", "Press 4 to update Stocks");
        printf("\n");
        printf("| %-38s |", "Press 5 to update All");
        printf("\n");
        printf("| %-38s |", "Press 6 to Exit");
        printf("\n");

        printf("|%.*s|\n", 40, dash);

        
        printf("\nYour Choice: ");
        scanf("%d", &choice);
    
    }while(1);

    

    
    //cleaning up threads, mutexes and semaphores

    pthread_join(newsT, NULL);
    pthread_join(sportsT, NULL);
    pthread_join(cryptoT, NULL);
    pthread_join(stocksT, NULL);

    pthread_mutex_destroy(&sports_mutex);
    pthread_mutex_destroy(&news_mutex);
    pthread_mutex_destroy(&crypto_mutex);
    pthread_mutex_destroy(&stocks_mutex);

    sem_destroy(&semCryptoEmpty);
    sem_destroy(&semCryptoFull);
    sem_destroy(&semNewsEmpty);
    sem_destroy(&semNewsFull);
    sem_destroy(&semStocksEmpty);
    sem_destroy(&semStocksFull);
    sem_destroy(&semSportsEmpty);
    sem_destroy(&semSportsFull);

    return 0;
}