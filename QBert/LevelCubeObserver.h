#pragma once
namespace qbert
{
	class LevelCubeObserver
	{
	public:
		LevelCubeObserver() = default;
		virtual ~LevelCubeObserver() = default;
		virtual void CubeDone() = 0;
		virtual void CubeActivated() = 0;
		virtual void CubeUndone() = 0;
	};
}
