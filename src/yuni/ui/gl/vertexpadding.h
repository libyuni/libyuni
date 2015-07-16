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
#ifndef __YUNI_GFX3D_VERTEX_PADDING_H__
# define __YUNI_GFX3D_VERTEX_PADDING_H__

# include <yuni/yuni.h>
# include <yuni/core/static/assert.h>

namespace Yuni
{
namespace Gfx3D
{


	/*!
	** \brief Pad with useless data to reach a multiple of 32 bytes
	**
	** Template specializations cater to all possible cases
	*/
	template<uint size>
	class VertexPadding
	{
	private:
		enum // anonymous
		{
			PaddingSize = (size < 32) ? 32 - size : (size < 64) ? 64 - size : 96 - size

		}; // enum anonymous

		YUNI_STATIC_ASSERT(size <= 96, InvalidSizeForVertexPadding);

	protected:
		VertexPadding()
		{
			for (uint i = 0; i < PaddingSize; ++i)
				pPadding[i] = '\0';
		}
		VertexPadding(const VertexPadding& other):
			pPadding(other.pPadding)
		{}

	private:
		char pPadding[PaddingSize];

	}; // class VertexPadding


	// Size is 32. No Padding
	template<>
	class VertexPadding<32u>
	{
	};


	// Size is 64. No Padding
	template<>
	class VertexPadding<64u>
	{
	};


	// Size is 96. No Padding
	template<>
	class VertexPadding<96u>
	{
	};


} // namespace Gfx3D
} // namespace Yuni

#endif // __YUNI_GFX3D_VERTEX_PADDING_H__
