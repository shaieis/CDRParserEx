#pragma once

#include "Queue.h"
#include "DB.h"
#include "OperatorDBRec.h"
#include "CustomerDBRec.h"

/* 
	Pops records from queue, processes them and inserts into DB.
	Stops when queue is empty and producer is done (indicated by producerIsDone function)
*/
void consumer(Queue<CDRRecord>& queue, DB<CustomerDBRec>& customerDB, DB<OperatorDBRec>& operatorDB);
