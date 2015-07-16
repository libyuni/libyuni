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
#ifndef YUNI_UNIT_IMPL



# define YUNI_UNIT_IMPL(CLASS,NAME,SYMBOL,COEFFTYPE,COEFF) \
	\
	template<typename T = double, typename P = T> \
	class CLASS final : public Abstract<Quantity, CLASS<T,P>, T, P, COEFFTYPE> \
	{ \
	public: \
		/*! Base class */ \
		typedef Abstract<Quantity, CLASS<T,P>, T,P, COEFFTYPE> AbstractType;  \
		/*! Type */ \
		typedef CLASS<T,P> Type; \
		/*! Unit of measurement */ \
		static const char* Symbol() {return SYMBOL;} \
		/*! Unit of measurement */ \
		static const char* UnitName() {return NAME;} \
		/*! Relation to SI */ \
		static COEFFTYPE RelationToSI() {return (COEFFTYPE)(COEFF);}\
		\
	public: \
		/*! Default constructor */ \
		CLASS() {} \
		/*! Constructor from a fundamental value */ \
		CLASS(const sint16 u) :AbstractType(u) {} \
		/*! Constructor from a fundamental value */ \
		CLASS(const sint32 u) :AbstractType(u) {} \
		/*! Constructor from a fundamental value */ \
		CLASS(const sint64 u) :AbstractType(u) {} \
		/*! Constructor from a fundamental value */ \
		CLASS(const uint16 u) :AbstractType(u) {} \
		/*! Constructor from a fundamental value */ \
		CLASS(const uint32 u) :AbstractType(u) {} \
		/*! Constructor from a fundamental value */ \
		CLASS(const uint64 u) :AbstractType(u) {} \
		/*! Constructor from a fundamental value */ \
		CLASS(const float u) :AbstractType(u) {} \
		/*! Constructor from a fundamental value */ \
		CLASS(const double u) :AbstractType(u) {} \
		/*! Constructor from another unit */ \
		template<class Q1, typename T1, class C1, typename P1, typename N1> \
		CLASS(const Abstract<Q1,C1, T1,P1, N1>& rhs) :AbstractType(rhs) {} \
		\
		/*! \name Operator `=` */ \
		/*@{*/ \
		/*! Builtin types */ \
		AbstractType& operator = (const sint16 u) {return AbstractType::operator = (u);} \
		AbstractType& operator = (const sint32 u) {return AbstractType::operator = (u);} \
		AbstractType& operator = (const sint64 u) {return AbstractType::operator = (u);} \
		AbstractType& operator = (const uint16 u) {return AbstractType::operator = (u);} \
		AbstractType& operator = (const uint32 u) {return AbstractType::operator = (u);} \
		AbstractType& operator = (const uint64 u) {return AbstractType::operator = (u);} \
		AbstractType& operator = (const float u) {return AbstractType::operator = (u);} \
		AbstractType& operator = (const double u) {return AbstractType::operator = (u);} \
		/*! Itself (same metric) */ \
		template<typename P1> \
		AbstractType& operator = (const Abstract<Quantity,Type,T,P1,COEFFTYPE>& rhs) {return AbstractType::operator = (rhs);} \
		/* Another metric system */ \
		template<class Q1, typename T1, class C1, typename P1, typename N1> \
		AbstractType& operator = (const Abstract<Q1,C1,T1,P1,N1>& rhs) \
		{return AbstractType::operator = (rhs);} \
		/*@}*/ \
	}



#endif

