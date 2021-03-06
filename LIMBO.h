﻿#pragma once

//#include <dxgi.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>

#include "Effect.h"

class LIMBO : public Effect
{
public:
	LIMBO(IDirect3DDevice9 *device, int width, int height);
	virtual ~LIMBO();

	void go(IDirect3DTexture9 *input, IDirect3DSurface9 *dst, float znear, float zfar);

private:

	D3DXHANDLE ConnectGlobal(const char* name);

	int width, height;

	ID3DXEffect *effect;

	D3DXHANDLE frameTexHandle, zFarHandle, zNearHandle;
};
