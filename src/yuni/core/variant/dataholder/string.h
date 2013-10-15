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
		virtual bool convertUsing(IDataConverter& cvtr) const
		{ return cvtr.convertFrom(pValue); }

		virtual IDataHolder* clone() const
		{ return new Data<String>(pValue); }

		virtual Yuni::Variant::InnerType type() const {return Yuni::Variant::tString;}

		virtual void clear() { pValue.clear(); }

		virtual IDataHolder* at(uint index) {return index < pValue.size() ? new Data<char>(pValue[index]) : NULL;}
		virtual const IDataHolder* at(uint index) const {return index < pValue.size() ? new Data<char>(pValue[index]) : NULL;}

		virtual void assignList(const IDataHolder::Vector&) {}
		virtual void assign(uint32 n) { pValue = n; }
		virtual void assign(sint32 n) { pValue = n; }
		virtual void assign(uint64 n) { pValue = n; }
		virtual void assign(sint64 n) { pValue = n; }
		virtual void assign(double n) { pValue = n; }
		virtual void assign(const String& n) { pValue = n; }
		virtual void assign(bool n) { pValue = n; }
		virtual void assign(char n) { pValue = n; }

		virtual void addList(const IDataHolder::Vector&) {}
		virtual void add(uint32 n) { pValue += n; }
		virtual void add(sint32 n) { pValue += n; }
		virtual void add(uint64 n) { pValue += n; }
		virtual void add(sint64 n) { pValue += n; }
		virtual void add(double n) { pValue += n; }
		virtual void add(const String& n) { pValue += n;}
		virtual void add(bool n) { pValue += n; }
		virtual void add(char n) { pValue += n; }

		virtual void sub(uint32) { }
		virtual void sub(sint32) { }
		virtual void sub(uint64) { }
		virtual void sub(sint64) { }
		virtual void sub(double) { }
		virtual void sub(const String& n) { pValue.replace(n, ""); }
		virtual void sub(bool) { }
		virtual void sub(char) { }

		virtual void mult(uint32) { }
		virtual void mult(sint32) { }
		virtual void mult(uint64) { }
		virtual void mult(sint64) { }
		virtual void mult(double) { }
		virtual void mult(bool) { /* do nothing*/ }
		virtual void mult(char) { }
		virtual void mult(const String&) { }

		virtual void div(uint32) { }
		virtual void div(sint32) { }
		virtual void div(uint64) { }
		virtual void div(sint64) { }
		virtual void div(double) { }
		virtual void div(bool) { }
		virtual void div(char) { }
		virtual void div(const String&) { }

		virtual bool isEquals(uint32 n) const {return pValue.to<uint32>() == n;}
		virtual bool isEquals(sint32 n) const {return pValue.to<sint32>() == n;}
		virtual bool isEquals(uint64 n) const {return pValue.to<uint64>() == n;}
		virtual bool isEquals(sint64 n) const {return pValue.to<sint64>() == n;}
		virtual bool isEquals(double n) const {return Math::Equals(pValue.to<double>(), n);}
		virtual bool isEquals(bool n) const {return n == pValue.to<bool>();}
		virtual bool isEquals(char n) const {return pValue.size() == 1 and n == pValue[0];}
		virtual bool isEquals(const String& n) const {return pValue == n;}

		virtual void loopbackAssign(IDataHolder& dataholder) const {dataholder.assign(pValue);}
		virtual void loopbackAdd(IDataHolder& dataholder) const {dataholder.add(pValue);}
		virtual void loopbackMultiply(IDataHolder& dataholder) const {dataholder.mult(pValue);}
		virtual void loopbackSub(IDataHolder& dataholder) const {dataholder.sub(pValue);}
		virtual void loopbackDiv(IDataHolder& dataholder) const {dataholder.div(pValue);}
		virtual bool loopbackIsEquals(IDataHolder& dataholder) const {return dataholder.isEquals(pValue);};

	private:
		//! The real data element.
		String pValue;

	}; // class Data<String>




} // namespace Variant
} // namespace Private
} // namespace Yuni

#include "dataholder.hxx"

#endif /* !__YUNI_CORE_VARIANT_VARIANT_DATAHOLDER_STRING_H__ */
