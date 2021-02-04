#pragma once

#ifndef _H_MDB_IF_H_
#define _H_MDB_IF_H_

#include "mdb.h"

#if defined(__cplusplus)
extern "C" {
#endif//

#pragma pack(push, 1)

struct MDB_INF
{
	int xmlflag;
	const char* xml;
	MDB_LOG_CB log_cb;
	NET_STATUS_CB status_cb;
	NET_MSG_CB msg_cb;
	MDB_NOTIFY_CB notify_cb;
};

MDB_API_EXPORT MDB_STATUS MDBStart(MDB_INF* inf);
MDB_API_EXPORT MDB_STATUS MDBStop();

#pragma pack(pop)

#if defined(__cplusplus)
}

namespace mdb {

}

#endif//__cplusplus

#endif//_H_MDB_IF_H_