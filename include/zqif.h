#pragma once

#ifndef _H_ZQDB_IF_H_
#define _H_ZQDB_IF_H_

#include "zq.h"

#if defined(__cplusplus)
extern "C" {
#endif//

#pragma pack(push, 1)
	
//typedef enum ZQDB_NOTIFY_TYPE
//{
//	NOTIFY_UNKNOWN = 0,
//	NOTIFY_INIT, //初始化数据
//	NOTIFY_META, //更新元数据
//	NOTIFY_ATTR, //更新属性
//	NOTIFY_INSERT, //更新数据
//	NOTIFY_UPDATE, //更新数据
//	NOTIFY_APPEND, //更新数据
//	NOTIFY_REMOVE, //删除数据
//	NOTIFY_UPDATE_FIELD, //更新数据
//}ZQDB_NOTIFY;
//struct ZQDB_NOTIFY
//{
//	ZQDB_NOTIFY_TYPE type;
//};
//typedef void(*ZQDB_NOTIFY_CB)(HZQDB h, ZQDB_NOTIFY* notify);

struct ZQDB_INF
{
	int xmlflag;
	const char* xml;
	MDB_LOG_CB log_cb;
	NET_STATUS_CB status_cb;
	NET_MSG_CB msg_cb;
	MDB_NOTIFY_CB notify_cb;
};

ZQDB_API_EXPORT MDB_STATUS ZQDBStart(ZQDB_INF* inf);
ZQDB_API_EXPORT MDB_STATUS ZQDBStop();

#pragma pack(pop)

#if defined(__cplusplus)
}

#endif//

#endif//_H_ZQDB_IF_H_