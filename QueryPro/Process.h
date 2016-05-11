/*
   The function process Query.
*/

#ifndef PROCESS_H
#define PROCESS_H

class Process {
	private:
		int MaxLenQuery;
		int MaxLenTitle;
	public:
		Process(int,int);
		void operator () (int) const;
};

#endif
