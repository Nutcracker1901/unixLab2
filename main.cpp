#include <iostream>
#include <thread>	
#include <mutex>	
#include <condition_variable>	
#include <chrono>

using namespace std;

condition_variable cv;
mutex cv_m;

bool is_provided = true;
int message_counter = 0;

void provide()
{
	while (true) {
		unique_lock<mutex> lk(cv_m);
		
		if (!is_provided)
        {
            lk.unlock();
            continue;
        }

		message_counter++;
		cout << "Provider thread message: " << message_counter << endl;
		
		is_provided = false;
		
		cv.notify_all();
		lk.unlock();		
		
		this_thread::sleep_for(chrono::milliseconds(1000));
	}
}
void consume()
{
	while (true) {
		unique_lock<mutex> lk(cv_m);
		
		cv.wait(lk, [] {return is_provided; });
		
		cout << "Consumer thread message: " << message_counter << endl;
		is_provided = false;
		
		lk.unlock();
	}
}

int main()
{
	thread thread_provider(provide);
	consume();

	return 0;
}