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
		virtual bool convertUsing(IDataConverter& cvtr) const
		{ return cvtr.convertFrom(pValue); }

		virtual IDataHolder* clone() const { return new ArrayData<T>(*this); }

		virtual Yuni::Variant::InnerType type() const {return Yuni::Variant::tArray;}

		virtual void clear() { pValue.clear(); }

		virtual void assignList(const IDataHolder::Vector& n)
		{
			pValue.clear();
			pValue.reserve(n.size());
			for (uint i = 0; i != n.size(); ++i)
				pValue.push_back(n[i]);
		}
		virtual void assign(uint32) { }
		virtual void assign(sint32) { }
		virtual void assign(uint64) { }
		virtual void assign(sint64) { }
		virtual void assign(double) { }
		virtual void assign(const String&) {  }
		virtual void assign(bool) { }
		virtual void assign(char) { }

		virtual void addList(const IDataHolder::Vector& n)
		{
			pValue.reserve(pValue.size() + n.size());
			for (uint i = 0; i != n.size(); ++i)
				pValue.push_back(n[i]);
		}
		virtual void add(uint32 n) { pValue.push_back(new Data<T>(n)); }
		virtual void add(sint32 n) { pValue.push_back(new Data<T>(n)); }
		virtual void add(uint64 n) { pValue.push_back(new Data<T>(n)); }
		virtual void add(sint64 n) { pValue.push_back(new Data<T>(n)); }
		virtual void add(double n) { pValue.push_back(new Data<T>(n)); }
		virtual void add(const String& n) { pValue.push_back(new Data<T>(n.to<T>())); }
		virtual void add(bool n) { pValue.push_back(new Data<T>(n)); }
		virtual void add(char n) { pValue.push_back(new Data<T>(n)); }

		virtual void sub(uint32) { }
		virtual void sub(sint32) { }
		virtual void sub(uint64) { }
		virtual void sub(sint64) { }
		virtual void sub(double) { }
		virtual void sub(const String&) { }
		virtual void sub(bool) { }
		virtual void sub(char) { }

		virtual void mult(uint32) { }
		virtual void mult(sint32) { }
		virtual void mult(uint64) { }
		virtual void mult(sint64) { }
		virtual void mult(double) { }
		virtual void mult(bool) { }
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

		virtual bool isEquals(uint32) const {return false;}
		virtual bool isEquals(sint32) const {return false;}
		virtual bool isEquals(uint64) const {return false;}
		virtual bool isEquals(sint64) const {return false;}
		virtual bool isEquals(double) const {return false;}
		virtual bool isEquals(bool) const {return false;}
		virtual bool isEquals(char) const {return false;}
		virtual bool isEquals(const String&) const {return false;}

		//! Method invokation, with no parameter
		virtual IDataHolder* invoke(const String& name)
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
		virtual IDataHolder* invoke(const String& name, IDataHolder* a1)
		{
			if (name == "add" or name == "append")
			{
				if (a1)
					pValue.push_back(a1);
				return nullptr;
			}
			return nullptr;
		}

		virtual IDataHolder* at(uint index)
		{
			return (index < pValue.size())
				? IDataHolder::Ptr::WeakPointer(pValue[index])
				: nullptr;
		}
		virtual const IDataHolder* at(uint index) const
		{
			return (index < pValue.size())
				? IDataHolder::Ptr::WeakPointer(pValue[index])
				: nullptr;
		}

		virtual void loopbackAssign(IDataHolder& dataholder) const {dataholder.assignList(pValue);}
		virtual void loopbackAdd(IDataHolder& dataholder) const {dataholder.addList(pValue);}
		virtual void loopbackMultiply(IDataHolder&) const {}
		virtual void loopbackSub(IDataHolder&) const {}
		virtual void loopbackDiv(IDataHolder&) const {}
		virtual bool loopbackIsEquals(IDataHolder&) const {assert(false and "not implemented");return false;};

	private:
		//! The real data element.
		IDataHolder::Vector  pValue;

	}; // class Data





} // namespace Variant
} // namespace Private
} // namespace Yuni

#endif // __YUNI_CORE_VARIANT_VARIANT_DATAHOLDER_ARRAY_H__
