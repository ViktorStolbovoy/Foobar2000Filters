#pragma once

class BinarySerializable
{
public:
	//position should point past the Element type
	BinarySerializable() : m_pastLast(nullptr)
	{
	}
	
	void InitFromBytes(uint8** data, uint8* pastLast)
	{
		m_pastLast = pastLast;
		SerializeInternal(data, false);
	}


	uint8* ToBytes(size_t* size) 
	{
		*size = GetSize();
		uint8 * res = new uint8[*size];
		uint8 * data = res;
		SerializeInternal(&data, true);
		return res;
	}

	size_t GetSize()
	{
		return SerializeInternal(nullptr, true);
	}

	void WriteToBuffer(uint8** buf)
	{
		SerializeInternal(buf, true);
	}

protected:
	virtual size_t SerializeInternal(uint8** data, bool isWriting) = 0;

	uint8* m_pastLast = nullptr;


	size_t Serialize(uint32 *val, uint8**data, bool isWriting)
	{
		if (data)
		{
			if (isWriting)
			{
				*((uint32*)*data) = *val;
			}
			else
			{
				if (*data + sizeof(uint32) > m_pastLast) throw std::logic_error("Attempt to read input data past the it end");
				*val = *((uint32*)*data);
				
			}
			(*data) += sizeof(uint32);
		}

		return sizeof(uint32);
	}

	size_t Serialize(uint8 *val, uint8**data, bool isWriting)
	{

		if (data)
		{
			if (isWriting)
			{
				**data = *val;
			}
			else
			{
				*val = **data;
			}
			(*data) ++;
		}
		return 1;
	}

	size_t Serialize(bool *val, uint8**data, bool isWriting)
	{

		if (data)
		{
			if (isWriting)
			{
				**data = (uint8)*val;
			}
			else
			{
				*val = **data != 0;
			}
			(*data)++;
		}
		return 1;
	}

	size_t Serialize(float *val, uint8**data, bool isWriting)
	{
		if (data)
		{
			if (isWriting)
			{
				*((float*)*data) = *val;
			}
			else
			{
				*val = *((float*)*data);
			}
			*data += sizeof(float);
		}
		return sizeof(float);
	}

	template <typename T>
	size_t Serialize(T **val, uint8 *len, uint8**data, bool isWriting)
	{
		size_t sz = *len * sizeof(T);
		if (data)
		{
			if (isWriting)
			{
				**data = *len;
				(*data)++;
				memcpy(*data, *val, sz);
			}
			else
			{
				*len = **data;
				(*data)++;
				*val = new T[*len];
				sz = *len * sizeof(T);
				memcpy(*val, *data, sz);

			}
			(*data) += sz;
		}
		return sz + 1;
	}

	template <class ItemType>
	size_t Serialize(std::vector<ItemType> *val, uint8**data, bool isWriting)
	{
		uint32 len = val->size();
		size_t szLen = Serialize(&len, data, isWriting);
		const size_t szItem = sizeof(ItemType);
		const size_t szData = len * szItem;
		
		if (data)
		{
			if (isWriting)
			{
				memcpy(*data, val->data(), szData);
			}
			else
			{
				val->resize(len);
				memcpy(val->data(), *data, szData);
			}
			(*data) += szData;
		}
		return szLen + szData;

	}
};
