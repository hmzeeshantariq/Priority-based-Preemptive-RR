#include <iostream>
#include <fstream>
#include <string>
#include "SinglyLinkedList.h"
using namespace std;

struct process;
void sort(List<process>&);
process readProcess(ifstream&);
bool runStateFunction(process*, int, List<process>*);
void contextSwitch(process, process, int);
void initWithZero(process &);
void displayAllQueue(List<process>*);

struct process {
	string procName = "";
	int priority = 0;
	int burst = 0;
	int arrival = 0;
	int turnAroundTime = 0;
	int waitCounter = 0;
	int timeQuantum = 0;
};

int main()
{
	process proc, arrivedProc, lateProc;
	ifstream f;
	bool shouldRead = true, compExecuted;
	string filename;
	List<process>readyQueue;
	List<process>compQueue;
	process* runState = NULL;
	Node<process>* nodePointer;

	// 0. The opening the process table file
	cout << "Enter Data Filename: ";
	getline(cin, filename);

	f.open(filename);
	if (f.fail()) {
		cout << "\nFile not found !\nExiting...\n";
		exit(1);
	}

	// 1. Implementing loop from 0 to 96
	for (int t = 0; t < 96; t++) {

		while (!f.eof() && shouldRead) {
			arrivedProc = readProcess(f);
			if (arrivedProc.arrival == t) {
				readyQueue.push(arrivedProc);
				cout << "\nProcess: " << arrivedProc.procName << " Inserted at t: " << t;
				sort(readyQueue);
			}
			else {
				lateProc = arrivedProc;
				shouldRead = false;
			}
			arrivedProc = readyQueue.pop();
		}

		if (lateProc.arrival == t) {
			readyQueue.push(lateProc);
			arrivedProc = lateProc;
			cout << "\nProcess: " << lateProc.procName << " Inserted at t: " << t;
			sort(readyQueue);
			shouldRead = true;
		}
		
		if (arrivedProc.arrival <= t) {
			// 3(b). If no process is in the run state, insert the new process into the run state
			if (runState == NULL && readyQueue.size() > 0) {
				arrivedProc = readyQueue.pop();
				proc = arrivedProc;
				runState = &proc;
				readyQueue.remove();
				if(readyQueue.size() > 0)
					arrivedProc = readyQueue.pop();
				else
					initWithZero(arrivedProc);
				compExecuted = runStateFunction(runState, t, &compQueue);
				if (compExecuted)
					runState = NULL;
			}
			// 3(c). if  P_n has a higher priority than P_r in the run state, then preempt
			else if (runState && arrivedProc.priority > (*runState).priority)
			{
				readyQueue.remove();
				readyQueue.push(*runState);
				sort(readyQueue);

				contextSwitch(arrivedProc, proc, t);
				proc = arrivedProc;

				compExecuted = runStateFunction(runState, t, &compQueue);
				if (compExecuted)
					runState = NULL;
			}
			// if both proc have same priority, use time quantum
			else if (runState && (arrivedProc.priority == (*runState).priority)) {
				if (runState->timeQuantum == 10) {
					runState->timeQuantum = 0;
					readyQueue.remove();
					readyQueue.push(*runState);
					sort(readyQueue);

					contextSwitch(arrivedProc, proc, t);
					proc = arrivedProc;
				}
				
				compExecuted = runStateFunction(runState, t, &compQueue);
				if (compExecuted)
					runState = NULL;
			}
			else {
				if(runState) {
					compExecuted = runStateFunction(runState, t, &compQueue);
					if (compExecuted)
						runState = NULL;
				}
			}
		}

		// 4. increment the turnaround time & wait time counter
		//for readyQueue
		for (int i = 0; i < readyQueue.size(); i++) {
			nodePointer = readyQueue.getNode(i);

			//for incrementing waiting time
			nodePointer->data.waitCounter++;

			//for incrementing turn around time
			nodePointer->data.turnAroundTime++;
		}
	}
	f.close();

	cout << "\n\nBelow are the Processes in Ready Queue, "
		<< "with their complete Info: \n\n";
	displayAllQueue(&readyQueue);

	cout << "\n\nBelow are the successfully executed Processes in order of Execution, "
		<< "with their complete Info: \n\n";
	compQueue.reverse();
	displayAllQueue(&compQueue);
	return 0;
}

void displayAllQueue(List<process>* compQ) {
	process proc;
	for (int i = compQ->size(); i > 0; i--) {
		proc = compQ->pop();
		compQ->remove();
		cout << "\nProcName: " << proc.procName;
		cout << "\nPriority: " << proc.priority;
		cout << "\nTurn Around Time: " << proc.turnAroundTime;
		cout << "\nTime Quantum: " << proc.timeQuantum; 
		cout << "\nWait Counter: " << proc.waitCounter << "\n";

	}
}

void contextSwitch(process newProc, process runProc, int t) {
	cout << "\nContext Switch:\t"
		<< "Time: " << t
		<< "\tP_n: " << newProc.procName
		<< "\t\t P_r: " << runProc.procName << "\n";
}

bool runStateFunction(process* proc, int t, List<process>* compQueue) {
	(*proc).burst--;
	(*proc).turnAroundTime++;
	(*proc).timeQuantum++;

	//process executed completely
	if ((*proc).burst == 0) {
		compQueue->push(*proc);
		proc = NULL;
		return true;
	}
	return false;
}

process readProcess(ifstream& fin) {
	process proc;
	fin >> proc.procName;
	fin >> proc.priority;
	fin >> proc.burst;
	fin >> proc.arrival;
	return proc;
}


void sort(List<process>& queue) {
	process temp;
	Node<process>* current, * bcurrent;
	current = queue.getHead();
	bcurrent = current->next;

	for (int i = queue.size() - 1; i >= 0; i--) {

		for (int j = 0; (j < (queue.size() - 1)) && (current && bcurrent); j++) {
			if (current->data.priority < bcurrent->data.priority) {
				temp = current->data;
				current->data = bcurrent->data;
				bcurrent->data = temp;
			}
			current = bcurrent;
			bcurrent = bcurrent->next;
		}
	}
}

void initWithZero(process& proc) {
	proc.procName = "";
	proc.priority = 0;
	proc.burst = 0;
	proc.arrival = 0;
	proc.turnAroundTime = 0;
	proc.waitCounter = 0;
	proc.timeQuantum = 0;
}