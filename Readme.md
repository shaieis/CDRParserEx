Home assignmet
==============
Author: Shai Eisenbud.

To run, enter path to a CDR file.

Assumptions:
============
* CDR file is not empty.
* All values in the CDR file are compliant with the specifications (String length, integer values etc.).
* Each line in the CDR file contains a valid record, i.e, no blank lines and no illformed records.
* For each record between customer A to B, a mirror record between customer B to A exists.

Architecture summary:
=====================
Four major components exist: a parser, a queue, a comsumer function (which runs in multiple threads) and a DB.
Parser: parses each line of the CDR file and pushes the result into the queue.

Queue: implemented using boost's lock-free queue. 

an intermediate component between the parser and the consumer threads; the parser pushes records into the queue and the consumer threads pop them out.
The queue is implemented using boost's lock-free queue and is bounded by a max size and a min size:
If the queue gets too large (>MAX_QUEUE_SIZE), It locks the parser from pushing any more until it becomes small enough to reopen (<REOPEN_QUEUE_SIZE).
If the queue becomes empty it will lock the consumer thread from trying to pop more.
In order to avoid busy waits it uses condition variables.

Consumer: pops records from the queue, runs some logic to create appropriate DB records and pushes them into the respective DB.  
 
DB: A template class which is implemented by 2 DB classes, the customer and operator DBs. The DBs aggregate the data received from the consumer threads.
The DB classes try to avoid locks as much as possible by using atomics. However, printing either of the DBs before the consumer threads finish pushing 
data into them can result in getting incorrect results.

Possible Optimizations:
=======================
1. Adding "move" copy constructors and operator= and support moving objects instead of passing by reference where applicable.
2. Using type specific classesto pass information between Parser->Queue->Consumer and Consumer->DB instead of using the internally used 
   record objects, which will result in smaller objects being transferred.
3. Implementing a more forgiving (maybe cyclic) lock-free queue which, unlike boosts's lock-free queue, would allow for classes with non 
   trivial destructors, such as strings, to be used instead of the fixed sized c-strings for operator names.
4. Making functions/ constructors noexcept where possible



