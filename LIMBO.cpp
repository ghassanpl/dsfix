#include "LIMBO.h"

#include <string>
#include <sstream>
#include <vector>
using namespace std;

#include "Settings.h"

LIMBO::LIMBO(IDirect3DDevice9 *device, int width, int height, float znear, float zfar)
	: Effect(device), width(width), height(height) {
	SDLOG(0, "Limbo construct\n");
	// Setup the defines for compiling the effect
	vector<D3DXMACRO> defines;
	stringstream s;

	/// depth scaling macro
	auto zn = std::to_string(znear);
	auto zf = std::to_string(zfar);
	defines.push_back({ "ZNEAR", zn.c_str() });
	defines.push_back({ "ZFAR", zf.c_str() });
	defines.push_back({ NULL, NULL });

	DWORD flags = D3DXFX_NOT_CLONEABLE;

	// Load effect from file
	SDLOG(0, "Limbo load\n");
	ID3DXBuffer* errors;
	HRESULT hr = D3DXCreateEffectFromFile(device, GetDirectoryFile("dsfix\\LIMBO.fx"), &defines.front(), NULL, flags, NULL, &effect, &errors);
	if (hr != D3D_OK) SDLOG(0, "ERRORS:\n %s\n", errors->GetBufferPointer());

	// get handles
	frameTexHandle = effect->GetParameterByName(NULL, "frameTex2D");
	if (frameTexHandle == nullptr)
		SDLOG(0, "ERROR: frameTex2D not found!\n");
}

LIMBO::~LIMBO() {
	SAFERELEASE(effect);
}

void LIMBO::go(IDirect3DTexture9 *input, IDirect3DSurface9 *dst) {
	device->SetVertexDeclaration(vertexDeclaration);

	UINT passes;

	// Vertical blur
	device->SetRenderTarget(0, dst);
	effect->SetTexture(frameTexHandle, input);
	effect->Begin(&passes, 0);
	effect->BeginPass(0);
	quad(width, height);
	effect->EndPass();
	effect->End();
}
