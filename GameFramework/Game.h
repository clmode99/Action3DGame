/*----------------------------------------------
GameFramework(Game.h)

date  :2017.06.30
Author:Miyu Hara

�������Direct3D��Game.h���������������̂ł�
----------------------------------------------*/
/* ���d�C���N���[�h�h�~ */
#pragma once

/* �C���N���[�h�t�@�C�� */
#include <Windows.h>
#include <wrl\client.h>
#include <memory>

#include <d3d11.h>
#include <d3d11_1.h>

#include "../StepTimer.h"

/* �N���X�̒�` */
class Game
{
public:
	virtual void Initialize(HWND window, int width, int height);		// ������
	virtual void Update(DX::StepTimer const& timer) = 0;				// �X�V
	virtual void Render()                           = 0;				// �`��

    void Tick();

    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    void GetDefaultSize( int& width, int& height ) const;

	Microsoft::WRL::ComPtr<ID3D11Device>&        GetDevice()  { return m_d3dDevice; }
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>& GetContext() { return m_d3dContext; }

protected:
	Game();

    void Clear();
    void Present();

private:
    void CreateDevice();
    void CreateResources();

    void OnDeviceLost();

protected:
    HWND                                            m_window;
    int                                             m_outputWidth;
    int                                             m_outputHeight;

    D3D_FEATURE_LEVEL                               m_featureLevel;
    Microsoft::WRL::ComPtr<ID3D11Device>            m_d3dDevice;
    Microsoft::WRL::ComPtr<ID3D11Device1>           m_d3dDevice1;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_d3dContext;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext1>    m_d3dContext1;

    Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
    Microsoft::WRL::ComPtr<IDXGISwapChain1>         m_swapChain1;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;

    DX::StepTimer                                   m_timer;
};