/*
** YUNI's default license is the GNU Lesser Public License (LGPL), with some
** exclusions (see below). This basically means that you can get the full source
** code for nothing, so long as you adhere to a few rules.
**
** Under the LGPL you may use YUNI for any purpose you wish, and modify it if you
** require, as long as you:
**
** Pass on the (modified) YUNI source code with your software, with original
** copyrights intact :
**  * If you distribute electronically, the source can be a separate download
**    (either from your own site if you modified YUNI, or to the official YUNI
**    website if you used an unmodified version) – just include a link in your
**    documentation
**  * If you distribute physical media, the YUNI source that you used to build
**    your application should be included on that media
** Make it clear where you have customised it.
**
** In addition to the LGPL license text, the following exceptions / clarifications
** to the LGPL conditions apply to YUNI:
**
**  * Making modifications to YUNI configuration files, build scripts and
**    configuration headers such as yuni/platform.h in order to create a
**    customised build setup of YUNI with the otherwise unmodified source code,
**    does not constitute a derived work
**  * Building against YUNI headers which have inlined code does not constitute a
**    derived work
**  * Code which subclasses YUNI classes outside of the YUNI libraries does not
**    form a derived work
**  * Statically linking the YUNI libraries into a user application does not make
**    the user application a derived work.
**  * Using source code obsfucation on the YUNI source code when distributing it
**    is not permitted.
** As per the terms of the LGPL, a "derived work" is one for which you have to
** distribute source code for, so when the clauses above define something as not
** a derived work, it means you don't have to distribute source code for it.
** However, the original YUNI source code with all modifications must always be
** made available.
*/
#pragma once
#include "program.h"



namespace Yuni
{
namespace Private
{
namespace VM
{

	namespace // anonymous
	{
		template<class T>
		struct OperandConverter
		{
			enum
			{
				byteCount = sizeof(T),
			};

			static void Write(char* data, uint& count, T value)
			{
				*(reinterpret_cast<T*>(data + count)) = value;
				count += byteCount;
			}
		};

	} // anonymous namespace



	inline void Program::add(InstructionType instruction)
	{
		if (instructionCount + 1 > instructionCapacity)
			increaseInstructionCapacity();
		instructions[instructionCount] = instruction;
		++instructionCount;
	}


	template<class T1>
	inline void Program::add(InstructionType instruction, T1 r1)
	{
		if (instructionCount + 1 > instructionCapacity)
			increaseInstructionCapacity();
		instructions[instructionCount] = instruction;
		++instructionCount;

		enum
		{
			byteCount = OperandConverter<T1>::byteCount,
		};
		if (operandCount + byteCount > operandCapacity)
			increaseOperandCapacity();
		OperandConverter<T1>::Write(operands, operandCount, r1);
	}


	template<class T1, class T2>
	inline void Program::add(InstructionType instruction, T1 r1, T2 r2)
	{
		if (instructionCount + 1 > instructionCapacity)
			increaseInstructionCapacity();
		instructions[instructionCount] = instruction;
		++instructionCount;

		enum
		{
			byteCount = OperandConverter<T1>::byteCount + OperandConverter<T2>::byteCount,
		};
		if (operandCount + byteCount > operandCapacity)
			increaseOperandCapacity();
		OperandConverter<T1>::Write(operands, operandCount, r1);
		OperandConverter<T2>::Write(operands, operandCount, r2);
	}


	template<class T1, class T2, class T3>
	inline void Program::add(InstructionType instruction, T1 r1, T2 r2, T3 r3)
	{
		if (instructionCount + 1 > instructionCapacity)
			increaseInstructionCapacity();
		instructions[instructionCount] = instruction;
		++instructionCount;

		enum
		{
			byteCount = OperandConverter<T1>::byteCount + OperandConverter<T2>::byteCount
				+ OperandConverter<T3>::byteCount,
		};
		if (operandCount + byteCount > operandCapacity)
			increaseOperandCapacity();
		OperandConverter<T1>::Write(operands, operandCount, r1);
		OperandConverter<T2>::Write(operands, operandCount, r2);
		OperandConverter<T3>::Write(operands, operandCount, r3);
	}


	template<class T1, class T2, class T3, class T4>
	inline void Program::add(InstructionType instruction, T1 r1, T2 r2, T3 r3, T4 r4)
	{
		if (instructionCount + 1 > instructionCapacity)
			increaseInstructionCapacity();
		instructions[instructionCount] = instruction;
		++instructionCount;

		enum
		{
			byteCount = OperandConverter<T1>::byteCount + OperandConverter<T2>::byteCount
				+ OperandConverter<T3>::byteCount + OperandConverter<T4>::byteCount,
		};
		if (operandCount + byteCount > operandCapacity)
			increaseOperandCapacity();
		OperandConverter<T1>::Write(operands, operandCount, r1);
		OperandConverter<T2>::Write(operands, operandCount, r2);
		OperandConverter<T3>::Write(operands, operandCount, r3);
		OperandConverter<T4>::Write(operands, operandCount, r4);
	}


	template<class T1, class T2, class T3, class T4, class T5>
	inline void Program::add(InstructionType instruction, T1 r1, T2 r2, T3 r3, T4 r4, T5 r5)
	{
		if (instructionCount + 1 > instructionCapacity)
			increaseInstructionCapacity();
		instructions[instructionCount] = instruction;
		++instructionCount;

		enum
		{
			byteCount = OperandConverter<T1>::byteCount + OperandConverter<T2>::byteCount
				+ OperandConverter<T3>::byteCount + OperandConverter<T4>::byteCount
				+ OperandConverter<T5>::byteCount,
		};
		if (operandCount + byteCount > operandCapacity)
			increaseOperandCapacity();
		OperandConverter<T1>::Write(operands, operandCount, r1);
		OperandConverter<T2>::Write(operands, operandCount, r2);
		OperandConverter<T3>::Write(operands, operandCount, r3);
		OperandConverter<T4>::Write(operands, operandCount, r4);
		OperandConverter<T5>::Write(operands, operandCount, r5);
	}




} // namespace VM
} // namespace Private
} // namespace Yuni
