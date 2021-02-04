#pragma once

#ifndef _H_NET_IF_H_
#define _H_NET_IF_H_

#include "net.h"

#if defined(__cplusplus)
extern "C" {
#endif//

#pragma pack(push, 1)

typedef struct NET_INF
{
	int xmlflag;
	const char* xml;
	NET_LOG_CB log_cb;
	NET_STATUS_CB status_cb;
	NET_MSG_CB msg_cb;
}NET_INF;
NET_API_EXPORT NET_STATUS NETStart(NET_INF* inf);
NET_API_EXPORT NET_STATUS NETStop();

#pragma pack(pop)

#if defined(__cplusplus)
}

namespace net {

}

#endif//__cplusplus

#endif//_H_NET_IF_H_