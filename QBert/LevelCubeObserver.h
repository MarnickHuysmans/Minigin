#pragma once
namespace qbert
{
	class LevelCubeObserver
	{
	public:
		LevelCubeObserver() = default;
		virtual ~LevelCubeObserver() = default;
		virtual void Done() = 0;
		virtual void Undone() = 0;
	};
}
