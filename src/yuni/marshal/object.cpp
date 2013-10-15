
#include "object.h"
#include <cassert>
#include "../core/string/escape.h"


namespace Yuni
{
namespace Marshal
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
					value.dictionary = new Object::InternalTable(*fromValue.dictionary);
					break;
				}
			case Object::otArray:
				{
					assert(fromValue.array);
					value.array = new Object::InternalArray(*fromValue.array);
					break;
				}
			default:
				value.biggest = fromValue.biggest;
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
					delete value.dictionary;
					break;
				}
			case Object::otArray:
				{
					delete value.array;
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
					value.array->push_back(Object());
					return value.array->back();
				}
			case Object::otNil:
				{
					type = Object::otArray;
					Object::InternalArray* array = new Object::InternalArray(1);
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
						if (value.dictionary->count(key) == 0)
							return (*value.dictionary)[key];
						++index;
						assert(index < (uint) -1 and "infinite loop");
					}
					while (true);
				}
			default:
				{
					// mutate
					Object::InternalArray* array = new Object::InternalArray();
					array->push_back(Object(type, value));
					array->push_back(Object());
					type = Object::otArray;
					value.array = array;
					return array->back();
				}
		}
		assert(false and "something is missing within the switch/case");
	}




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
						(*pValue.array) = *rhs.pValue.array;
						break;
					}
				case otDictionary:
					{
						(*pValue.dictionary) = *rhs.pValue.dictionary;
						break;
					}
				default:
					// raw copy
					pValue = rhs.pValue;
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


	uint Object::size() const
	{
		switch (pType)
		{
			case otArray:
				return (uint )pValue.array->size();
			case otDictionary:
				return (uint) pValue.dictionary->size();
			case otNil:
				return 0;
			default:
				return 1;
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
					return (*pValue.dictionary)[key];
				}
			case otArray:
				{
					InternalArray& array = *pValue.array;
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
						delete pValue.array;

						pValue.dictionary = dict;
						pType = otDictionary;
						return (*dict)[key];
					}
				}
			case otNil:
				{
					pType = otDictionary;
					pValue.dictionary = new InternalTable();
					return (*pValue.dictionary)[key];
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


	template<bool PrettyT, class StreamT>
	inline void Object::valueToJSON(StreamT& out, uint depth) const
	{
		switch (pType)
		{
			case otDictionary:
				{
					InternalTable& table = *pValue.dictionary;
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
						if ((uint) child.pType < (uint) Object::firstComplexDatatype)
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
						if ((uint) child.pType < (uint) Object::firstComplexDatatype)
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
					InternalArray& array = *pValue.array;

					for (uint index = 0; index != array.size(); ++index)
					{
						AppendIndentSpaces(out, depth);
						const Object& child = array[index];
						if ((uint) child.pType < (uint) Object::firstComplexDatatype)
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




} // namespace Marshal
} // namespace Yuni

