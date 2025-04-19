#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUM_THREADS 4

volatile int running = 1;

typedef struct
{
    char news[256];
    char sports[256];
    char crypto[256];
    char stocks[256];
} DashboardData;

DashboardData sharedData;

pthread_mutex_t data_mutex;

void *news_thread(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&data_mutex);
        snprintf(sharedData.news, sizeof(sharedData.news), "Breaking: AI takes over OS projects!");
        pthread_mutex_unlock(&data_mutex);
        sleep(3 + (rand() % 2)); // Sleep 3 to 4 seconds randomly
    }
    return NULL;
}

void *sports_thread(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&data_mutex);
        snprintf(sharedData.sports, sizeof(sharedData.sports), "KXIP beats RCB with 5 wickets in low scoring IPL thriller!");
        pthread_mutex_unlock(&data_mutex);
        sleep(3 + (rand() % 2)); // Sleep 3 to 4 seconds randomly
    }
    return NULL;
}

void *crypto_thread(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&data_mutex);
        snprintf(sharedData.crypto, sizeof(sharedData.crypto), "BTC currently stands 84,975 USDT!");
        pthread_mutex_unlock(&data_mutex);
        sleep(3 + (rand() % 2)); // Sleep 3 to 4 seconds randomly
    }
    return NULL;
}

void *stocks_thread(void *arg)
{
    while (running)
    {
        pthread_mutex_lock(&data_mutex);
        snprintf(sharedData.stocks, sizeof(sharedData.stocks), "SSGC share price rose to Rs42.47");
        pthread_mutex_unlock(&data_mutex);
        sleep(3 + (rand() % 2)); // Sleep 3 to 4 seconds randomly
    }
    return NULL;
}

int main()
{   
    srand(time(NULL));


    pthread_mutex_init(&data_mutex, NULL);

    pthread_t newsT, sportsT, cryptoT, stocksT;

    pthread_create(&newsT, NULL, news_thread, NULL);
    pthread_create(&sportsT, NULL, sports_thread, NULL);
    pthread_create(&cryptoT, NULL, crypto_thread, NULL);
    pthread_create(&stocksT, NULL, stocks_thread, NULL);

    char input[10];
    while (1) {
        pthread_mutex_lock(&data_mutex);
        printf("\n====== DASHBOARD ======\n");
        printf("News   : %s\n", sharedData.news);
        printf("Sports : %s\n", sharedData.sports);
        printf("Crypto : %s\n", sharedData.crypto);
        printf("Stocks : %s\n", sharedData.stocks);
        pthread_mutex_unlock(&data_mutex);
    
        printf("\nPress ENTER to refresh or type 'exit' to quit: ");
        fgets(input, sizeof(input), stdin);
    
        if (strncmp(input, "exit", 4) == 0) {
            running = 0; // Signal threads to stop
            printf("Exiting dashboard...\n");
            break;
        }
    
        system("clear");
        sleep(1);
    }
    

    pthread_join(newsT, NULL);
    pthread_join(sportsT, NULL);
    pthread_join(cryptoT, NULL);
    pthread_join(stocksT, NULL);

    pthread_mutex_destroy(&data_mutex);

    return 0;
}
