#pragma once

#ifndef _H_ZQDBMGR_H_
#define _H_ZQDBMGR_H_

#include "zqif.h"

#if defined(LIB_ZQDBMGR_API) && defined(WIN32)
#ifdef LIB_ZQDBMGR_API_EXPORT
#define ZQDBMGR_API_EXPORT __declspec(dllexport)
#else
#define ZQDBMGR_API_EXPORT __declspec(dllimport)
#endif
#else
#define ZQDBMGR_API_EXPORT 
#endif

#if defined(__cplusplus)
extern "C" {
#endif//

#pragma pack(push, 1)

ZQDBMGR_API_EXPORT MDB_STATUS ZQDBMgrStart(ZQDB_INF* inf);
ZQDBMGR_API_EXPORT MDB_STATUS ZQDBMgrStop();

#pragma pack(pop)

#if defined(__cplusplus)
}

#endif//

#endif//_H_ZQDBMGR_H_