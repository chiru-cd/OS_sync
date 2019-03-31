#include <iostream>
#include <queue>
#include <atomic>
#include <unistd.h>
#include <pthread.h>
#include <mutex>
using namespace std;

class semaphore {
private:
	mutex m;
	atomic<int> s;
public:
	semaphore(int init): s(init) {}
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

mutex mutex_buffer;
semaphore bound_l(0);
semaphore bound_h(10);
std::queue<int> buffer;
int i = 0;

void fillBuffer() {
	int y = i++;
	cout << "Item filled: " << y << endl;
	sleep(2);
	buffer.push(y);
}

void emptyBuffer() {
	cout << "Item to be emptied: " << buffer.front() << " " << buffer.size() << " " << endl;
	sleep(1);
	buffer.pop();
}

void *produce(void *id) 
{
    while (true) 
    {
        bound_h.Pause();
        mutex_buffer.lock();
        fillBuffer();
        cout << "unlocking" << endl;
        mutex_buffer.unlock();
        bound_l.Play();
    }
}

void *consume(void *id) 
{
    while (true) 
    {
        bound_l.Pause();
        mutex_buffer.lock();
        emptyBuffer();
        cout << "unlocking" << endl;
        mutex_buffer.unlock();
        bound_h.Play();
    }
}

int32_t main()
{
	ios::sync_with_stdio(false); cin.tie(NULL); cout.precision(10);
	srand(time(NULL));
	pthread_t producer[2];
	pthread_t consumer[2];
	pthread_create(&producer[0], NULL, produce, NULL);
	pthread_create(&consumer[0], NULL, consume, NULL);
	pthread_create(&producer[1], NULL, produce, NULL);
	pthread_create(&consumer[1], NULL, consume, NULL);
	pthread_exit(NULL);
}
