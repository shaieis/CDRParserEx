/*
	Author: Shai Eisenbud
*/


#include <iostream>
#include <fstream>
#include <thread>
#include <vector>

#include "Parser.h"
#include "Queue.h"
#include "ThreadFunc.h"
#include "DB.h"
#include "OperatorDBRec.h"

constexpr size_t MAX_QUEUE_SIZE = 10000; // the size in which to close the queue for the parser to insert
constexpr size_t REOPEN_QUEUE_SIZE = 5000; // the size in which to reopen the queue for the parser to insert
constexpr size_t MIN_NUM_OF_THREADS = 5;

std::string getFilePath(int argc, char** argv)
{
	std::string path;
	switch (argc)
	{
	case 1:
		std::cout << "Please enter a path to a CDR file: ";
		std::cin >> path;
		break;
	case 2:
		path = argv[1];
		break;
	default:
		std::cout << "Invalid number of parameters. Quitting";
	}

	return path;
}

int main(int argc, char** argv)
{
	// read path
	std::string path = getFilePath(argc, argv);

	if (path.empty()) return -1;

	// open file
	std::ifstream file(path);
	if (file.fail())
	{
		std::cout << "Cannot open file. Quitting";
		return -1;
	}

	Queue<CDRRecord> queue(MAX_QUEUE_SIZE, REOPEN_QUEUE_SIZE);

	Parser parser(file);

	DB<OperatorDBRec> operatorDB("MMC/MNC");
	DB<CustomerDBRec> customerDB("MSISDN");
	
	size_t numCores = std::thread::hardware_concurrency();
	std::cout << "This computer supports " << numCores << " concurrent threads." << std::endl;

	size_t numThreads = numCores <= MIN_NUM_OF_THREADS ? MIN_NUM_OF_THREADS : numCores - 1; // at least 5 cores. if more cores available, -1 to account for main thread
	
	// create queue consumer threads
	std::vector<std::thread> consumerThreads(numThreads);
	for (size_t i = 0; i < numThreads; ++i)
	{
		consumerThreads[i] = std::thread(consumer, std::ref(queue), std::ref(customerDB), std::ref(operatorDB));
	}

	parser.parseIntoQueue(queue);
	queue.close();

	for (size_t i = 0; i < numThreads; ++i)
	{
		consumerThreads[i].join();
	}

	std::cout << std::endl << "Operator DB:" << std::endl;
	std::cout << operatorDB;
	std::cout << std::endl << "Customer DB:" << std::endl;
	std::cout << customerDB;
}
