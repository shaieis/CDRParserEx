#include <iostream>

#include "ThreadFunc.h"
#include "CDRRecord.h"

void consumer(Queue<CDRRecord>& queue, DB<CustomerDBRec>& customerDB, DB<OperatorDBRec>& operatorDB)
{
	CDRRecord rec;

	// try to pop
	while (queue.pop(rec)) // pop returns false when queue is closed and empty
	{
		// create DB records
		CustomerDBRec custRec(rec);
		OperatorDBRec opRec(rec);

		// push records into DBS
		customerDB.update(rec.getMSISDN(), custRec);
		operatorDB.update(rec.getMMC_MNC(), opRec);
	}
}
