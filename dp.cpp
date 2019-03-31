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

std::vector<semaphore> S(5);
mutex mu;

int phil_array[5] = { 0, 1, 2, 3, 4 }; 
  
void* phil_function(void* num) 
{ 
    int* i = (int *)num; 
    while (1) {
  		mu.lock();
  		S[*i].Pause();
  		S[(*i + 1) % 5].Pause();
    	mu.unlock();
    	cout << "Philosopher " <<*i<<" is dining" << endl ;
    	sleep(rand()%2);
  		S[*i].Play();
  		S[(*i + 1) % 5].Play();
        usleep((rand()) % 1000 * 1000);
    } 
}
 
int main() 
{ 
    int i; 
    pthread_t thread_id[5]; 
    for (i = 0; i < 5; i++) { 
        pthread_create(&thread_id[i], NULL, phil_function, &phil_array[i]); 
    }
    for (i = 0; i < 5; i++) 
        pthread_join(thread_id[i], NULL); 
}