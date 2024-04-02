#pragma once
#include "kara/KaraConverter.hpp"
#include "pe/PEBuffer.hpp"
#include "PCBlob.hpp"

class PCConverter
{
public:
	void Convert(const PEBuffer& buffer, uintptr_t function, PCBlob& blob) const;

private:
	KaraConverter m_Kara;
};