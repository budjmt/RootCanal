#include "PostProcess.h"

#include "Shader.h"

PostProcess::PostProcess(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, ID3D11SamplerState* _sampler, ID3D11DepthStencilView* _depthStencilView)
{
	sampler = _sampler;
	device = _device;
	deviceContext = _deviceContext;
	depthStencilView = _depthStencilView;
	ppVS = Shader::getShader<SimpleVertexShader>(L"FinalVertex");
	ppPS = Shader::getShader<SimplePixelShader>(L"FinalPixel");
    sceneType = SceneType::DEFAULT;
}

PostProcess::~PostProcess()
{
	/*ReleaseMacro(device); ReleaseMacro(deviceContext); 
	ReleaseMacro(sampler); ReleaseMacro(depthStencilView);*/
}

void PostProcess::AddEffect(int i, PostProcessBase* effect) {
	if (i == 0) normalChain.push_back(effect);
	else xrayChain.push_back(effect);
	//ppChain.push_back(effect);
}

void PostProcess::setChain(int i) {
	if (i == 0) ppChain = xrayChain;
	else ppChain = normalChain;
}

void PostProcess::setSceneType( SceneType type )
{
    sceneType = type;
}

#include "Keyboard.h"

void PostProcess::draw( ID3D11ShaderResourceView* ppSRV, ID3D11RenderTargetView* renderTargetView, ID3D11RenderTargetView* backBufferView ) {

    if( sceneType == SceneType::GAME || StateManager::getInstance().forceXraySwitch() ) {
        Keyboard& keys = Keyboard::getInstance();

        static bool keyDown = false;
        if( StateManager::getInstance().forceXraySwitch() || (keys.isDown( VK_TAB ) && !keyDown) ) {
            keyDown = true;
            if( chainSwap % 2 == 0 ) { ppChain = normalChain; }
            else { ppChain = xrayChain; }
            chainSwap++;
            StateManager::getInstance().forceXraySwitch( false );
        }
        if( !keys.isDown( VK_TAB ) && keyDown ) {
            keyDown = false;
        }
    } else {
        ppChain = xrayChain;
        chainSwap = 0;
    }

	SRV* srv = ppSRV;
	const float color[4] = { 0.1f, 0.1f, 0.1f, 0.1f };
	//D3D11_SAMPLER_DESC samplerDesc = {};

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	// Turn off existing vert/index buffers
	ID3D11Buffer* nothing = 0;
	deviceContext->IASetVertexBuffers(0, 1, &nothing, &stride, &offset);
	deviceContext->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);

	ppVS->SetShader();

	for (size_t i = 0; i < ppChain.size(); ++i) {
        if( !ppChain[i]->disabled() ) {
            srv = ppChain[i]->draw( srv );
        }
	}

	//srv = ppChain[0]->draw(srv);

	// Reset states
	deviceContext->RSSetState(0);
	deviceContext->OMSetDepthStencilState(0, 0);

	// Done with set to final render state
	deviceContext->OMSetRenderTargets(1, &backBufferView, 0);
	deviceContext->ClearRenderTargetView(backBufferView, color);

	//ppVS->SetShader();

	ppPS->SetShaderResourceView("pixels", srv);
	ppPS->SetShader();


	// Finally - DRAW!
	deviceContext->Draw(3, 0);
}