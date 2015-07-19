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
#include "assembly.h"


namespace Yuni
{
namespace VM
{

	inline void Assembly::nop()
	{
		pProgram.add(Private::VM::Instruction::nop);
	}

	inline void Assembly::exit()
	{
		pProgram.add(Private::VM::Instruction::exit);
	}

	inline void Assembly::exitCode(uint8 r1)
	{
		pProgram.add(Private::VM::Instruction::exitCode, r1);
	}

	inline void Assembly::exitCodei(sint8 i)
	{
		pProgram.add(Private::VM::Instruction::exitCode, i);
	}

	inline void Assembly::intrinsic()
	{
		pProgram.add(Private::VM::Instruction::intrinsic, static_cast<uint8>(0));
	}

	inline void Assembly::intrinsic(uint8 r1)
	{
		pProgram.add(Private::VM::Instruction::intrinsic, static_cast<uint8>(1), r1);
	}

	inline void Assembly::intrinsic(uint8 r1, uint8 r2)
	{
		pProgram.add(Private::VM::Instruction::intrinsic, static_cast<uint8>(2), r1, r2);
	}

	inline void Assembly::intrinsic(uint8 r1, uint8 r2, uint8 r3)
	{
		pProgram.add(Private::VM::Instruction::intrinsic, static_cast<uint8>(3), r1, r2, r3);
	}

	inline void Assembly::intrinsic(uint8 r1, uint8 r2, uint8 r3, uint8 r4)
	{
		pProgram.add(Private::VM::Instruction::intrinsic, static_cast<uint8>(4), r1, r2, r3, r4);
	}




	inline void Assembly::add(uint8 ret, uint8 r1, uint8 r2)
	{
		pProgram.add(Private::VM::Instruction::add, ret, r1, r2);
	}

	inline void Assembly::addu(uint8 ret, uint8 r1, uint8 r2)
	{
		pProgram.add(Private::VM::Instruction::addu, ret, r1, r2);
	}

	inline void Assembly::addi(uint8 ret, uint8 r1, sint64 i)
	{
		pProgram.add(Private::VM::Instruction::addi, ret, r1, i);
	}

	inline void Assembly::addui(uint8 ret, uint8 r1, uint64 i)
	{
		pProgram.add(Private::VM::Instruction::addui, ret, r1, i);
	}



	inline bool Assembly::validate() const
	{
		return pProgram.validate();
	}


	inline int execute()
	{
		return pProgram.execute();
	}



} // namespace VM
} // namespace Yuni
