#pragma once
#include "Component.h"

namespace dae
{
	class TextComponent;

	class FPS final : public Component
	{
	public:
		void Start() override;
		void Update() override;

		FPS() = default;
		virtual ~FPS() = default;
		FPS(const FPS& other) = delete;
		FPS(FPS&& other) noexcept = delete;
		FPS& operator=(const FPS& other) = delete;
		FPS& operator=(FPS&& other) noexcept = delete;

	private:
		TextComponent* m_TextComponent = nullptr;
		float m_Counter{};
		float m_DeltaTime{};
		int m_DeltaCount{};
	};
}
