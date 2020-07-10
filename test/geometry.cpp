/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * Copyright (C) 2019, Google Inc.
 *
 * geometry.cpp - Geometry classes tests
 */

#include <iostream>

#include <libcamera/geometry.h>

#include "test.h"

using namespace std;
using namespace libcamera;

class GeometryTest : public Test
{
protected:
	bool compare(const Size &lhs, const Size &rhs,
		     bool (*op)(const Size &lhs, const Size &rhs),
		     const char *opName, bool expect)
	{
		bool result = op(lhs, rhs);

		if (result != expect) {
			cout << "Size(" << lhs.width << ", " << lhs.height << ") "
			     << opName << " "
			     << "Size(" << rhs.width << ", " << rhs.height << ") "
			     << "test failed" << std::endl;
			return false;
		}

		return true;
	}

	int run()
	{
		if (!Size().isNull() || !Size(0, 0).isNull()) {
			cout << "Null size incorrectly reported as not null" << endl;
			return TestFail;
		}

		if (Size(0, 100).isNull() || Size(100, 0).isNull() || Size(100, 100).isNull()) {
			cout << "Non-null size incorrectly reported as null" << endl;
			return TestFail;
		}

		/* Test alignedDownTo(), alignedUpTo(), boundedTo() and expandedTo() */
		if (Size(0, 0).alignedDownTo(16, 8) != Size(0, 0) ||
		    Size(1, 1).alignedDownTo(16, 8) != Size(0, 0) ||
		    Size(16, 8).alignedDownTo(16, 8) != Size(16, 8)) {
			cout << "Size::alignedDownTo() test failed" << endl;
			return TestFail;
		}

		if (Size(0, 0).alignedUpTo(16, 8) != Size(0, 0) ||
		    Size(1, 1).alignedUpTo(16, 8) != Size(16, 8) ||
		    Size(16, 8).alignedUpTo(16, 8) != Size(16, 8)) {
			cout << "Size::alignedUpTo() test failed" << endl;
			return TestFail;
		}

		if (Size(0, 0).boundedTo({ 100, 100 }) != Size(0, 0) ||
		    Size(200, 50).boundedTo({ 100, 100 }) != Size(100, 50) ||
		    Size(50, 200).boundedTo({ 100, 100 }) != Size(50, 100)) {
			cout << "Size::boundedTo() test failed" << endl;
			return TestFail;
		}

		if (Size(0, 0).expandedTo({ 100, 100 }) != Size(100, 100) ||
		    Size(200, 50).expandedTo({ 100, 100 }) != Size(200, 100) ||
		    Size(50, 200).expandedTo({ 100, 100 }) != Size(100, 200)) {
			cout << "Size::expandedTo() test failed" << endl;
			return TestFail;
		}

		/* Test Size equality and inequality. */
		if (!compare(Size(100, 100), Size(100, 100), &operator==, "==", true))
			return TestFail;
		if (!compare(Size(100, 100), Size(100, 100), &operator!=, "!=", false))
			return TestFail;

		if (!compare(Size(100, 100), Size(200, 100), &operator==, "==", false))
			return TestFail;
		if (!compare(Size(100, 100), Size(200, 100), &operator!=, "!=", true))
			return TestFail;

		if (!compare(Size(100, 100), Size(100, 200), &operator==, "==", false))
			return TestFail;
		if (!compare(Size(100, 100), Size(100, 200), &operator!=, "!=", true))
			return TestFail;

		/* Test Size ordering based on combined with and height. */
		if (!compare(Size(100, 100), Size(200, 200), &operator<, "<", true))
			return TestFail;
		if (!compare(Size(100, 100), Size(200, 200), &operator<=, "<=", true))
			return TestFail;
		if (!compare(Size(100, 100), Size(200, 200), &operator>, ">", false))
			return TestFail;
		if (!compare(Size(100, 100), Size(200, 200), &operator>=, ">=", false))
			return TestFail;

		if (!compare(Size(200, 200), Size(100, 100), &operator<, "<", false))
			return TestFail;
		if (!compare(Size(200, 200), Size(100, 100), &operator<=, "<=", false))
			return TestFail;
		if (!compare(Size(200, 200), Size(100, 100), &operator>, ">", true))
			return TestFail;
		if (!compare(Size(200, 200), Size(100, 100), &operator>=, ">=", true))
			return TestFail;

		/* Test Size ordering based on area (with overlapping sizes). */
		if (!compare(Size(200, 100), Size(100, 400), &operator<, "<", true))
			return TestFail;
		if (!compare(Size(200, 100), Size(100, 400), &operator<=, "<=", true))
			return TestFail;
		if (!compare(Size(200, 100), Size(100, 400), &operator>, ">", false))
			return TestFail;
		if (!compare(Size(200, 100), Size(100, 400), &operator>=, ">=", false))
			return TestFail;

		if (!compare(Size(100, 400), Size(200, 100), &operator<, "<", false))
			return TestFail;
		if (!compare(Size(100, 400), Size(200, 100), &operator<=, "<=", false))
			return TestFail;
		if (!compare(Size(100, 400), Size(200, 100), &operator>, ">", true))
			return TestFail;
		if (!compare(Size(100, 400), Size(200, 100), &operator>=, ">=", true))
			return TestFail;

		/* Test Size ordering based on width (with identical areas). */
		if (!compare(Size(100, 200), Size(200, 100), &operator<, "<", true))
			return TestFail;
		if (!compare(Size(100, 200), Size(200, 100), &operator<=, "<=", true))
			return TestFail;
		if (!compare(Size(100, 200), Size(200, 100), &operator>, ">", false))
			return TestFail;
		if (!compare(Size(100, 200), Size(200, 100), &operator>=, ">=", false))
			return TestFail;

		if (!compare(Size(200, 100), Size(100, 200), &operator<, "<", false))
			return TestFail;
		if (!compare(Size(200, 100), Size(100, 200), &operator<=, "<=", false))
			return TestFail;
		if (!compare(Size(200, 100), Size(100, 200), &operator>, ">", true))
			return TestFail;
		if (!compare(Size(200, 100), Size(100, 200), &operator>=, ">=", true))
			return TestFail;

		return TestPass;
	}
};

TEST_REGISTER(GeometryTest)
