// ******************************
//
//  author: jiangz_os@sari.ac.cn
//
// ******************************

#include <unistd.h>
#include <cstdlib>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

static const int kItemRepositorySize  = 4 + 1; // task capability equal kItemRepositorySize - 1.

struct ItemRepository {
	size_t read_position;
	size_t write_position;
	std::mutex mtx;
	std::condition_variable repo_not_full;
	std::condition_variable repo_not_empty;
} gItemRepository;

typedef struct ItemRepository ItemRepository;


void ProduceItem(ItemRepository *ir)
{
	std::unique_lock<std::mutex> lock(ir->mtx);
	// item buffer is full, just wait here.
	while(((ir->write_position + 1) % kItemRepositorySize)
			== ir->read_position) {
		std::cout << "Producer is waiting for an empty slot...\n";
		(ir->repo_not_full).wait(lock);
	}

	(ir->write_position)++;

	if (ir->write_position == kItemRepositorySize)
		ir->write_position = 0;

	(ir->repo_not_empty).notify_all();
	lock.unlock();
}

void ConsumeItem(ItemRepository *ir)
{
	std::unique_lock<std::mutex> lock(ir->mtx);
	// item buffer is empty, just wait here.
	while(ir->write_position == ir->read_position) {
		std::cout << "Consumer is waiting for items...\n";
		(ir->repo_not_empty).wait(lock);
	}

	(ir->read_position)++;

	if (ir->read_position >= kItemRepositorySize)
		ir->read_position = 0;

	(ir->repo_not_full).notify_all();
	lock.unlock();
}

void ProducerTask()
{
	while(1) {
		sleep(1);
		ProduceItem(&gItemRepository);

		std::cout << "Producer a item." << std::endl;
		sleep(1);
	}
	std::cout << "Producer thread " << std::this_thread::get_id()
		<< " is exiting..." << std::endl;
}

void ConsumerTask(int argc, char* argv[])
{
	while(1) {
		sleep(1);
		ConsumeItem(&gItemRepository);

		std::cout << "Consumer a item." << std::endl;
		sleep(10);
	}
	std::cout << "Consumer thread " << std::this_thread::get_id()
		<< " is exiting..." << std::endl;
}

void InitItemRepository(ItemRepository *ir)
{
	ir->write_position = 0;
	ir->read_position = 0;
}

int main(int argc, char* argv[])
{
	InitItemRepository(&gItemRepository);

	std::thread producer1(ProducerTask);
	//std::thread producer2(ProducerTask);
	//std::thread producer3(ProducerTask);
	//std::thread producer4(ProducerTask);

	std::thread consumer1(ConsumerTask,argc,argv);
	//std::thread consumer2(ConsumerTask,argc,argv);
	//std::thread consumer3(ConsumerTask,argc,argv);
	//std::thread consumer4(ConsumerTask,argc,argv);

	producer1.join(); consumer1.join();
	//producer2.join(); consumer2.join();
	//producer3.join(); consumer3.join();
	//producer4.join(); consumer4.join();

	return 0;
}          
