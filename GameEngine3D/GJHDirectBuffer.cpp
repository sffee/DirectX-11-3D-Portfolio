#include "GJHDirectBuffer.h"
#include <GJHGameEngineDebug.h>

std::map<int, std::map<ShaderType, std::map<int, ID3D11Buffer*>>> GJHDirectBuffer::ConstBufferPool;