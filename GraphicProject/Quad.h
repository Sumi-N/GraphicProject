#pragma once
#include "Object.h"
#include "FrameBuffer.h"

class Quad : public Object
{
public:
	Quad();
	~Quad();

public:
	void Init();
	void Bind(FrameBuffer framebuffer);
};

