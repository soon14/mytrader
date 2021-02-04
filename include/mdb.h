#pragma once

#ifndef _H_MDB_H_
#define _H_MDB_H_

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

//net
#define NET_API_EXPORT MDB_API_EXPORT
#include <net.h>

//mdbase
#include <mdbase.h>

#if defined(__cplusplus)
extern "C" {
#endif//

#pragma pack(push, 1)

MDB_API_EXPORT bool MDBIsSrv();
static inline bool MDBIsApi() { return !MDBIsSrv(); }
MDB_API_EXPORT bool MDBIsRpc();
static inline bool MDBIsIpc() { return !MDBIsRpc(); }

//MDB_API_EXPORT HNNODE MDBListen();
MDB_API_EXPORT HNNODE MDBCli();

MDB_API_EXPORT size_t MDBReqID(size_t type);
MDB_API_EXPORT size_t MDBReqType(size_t id);

#define MDB_OPEN_FLAG_SYNC 0x10 //同步数据
#define MDB_OPEN_FLAG_SYNC_WAIT 0x20 //同步等待
#define MDB_OPEN_FLAG_SYNC_SUBSCRIBE 0x40 //同步订阅
//MDB_API_EXPORT HMTABLE MDBOpenTable(HMDB hdb, const char* key, size_t flags);

#pragma pack(pop)

#if defined(__cplusplus)
}

//#ifndef LIB_MDB_API_EXPORT

namespace mdb {

#if USE_NET_MSG
	class Msg : public net::Msg
	{
		typedef net::Msg Base;
	public:	
		using Base::Base;

		inline NET_STATUS SetReqID(size_t req_type) { return SetID(MDBReqID(req_type)); }
		inline size_t GetReqID() { return GetID(); }
		inline size_t GetReqType() const { return MDBReqType(GetID()); }
	};
#endif
}

//#endif//LIB_MDB_API_EXPORT

#endif//__cplusplus

#endif//_H_MDB_H_