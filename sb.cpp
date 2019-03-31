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


semaphore barberOn(0);
semaphore waitingRoom(1); //    # to increase or decrease no. of seats if==1
semaphore customerOn(0)     ;  //  # no. of ready customers
int waitingSeats = 3;    //# no. of seats

void *Barber(void *b) {
  while(true) {
    customerOn.Pause();            // # get a customer
    waitingRoom.Pause()       ; // # change no. of seats
    waitingSeats += 1;   
    cout << "Process has started." << endl;  // #printed when barber is working
    sleep(1);
    barberOn.Play();       //  # barber ready
    waitingRoom.Play();       
      } //            

}
void *Customer(void *b) {
  while(true) {
    waitingRoom.Pause();
    if (waitingSeats > 0) {
      waitingSeats--;
      customerOn.Play();
      waitingRoom.Play();
      barberOn.Pause();
    } else {
      waitingRoom.Play();
    }   
  } 

}

int main() 
{ 
    int i; 
    pthread_t thread_id[5]; 
    pthread_create(&thread_id[0], NULL, Customer, NULL); 
    pthread_create(&thread_id[1], NULL, Barber, NULL); 
    pthread_join(thread_id[0], NULL);
    pthread_join(thread_id[1], NULL);
}