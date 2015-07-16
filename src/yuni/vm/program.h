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
#ifndef __YUNI_VM_PROGRAM_H__
# define __YUNI_VM_PROGRAM_H__

# include "../yuni.h"
# include "std.h"
# include "fwd.h"


namespace Yuni
{
namespace Private
{
namespace VM
{

	typedef Yuni::VM::InstructionType  InstructionType;



	class Program
	{
	public:

	public:
		Program();
		~Program();

		void clear();

		void add(InstructionType instruction);

		template<class T1>
		void add(InstructionType instruction, T1 r1);

		template<class T1, class T2>
		void add(InstructionType instruction, T1 r1, T2 r2);

		template<class T1, class T2, class T3>
		void add(InstructionType instruction, T1 r1, T2 r2, T3 r3);

		template<class T1, class T2, class T3, class T4>
		void add(InstructionType instruction, T1 r1, T2 r2, T3 r3, T4 r4);

		template<class T1, class T2, class T3, class T4, class T5>
		void add(InstructionType instruction, T1 r1, T2 r2, T3 r3, T4 r4, T5 r5);

		void reserveInstructions(uint count);
		void reserveOperands(uint count);

		/*!
		** \brief Validate the assembly code
		*/
		bool validate() const;

		/*!
		** \brief Execute the program
		**
		** The program should be validated first
		*/
		int execute();

	protected:
		void increaseInstructionCapacity();
		void increaseInstructionCapacity(uint chunkSize);
		void increaseOperandCapacity();
		void increaseOperandCapacity(uint chunkSize);

	public:
		//! Continuous list of instructions
		InstructionType* instructions;
		//! The number of instructions
		uint instructionCount;
		//! The capacity
		uint instructionCapacity;
		//! Continuous list of operands for each instructions
		char* operands;
		//! The number of operands
		uint operandCount;
		//!
		uint operandCapacity;

	}; // class Program





} // namespace VM
} // namespace Private
} // namespace Yuni

# include "program.hxx"

#endif // __YUNI_VM_PROGRAM_H__
