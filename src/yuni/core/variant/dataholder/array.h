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
#ifndef __YUNI_CORE_VARIANT_VARIANT_DATAHOLDER_ARRAY_H__
# define __YUNI_CORE_VARIANT_VARIANT_DATAHOLDER_ARRAY_H__


namespace Yuni
{
namespace Private
{
namespace Variant
{

	/*!
	** \brief Concrete variant data container
	*/
	template<class T>
	class ArrayData final : public IDataHolder
	{
	public:
		ArrayData() {}
		//! Constructor from the variable type
		ArrayData(const ArrayData<T>& rhs) :
			pValue(rhs.pValue)
		{}

		//! Destructor
		virtual ~ArrayData() {}

	protected:
		virtual bool convertUsing(IDataConverter& cvtr) const override
		{ return cvtr.convertFrom(pValue); }

		virtual IDataHolder* clone() const override { return new ArrayData<T>(*this); }

		virtual Yuni::VariantInnerType type() const override {return Yuni::variantTArray;}

		virtual void clear() override { pValue.clear(); }

		virtual void assignList(const IDataHolder::Vector& n) override
		{
			pValue.clear();
			pValue.reserve(n.size());
			for (uint i = 0; i != n.size(); ++i)
				pValue.push_back(n[i]);
		}
		virtual void assign(uint32) override {}
		virtual void assign(sint32) override {}
		virtual void assign(uint64) override {}
		virtual void assign(sint64) override {}
		virtual void assign(double) override {}
		virtual void assign(const String&) override {}
		virtual void assign(bool) override {}
		virtual void assign(char) override {}

		virtual void addList(const IDataHolder::Vector& n) override
		{
			pValue.reserve(pValue.size() + n.size());
			for (uint i = 0; i != n.size(); ++i)
				pValue.push_back(n[i]);
		}
		virtual void add(uint32 n) override { pValue.push_back(new Data<T>(n)); }
		virtual void add(sint32 n) override { pValue.push_back(new Data<T>(n)); }
		virtual void add(uint64 n) override { pValue.push_back(new Data<T>(n)); }
		virtual void add(sint64 n) override { pValue.push_back(new Data<T>(n)); }
		virtual void add(double n) override { pValue.push_back(new Data<T>(n)); }
		virtual void add(const String& n) override { pValue.push_back(new Data<T>(n.to<T>())); }
		virtual void add(bool n) { pValue.push_back(new Data<T>(n)); }
		virtual void add(char n) { pValue.push_back(new Data<T>(n)); }

		virtual void sub(uint32) override {}
		virtual void sub(sint32) override {}
		virtual void sub(uint64) override {}
		virtual void sub(sint64) override {}
		virtual void sub(double) override {}
		virtual void sub(const String&) override {}
		virtual void sub(bool) override {}
		virtual void sub(char) override {}

		virtual void mult(uint32) override {}
		virtual void mult(sint32) override {}
		virtual void mult(uint64) override {}
		virtual void mult(sint64) override {}
		virtual void mult(double) override {}
		virtual void mult(bool) override {}
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

		virtual bool isEquals(uint32) const override { return false; }
		virtual bool isEquals(sint32) const override { return false; }
		virtual bool isEquals(uint64) const override { return false; }
		virtual bool isEquals(sint64) const override { return false; }
		virtual bool isEquals(double) const override { return false; }
		virtual bool isEquals(bool) const override { return false; }
		virtual bool isEquals(char) const override { return false; }
		virtual bool isEquals(const String&) const override { return false; }

		//! Method invokation, with no parameter
		virtual IDataHolder* invoke(const String& name) override
		{
			if (name == "size")
			{
				return new Data<uint>(pValue.size());
			}
			if (name == "clear")
			{
				pValue.clear();
				return nullptr;
			}
			return nullptr;
		}
		//! Method invokation, with 1 parameter
		virtual IDataHolder* invoke(const String& name, IDataHolder* a1) override
		{
			if (name == "add" or name == "append")
			{
				if (a1)
					pValue.push_back(a1);
				return nullptr;
			}
			return nullptr;
		}

		virtual IDataHolder* at(uint index) override
		{
			return (index < pValue.size())
				? IDataHolder::Ptr::WeakPointer(pValue[index])
				: nullptr;
		}
		virtual const IDataHolder* at(uint index) const override
		{
			return (index < pValue.size())
				? IDataHolder::Ptr::WeakPointer(pValue[index])
				: nullptr;
		}

		virtual void loopbackAssign(IDataHolder& dataholder) const override { dataholder.assignList(pValue); }
		virtual void loopbackAdd(IDataHolder& dataholder) const override { dataholder.addList(pValue); }
		virtual void loopbackMultiply(IDataHolder&) const override {}
		virtual void loopbackSub(IDataHolder&) const override {}
		virtual void loopbackDiv(IDataHolder&) const override {}
		virtual bool loopbackIsEquals(IDataHolder&) const override { assert(false and "not implemented"); return false; };

	private:
		//! The real data element.
		IDataHolder::Vector  pValue;

	}; // class Data





} // namespace Variant
} // namespace Private
} // namespace Yuni

#endif // __YUNI_CORE_VARIANT_VARIANT_DATAHOLDER_ARRAY_H__
