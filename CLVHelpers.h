// (c) 2017 Dewey Taylor

#ifndef CLVHELPERS_H
#define CLVHELPERS_H

#include <ColumnListView.h>
#include <ColumnTypes.h>

class OneStringRow : public BRow
{
	public:
					OneStringRow(const char*);
					~OneStringRow();
	private:
		const char*	val;
};

#endif
