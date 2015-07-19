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
#include <yuni/io/filename-manipulation.h>
#include "shadermanager.h"



namespace Yuni
{
namespace Gfx3D
{


	const ShaderProgram::Ptr ShaderManager::getFromFiles(const AnyString& vShader, const AnyString& fShader)
	{
		String fullPath;
		// Use the canonicalized path as key
		Yuni::IO::Canonicalize(fullPath, vShader);

		// Search for this shader in the vertex shader cache
		VertexShader::Ptr vertex;
		auto vsIt = pVertexShaders.find(fullPath);
		if (pVertexShaders.end() != vsIt)
			vertex = vsIt->second;
		else
		{
			// Create and compile it
			vertex = new VertexShader(fullPath);
			if (!vertex->valid())
				return nullptr;
			// Add the shader to cache
			pVertexShaders[fullPath] = vertex;
		}

		// Use the canonicalized path as key
		Yuni::IO::Canonicalize(fullPath, fShader);
		// Search for this shader in the fragment shader cache
		FragmentShader::Ptr fragment;
		auto fsIt = pFragmentShaders.find(fullPath);
		if (pFragmentShaders.end() != fsIt)
			fragment = fsIt->second;
		else
		{
			// Create and compile it
			fragment = new FragmentShader(fullPath);
			if (!fragment->valid())
				return nullptr;
			// Add the shader to cache
			pFragmentShaders[fullPath] = fragment;
		}

		// Search for this pair of shaders in the shader program cache
		ShaderPair key(vertex->id(), fragment->id());
		auto progIt = pPrograms.find(key);
		if (pPrograms.end() != progIt)
			return progIt->second;

		// Add the pair to cache
		ShaderProgram::Ptr newProg = new ShaderProgram(vertex, fragment);
		pPrograms[key] = newProg;
		return newProg;
	}


	const ShaderProgram::Ptr ShaderManager::getFromMemory(const AnyString& vsCode, const AnyString& fsCode)
	{
		// Use the shader code directly as key, we can't do much better than this I'm afraid.
		VertexShader::Ptr vertex;
		auto vsIt = pVertexShaders.find(vsCode);
		if (pVertexShaders.end() != vsIt)
			vertex = vsIt->second;
		else
		{
			// Create and compile it
			vertex = new VertexShader();
			vertex->loadFromMemory(vsCode);
			if (!vertex->valid())
				return nullptr;
			// Add the shader to cache
			pVertexShaders[vsCode] = vertex;
		}

		FragmentShader::Ptr fragment;
		auto fsIt = pFragmentShaders.find(fsCode);
		if (pFragmentShaders.end() != fsIt)
			fragment = fsIt->second;
		else
		{
			// Create and compile it
			fragment = new FragmentShader();
			fragment->loadFromMemory(fsCode);
			if (!fragment->valid())
				return nullptr;
			// Add the shader to cache
			pFragmentShaders[fsCode] = fragment;
		}

		// Search for this pair of shaders in the shader program cache
		ShaderPair key(vertex->id(), fragment->id());
		auto progIt = pPrograms.find(key);
		if (pPrograms.end() != progIt)
			return progIt->second;

		// Add the pair to cache
		ShaderProgram::Ptr newProg = new ShaderProgram(vertex, fragment);
		pPrograms[key] = newProg;
		return newProg;
	}





} // namespace Gfx3D
} // namespace Yuni
