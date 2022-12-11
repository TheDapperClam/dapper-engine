#pragma once

#include <ImGuiWindow.h>
#include <string>

namespace dap {
	class ImGuiDrawCallStepper : public dap::ImGuiWindow {
	private:
		static bool IsStepping;
		static unsigned int CurrentStep;

	public:
		static bool GetIsStepping ();
		static unsigned int GetCurrentStep ();

	public:
		ImGuiDrawCallStepper ( std::string title );

	public:
		virtual void ContentBegin () override;
	};
}
