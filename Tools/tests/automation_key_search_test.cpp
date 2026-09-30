#include "../../Modules/juce_organicui/automation/AutomationKeySearch.h"

#include <cassert>
#include <iostream>
#include <limits>
#include <vector>

static int previousReference(const std::vector<double>& keys, double pos, bool trueIfEqual)
{
	if (keys.empty()) return -1;
	if (pos < keys.front() || pos == 0.0) return 0;
	for (int i = static_cast<int>(keys.size()) - 1; i >= 0; --i)
		if (keys[i] < pos || (trueIfEqual && keys[i] == pos)) return i;
	return -1;
}

static int nextReference(const std::vector<double>& keys, double pos, bool trueIfEqual)
{
	if (keys.empty()) return -1;
	if (pos < keys.front() || pos == 0.0) return 0;
	for (int i = 0; i < static_cast<int>(keys.size()); ++i)
		if (keys[i] > pos || (trueIfEqual && keys[i] == pos)) return i;
	return -1;
}

static void check(const std::vector<double>& keys, double pos)
{
	const auto at = [&](int i) { return keys[static_cast<size_t>(i)]; };
	for (bool equal : { false, true })
	{
		assert(AutomationKeySearch::previous(static_cast<int>(keys.size()), pos, equal, at)
			== previousReference(keys, pos, equal));
		assert(AutomationKeySearch::next(static_cast<int>(keys.size()), pos, equal, at)
			== nextReference(keys, pos, equal));
	}
}

int main()
{
	for (const auto& keys : { std::vector<double>{}, { 0.0 }, { 0.0, 1.0, 1.0, 2.0 },
		{ -1.0, 0.0, 0.0, 2.0 }, { 1000000.0000001, 1000000.0000002 } })
	{
		for (double pos : { -std::numeric_limits<double>::infinity(), -1.0, -0.1, 0.0,
			0.5, 1.0, 1.5, 2.0, 3.0, 1000000.0000001, 1000000.0000002,
			std::numeric_limits<double>::infinity(), std::numeric_limits<double>::quiet_NaN() })
			check(keys, pos);
	}

	std::vector<double> many;
	for (int i = 0; i < 5000; ++i) many.push_back((i - i % 7) / 10.0);
	for (int i = -10; i < 15000; ++i) check(many, i * 0.037);
	std::cout << "Automation key search passed\n";
}
