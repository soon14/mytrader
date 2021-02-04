#pragma once
#ifndef _H_MDBASE_H_
#define _H_MDBASE_H_

#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

#ifndef ssize_t
typedef intmax_t ssize_t;
#endif//

#ifndef size_t
typedef uintmax_t size_t;
#endif//

#ifndef MDB_API_EXPORT

#if defined(LIB_MDB_API) && defined(WIN32)
#ifdef LIB_MDB_API_EXPORT
#define MDB_API_EXPORT __declspec(dllexport)
#else
#define MDB_API_EXPORT __declspec(dllimport)
#endif
#else
#define MDB_API_EXPORT 
#endif

#endif//MDB_API_EXPORT

#if defined(__cplusplus)
extern "C" {
#endif//

#pragma pack(push, 1)

typedef enum MDB_STATUS
{
	MDB_STATUS_OK = 0,
	MDB_STATUS_ERROR,
	MDB_STATUS_DB_NOT_OPEN,
	MDB_STATUS_TABLE_NOT_OPEN,
	MDB_STATUS_INVALID_PARAM,
	MDB_STATUS_BAD_ALLOC,
	MDB_STATUS_EXCEPTION,
	MDB_STATUS_NOT_EXIST,
	MDB_STATUS_MEM_OVERFLOW,
	MDB_STATUS_IN_USE,
}MDB_STATUS;

#define MDB_LOG_LEVEL_NONE	0
#define MDB_LOG_LEVEL_ERROR 1
#define MDB_LOG_LEVEL_WARN	2
#define MDB_LOG_LEVEL_INFO	3
#define MDB_LOG_LEVEL_DEBUG 4
typedef void (*MDB_LOG_CB)(int level, const char* logstr);

typedef enum MDB_DB_TYPE
{
	MDB_DB_TYPE_UNKNOWN = 0,
	MDB_DB_TYPE_CACHE, //进程内DB，程序奔溃数据丢失
	MDB_DB_TYPE_SHARE, //进程间共享内存DB，程序崩溃DB数据不丢失
	MDB_DB_TYPE_PERSISTENCE, //持久化内存DB，支持进程间共享、跨机器同步以及掉电不丢失
}MDB_DB_TYPE;

typedef struct MDB_DB
{
	MDB_DB_TYPE type;
}MDB_DB;

typedef struct MDB_DB *HMDB;

//#define MDB_TABLE_TYPE_FLAG_STD 0x80 //标准表，属性存放字段属性，数据结构由属性字段决定，即MDB_FIELD作为列表存储在attr里，数据data按着字段存储
//#define MDB_TABLE_TYPE_FLAG_VAR 0x40 //可变表，存不下数据时自动调整表大小以存下所有数据
typedef enum MDB_TABLE_TYPE
{
	MDB_TABLE_TYPE_UNKNOWN = 0,
	//原始数据，可直接访问DB内存，即实现零拷贝读写数据，固定大小
	//MDB_TABLE_TYPE_DATA,
	//原始数据，可直接访问DB内存，即实现零拷贝读写数据，可变大小
	//MDB_TABLE_TYPE_VAR_DATA,
	//原始数据表，可直接访问DB内存，即实现零拷贝读写数据，最大数据量为max_elem_num
	MDB_TABLE_TYPE_RAW, 
	//标准数据表，可直接访问DB内存，即实现零拷贝读写数据，最大数据量为max_elem_num
	MDB_TABLE_TYPE_STD, 
	//普通可变数据表，不能直接访问DB内存，需要从DB内存拷贝出来，可变是指最大数据量不受限制
	MDB_TABLE_TYPE_VAR, 
	//标准可变数据表，不能直接访问DB内存，需要从DB内存拷贝出来，可变是指最大数据量不受限制
	MDB_TABLE_TYPE_STD_VAR,
	//表视图，根据需要对表进行排序
	MDB_TABLE_TYPE_RAW_VIEW,
	//表视图，根据需要对表进行排序
	MDB_TABLE_TYPE_STD_VIEW,
	//字典表，key-value
	MDB_TABLE_TYPE_DICT,
}MDB_TABLE_TYPE;

typedef struct MDB_TABLE
{
	MDB_TABLE_TYPE type;
}MDB_TABLE;

typedef struct MDB_TABLE *HMTABLE;

typedef enum MDB_FIELD_TYPE
{
	MDB_FIELD_TYPE_UNKNOWN = 0,
	MDB_FIELD_TYPE_CHAR,
	MDB_FIELD_TYPE_BYTE,
	MDB_FIELD_TYPE_INT16,
	MDB_FIELD_TYPE_UINT16,
	MDB_FIELD_TYPE_INT32,
	MDB_FIELD_TYPE_UINT32,
	MDB_FIELD_TYPE_INT64,
	MDB_FIELD_TYPE_UINT64,
	MDB_FIELD_TYPE_FLOAT,
	MDB_FIELD_TYPE_DOUBLE,
	MDB_FIELD_TYPE_STRING,
	MDB_FIELD_TYPE_BLOB,
}MDB_FIELD_TYPE;
typedef uint64_t MDB_FIELD_ID;
#define MAX_FIELD_NAME sizeof(uint64_t)
typedef char MDB_FIELD_NAME_ID[MAX_FIELD_NAME];
typedef struct MDB_FIELD
{
	union
	{
		MDB_FIELD_ID id; //唯一标识
		MDB_FIELD_NAME_ID name; //唯一名字标识，最大长度为MAX_FIELD_NAME
	};
	uint64_t type : 5; //FIELD类型
	uint64_t size : 24; //FIELD内存大小
	uint64_t offset : 32; //FIELD在TABLE中的偏移
	uint64_t reserved : 3; //FIELD保留位,具体应用使用
}MDB_FIELD;
//#define MDBFieldInit(f,id,t,sz) f.id=id,f.type=t,f.size=sz
//#define MDBFieldInitFromName(f,n,t,sz) strncpy(f.name,n,MAX_FIELD_NAME),f.type=t,f.size=sz
//#define MDBFieldDeclare(f,id,t,sz) MDB_FIELD f = {0};MDBFieldInit(f,id,t,sz)
//#define MDBFieldDeclareFromName(f,n,t,sz) MDB_FIELD f = {0};MDBFieldInitFromName(f,n,t,sz)
static inline bool MDBFieldTypeIsNumber(size_t type)
{
	switch (type)
	{
	case MDB_FIELD_TYPE_CHAR:
	case MDB_FIELD_TYPE_BYTE:
	case MDB_FIELD_TYPE_INT16:
	case MDB_FIELD_TYPE_UINT16:
	case MDB_FIELD_TYPE_INT32:
	case MDB_FIELD_TYPE_UINT32:
	case MDB_FIELD_TYPE_INT64:
	case MDB_FIELD_TYPE_UINT64:
	case MDB_FIELD_TYPE_FLOAT:
	case MDB_FIELD_TYPE_DOUBLE:
		return true;
		break;
	default:
		break;
	}
	return false;
}
static inline size_t MDBFieldNormalize(MDB_FIELD* fields, size_t field_num)
{
	if (!fields || !field_num) {
		return 0;
	}
	size_t offset = 0;
	for (size_t i = 0; i < field_num; i++)
	{
		fields[i].offset = offset;
		switch (fields[i].type)
		{
		case MDB_FIELD_TYPE_CHAR:
		case MDB_FIELD_TYPE_BYTE:
			fields[i].size = 1;
			break;
		case MDB_FIELD_TYPE_INT16:
		case MDB_FIELD_TYPE_UINT16:
			fields[i].size = 2;
			break;
		case MDB_FIELD_TYPE_INT32:
		case MDB_FIELD_TYPE_UINT32:
			fields[i].size = 4;
			break;
		case MDB_FIELD_TYPE_INT64:
		case MDB_FIELD_TYPE_UINT64:
			fields[i].size = 8;
			break;
		case MDB_FIELD_TYPE_FLOAT:
			fields[i].size = 4;
			break;
		case MDB_FIELD_TYPE_DOUBLE:
			fields[i].size = 8;
			break;
		default:
			//fields[i].size;
			break;
		}
		offset += fields[i].size;
	}
	return offset;
}
static inline size_t MDBFieldTypeSize(size_t type, size_t size)
{
	switch (type)
	{
	case MDB_FIELD_TYPE_CHAR:
	case MDB_FIELD_TYPE_BYTE:
		return 1;
		break;
	case MDB_FIELD_TYPE_INT16:
	case MDB_FIELD_TYPE_UINT16:
		return 2;
		break;
	case MDB_FIELD_TYPE_INT32:
	case MDB_FIELD_TYPE_UINT32:
		return 4;
		break;
	case MDB_FIELD_TYPE_INT64:
	case MDB_FIELD_TYPE_UINT64:
		return 8;
		break;
	case MDB_FIELD_TYPE_FLOAT:
		return 4;
		break;
	case MDB_FIELD_TYPE_DOUBLE:
		return 8;
		break;
	case MDB_FIELD_TYPE_STRING:
	case MDB_FIELD_TYPE_BLOB:
		return size;
		break;
	default:
		break;
	}
	return 0;
}
static inline size_t MDBFieldCalcSize(const MDB_FIELD* fields, size_t field_num)
{
	if (!fields || !field_num) {
		return 0;
	}
	size_t offset = 0;
	for (size_t i = 0; i < field_num; i++)
	{
		offset += fields[i].size;
	}
	return offset;
}

typedef enum MDB_NOTIFY
{
	NOTIFY_UNKNOWN = 0,
	NOTIFY_INIT, //初始化数据
	NOTIFY_META, //更新元数据
	NOTIFY_ATTR, //更新属性
	NOTIFY_INSERT, //更新数据
	NOTIFY_UPDATE, //更新数据
	NOTIFY_APPEND, //更新数据
	NOTIFY_REMOVE, //删除数据
	NOTIFY_UPDATE_FIELD, //更新数据
}MDB_NOTIFY;
struct MDB_NOTIFY_DATA
{
	MDB_NOTIFY notify;
};
struct MDB_NOTIFY_INIT
{
	MDB_NOTIFY notify; 
	MDB_TABLE_TYPE type;
	void* attr;
	size_t attr_sz;
	void* data;
	size_t elem_sz;
	size_t elem_num;
	size_t max_elem_num;
	size_t flags;
};
#define MDB_NOTIFY_META_DELETE_FLAG 0x01
#define MDB_NOTIFY_META_ENABLE_FLAG 0x02
#define MDB_NOTIFY_META_DISABLE_FLAG 0x04
struct MDB_NOTIFY_META
{
	MDB_NOTIFY notify;
	size_t flags;
};
struct MDB_NOTIFY_ATTR
{
	MDB_NOTIFY notify;
	const void* attr;
	size_t attr_sz;
};
struct MDB_NOTIFY_INSERT
{
	MDB_NOTIFY notify;
	size_t pos;
	const void *data;
	size_t elem_num;
};
struct MDB_NOTIFY_UPDATE
{
	MDB_NOTIFY notify;
	size_t pos;
	const void *data;
	size_t elem_num;
};
struct MDB_NOTIFY_APPEND
{
	MDB_NOTIFY notify;
	size_t pos;
	const void *data;
	size_t elem_num;
};
struct MDB_NOTIFY_REMOVE
{
	MDB_NOTIFY notify;
	size_t pos;
	size_t elem_num;
};
struct MDB_NOTIFY_UPDATE_FIELD
{
	MDB_NOTIFY notify;
	MDB_FIELD* field;
	size_t field_num;
	size_t pos;
	const void *data;
	size_t elem_num;
};
typedef void (*MDB_NOTIFY_CB)(HMDB hdb, HMTABLE htb, MDB_NOTIFY_DATA* notify);

struct MDB_DB_INF
{
	int xmlflag;
	const char* xml;
	MDB_LOG_CB log;
	MDB_NOTIFY_CB cb;
};

MDB_API_EXPORT MDB_STATUS MDBDeleteDB(const char* name, int type);

MDB_API_EXPORT const char* MDBGetName(HMDB hdb);

MDB_API_EXPORT HMDB MDBOpenDB(MDB_DB_INF* inf);
MDB_API_EXPORT HMDB MDBFindDB(const char* name, int type);
MDB_API_EXPORT MDB_STATUS MDBCloseDB(HMDB hdb);

typedef void (*MDB_ENUM_TABLE_CB)(const char* key, void* data);
MDB_API_EXPORT MDB_STATUS MDBEnumTable(HMDB hdb, const char* key, MDB_ENUM_TABLE_CB cb, void* data);

MDB_API_EXPORT void MDBResetAllTableRef(HMDB hdb);
MDB_API_EXPORT void MDBDeleteTable(HMDB hdb, const char* key);

MDB_API_EXPORT const char* MDBTableGetName(HMDB hdb, HMTABLE htb);
MDB_API_EXPORT void MDBCloseTable(HMDB hdb, HMTABLE htb);

#define MDB_OPEN_FLAG_NORMAL 0
#define MDB_OPEN_FLAG_INCLUDE_DELETE 0x01 //打开包含已经标记为删除表
#define MDB_OPEN_FLAG_RESET_REF 0x02 //重置引用
#define MDB_OPEN_FLAG_VIEW 0x04 //打开VIEW
MDB_API_EXPORT HMTABLE MDBOpenTable(HMDB hdb, const char* key, size_t flags);
#define MDB_OPEN_AND_INIT_DATA_FLAG_CYCLE 0X01 //循环数据表,数据满时覆盖最前面数据
#define MDB_OPEN_AND_INIT_DATA_FLAG_UNSTABLE 0X02 //不稳定表，即运行时可能会被删除的表
#define MDB_OPEN_AND_INIT_DATA_FLAG_TEMP 0X04 //临时表，程序退出后即消失的表
MDB_API_EXPORT HMTABLE MDBOpenAndInitTable(HMDB hdb, const char* key, void* attr, size_t attr_sz, void* data, size_t elem_sz, size_t elem_num, size_t max_elem_num, size_t flags);
MDB_API_EXPORT HMTABLE MDBOpenAndInitVarTable(HMDB hdb, const char* key, void* attr, size_t attr_sz, void* data, size_t elem_sz, size_t elem_num, size_t max_elem_num, size_t flags);
MDB_API_EXPORT HMTABLE MDBOpenAndInitStdTable(HMDB hdb, const char* key, MDB_FIELD* fields, size_t field_num, void* data, size_t elem_num, size_t max_elem_num, size_t flags);
MDB_API_EXPORT HMTABLE MDBOpenAndInitStdVarTable(HMDB hdb, const char* key, MDB_FIELD* fields, size_t field_num, void* data, size_t elem_num, size_t max_elem_num, size_t flags);

typedef bool (*MDB_LESS_CB)(const void*, const void*);
MDB_API_EXPORT HMTABLE MDBOpenAndInitView(const char* key, HMDB hdb, HMTABLE htb, MDB_LESS_CB less);
MDB_API_EXPORT HMTABLE MDBOpenAndInitStdView(const char* key, HMDB hdb, HMTABLE htb, MDB_FIELD* fields, size_t field_num);
MDB_API_EXPORT HMDB MDBViewGetDB(HMTABLE hv);
MDB_API_EXPORT HMTABLE MDBViewGetTable(HMTABLE hv);
MDB_API_EXPORT uint32_t MDBViewGetValuePos(HMTABLE hv, size_t pos);
MDB_API_EXPORT MDB_STATUS MDBViewGetValuePosEx(HMTABLE hv, size_t pos, uint32_t* pos_val, size_t* pos_num);
MDB_API_EXPORT MDB_STATUS MDBViewLowerBound(HMTABLE hv, const void* keydata, size_t* pos);
MDB_API_EXPORT MDB_STATUS MDBViewUpperBound(HMTABLE hv, const void* keydata, size_t* pos);
MDB_API_EXPORT MDB_STATUS MDBViewEqualRange(HMTABLE hv, const void* keydata, size_t* from, size_t* to);
MDB_API_EXPORT MDB_STATUS MDBViewSearch(HMTABLE hv, const void* keydata, size_t* pos);

MDB_API_EXPORT size_t MDBTableIsRef(HMDB hdb, HMTABLE htb);
MDB_API_EXPORT bool MDBTableIsDelete(HMDB hdb, HMTABLE htb);
MDB_API_EXPORT bool MDBTableIsDisable(HMDB hdb, HMTABLE htb);
MDB_API_EXPORT MDB_STATUS MDBTableEnable(HMDB hdb, HMTABLE htb);
MDB_API_EXPORT MDB_STATUS MDBTableDisable(HMDB hdb, HMTABLE htb);

MDB_API_EXPORT size_t MDBTableTryLockRead(HMDB hdb, HMTABLE htb, size_t time); //尝试锁定一段时间，单位毫秒
MDB_API_EXPORT size_t MDBTableTryLockWrite(HMDB hdb, HMTABLE htb, size_t time); //尝试锁定一段时间，单位毫秒
MDB_API_EXPORT size_t MDBTableLockRead(HMDB hdb, HMTABLE htb, size_t wait, size_t time); //锁定一段时间，单位毫秒,超过等待时间锁定失败
MDB_API_EXPORT size_t MDBTableLockWrite(HMDB hdb, HMTABLE htb, size_t wait, size_t time); //锁定一段时间，单位毫秒,超过等待时间锁定失败
MDB_API_EXPORT void MDBTableUnLockRead(HMDB hdb, HMTABLE htb); //释放锁
MDB_API_EXPORT void MDBTableUnLockWrite(HMDB hdb, HMTABLE htb); //释放锁

MDB_API_EXPORT void* MDBTableGetPtr(HMDB hdb, HMTABLE htb, size_t* sz);
MDB_API_EXPORT void* MDBTableGetAttr(HMDB hdb, HMTABLE htb, size_t* attr_sz);
MDB_API_EXPORT MDB_STATUS MDBTableSetAttr(HMDB hdb, HMTABLE htb, const void* attr, size_t attr_sz);
MDB_API_EXPORT MDB_STATUS MDBTableCopyAttr(HMDB hdb, HMTABLE htb, void* attr, size_t* attr_sz);
MDB_API_EXPORT size_t MDBTableGetValueMaxCount(HMDB hdb, HMTABLE htb);
MDB_API_EXPORT size_t MDBTableGetValueCount(HMDB hdb, HMTABLE htb, size_t* elem_sz);
//MDB_API_EXPORT MDB_STATUS MDBTableSetValueCount(HMDB hdb, HMTABLE htb, size_t elem_num);
MDB_API_EXPORT void* MDBTableGetValue(HMDB hdb, HMTABLE htb, size_t pos);
MDB_API_EXPORT MDB_STATUS MDBTableCopyValue(HMDB hdb, HMTABLE htb, size_t pos, void* data, size_t* elem_num);
MDB_API_EXPORT MDB_STATUS MDBTableSetValue(HMDB hdb, HMTABLE htb, size_t pos, const void* data, size_t elem_num);
MDB_API_EXPORT MDB_STATUS MDBTableAppendValue(HMDB hdb, HMTABLE htb, const void* data, size_t elem_num);
MDB_API_EXPORT MDB_STATUS MDBTableInsertValue(HMDB hdb, HMTABLE htb, size_t pos, const void* data, size_t elem_num);
MDB_API_EXPORT MDB_STATUS MDBTableRemoveValue(HMDB hdb, HMTABLE htb, size_t pos, size_t elem_num);
MDB_API_EXPORT MDB_STATUS MDBTablePushFrontValue(HMDB hdb, HMTABLE htb, const void* data, size_t num);
MDB_API_EXPORT MDB_STATUS MDBTablePushBackValue(HMDB hdb, HMTABLE htb, const void* data, size_t num);
MDB_API_EXPORT MDB_STATUS MDBTablePopFrontValue(HMDB hdb, HMTABLE htb, void* data, size_t* num);
MDB_API_EXPORT MDB_STATUS MDBTablePopBackValue(HMDB hdb, HMTABLE htb, void* data, size_t* num);

typedef void (*MDB_ENUM_FIELD_CB)(const MDB_FIELD* field, void* data);
MDB_API_EXPORT MDB_STATUS MDBTableEnumField(HMDB hdb, HMTABLE htb, MDB_ENUM_FIELD_CB cb, void* data);
MDB_API_EXPORT MDB_STATUS MDBTableNormalizeField(HMDB hdb, HMTABLE htb, MDB_FIELD* field, size_t field_num);

//读取指定字段表值，value结构体必须和表字段一致
MDB_API_EXPORT MDB_STATUS MDBTableGetFieldValue(HMDB hdb, HMTABLE htb, MDB_FIELD* field, size_t field_num, size_t pos, size_t num, void* value);
MDB_API_EXPORT MDB_STATUS MDBTableSetFieldValue(HMDB hdb, HMTABLE htb, MDB_FIELD* field, size_t field_num, size_t pos, size_t num, const void* value);

MDB_API_EXPORT MDB_STATUS MDBFieldGetValueAs(const void* data, MDB_FIELD* field, size_t field_num, void* value, size_t size);
MDB_API_EXPORT MDB_STATUS MDBFieldSetValueAs(void* data, MDB_FIELD* field, size_t field_num, const void* value, size_t size);
// MDB_API_EXPORT MDB_STATUS MDBFieldGetValueAsInt(const void* data, MDB_FIELD* field, ssize_t* value);
// MDB_API_EXPORT MDB_STATUS MDBFieldSetValueAsInt(void* data, MDB_FIELD* field, const ssize_t value);
// MDB_API_EXPORT MDB_STATUS MDBFieldGetValueAsDouble(const void* data, MDB_FIELD* field, double* value);
// MDB_API_EXPORT MDB_STATUS MDBFieldSetValueAsDouble(void* data, MDB_FIELD* field, const double value);

//可以自定义结构体和field一致时直接读写表
MDB_API_EXPORT MDB_STATUS MDBTableGetFieldValueAs(HMDB hdb, HMTABLE htb, MDB_FIELD* field, size_t field_num, size_t pos, size_t num, void* value, size_t size);
MDB_API_EXPORT MDB_STATUS MDBTableSetFieldValueAs(HMDB hdb, HMTABLE htb, MDB_FIELD* field, size_t field_num, size_t pos, size_t num, const void* value, size_t size);
//可以自定义结构体部分字段直接读写表
MDB_API_EXPORT MDB_STATUS MDBTableGetFieldValueAsField(HMDB hdb, HMTABLE htb, MDB_FIELD* field, MDB_FIELD* as_field, size_t field_num, size_t pos, size_t num, void* value, size_t size);
MDB_API_EXPORT MDB_STATUS MDBTableSetFieldValueAsField(HMDB hdb, HMTABLE htb, MDB_FIELD* field, MDB_FIELD* as_field, size_t field_num, size_t pos, size_t num, const void* value, size_t size);
//可以自定义格式读写表部分字段
MDB_API_EXPORT MDB_STATUS MDBTableGetFieldValueAsStr(HMDB hdb, HMTABLE htb, MDB_FIELD* field, size_t field_num, size_t pos, size_t num, char*** value, size_t** size, const char** format);
MDB_API_EXPORT MDB_STATUS MDBTableSetFieldValueAsStr(HMDB hdb, HMTABLE htb, MDB_FIELD* field, size_t field_num, size_t pos, size_t num, const char*** value, const size_t** size);
MDB_API_EXPORT MDB_STATUS MDBTableGetFieldValueAsInt(HMDB hdb, HMTABLE htb, MDB_FIELD* field, size_t field_num, size_t pos, size_t num, ssize_t* value);
MDB_API_EXPORT MDB_STATUS MDBTableSetFieldValueAsInt(HMDB hdb, HMTABLE htb, MDB_FIELD* field, size_t field_num, size_t pos, size_t num, const ssize_t* value);
MDB_API_EXPORT MDB_STATUS MDBTableGetFieldValueAsDouble(HMDB hdb, HMTABLE htb, MDB_FIELD* field, size_t field_num, size_t pos, size_t num, double* value);
MDB_API_EXPORT MDB_STATUS MDBTableSetFieldValueAsDouble(HMDB hdb, HMTABLE htb, MDB_FIELD* field, size_t field_num, size_t pos, size_t num, const double* value);

#pragma pack(pop)

#if defined(__cplusplus)
}

//#ifndef LIB_MDB_API_EXPORT

namespace mdb {

	struct FIELDLess
	{
		bool operator() (const MDB_FIELD& x, const MDB_FIELD& y) const {
			return x.id < y.id;
		}
	};

	class Field : public MDB_FIELD
	{
	public:
		Field(uint64_t id, MDB_FIELD_TYPE type = MDB_FIELD_TYPE_UNKNOWN, uint32_t size = 0, uint32_t offset = 0, uint32_t reserved = 0)
			//:id(id), type(type), size(MDBFieldTypeSize(type, size)), offset(offset), reserved(reserved)
		{
			this->id = id;
			this->type = type;
			this->size = MDBFieldTypeSize(type, size);
			this->offset = offset;
			this->reserved = reserved;
		}
		Field(const char* name, MDB_FIELD_TYPE type = MDB_FIELD_TYPE_UNKNOWN, uint32_t size = 0, uint32_t offset = 0, uint32_t reserved = 0)
		{
			strncpy(this->name, name, MAX_FIELD_NAME);
			this->type = type;
			this->size = MDBFieldTypeSize(type, size);
			this->offset = offset;
			this->reserved = reserved;
		}

		inline operator MDB_FIELD*() { return this; }
		inline operator const MDB_FIELD*() const { return this; }
		inline operator const uint64_t() const { return id; }
		inline operator const char*() const { return name; }

		inline bool operator<(const Field &o) const
		{
			return id < o.id;
		}
	};

	enum class TableGuardType
	{
		READ,
		WRITE, 
	};

	template<class TTable, TableGuardType Type>
	class TableGuard
	{
	private:
		TTable& tb_;
		size_t lock_ = 0;
	public:
		TableGuard(TTable& tb, size_t time = 100) :tb_(tb) {
			if(time) {
				Lock(time);
			}
		}
		~TableGuard() {
			UnLock();
		}

		bool TryLock(size_t time) {
			if (Type==TableGuardType::WRITE) {
				lock_ = tb_.TryLockWrite(0, time);
			}
			else {
				lock_ = tb_.TryLockRead(0, time);
			}
			return lock_ != 0;
		}
		bool TryLock_for(size_t wait, size_t time) {
			if (Type==TableGuardType::WRITE) {
				lock_ = tb_.TryLockWrite(wait, time);
			}
			else {
				lock_ = tb_.TryLockRead(wait, time);
			}
			return lock_ != 0;
		}

		void Lock(size_t time) {
			if (Type==TableGuardType::WRITE) {
				lock_ = tb_.LockWrite(0, time);
			}
			else {
				lock_ = tb_.LockRead(0, time);
			}
		}
		void Lock_for(size_t wait, size_t time) {
			if (Type==TableGuardType::WRITE) {
				lock_ = tb_.LockWrite(wait, time);
			}
			else {
				lock_ = tb_.LockRead(wait, time);
			}
		}

		void UnLock()
		{
			if (lock_) {
				if (Type==TableGuardType::WRITE) {
					tb_.UnLockWrite();
				}
				else {
					tb_.UnLockRead();
				}
				lock_ = 0;
			}
		}
	};

	class Database
	{
	protected:
		HMDB hdb_ = nullptr;
	public:
		Database(HMDB hdb = nullptr):hdb_(hdb) {}
		~Database() {
			Close();
		}

		inline static MDB_STATUS Delete(const char* name, int type) { return MDBDeleteDB(name, type); }

		inline void Close() { 
			if (hdb_) {
				MDBCloseDB(hdb_);
				hdb_ = nullptr;
			}
		}
		inline HMDB Open(MDB_DB_INF* inf) { hdb_ = MDBOpenDB(inf); return hdb_; }
		inline bool IsOpen() { return hdb_ != nullptr; }
		inline operator HMDB() const { return hdb_; }
		inline HMDB handle() { return hdb_; }

		inline const char* name() { return MDBGetName(hdb_); }

		inline MDB_STATUS EnumTable(const char* key, MDB_ENUM_TABLE_CB cb, void* data) { MDBEnumTable(hdb_, key, cb, data); }

		inline void ResetAllTableRef() { MDBResetAllTableRef(hdb_); }
		inline void DeleteTable(const char* key) { MDBDeleteTable(hdb_, key); }
	};

	class Table
	{
	protected:
		Database* db_;
		HMTABLE htb_ = nullptr;
	public:
		Table(Database* db, HMTABLE htb = nullptr):db_(db),htb_(htb) { 
			
		}
		~Table() {
			Close();
			db_ = nullptr;
		}

		inline void Close() { 
			if (htb_) {
				MDBCloseTable(*db_, htb_);
				htb_ = nullptr;
			}
		}
		inline HMTABLE Open(const char* key, size_t flags = MDB_OPEN_FLAG_NORMAL) { 
			htb_ = MDBOpenTable(*db_, key, flags); 
			return htb_; 
		}
		inline HMTABLE OpenAndInit(const char* key, void* attr, size_t attr_sz, void* data, size_t elem_sz, size_t elem_num, size_t max_elem_num, size_t flags = 0) {
			htb_ = MDBOpenAndInitTable(*db_, key, attr, attr_sz, data, elem_sz, elem_num, max_elem_num, flags);
			return htb_;
		}
		inline HMTABLE OpenAndInitVar(const char* key, void* attr, size_t attr_sz, void* data, size_t elem_sz, size_t elem_num, size_t max_elem_num, size_t flags = 0) {
			htb_ = MDBOpenAndInitVarTable(*db_, key, attr, attr_sz, data, elem_sz, elem_num, max_elem_num, flags);
			return htb_;
		}
		inline HMTABLE OpenAndInitStd(const char* key, MDB_FIELD* fields, size_t field_num, void* data, size_t elem_num, size_t max_elem_num, size_t flags = 0) {
			htb_ = MDBOpenAndInitStdTable(*db_, key, fields, field_num, data, elem_num, max_elem_num, flags);
			return htb_;
		}
		inline HMTABLE OpenAndInitStdVar(const char* key, MDB_FIELD* fields, size_t field_num, void* data, size_t elem_num, size_t max_elem_num, size_t flags = 0) {
			htb_ = MDBOpenAndInitStdVarTable(*db_, key, fields, field_num, data, elem_num, max_elem_num, flags);
			return htb_;
		}

		inline bool IsOpen() { return htb_ != nullptr; }
		inline operator HMTABLE() const { return htb_; }
		inline HMTABLE handle() { return htb_; }

		inline Database* db() { return db_; }
		
		inline const char* name() { return MDBTableGetName(*db_, htb_); }

		inline size_t IsRef() { return MDBTableIsRef(*db_, htb_); }
		inline bool IsDelete() { return MDBTableIsDelete(*db_, htb_); }
		inline bool IsDisable() { return MDBTableIsDisable(*db_, htb_); }
		inline MDB_STATUS Enable() { return MDBTableEnable(*db_, htb_); }
		inline MDB_STATUS Disable() { return MDBTableDisable(*db_, htb_); }

		inline size_t TryLockRead(size_t time) { return MDBTableTryLockRead(*db_, htb_, time); }
		inline size_t TryLockWrite(size_t time) { return MDBTableTryLockWrite(*db_, htb_, time); }
		inline size_t LockRead(size_t wait, size_t time) { return MDBTableLockRead(*db_, htb_, wait, time); }
		inline size_t LockWrite(size_t wait, size_t time) { return MDBTableLockWrite(*db_, htb_, wait, time); }
		inline void UnLockRead() { MDBTableUnLockRead(*db_, htb_); }
		inline void UnLockWrite() { MDBTableUnLockWrite(*db_, htb_); }

		inline void* GetPtr(size_t* sz) { return MDBTableGetPtr(*db_, htb_, sz); }
		inline void* GetAttr(size_t* attr_sz = nullptr) { return MDBTableGetAttr(*db_, htb_, attr_sz); }
		inline MDB_STATUS SetAttr(const void* attr, size_t attr_sz) { return MDBTableSetAttr(*db_, htb_, attr, attr_sz); }
		inline MDB_STATUS CopyAttr(void* attr, size_t* attr_sz) { return MDBTableCopyAttr(*db_, htb_, attr, attr_sz); }
		inline size_t GetValueMaxCount() { return MDBTableGetValueMaxCount(*db_, htb_); }
		inline size_t GetValueCount(size_t* elem_sz = nullptr) { return MDBTableGetValueCount(*db_, htb_, elem_sz); }
		//inline size_t SetValueCount(size_t elem_num) { return MDBTableSetValueCount(*db_, htb_, elem_num); }
		inline void* GetValue(size_t pos) { return MDBTableGetValue(*db_, htb_, pos); }
		inline MDB_STATUS CopyValue(size_t pos, void* data, size_t* elem_num) { return MDBTableCopyValue(*db_, htb_, pos, data, elem_num); }
		inline MDB_STATUS SetValue(size_t pos, const void* data, size_t elem_num) { return MDBTableSetValue(*db_, htb_, pos, data, elem_num); }
		inline MDB_STATUS AppendValue(const void* data, size_t elem_num) { return MDBTableAppendValue(*db_, htb_, data, elem_num); }
		inline MDB_STATUS InsertValue(size_t pos, const void* data, size_t elem_num) { return MDBTableInsertValue(*db_, htb_, pos, data, elem_num); }
		inline MDB_STATUS RemoveValue(size_t pos, size_t elem_num) { return MDBTableRemoveValue(*db_, htb_, pos, elem_num); }
		inline MDB_STATUS PushFrontValue(const void* data, size_t num) { return MDBTablePushFrontValue(*db_, htb_, data, num); }
		inline MDB_STATUS PushBackValue(const void* data, size_t num) { return MDBTablePushBackValue(*db_, htb_, data, num); }
		inline MDB_STATUS PopFrontValue(void* data, size_t* num) { return MDBTablePopFrontValue(*db_, htb_, data, num); }
		inline MDB_STATUS PopBackValue(void* data, size_t* num) { return MDBTablePopBackValue(*db_, htb_, data, num); }

		inline MDB_STATUS EnumField(MDB_ENUM_FIELD_CB cb, void* data) { return MDBTableEnumField(*db_, htb_, cb, data); }
		inline MDB_STATUS NormalizeField(MDB_FIELD* field, size_t field_num) { return MDBTableNormalizeField(*db_, htb_, field, field_num); }

		inline MDB_STATUS GetFieldValue(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, void* value) { return MDBTableGetFieldValue(*db_, htb_, field, field_num, pos, num, value); }
		inline MDB_STATUS SetFieldValue(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, const void* value) { return MDBTableSetFieldValue(*db_, htb_, field, field_num, pos, num, value); }
		inline MDB_STATUS GetFieldValueAs(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, void* value, size_t size) { return MDBTableGetFieldValueAs(*db_, htb_, field, field_num, pos, num, value, size); }
		inline MDB_STATUS SetFieldValueAs(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, const void* value, size_t size) { return MDBTableSetFieldValueAs(*db_, htb_, field, field_num, pos, num, value, size); }
		inline MDB_STATUS GetFieldValueAsField(MDB_FIELD* field, MDB_FIELD* as_field, size_t field_num, size_t pos, size_t num, void* value, size_t size) { return MDBTableGetFieldValueAsField(*db_, htb_, field, as_field, field_num, pos, num, value, size); }
		inline MDB_STATUS SetFieldValueAsField(MDB_FIELD* field, MDB_FIELD* as_field, size_t field_num, size_t pos, size_t num, const void* value, size_t size) { return MDBTableSetFieldValueAsField(*db_, htb_, field, as_field, field_num, pos, num, value, size); }
		inline MDB_STATUS GetFieldValueAsStr(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, char*** value, size_t** size, const char** format = nullptr) { return MDBTableGetFieldValueAsStr(*db_, htb_, field, field_num, pos, num, value, size, format); }
		inline MDB_STATUS SetFieldValueAsStr(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, const char*** value, const size_t** size) { return MDBTableSetFieldValueAsStr(*db_, htb_, field, field_num, pos, num, value, size); }
		inline MDB_STATUS GetFieldValueAsInt(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, ssize_t* value) { return MDBTableGetFieldValueAsInt(*db_, htb_, field, field_num, pos, num, value); }
		inline MDB_STATUS SetFieldValueAsInt(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, const ssize_t* value) { return MDBTableSetFieldValueAsInt(*db_, htb_, field, field_num, pos, num, value); }
		inline MDB_STATUS GetFieldValueAsDouble(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, double* value) { return MDBTableGetFieldValueAsDouble(*db_, htb_, field, field_num, pos, num, value); }
		inline MDB_STATUS SetFieldValueAsDouble(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, const double* value) { return MDBTableSetFieldValueAsDouble(*db_, htb_, field, field_num, pos, num, value); }
		inline MDB_STATUS GetFieldValue(MDB_FIELD& field, size_t pos, void* value) { 
			return GetFieldValue(&field, 1, pos, 1, value);
		}
		inline MDB_STATUS SetFieldValue(MDB_FIELD& field, size_t pos, const void* value) {
			return SetFieldValue(&field, 1, pos, 1, value);
		}
		inline MDB_STATUS GetFieldValueAs(MDB_FIELD& field, size_t pos, void* value, size_t size) {
			return GetFieldValueAs(&field, 1, pos, 1, value, size);
		}
		inline MDB_STATUS SetFieldValueAs(MDB_FIELD& field, size_t pos, const void* value, size_t size) {
			return SetFieldValueAs(&field, 1, pos, 1, value, size);
		}
		inline MDB_STATUS GetFieldValueAsField(MDB_FIELD& field, MDB_FIELD& as_field, size_t pos, void* value, size_t size) {
			return GetFieldValueAsField(&field, &as_field, 1, pos, 1, value, size);
		}
		inline MDB_STATUS SetFieldValueAsField(MDB_FIELD& field, MDB_FIELD& as_field, size_t pos, const void* value, size_t size) {
			return SetFieldValueAsField(&field, &as_field, 1, pos, 1, value, size);
		}
		inline char* GetFieldValueAsStr(MDB_FIELD& field, size_t pos, char* value, size_t size, const char* format = nullptr) {
			char** val = &value;
			size_t* sz = &size;
			GetFieldValueAsStr(&field, 1, pos, 1, &val, &sz, &format);
			return value;
		}
		inline MDB_STATUS SetFieldValueAsStr(MDB_FIELD& field, size_t pos, const char* value, const size_t size) {
			const char** val = &value;
			const size_t* sz = &size;
			return SetFieldValueAsStr(&field, 1, pos, 1, &val, &sz);
		}
		inline ssize_t GetFieldValueAsInt(MDB_FIELD& field, size_t pos, ssize_t def = 0) {
			ssize_t val = 0;
			if (MDB_STATUS_OK == GetFieldValueAsInt(&field, 1, pos, 1, &val)) {
				return val;
			}
			return def;
		}
		inline MDB_STATUS SetFieldValueAsInt(MDB_FIELD& field, size_t pos, const ssize_t value) {
			return SetFieldValueAsInt(&field, 1, pos, 1, &value);
		}
		inline double GetFieldValueAsDouble(MDB_FIELD& field, size_t pos, double def = 0.) {
			double val = 0;
			if (MDB_STATUS_OK == GetFieldValueAsDouble(&field, 1, pos, 1, &val)) {
				return val;
			}
			return def;
		}
		inline MDB_STATUS SetFieldValueAsDouble(MDB_FIELD& field, size_t pos, const double value) {
			return SetFieldValueAsDouble(&field, 1, pos, 1, &value);
		}
	};

	class TableView
	{
	protected:
		Table* tb_;
		HMTABLE hv_ = nullptr;
	public:
		TableView(Table* tb, HMTABLE hv = nullptr) :tb_(tb), hv_(hv) {

		}
		~TableView() {
			Close();
			tb_ = nullptr;
		}

		inline void Close() {
			if (hv_) {
				MDBCloseTable(*tb_->db(), hv_);
				hv_ = nullptr;
			}
		}

		inline HMTABLE Open(const char* key) { 
			hv_ = MDBOpenTable(*db(), key, MDB_OPEN_FLAG_VIEW); 
			return hv_; 
		}
		inline HMTABLE OpenAndInit(const char* key, MDB_LESS_CB less) { 
			hv_ = MDBOpenAndInitView(key, *db(), *tb(), less); 
			return hv_; 
		}
		inline HMTABLE OpenAndInitStd(const char* key, MDB_FIELD* fields, size_t field_num) { 
			hv_ = MDBOpenAndInitStdView(key, *db(), *tb(), fields, field_num); 
			return hv_; 
		}

		inline operator HMTABLE() const { return hv_; }
		inline HMTABLE handle() { return hv_; }

		inline Table* tb() { return tb_; }
		inline Database* db() { return tb_->db(); }

		//inline const char* name() { return MDBTableGetName(*db_, htb_); }

		//inline HMDB MDBViewGetDB(HMTABLE hv);
		//inline HMTABLE MDBViewGetTable(HMTABLE hv);
		inline uint32_t GetValuePos(size_t pos) { return MDBViewGetValuePos(hv_, pos); }
		inline MDB_STATUS GetValuePosEx(size_t pos, uint32_t* pos_val, size_t* pos_num) { return MDBViewGetValuePosEx(hv_, pos, pos_val, pos_num); }
		inline MDB_STATUS LowerBound(const void* keydata, size_t* pos) { return MDBViewLowerBound(hv_, keydata, pos); }
		inline MDB_STATUS UpperBound(const void* keydata, size_t* pos) { return MDBViewUpperBound(hv_, keydata, pos); }
		inline MDB_STATUS EqualRange(const void* keydata, size_t* from, size_t* to) { return MDBViewEqualRange(hv_, keydata, from, to); }
		inline MDB_STATUS Search(const void* keydata, size_t* pos) { return MDBViewSearch(hv_, keydata, pos); }

		inline void* GetValue(size_t pos) { return tb_->GetValue(GetValuePos(pos)); }
		inline MDB_STATUS CopyValue(size_t pos, void* data, size_t* elem_num) {
			if (!data || !elem_num)
				return MDB_STATUS_INVALID_PARAM;
			size_t elem_sz = 0;
			size_t count = tb_->GetValueCount(&elem_sz);
			if (pos + *elem_num < count && elem_sz) {
				for (size_t i = 0; i <*elem_num; i++)
				{
					memcpy((char*)data + i * elem_sz, GetValue(pos + i), elem_sz);
				}
			}
			return MDB_STATUS_INVALID_PARAM;
		}
		inline MDB_STATUS InsertValue(const void* data, size_t elem_num = 1) {
			return tb_->AppendValue(data, elem_num);
		}
		inline MDB_STATUS RemoveValue(const void* data) {
			size_t pos = 0;
			if (MDB_STATUS_OK == MDBViewSearch(hv_, data, &pos)) {
				return MDBTableRemoveValue(*db(), *tb(), pos, 1);
			}
			return MDB_STATUS_NOT_EXIST;
		}
		inline MDB_STATUS UpdateValue(const void* data) {
			size_t pos = 0;
			if (MDB_STATUS_OK == MDBViewSearch(hv_, data, &pos)) {
				return MDBTableSetValue(*db(), *tb(), pos, data, 1);
			}
			return MDB_STATUS_NOT_EXIST;
		}

		//inline MDB_STATUS GetFieldValue(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, void* value, size_t size) { return MDBTableGetFieldValue(*db_, htb_, field, field_num, pos, num, value, size); }
		//inline MDB_STATUS SetFieldValue(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, const void* value, size_t size) { return MDBTableSetFieldValue(*db_, htb_, field, field_num, pos, num, value, size); }
		//inline MDB_STATUS GetFieldValueAs(MDB_FIELD* field, MDB_FIELD* as_field, size_t field_num, size_t pos, size_t num, void* value, size_t size) { return MDBTableGetFieldValueAs(*db_, htb_, field, as_field, field_num, pos, num, value, size); }
		//inline MDB_STATUS SetFieldValueAs(MDB_FIELD* field, MDB_FIELD* as_field, size_t field_num, size_t pos, size_t num, const void* value, size_t size) { return MDBTableSetFieldValueAs(*db_, htb_, field, as_field, field_num, pos, num, value, size); }
		//inline MDB_STATUS GetFieldValueAsStr(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, char*** value, size_t** size, const char** format = nullptr) { return MDBTableGetFieldValueAsStr(*db_, htb_, field, field_num, pos, num, value, size, format); }
		//inline MDB_STATUS SetFieldValueAsStr(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, const char*** value, const size_t** size) { return MDBTableSetFieldValueAsStr(*db_, htb_, field, field_num, pos, num, value, size); }
		//inline MDB_STATUS GetFieldValueAsInt(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, ssize_t* value) { return MDBTableGetFieldValueAsInt(*db_, htb_, field, field_num, pos, num, value); }
		//inline MDB_STATUS SetFieldValueAsInt(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, const ssize_t* value) { return MDBTableSetFieldValueAsInt(*db_, htb_, field, field_num, pos, num, value); }
		//inline MDB_STATUS GetFieldValueAsDouble(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, double* value) { return MDBTableGetFieldValueAsDouble(*db_, htb_, field, field_num, pos, num, value); }
		//inline MDB_STATUS SetFieldValueAsDouble(MDB_FIELD* field, size_t field_num, size_t pos, size_t num, const double* value) { return MDBTableSetFieldValueAsDouble(*db_, htb_, field, field_num, pos, num, value); }
	};
}

//#endif//LIB_MDB_API_EXPORT

#endif//__cplusplus

#endif//_H_MDBASE_H_