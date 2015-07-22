#include "LIMBO.h"

#include <string>
#include <sstream>
#include <vector>
using namespace std;

#include "Settings.h"

LIMBO::LIMBO(IDirect3DDevice9 *device, int width, int height)
	: Effect(device), width(width), height(height) {
	SDLOG(0, "Limbo construct\n");
	// Setup the defines for compiling the effect
	vector<D3DXMACRO> defines;
	stringstream s;

	defines.push_back({ NULL, NULL });

	DWORD flags = D3DXFX_NOT_CLONEABLE;

	// Load effect from file
	SDLOG(0, "Limbo load\n");
	ID3DXBuffer* errors;
	HRESULT hr = D3DXCreateEffectFromFile(device, GetDirectoryFile("dsfix\\LIMBO.fx"), &defines.front(), NULL, flags, NULL, &effect, &errors);
	if (hr != D3D_OK) SDLOG(0, "ERRORS:\n %s\n", errors->GetBufferPointer());

	// get handles
	frameTexHandle = ConnectGlobal("frameTex2D");
	zFarHandle = ConnectGlobal("ZFAR");
	zNearHandle = ConnectGlobal("ZNEAR");
}

D3DXHANDLE LIMBO::ConnectGlobal(const char* name)
{
	auto result = effect->GetParameterByName(NULL, name);
	if (!result)
		SDLOG(0, "ERROR: %s global not found!\n", name);
	return result;
}

LIMBO::~LIMBO() {
	SAFERELEASE(effect);
}

void LIMBO::go(IDirect3DTexture9 *input, IDirect3DSurface9 *dst, float znear, float zfar) {
	device->SetVertexDeclaration(vertexDeclaration);

	UINT passes;

	device->SetRenderTarget(0, dst);
	effect->SetTexture(frameTexHandle, input);
	effect->SetFloat(zFarHandle, zfar);
	effect->SetFloat(zNearHandle, znear);
	effect->Begin(&passes, 0);
	effect->BeginPass(0);
	quad(width, height);
	effect->EndPass();
	effect->End();
}
