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
	// �`��R���e�L�X�g�̏����� 
	graphicsContext.Create(dxObject->GetDevice());
	renderTargetBuffer.Create(dxObject->GetDevice(), dxObject->GetSwapChain());

	auto shaderPassPool = serviceLocator.Resolve<ShaderPassPool>();
	shaderPassPool->SetRootSignature(dxObject->GetRootSignature());
}

// ����������
void RenderPipeline::OnInit(HWND hwnd)
{
}

void RenderPipeline::OnPostInit()
{
	graphicsContext.Close();

	// �R�}���h���X�g�̎��s
	const std::unique_ptr<CommandQueue>& commandQueue = dxObject->GetCommandQueue();
	commandQueue->ExecuteCommandList(graphicsContext.GetCommandLists());
	commandQueue->WaitForFence();
}

// �X�V����
void RenderPipeline::OnUpdate()
{
	onUpdateEvent.Run();
}

// �`�揈��
void RenderPipeline::OnRender()
{
	graphicsContext.Reset();

	// �o�b�N�o�b�t�@���擾
	ID3D12Resource* backBuffer = renderTargetBuffer.GetCurrentBackBuffer();

	// ���\�[�X�o���A�̐ݒ� (PRESENT -> RENDER_TARGET)
	graphicsContext.TransitionResource(backBuffer, D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// �r���[�|�[�g�ƃV�U�[��`�̐ݒ�
	graphicsContext.SetViewportAndRect(viewPort, scissorRect);

	// ���[�g�V�O�l�`���̐ݒ�
	graphicsContext.SetRootSignature(dxObject->GetRootSignature());

	// �f�B�X�N���v�^�q�[�v�̐ݒ�
	graphicsContext.SetDescriptorHeaps(dxObject->GetDescriptorHeaps(), 1);

	// �����_�[�^�[�Q�b�g�̐ݒ�
	auto rtvHandle = renderTargetBuffer.GetRTVHandle();
	auto dsvHandle = dxObject->GetDSVHandle();
	graphicsContext.SetRenderTarget(rtvHandle, dsvHandle);

	// �����_�[�^�[�Q�b�g�̃N���A
	graphicsContext.ClearBackground(rtvHandle);
	graphicsContext.ClearStencil(dsvHandle);

	// �`��C�x���g�̎��s
	onRenderEvent.Run(graphicsContext);

	// ���\�[�X�o���A�̐ݒ� (RENDER_TARGET -> PRESENT)
	graphicsContext.TransitionResource(backBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	// ���߂̃N���[�Y
	graphicsContext.Close();

	// �R�}���h���X�g�̎��s
	const std::unique_ptr<CommandQueue>& commandQueue = dxObject->GetCommandQueue();
	commandQueue->ExecuteCommandList(graphicsContext.GetCommandLists());

	// ��ʂ̃X���b�v
	renderTargetBuffer.SwapBuffer();

	// �t�F���X�̑ҋ@
	commandQueue->WaitForFence();
}

