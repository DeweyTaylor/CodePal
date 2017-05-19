// (c) 2017 Dewey Taylor

#ifndef CLVHELPERS_H
#define CLVHELPERS_H

#include <ColumnListView.h>
#include <ColumnTypes.h>

class OneStringRow : public BRow
{
	public:
					OneStringRow(const char*, const char* = 0);
					~OneStringRow();
		const char*	GetTypeID() { return fTypeID; }
		const char*	GetValue() { return val; }
	private:
		const char*	val;
		const char* fTypeID;
};

#endif
