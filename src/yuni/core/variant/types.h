#ifndef __YUNI_CORE_VARIANT_TYPES_H__
# define __YUNI_CORE_VARIANT_TYPES_H__



namespace Yuni
{

	//! Variant internal data type
	enum VariantInnerType
	{
		//! Nil / Not assigned
		variantTNil,
		//! Bool
		variantTBool,
		//! char
		variantTChar,
		//! sint32
		variantTInt32,
		//! sint64
		variantTInt64,
		//! uint32
		variantTUInt32,
		//! uint64
		variantTUInt64,
		//! string
		variantTString,
		//! struct, with members
		variantTClass,
		//! array of variants
		variantTArray
	};




} // namespace Yuni

#endif // __YUNI_CORE_VARIANT_VARIANT_H__
