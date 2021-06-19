#pragma once

namespace MAGE::Core
{
	class WindowMessageHandler
	{
	public:
		//Return type, function pointer parameter, parameter list
		//Then define it as a "type" like an integer so its easier to use
		using Callback = LRESULT(CALLBACK*)(HWND, UINT, WPARAM, LPARAM);
		void Hook(HWND window, Callback cb);
		void Unhook();

		//(callback doesnt care just pass it on)
		LRESULT ForwardMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		HWND mWindow = nullptr;
		Callback mPreviousCallback = nullptr;
	};

}// namespace MAGE::Core
