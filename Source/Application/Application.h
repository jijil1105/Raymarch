#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXTex.h>

#include <wrl.h>
#include "../System/d3dx12.h"

#include <stdexcept>
#include <vector>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTex.lib")

//#define Use3DCube

using Microsoft::WRL::ComPtr;

struct alignas(16) cBuffer0
{
	DirectX::XMMATRIX mat; //64byte (4byte*4*4)
	float elapsedTime; // 4byte
	DirectX::XMFLOAT2 resolution; // 8byte (4byte*2)
	float padding; // 4byte (合計16byte)
};

class DXApplication
{
public:
	DXApplication(unsigned int width, unsigned int height, std::wstring title);
	void OnInit(HWND hwnd, float elapsedTime, int width, int height);
	void OnUpdate(float elapseTime);
	void OnRender();
	void OnDestroy();

	const WCHAR* GetTitle() const { return title_.c_str(); }
	unsigned int GetWindowWidth() const { return windowWidth_; }
	unsigned int GetWindowHeight() const { return windowHeight_; }
private:
	static const unsigned int kFrameCount = 2;

	std::wstring title_;
	unsigned int windowWidth_;
	unsigned int windowHeight_;

	CD3DX12_VIEWPORT viewport_ = {}; // ビューポート
	CD3DX12_RECT scissorrect_ = {};  // シザー短形

	// パイプラインオブジェクト
	ComPtr<ID3D12Device> device_ = nullptr;
	ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
	ComPtr<ID3D12GraphicsCommandList> commandList_ = nullptr;
	ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;
	ComPtr<IDXGISwapChain4> swapchain_ = nullptr;
	ComPtr<ID3D12DescriptorHeap> rtvHeap_ = nullptr;              // レンダーターゲットヒープ
	ComPtr<ID3D12DescriptorHeap> basicHeap_ = nullptr;            // 基本情報の受け渡し用(SRV + CBV)
	ComPtr<ID3D12Resource> renderTargets_[kFrameCount] = {}; // バックバッファー
	ComPtr<ID3D12PipelineState> pipelinestate_ = nullptr;         // パイプラインステート
	ComPtr<ID3D12RootSignature> rootsignature_ = nullptr;         // ルートシグネチャ

	// リソース
	ComPtr<ID3D12Resource> vertexBuffer_ = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_ = {};
	ComPtr<ID3D12Resource> indexBuffer_ = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_ = {};
	ComPtr<ID3D12Resource> constBuffer_ = nullptr;
	ComPtr<ID3D12Resource> textureBuffer_ = nullptr;

	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT2 uv;
	};

#ifdef Use3DCube
	// インデックス定義
	unsigned short m_indices[36] = {
		0,  1,  2,  0,  2,  3,  // 前面
		4,  5,  6,  4,  6,  7,  // 背面
		8,  9,  10, 8,  10, 11, // 上面
		12, 13, 14, 12, 14, 15, // 底面
		16, 17, 18, 16, 18, 19, // 右側面
		20, 21, 22, 20, 22, 23  // 左側面
	};
#else
	// インデックス定義
	unsigned short m_indices[6] = {
		0,  1,  2,  0,  2,  3,  // 前面
	};
#endif

	// フェンス
	ComPtr<ID3D12Fence> fence_ = nullptr;
	UINT64 fenceValue_ = {};
	HANDLE fenceEvent_ = {};

	// 3D座標変換用行列
	DirectX::XMMATRIX worldMatrix_ = {};
	DirectX::XMMATRIX viewMatrix_ = {};
	DirectX::XMMATRIX projMatrix_ = {};
	//DirectX::XMMATRIX* mapMatrix_;

	cBuffer0* mapCBuffer_ = nullptr;

	// オブジェクトのパラメータを想定
	float angle_ = 180.0f;
	float scale_ = 3.0f;

	void LoadPipeline(HWND hwnd);
	void LoadAssets(float elapseTime, int width, int height);

	void CreateD3D12Device(IDXGIFactory6* dxgiFactory, ID3D12Device** d3d12device);
	void ThrowIfFailed(HRESULT hr);
};
