#pragma once

#include <functional>
#include <vector>

template<typename... Args>
class EventHandler
{
public:
	void AddListener(const std::function<void(Args...)>& listener)
	{
		listeners.emplace_back(listener);
	}

	void Run(Args... args)
	{
		for (const auto& listener : listeners)
		{
			listener(args...);
		}
	}

private:
	std::vector<std::function<void(Args...)>> listeners;
};
