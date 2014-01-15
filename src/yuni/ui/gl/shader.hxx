#ifndef __YUNI_UI_GL_SHADER_HXX__
# define __YUNI_UI_GL_SHADER_HXX__



namespace Yuni
{
namespace Gfx3D
{


	inline IShader::IShader()
		: pID((uint) -1)
	{}


	# ifdef YUNI_HAS_CPP_MOVE
	inline IShader(IShader&& rhs)
		: pID(rhs.pID)
	{
		rhs.pID = invalidID;
	}
	# endif


	inline bool IShader::valid() const
	{
		return pID != (uint) -1;
	}


	inline uint IShader::id() const
	{
		return pID;
	}






} // namespace Gfx3D
} // namespace Yuni

#endif // __YUNI_UI_GL_SHADER_HXX__
