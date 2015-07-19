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
#include "program.h"
#include "instructions.h"
#include <cstdlib>
#include <cstring>


namespace Yuni
{
namespace Private
{
namespace VM
{

	namespace // anonymous
	{

		class ProcessorData
		{
		public:
			ProcessorData()
				:exitCode(0)
			{
				memset(gpr, 0, sizeof(gpr));
				memset(spr, 0, sizeof(spr));
				memset(dpr, 0, sizeof(dpr));
			}

		public:
			//! General purpose registers
			uint64 gpr[16];
			//! Single-precision registers
			float spr[16];
			//! Double-precision registers
			double dpr[16];
			//! Exit code
			int exitCode;

		}; // class ProcessorData


	} // anonymous namespace



	int Program::execute()
	{
		// Implementation : Direct dispatch
		// Next instruction
		# define NEXT  goto **vp++

		// Converting each instructions into a list of goto jump
		void** jumps;
		{
			uint count = instructionCount;
			if (!count)
				return 0;
			jumps = (void**)::malloc(sizeof(void**) * (count + 1));
			void* const aliases[Instruction::max] =
			{
				&&j_exit,  // exit
				&&j_intrinsic,    // intrinsic
				&&j_add,
				&&j_addu,
				&&j_addi,
				&&j_addui,
				&&j_nop,
				&&j_exitcode,
				&&j_exitcodei
			};
			// We assume here that all instructions are valid
			// The first instruction will always be the 'exit' instruction
			jumps[0] = &&j_exit;
			for (uint i = 0; i != count; ++i)
				jumps[i + 1] = aliases[instructions[i]];
		}

		// data for our virtual processor
		ProcessorData data;
		// pointer, on the first instruction
		void** vp = ((void**) jumps) + 1;
		// The current operand
		uint op = 0;

		// execute the first instruction
		NEXT;

		// implementations of all instructions
		j_intrinsic:
			{
				uint params = (uint) operands[op++];
				// FIXME
				op += params;
				NEXT;
			}

		j_add:
			{
				uint ret = (uint) operands[op++];
				uint r1  = (uint) operands[op++];
				uint r2  = (uint) operands[op++];
				*((sint64*)(data.gpr) + ret) = (sint64)(data.gpr[r1]) + (sint64)(data.gpr[r2]);
				NEXT;
			}
		j_addu:
			{
				uint ret = (uint) operands[op++];
				uint r1  = (uint) operands[op++];
				uint r2  = (uint) operands[op++];
				data.gpr[ret] = data.gpr[r1] + data.gpr[r2];
				NEXT;
			}
		j_addi:
			{
				uint ret = (uint) operands[op++];
				uint r1  = (uint) operands[op++];
				const sint64 i = *((sint64*)(operands + op));
				op += 8;
				*((sint64*)(data.gpr) + ret) = (sint64)(data.gpr[r1]) + i;
				NEXT;
			}
		j_addui:
			{
				uint ret = (uint) operands[op++];
				uint r1  = (uint) operands[op++];
				uint64 i = *((uint64*)(operands + op));
				op += 8;
				data.gpr[ret] = data.gpr[r1] + i;
				NEXT;
			}
		j_nop:
			{
				// do nothing on purpose
				NEXT;
			}
		j_exitcode:
			{
				uint r1 = (uint) operands[op++];
				data.exitCode = (int) data.gpr[r1];
				NEXT;
			}
		j_exitcodei:
			{
				data.exitCode = (int) operands[op++];
				NEXT;
			}

		j_exit:
		(void)::free(jumps);
		return data.exitCode;
		# undef NEXT
	}





} // namespace VM
} // namespace Private
} // namespace Yuni

