#pragma once

class SlangShader
{
public:
	SlangShader();
	virtual ~SlangShader();

public:
	void Initialize();

private:
	SlangShader _slang;
};