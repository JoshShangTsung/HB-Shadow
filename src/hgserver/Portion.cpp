#include "Portion.h"
#include <cstring>

bool operator==(const CPortion::E &a, const CPortion::E &b) {
	return a.id_ == b.id_ && a.num_ == b.num_;
}

bool operator!=(const CPortion::E &a, const CPortion::E &b) {
	return !(a==b);
}
