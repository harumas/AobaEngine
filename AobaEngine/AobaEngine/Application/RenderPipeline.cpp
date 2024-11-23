#include "RenderPipeline.h"

#include "DxObject.h"
#include "Win32Application.h"
#include "Component/ShaderPass/ShaderPassPool.h"
#include "Utility/ServiceLocator.h"

RenderPipeline::RenderPipeline(const ServiceLocator& serviceLocator)
	:viewPort(0.0f, 0.0f, appContext->GetWindowWidth(), appContext->GetWindowHeight()),
	scissorRect(0, 0, appContext->GetWindowWidth(), appContext->GetWindowWidth()),
	appContext(serviceLocator.Resolve<IAppContext>()),
	dxObject(serviceLocator.Resolve<DxObject>())
{
	// 描画コンテキストの初期化 
	graphicsContext.Create(dxObject->GetDevice());
	renderTargetBuffer.Create(dxObject->GetDevice(), dxObject->GetSwapChain());

	auto shaderPassPool = serviceLocator.Resolve<ShaderPassPool>();
	shaderPassPool->SetRootSignature(dxObject->GetRootSignature());
}

// 初期化処理
void RenderPipeline::OnInit(HWND hwnd)
{
}

void RenderPipeline::OnPostInit()
{
	graphicsContext.Close();

	// コマンドリストの実行
	const std::unique_ptr<CommandQueue>& commandQueue = dxObject->GetCommandQueue();
	commandQueue->ExecuteCommandList(graphicsContext.GetCommandLists());
	commandQueue->WaitForFence();
}

// 更新処理
void RenderPipeline::OnUpdate()
{
	onUpdateEvent.Run();
}

// 描画処理
void RenderPipeline::OnRender()
{
	graphicsContext.Reset();

	// バックバッファを取得
	ID3D12Resource* backBuffer = renderTargetBuffer.GetCurrentBackBuffer();

	// リソースバリアの設定 (PRESENT -> RENDER_TARGET)
	graphicsContext.TransitionResource(backBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// ビューポートとシザー矩形の設定
	graphicsContext.SetViewportAndRect(viewPort, scissorRect);

	// ルートシグネチャの設定
	graphicsContext.SetRootSignature(dxObject->GetRootSignature());

	// ディスクリプタヒープの設定
	graphicsContext.SetDescriptorHeaps(dxObject->GetDescriptorHeaps(), 1);

	// レンダーターゲットの設定
	auto rtvHandle = renderTargetBuffer.GetRTVHandle();
	auto dsvHandle = dxObject->GetDSVHandle();
	graphicsContext.SetRenderTarget(rtvHandle, dsvHandle);

	// レンダーターゲットのクリア
	graphicsContext.ClearBackground(rtvHandle);
	graphicsContext.ClearStencil(dsvHandle);

	// 描画イベントの実行
	onRenderEvent.Run(graphicsContext);

	// リソースバリアの設定 (RENDER_TARGET -> PRESENT)
	graphicsContext.TransitionResource(backBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	// 命令のクローズ
	graphicsContext.Close();

	// コマンドリストの実行
	const std::unique_ptr<CommandQueue>& commandQueue = dxObject->GetCommandQueue();
	commandQueue->ExecuteCommandList(graphicsContext.GetCommandLists());

	// 画面のスワップ
	renderTargetBuffer.SwapBuffer();

	// フェンスの待機
	commandQueue->WaitForFence();
}

