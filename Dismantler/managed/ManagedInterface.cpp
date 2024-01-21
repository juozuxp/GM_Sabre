#include <Disassembler.hpp>
#include <Visualizer.hpp>

#include "ManagedObject.hpp"
#include "ManagedArray.hpp"
#include "ManagedString.hpp"

#include <Windows.h>

//extern "C"
//__declspec(dllexport)
//void Dismantler_Init(Disassembler** handle)
//{
//	*handle = new Disassembler();
//}
//
//extern "C"
//__declspec(dllexport)
//void Dismantler_Destroy(Disassembler* handle)
//{
//	delete handle;
//}
//
//extern "C"
//__declspec(dllexport)
//void Dismantler_Dissasemble(Disassembler* handle, void* address, uint64_t size, ManagedInterface::ManagedArray<ILInstruction>* array)
//{
//	*array = ManagedInterface::ManagedArray<ILInstruction>(handle->Disassemble(address, size));
//}
//
//extern "C"
//__declspec(dllexport)
//void ManagedArray_Destroy(void* array)
//{
//	delete[] array;
//}

extern "C"
__declspec(dllexport)
void Managed_Destroy(ManagedObject* handle)
{
	delete handle;
}

extern "C"
__declspec(dllexport)
ManagedArray<ManagedString>* Disassembler_Dissasemble(void* code, uint32_t size)
{
	Disassembler disassembler;
	Visualizer visualizer;

	std::vector<ILInstruction> instructions = disassembler.Disassemble(code, size);
	std::vector<std::wstring> strings = visualizer.ToStringArray(instructions);

	std::vector<ManagedString*> managed;

	managed.reserve(strings.size());
	for (const std::wstring& string : strings)
	{
		managed.push_back(new ManagedString(string));
	}

	return new ManagedArray<ManagedString>(managed);
}

extern "C"
__declspec(dllexport)
void Test_GetArray(ManagedArray<ManagedString>** array)
{
	std::vector<ManagedString*> strings;

	strings.push_back(new ManagedString(L"This"));
	strings.push_back(new ManagedString(L"Is"));
	strings.push_back(new ManagedString(L"Cool"));

	*array = new ManagedArray<ManagedString>(strings);
}