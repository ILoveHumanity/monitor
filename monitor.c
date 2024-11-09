#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // создаем мьютекс
pthread_cond_t event_waiting = PTHREAD_COND_INITIALIZER; // создаем переменную состояния
int ready = 0;

// потоковые функции
void* sender_func(void* data){
    while(1) {
        sleep(1); // каждую секунду
        pthread_mutex_lock(&lock); // блокируем мьютекс
        if (ready == 1) // если событие не обработано
        {
            pthread_mutex_unlock(&lock); // разблокируем мьютекс и снова ждем
        }
        else
        {
            ready = 1; // производим событие
            printf("provided\n");
            pthread_cond_signal(&event_waiting); // оповещаем поток-получатель
            pthread_mutex_unlock(&lock); // разблокируем мьютекс
        }
    }
}

void* reciver_func(void* data){
    while(1){
        pthread_mutex_lock(&lock); // блокируем мьютекс
        while (ready == 0)
        {
            pthread_cond_wait(&event_waiting, &lock); // ожидаем оповещения о событии
            printf("awoke\n");
        }
        ready = 0; // обрабатываем событие
        printf("consumed\n");
        pthread_mutex_unlock(&lock); // разблокируем мьютекс
    }
}
 
int main(){
	//создаем идентификаторы потоков
	pthread_t sender;
	pthread_t reciver;
 
	//создаем потоки по идентификаторам и потоковым функциям
	pthread_create(&sender, NULL, sender_func, NULL);
	pthread_create(&reciver, NULL, reciver_func, NULL);
	
	//ожидаем завершения потоков
	pthread_join(sender, NULL);
	pthread_join(reciver, NULL);
	
	
	pthread_mutex_destroy(&lock); // удаляем мьютекс
	pthread_cond_destroy(&event_waiting); // удаляем переменную состояния
	return 0;
}