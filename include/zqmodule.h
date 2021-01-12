#pragma once

#ifndef _H_ZQDB_MODULE_H_
#define _H_ZQDB_MODULE_H_

#include "zq.h"

#if defined(LIB_ZQDB_MODULE_API) && defined(WIN32)
#ifdef LIB_ZQDB_MODULE_API_EXPORT
#define ZQDB_MODULE_API_EXPORT __declspec(dllexport)
#else
#define ZQDB_MODULE_API_EXPORT __declspec(dllimport)
#endif
#else
#define ZQDB_MODULE_API_EXPORT 
#endif

#if defined(__cplusplus)
extern "C" {
#endif//

#pragma pack(push, 1)

typedef NET_STATUS (*fnRequest)(NET_MSG* msg, NET_MSG** rsp, size_t timeout);

struct ZQDB_MODULE_INF
{
	int xmlflag;
	const char* xml;
	MDB_LOG_CB log_cb; //[in]
	NET_STATUS_CB status_cb; //[out]
	NET_MSG_CB msg_cb; //[out]
	MDB_NOTIFY_CB notify_cb; //[out]
	fnRequest request; //[out]
};

typedef bool (*fnZQDBModuleStart)(ZQDB_MODULE_INF* inf);
typedef void (*fnZQDBModuleStop)();

ZQDB_MODULE_API_EXPORT bool ZQDBModuleStart(ZQDB_MODULE_INF* inf);
ZQDB_MODULE_API_EXPORT void ZQDBModuleStop();

#pragma pack(pop)

#if defined(__cplusplus)
}

#endif//

#endif//_H_ZQDB_MODULE_H_