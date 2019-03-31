#include <bits/stdc++.h>
#include <queue>
#include <atomic>
#include <unistd.h>
#include <pthread.h>
using namespace std;

class semaphore {
private:
	mutex m;
	atomic<int> s;
public:
	semaphore(int init = 1): s(init) {}
	void Pause() {
		while (1) {
			while (s <= 0);
			m.lock();
			if (s <= 0) {
				m.unlock();
				continue;
			}
			s--;
			m.unlock();
			break;
		} 
	}
	void Play() {
		s++;
	}
};


	
semaphore Cigarette_Smoke(0);
semaphore Smoke_Lighter(0);
semaphore Lighter_Cigarette(0);
semaphore Finish(1);

void *middle(void *a) 
{	
		while (true) {

      Finish.Pause();
      int r = rand() % 3;
      switch( r ) {
            case 0: Cigarette_Smoke.Play();
            break;
            case 1: Smoke_Lighter.Play();
            break;
            case 2: Lighter_Cigarette.Play();
            break;
      }
		} 
}

void *Ajay(void *a)
{
      while(true) {
            Cigarette_Smoke.Pause();
            cout << "Ajay is smoking."<< endl;
            sleep(2);
            Finish.Play();
      }
}

void *Aman(void *b)
{
      while(true) {
            Smoke_Lighter.Pause();
            cout << "Aman is smoking."<< endl;
            sleep(2);
            Finish.Play();
      }
}

void *Arun(void *a)
{
      while(true) {
            Lighter_Cigarette.Pause();
            cout << "Arun is smoking."<< endl;
            sleep(2);
            Finish.Play();
      }
}


int main() 
{ 
    int i; 
    pthread_t thread_id[4]; 
    pthread_create(&thread_id[0], NULL, Ajay, NULL); 
    pthread_create(&thread_id[1], NULL, Aman, NULL); 
    pthread_create(&thread_id[2], NULL, Arun, NULL); 
    pthread_create(&thread_id[3], NULL, middle, NULL); 
    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);
    pthread_join(thread_id[3], NULL);
    pthread_join(thread_id[2], NULL);
}