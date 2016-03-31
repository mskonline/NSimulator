# NSimulator
A Simple Network Simulator written in C++ using Qt application framework and Boost C++ libraries

Goal :

The goal of NSimulator is to simulate traffic flowing through a router, measure various Queing behavior and its performance under various Quality of service parameters.

Architecture:
![Alt text](./docs/architecture_p2.png?raw=true "Router Architecture")

Input Adapter:
* Reads the packet from input file into the input queue
* Consults the forwarding table and  forwards the packets to the designated output adapter and queue.
* 3 Output adapter threads are started initially to fill in the corresponding buffers at the output adapter before the start of the          simulation
* The threads complete when all the packets are read from input files

Output Adapter:
* Initiates the queue push and pop tasks
* The queue push tasks empties the buffer and pushes the packets into the queue at a regulated rate
* The queue pop task empties both the queue in round robin scheduling fashion and writes it to the output files at a regulated rate.
* The queue pop task  also keep track of the queue depth and packet residence time
* Logs the number of packets , total packets processed.
* The adapter completes when the queue pop task completely empties both the queues

Router:
* Initiates all the input and output adaptors
* Monitors the adaptors 
* Performs the analysis on each of the queues on the output adapter

Fabric:
* Pushing packets from the input adapters to the buffers in the output adapters

Working:
* There are three input files containing various packets either of fixed size or variable size.
* These packets are read and forwarded to the buffers in the output adapter. 
* From the input adapter, packets are sent to the switch fabric module, where it determines the destination output adapter and the      queue of the packet 
* The packets can go any of six different buffers from the switch fabric module
* Each output link has two output queues associated with it.
* The packets are pushed from two queues in Round Robin Scheduling fashion.
* From the output queue, the packets are sent to output file at a particular output rate which is different for different output        ports.


Implementation:
* The System is implemented in C++ using Qt and Boost C++ libraries
* We have taken Thread approach for various components in the system which include the 3 input adapters, 3 output adapters, 6 output    queue push tasks, 3 output queue pop tasks. Total of 12 threads run in the system
* The input adapter threads are started first to fill in all the buffers at output adapter. After which, the output adapters and the    underlying queue push and pop threads are started for simulation.
* We have used the concept of lock free Queues available in Boost C++ library. The lock free Queues enables threads to simultaneously   push and pop data without being blocked by one another. That is, the queue need not be locked when one operation is on going. This    enables us to closely simulate an ideal real time Queue


