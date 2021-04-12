#pragma once
#include <string>

namespace dae
{
	class Observer
	{
	public:
		Observer() = default;
		virtual ~Observer() = default;
		virtual void Notify(const std::string& message = "") = 0;
		
		Observer(const Observer& other) = delete;
		Observer(Observer&& other) noexcept = delete;
		Observer& operator=(const Observer& other) = delete;
		Observer& operator=(Observer&& other) noexcept = delete;
	};
}
