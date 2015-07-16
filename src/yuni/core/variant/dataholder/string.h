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
#ifndef __YUNI_CORE_VARIANT_VARIANT_DATAHOLDER_STRING_H__
# define __YUNI_CORE_VARIANT_VARIANT_DATAHOLDER_STRING_H__


namespace Yuni
{
namespace Private
{
namespace Variant
{

	/*!
	** \brief Concrete variant data container (string)
	*/
	template<>
	class Data<String> final : public IDataHolder
	{
	public:
		//! Constructor from the variable type
		explicit Data(const String& src) :
			pValue(src)
		{}

		//! Destructor
		virtual ~Data() {}

	protected:
		virtual bool convertUsing(IDataConverter& cvtr) const override
		{ return cvtr.convertFrom(pValue); }

		virtual IDataHolder* clone() const override
		{ return new Data<String>(pValue); }

		virtual Yuni::VariantInnerType type() const override {return Yuni::variantTString;}

		virtual void clear() override { pValue.clear(); }

		virtual IDataHolder* at(uint index) override { return index < pValue.size() ? new Data<char>(pValue[index]) : NULL; }
		virtual const IDataHolder* at(uint index) const override { return index < pValue.size() ? new Data<char>(pValue[index]) : NULL; }

		virtual void assignList(const IDataHolder::Vector&) override {}
		virtual void assign(uint32 n) override { pValue = n; }
		virtual void assign(sint32 n) override { pValue = n; }
		virtual void assign(uint64 n) override { pValue = n; }
		virtual void assign(sint64 n) override { pValue = n; }
		virtual void assign(double n) override { pValue = n; }
		virtual void assign(const String& n) override { pValue = n; }
		virtual void assign(bool n) override { pValue = n; }
		virtual void assign(char n) override { pValue = n; }

		virtual void addList(const IDataHolder::Vector&) override {}
		virtual void add(uint32 n) override { pValue += n; }
		virtual void add(sint32 n) override { pValue += n; }
		virtual void add(uint64 n) override { pValue += n; }
		virtual void add(sint64 n) override { pValue += n; }
		virtual void add(double n) override { pValue += n; }
		virtual void add(const String& n) override { pValue += n; }
		virtual void add(bool n) override { pValue += n; }
		virtual void add(char n) override { pValue += n; }

		virtual void sub(uint32) override {}
		virtual void sub(sint32) override {}
		virtual void sub(uint64) override {}
		virtual void sub(sint64) override {}
		virtual void sub(double) override {}
		virtual void sub(const String& n) override { pValue.replace(n, ""); }
		virtual void sub(bool) override {}
		virtual void sub(char) override {}

		virtual void mult(uint32) override {}
		virtual void mult(sint32) override {}
		virtual void mult(uint64) override {}
		virtual void mult(sint64) override {}
		virtual void mult(double) override {}
		virtual void mult(bool) override { /* do nothing*/ }
		virtual void mult(char) override {}
		virtual void mult(const String&) override {}

		virtual void div(uint32) override {}
		virtual void div(sint32) override {}
		virtual void div(uint64) override {}
		virtual void div(sint64) override {}
		virtual void div(double) override {}
		virtual void div(bool) override {}
		virtual void div(char) override {}
		virtual void div(const String&) override {}

		virtual bool isEquals(uint32 n) const override { return pValue.to<uint32>() == n; }
		virtual bool isEquals(sint32 n) const override { return pValue.to<sint32>() == n; }
		virtual bool isEquals(uint64 n) const override { return pValue.to<uint64>() == n; }
		virtual bool isEquals(sint64 n) const override { return pValue.to<sint64>() == n; }
		virtual bool isEquals(double n) const override { return Math::Equals(pValue.to<double>(), n); }
		virtual bool isEquals(bool n) const override { return n == pValue.to<bool>(); }
		virtual bool isEquals(char n) const override { return pValue.size() == 1 and n == pValue[0]; }
		virtual bool isEquals(const String& n) const override { return pValue == n; }

		virtual void loopbackAssign(IDataHolder& dataholder) const override { dataholder.assign(pValue); }
		virtual void loopbackAdd(IDataHolder& dataholder) const override { dataholder.add(pValue); }
		virtual void loopbackMultiply(IDataHolder& dataholder) const override { dataholder.mult(pValue); }
		virtual void loopbackSub(IDataHolder& dataholder) const override { dataholder.sub(pValue); }
		virtual void loopbackDiv(IDataHolder& dataholder) const override { dataholder.div(pValue); }
		virtual bool loopbackIsEquals(IDataHolder& dataholder) const override { return dataholder.isEquals(pValue); };

	private:
		//! The real data element.
		String pValue;

	}; // class Data<String>




} // namespace Variant
} // namespace Private
} // namespace Yuni

#endif /* !__YUNI_CORE_VARIANT_VARIANT_DATAHOLDER_STRING_H__ */
