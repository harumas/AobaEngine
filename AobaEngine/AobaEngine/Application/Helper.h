#pragma once
#include <dxgi1_6.h>
#include <stdexcept>

inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		// hr‚ÌƒGƒ‰[“à—e‚ğthrow‚·‚é
		char s_str[64] = {};
		sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
		std::string errMessage = std::string(s_str);
		throw std::runtime_error(errMessage);
	}
}

inline void ThrowMessage(std::string message)
{
	throw std::runtime_error(message);
}
