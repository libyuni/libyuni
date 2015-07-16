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
#ifndef __YUNI_PRIVATE_MEDIA_OPENAL_H__
# define __YUNI_PRIVATE_MEDIA_OPENAL_H__

# include "../../yuni.h"
# include <list>
# include "../../core/vector3D.h"
# include "../../core/point3D.h"
# ifdef YUNI_OS_MACOS
#	include <OpenAL/al.h>
#	include <OpenAL/alc.h>
# else
#	include <al.h>
#	include <alc.h>
# endif

namespace Yuni
{
namespace Private
{
namespace Media
{

	/*!
	** \brief OpenAL wrapper
	*/
	class OpenAL final
	{
	public:
		//! \name Enums
		//@{
		enum DistanceModel
		{
			None,
			InverseDistance,
			InverseDistanceClamped,
			LinearDistance,
			LinearDistanceClamped,
			ExponentDistance,
			ExponentDistanceClamped
		};
		//@}


	public:
		/*!
		** \brief Initialize OpenAL device and context
		*/
		static bool Init();

		/*!
		** \brief Close OpenAL context and device
		*/
		static bool Close();

		/*!
		** \brief Convert to an OpenAL format
		** \param bits Number of bits per sample
		** \param channels Number of channels
		** \returns An ALenum containing the format, 0 if none found
		*/
		static ALenum GetFormat(uint bits, uint channels);

		static void SetDistanceModel(DistanceModel model);


		/*!
		** \brief Create an OpenAL buffer
		** \param[out] A pointer to the ID of the created buffer
		** \returns false on error, true otherwise
		*/
		static bool CreateBuffer(uint* buffer);
		/*!
		** \brief Create OpenAL buffers
		** \param[in] nbBuffers Number of buffers to create
		** \param[out] An array of IDs of the created buffers
		** \returns false on error, true otherwise
		*/
		static bool CreateBuffers(int nbBuffers, uint* buffers);

		static void DestroyBuffers(int nbBuffers, uint* buffers);

		static void SetListener(float position[3], float velocity[3], float orientation[6]);

		/*!
		** \brief Create an OpenAL source
		** \returns The source's ID, 0 if an error is encountered.
		*/
		static uint CreateSource(Point3D<> position, Vector3D<> velocity,
			Vector3D<> direction, float pitch, float gain, bool attenuate, bool loop);

		//! Destroy an OpenAL source
		static void DestroySource(uint source);

		//! Play an OpenAL source
		static bool PlaySource(uint source);

		//! Stop an OpenAL source
		static bool StopSource(uint source);

		//! Pause an OpenAL source
		static bool PauseSource(uint source);

		//! Is the source currently playing ?
		static bool IsSourcePlaying(uint source);

		//! Is the source currently playing ?
		static bool IsSourcePaused(uint source);

		//! Modify characteristics of an existing source
		static bool ModifySource(uint source, float pitch, float gain,
			bool attenuate, bool loop);
		//! Move an existing source
		static bool MoveSource(uint source, const Point3D<>& position,
			const Vector3D<>& velocity, const Vector3D<>& direction);

		//! Bind a sound buffer to an OpenAL source
		static bool BindBufferToSource(uint buffer, uint source);

		//! Unbind an OpenAL source from any buffer
		static void UnbindBufferFromSource(uint source);

		//! Queue a sound buffer for playing on a source
		static bool QueueBufferToSource(uint buffer, uint source);

		//! Unqueue a sound buffer from a source
		static uint UnqueueBufferFromSource(uint source);

		//! Get current playback position on a source
		static float SourcePlaybackPosition(uint source);

		//! Set current playback position on a source
		static void SetSourcePlaybackPosition(uint source, float position);

		//! Set data on a sound buffer
		static bool SetBufferData(uint buffer, int format, void* data, size_t count, int rate);

	}; // class OpenAL





} // namespace Media
} // namespace Private
} // namespace Yuni

#endif // __YUNI_PRIVATE_MEDIA_OPENAL_H__
