#include <iostream>
#include <queue>
#include <atomic>
#include <unistd.h>
#include <mutex>
#include <pthread.h>
using namespace std;

class semaphore {
private:
	mutex m;
	atomic<int> s;
public:
	semaphore(int init): s(init) {}
	void Pause() {
		while (true) {
			while (s <= 0);
			m.lock();
			if (s <= 0) { m.unlock(); continue; }
			s--;
			m.unlock();
			break;
		}
	}
	void Play() { 
		s++;
	}
};

mutex m;
semaphore w(1);
std::queue<int> Buffer;
int i = 0;
atomic<int> rc{0};

void fillBuffer() {
	int item = i++;
	printf("%d is filled into Buffer\n", item);   
	fflush(stdout);
	usleep(30000);
	Buffer.push(item);
}

int emptyBuffer() {
	int item = Buffer.front();
	printf("%d emptied from Buffer\n", Buffer.front());
	fflush(stdout);
	usleep(20000);
	Buffer.pop();
	return item;
}

// buffer filling and emptying processor variant


void *Write(void *B)
{
    while (true) 
    {
		w.Pause();
		sleep(2);
		fillBuffer();
		w.Play();
    }
}

void *Read(void *B) 
{
    while (true) 
    {
    	m.lock();
		rc++;
		if (rc == 1) w.Pause();
		m.unlock();
		sleep(1);

		emptyBuffer();

		m.lock();
		rc--;
		if (rc == 0) w.Play();
		m.unlock();
    }
}

int32_t main()
{
	ios::sync_with_stdio(false); cin.tie(NULL); cout.precision(10);
	srand(time(NULL));
	pthread_t Reader;
	pthread_t Writer;
	pthread_create(&Writer, NULL, Write, NULL);
	pthread_create(&Reader, NULL, Read, NULL);
	pthread_exit(NULL);
}
