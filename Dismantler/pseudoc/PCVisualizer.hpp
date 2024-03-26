#pragma once
#include "PCBlob.hpp"

class PCVisualizer
{
public:
	void Print(const PCBlob& blob) const;

private:
	void PrintOperand(const PCBlob& blob, const PCOperand& operand) const;
};