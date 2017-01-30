// (c) 2017 Dewey Taylor

#include "CLVHelpers.h"

OneStringRow::OneStringRow(const char* str)
	:
	BRow(), val(str)
{
	SetField(new BStringField(val), 0);
}

OneStringRow::~OneStringRow()
{
}
