#pragma once
#include <string>

struct IAppContext
{
	virtual ~IAppContext() = default;
	virtual unsigned int GetWindowWidth() = 0;
	virtual unsigned int GetWindowHeight() = 0;
	virtual float GetWindowAspectRatio() = 0;
	virtual std::wstring GetTitle() = 0;
};

// アプリケーションの設定を保持するクラス
struct AppContext final : public IAppContext
{
	AppContext(unsigned int width, unsigned int height, std::wstring title)
		: width(width), height(height), title(std::move(title))
	{
	}

	AppContext(const AppContext&) = delete;

	/*
	 *@fn
	 * ウィンドウの幅を取得します
	 *@return ウィンドウの幅
	 */
	unsigned int GetWindowWidth() override
	{
		return width;
	}

	/*
	 *@fn
	 * ウィンドウの高さを取得します
	 *@return ウィンドウの高さ
	 */
	unsigned int GetWindowHeight() override
	{
		return height;
	}

	/*
	 *@fn
	 * ウィンドウの名前を取得します
	 *@return ウィンドウの名前
	 */
	std::wstring GetTitle() override
	{
		return title;
	}

	/*
	 *@fn
	 * ウィンドウのアスペクト比を取得します
	 *@return ウィンドウのアスペクト比
	 */
	float GetWindowAspectRatio() override
	{
		return static_cast<float>(width) / static_cast<float>(height);
	}

private:
	unsigned int width;
	unsigned int height;
	std::wstring title;
};

