// (c) 2017 Dewey Taylor

#include "CLVHelpers.h"

OneStringRow::OneStringRow(const char* str, const char* type)
	:
	BRow(), val(str), fTypeID(type)
{
	SetField(new BStringField(val), 0);
}

OneStringRow::~OneStringRow()
{
}

