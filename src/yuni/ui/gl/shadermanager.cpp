#include <yuni/io/filename-manipulation.h>
#include "shadermanager.h"

namespace Yuni
{
namespace Gfx3D
{


	const ShaderProgram::Ptr ShaderManager::get(const AnyString& vShader, const AnyString& fShader)
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


} // namespace Gfx3D
} // namespace Yuni
