#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <d3dx12.h>

#include "FbxLoader.h"
#include "Helper.h"

#include <stdexcept>
#include <vector>

#include <basetsd.h>
#include <dxgi1_5.h>
#include <functional>
#include <windows.h>
#include <wrl/client.h>
#include <memory>
#include <string>

#include "AppContext.h"
#include "DxObject.h"
#include "CommandQueue.h"
#include "GraphicsContext.h"
#include "RenderTargetBuffer.h"
#include "Utility/EventHandler.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTex.lib")

class ServiceLocator;
using Microsoft::WRL::ComPtr;

class RenderPipeline
{
public:
	EventHandler<void> onUpdateEvent;
	EventHandler<GraphicsContext&> onRenderEvent;

	explicit RenderPipeline(const ServiceLocator& serviceLocator);

	RenderPipeline(const RenderPipeline&) = delete;
	RenderPipeline& operator=(const RenderPipeline&) = delete;
	RenderPipeline(RenderPipeline&&) = delete;
	RenderPipeline& operator=(RenderPipeline&&) = delete;

	void OnInit(HWND hwnd);
	void OnPostInit();
	void OnUpdate();
	void OnRender();

private:
	~RenderPipeline() = default;

	// パイプラインオブジェクト
	GraphicsContext graphicsContext;
	RenderTargetBuffer renderTargetBuffer;

	CD3DX12_VIEWPORT viewPort; // ビューポート
	CD3DX12_RECT scissorRect;  // シザー短形

	std::shared_ptr<IAppContext> appContext;
	std::shared_ptr<DxObject> dxObject;
};
