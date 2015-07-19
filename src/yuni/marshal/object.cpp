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
#include "object.h"
#include <cassert>
#include "../core/string/escape.h"
#include "../core/dictionary.h"
#include <vector>



namespace Yuni
{
namespace Marshal
{

	enum
	{
		//! Index within enum Type of the first complex datatype (for internal uses)
		firstComplexDatatype = Object::otArray,
	};

	//! Array of object
	typedef std::vector<Object>  InternalArray;
	//! Object mapping
	typedef Dictionary<String, Object>::Hash  InternalTable;


	namespace // anonymous
	{

		template<class UnionT>
		static inline void ObjectCopy(Object::Type& type, UnionT& value, Object::Type fromType, const UnionT& fromValue)
		{
			switch ((type = fromType))
			{
				case Object::otString:
				{
					assert(fromValue.string);
					value.string = new String(*fromValue.string);
					break;
				}
				case Object::otDictionary:
				{
					assert(fromValue.dictionary);
					value.dictionary = new InternalTable(*((InternalTable*) fromValue.dictionary));
					break;
				}
				case Object::otArray:
				{
					assert(fromValue.array);
					value.array = new InternalArray(*((InternalArray*) fromValue.array));
					break;
				}
				default:
					value.blob = fromValue.blob;
			}
		}


		template<class UnionT>
		static inline void ObjectRelease(Object::Type type, UnionT& value)
		{
			switch (type)
			{
				case Object::otString:
				{
					delete value.string;
					break;
				}
				case Object::otDictionary:
				{
					delete ((InternalTable*) value.dictionary);
					break;
				}
				case Object::otArray:
				{
					delete ((InternalArray*) value.array);
					break;
				}
				default:
					break; // does nothing
			}
		}


		template<class UnionT>
		static inline Object& ObjectPushBack(Object::Type& type, UnionT& value)
		{
			switch (type)
			{
				case Object::otArray:
				{
					((InternalArray*) value.array)->push_back(Object());
					return ((InternalArray*) value.array)->back();
				}
				case Object::otNil:
				{
					type = Object::otArray;
					InternalArray* array = new InternalArray(1);
					value.array = array;
					return array->back();
				}
				case Object::otDictionary:
				{
					// not really efficient, but it would make the job whatever it takes
					String key;
					uint index = 0;
					do
					{
						key = index;
						if (((InternalTable*) value.dictionary)->count(key) == 0)
							return (*((InternalTable*)value.dictionary))[key];
						++index;
						assert(index < (uint) -1 and "infinite loop");
					}
					while (true);
					break;
				}
				default:
				{
					// mutate
					InternalArray* array = new InternalArray();
					array->push_back(Object(type, value));
					array->push_back(Object());
					type = Object::otArray;
					value.array = array;
					return array->back();
				}
			}
			assert(false and "something is missing within the switch/case");
		}

	} // anonymous namespace








	Object::~Object()
	{
		ObjectRelease(pType, pValue);
	}


	Object::Object(const Object& rhs)
	{
		ObjectCopy(pType, pValue, rhs.pType, rhs.pValue);
	}


	void Object::clear()
	{
		ObjectRelease(pType, pValue);
		pType = otNil;
	}


	void Object::assign(const Object& rhs)
	{
		// using condition / switch to reduce memory allocation
		if (pType == rhs.pType)
		{
			switch (pType)
			{
				case otString:
				{
					(*pValue.string) = *rhs.pValue.string;
					break;
				}
				case otArray:
				{
					(* ((InternalArray*) pValue.array)) = * ((InternalArray*) rhs.pValue.array);
					break;
				}
				case otDictionary:
				{
					(*((InternalTable*) pValue.dictionary)) = * ((InternalTable*) rhs.pValue.dictionary);
					break;
				}
				default:
				{
					// raw copy
					pValue = rhs.pValue;
				}
			}
		}
		else
		{
			ObjectRelease(pType, pValue);
			ObjectCopy(pType, pValue, rhs.pType, rhs.pValue);
		}
	}


	void Object::assign(bool boolean)
	{
		ObjectRelease(pType, pValue);
		pType = otBool;
		pValue.boolean = boolean;
	}


	void Object::assign(double decimal)
	{
		ObjectRelease(pType, pValue);
		pType = otDouble;
		pValue.decimal = decimal;
	}


	void Object::assign(int integer)
	{
		ObjectRelease(pType, pValue);
		pType = otInteger;
		pValue.integer = integer;
	}


	void Object::assign(sint64 integer)
	{
		ObjectRelease(pType, pValue);
		pType = otInteger;
		pValue.integer = integer;
	}


	void Object::assign(const AnyString& string)
	{
		if (pType == otString)
		{
			(*pValue.string) = string;
		}
		else
		{
			ObjectRelease(pType, pValue);
			pType = otString;
			pValue.string = new String(string);
		}
	}


	size_t Object::size() const
	{
		switch (pType)
		{
			case otArray:
				assert(pValue.array != nullptr);
				return ((InternalArray*) pValue.array)->size();
			case otDictionary:
				assert(pValue.dictionary != nullptr);
				return ((InternalTable*) pValue.dictionary)->size();
			case otNil:
				return 0u;
			default:
				return 1u;
		}
	}


	void Object::append(const Object& rhs)
	{
		ObjectPushBack(pType, pValue).assign(rhs);
	}


	Object& Object::operator [] (const String& key)
	{
		switch (pType)
		{
			case otDictionary:
			{
				return (* ((InternalTable*) pValue.dictionary))[key];
			}
			case otArray:
			{
				InternalArray& array = * ((InternalArray*) pValue.array);
				uint index = 0;
				if (key.empty() or key.to(index))
				{
					if (index >= array.size())
						array.resize(index + 1);
					return array[index];
				}
				else
				{
					// mutate into a dictionary
					InternalTable* dict = new InternalTable();
					String k;
					for (uint i = 0; i != array.size(); ++i)
						(*dict)[(k = i)].swap(array[i]);
					delete ((InternalArray*) pValue.array);

					pValue.dictionary = dict;
					pType = otDictionary;
					return (*dict)[key];
				}
			}
			case otNil:
			{
				pType = otDictionary;
				pValue.dictionary = new InternalTable();
				return (* ((InternalTable*) pValue.dictionary))[key];
			}
			default:
			{
				pType = otDictionary;
				InternalTable* dict = new InternalTable();
				(*dict)["0"] = *this;
				pValue.dictionary = dict;
				return (*dict)[key];
			}
		}
	}







	namespace // anonymous
	{

		template<class StreamT, class ValueT>
		static inline bool ObjectBuiltinTypeToJSON(StreamT& out, Object::Type type, const ValueT& value)
		{
			switch (type)
			{
				case Object::otString:
				{
					out += '"';
					AppendEscapedString(out, *value.string, '"');
					out += '"';
					return true;
				}
				case Object::otInteger:
				{
					out << value.integer;
					return true;
				}
				case Object::otBool:
				{
					out << ((value.boolean) ? '1' : '0');
					return true;
				}
				case Object::otDouble:
				{
					out << value.decimal;
					return true;
				}
				case Object::otNil:
				{
					out << "null";
					return true;
				}
				default:
					return false;
			}
			return false;
		}


		template<class StreamT>
		static inline void AppendIndentSpaces(StreamT& out, uint depth, uint tabsize = 4)
		{
			assert(tabsize <= 16);
			for (uint i = 0; i != depth; ++i)
				out.append("                ", tabsize);
		}

	} // anonymous namespace



	template<bool PrettyT, class StreamT>
	inline void Object::valueToJSON(StreamT& out, uint depth) const
	{
		switch (pType)
		{
			case otDictionary:
			{
				InternalTable& table = * ((InternalTable*) pValue.dictionary);
				if (table.empty())
				{
					out.append("{ }", 3);
					break;
				}

				if (PrettyT)
					out.append("{\n", 2);
				else
					out += '{';

				// manually handling the first item, to not be annyoyed again by commas
				if (PrettyT)
					AppendIndentSpaces(out, depth);
				InternalTable::const_iterator it = table.begin();
				out += '"';
				AppendEscapedString(out, it->first, '"');
				out += "\": ";
				{
					const Object& child = it->second;
					if ((uint) child.pType < (uint) firstComplexDatatype)
					{
						// simple type
						ObjectBuiltinTypeToJSON(out, child.pType, child.pValue);
					}
					else
					{
						// complex type, recursive call
						child.valueToJSON<PrettyT>(out, depth + 1);
					}
				}
				++it;

				// each item
				InternalTable::const_iterator end = table.end();
				for (; it != end; ++it)
				{
					if (PrettyT)
					{
						out.append(",\n", 2);
						AppendIndentSpaces(out, depth);
					}
					else
						out += ',';

					out += '"';
					AppendEscapedString(out, it->first, '"');
					out += "\": ";
					const Object& child = it->second;
					if ((uint) child.pType < (uint) firstComplexDatatype)
					{
						// simple type
						ObjectBuiltinTypeToJSON(out, child.pType, child.pValue);
					}
					else
					{
						// complex type, recursive call
						child.valueToJSON<PrettyT>(out, depth + 1);
					}
				}

				// final
				if (PrettyT)
				{
					out += '\n';
					--depth;
					AppendIndentSpaces(out, depth);
				}
				out += '}';
				break;
			}

			case otArray:
			{
				out << "[\n";
				InternalArray& array = * ((InternalArray*) pValue.array);

				for (uint index = 0; index != array.size(); ++index)
				{
					AppendIndentSpaces(out, depth);
					const Object& child = array[index];
					if ((uint) child.pType < (uint) firstComplexDatatype)
					{
						// simple type
						ObjectBuiltinTypeToJSON(out, child.pType, child.pValue);
					}
					else
					{
						// complex type, recursive call
						child.valueToJSON<PrettyT>(out, depth + 1);
					}
					out << ",\n";
				}

				--depth;
				AppendIndentSpaces(out, depth);
				out << "]\n";
				break;
			}
			default:
			{
				ObjectBuiltinTypeToJSON(out, pType, pValue);
				break;
			}
		}
	}


	void Object::toJSON(Clob& out, bool pretty) const
	{
		if (pretty)
			valueToJSON<true> (out, 1);
		else
			valueToJSON<false>(out, 1);
	}


	#ifdef YUNI_HAS_CPP_MOVE
	inline Object& Object::operator = (Object&& rhs)
	{
		ObjectRelease(pType, pValue);
		pType = rhs.pType;
		pValue = rhs.pValue;
		rhs.pType = otNil;
		return *this;
	}
	#endif



} // namespace Marshal
} // namespace Yuni
