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
#ifndef __YUNI_AUDIO_PLAYLIST_H__
# define __YUNI_AUDIO_PLAYLIST_H__

# include <vector>
# include "sound.h"

namespace Yuni
{
namespace Audio
{



	/*!
	** \brief A play list contains an ordered list of sounds to be played
	*/
	class PlayList: public Policy::ObjectLevelLockable<PlayList>
	{
	public:
		//! \name Typedefs
		//@{

		//! The most suitable smart pointer for the class
		typedef SmartPtr<PlayList> Ptr;
		//! Size type
		typedef uint Size;
		//! The Threading Policy
		typedef Policy::ObjectLevelLockable<ISound> ThreadingPolicy;

		//@}

	public:
		//! Empty constructor
		PlayList()
			: pCurrentIndex(0), pLoop(false)
		{}

	public:
		Size size() const { return pSoundList.size(); }

		/*!
		** \brief Add a sound at the end of the playlist
		*/
		void append(SmartPtr<ISound>& sound);

		template<typename StringT>
		void remove(const StringT& soundID);

		void play() const;
		void pause(bool paused);

		void previous();
		void next();
		void shuffle();

	private:
		//! All the sounds currently in the play list (ordered)
		ISound::Vector pSoundList;
		//! Index of the current song being played / next to play
		uint pCurrentIndex;
		//! Should we loop around the selection?
		bool pLoop;

	}; // class PlayList



} // namespace Audio
} // namespace Yuni

#endif  // __YUNI_AUDIO_PLAYLIST_H__
