#pragma once

#ifndef _H_NET_H_
#define _H_NET_H_

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

#ifndef NET_API_EXPORT

#if defined(LIB_NET_API) && defined(WIN32)
#ifdef LIB_NET_API_EXPORT
#define NET_API_EXPORT __declspec(dllexport)
#else
#define NET_API_EXPORT __declspec(dllimport)
#endif
#else
#define NET_API_EXPORT 
#endif

#endif//NET_API_EXPORT

#ifndef USE_NET_MSG
#define USE_NET_MSG 1
#endif//USE_NET_MSG

#if defined(__cplusplus)
extern "C" {
#endif//

#pragma pack(push, 1)

typedef enum NET_STATUS
{
	NET_STATUS_OK = 0,
	NET_STATUS_ERROR,
	NET_STATUS_INVALID_PARAM,
	NET_STATUS_BAD_ALLOC,
	NET_STATUS_EXCEPTION,
	NET_STATUS_MEM_OVERFLOW,
}NET_STATUS;

#define NET_LOG_LEVEL_NONE	0
#define NET_LOG_LEVEL_ERROR 1
#define NET_LOG_LEVEL_WARN	2
#define NET_LOG_LEVEL_INFO	3
#define NET_LOG_LEVEL_DEBUG 4
typedef void (*NET_LOG_CB)(int level, const char* logstr);

typedef enum NET_NODE_TYPE
{
	NET_NODE_TYPE_UNKNOWN = 0,
	NET_NODE_TYPE_CONNECT,
	NET_NODE_TYPE_LISTEN,
	NET_NODE_TYPE_PEER,
}NET_NODE_TYPE;
typedef struct NET_NODE
{
	NET_NODE_TYPE type;
}NET_NODE;
typedef struct NET_NODE *HNNODE;

typedef enum NET_NODE_STATUS
{
	NET_NODE_STATUS_DISCONNECT = 0,
	NET_NODE_STATUS_CONNECTING,
	NET_NODE_STATUS_CONNECTED,
}NET_NODE_STATUS;
typedef void (*NET_STATUS_CB)(HNNODE h, NET_NODE_STATUS status); //状态回调
typedef enum NET_MSG_TYPE
{
	NET_MSG_TYPE_UNKNOWN = 0,
	NET_MSG_TYPE_NOTIFY,
	NET_MSG_TYPE_REQUEST_DO,
	NET_MSG_TYPE_REQUEST_QUERY,
	NET_MSG_TYPE_RESPONSE,
}NET_MSG_TYPE;
typedef struct NET_MSG
{
	NET_MSG_TYPE type;
}NET_MSG;
typedef struct NET_MSG *HNMSG;
typedef int (*NET_MSG_CB)(HNMSG h); //消息回调

typedef struct NET_LISTEN_INF
{
	int xmlflag;
	const char* xml;
	NET_LOG_CB log_cb;
	NET_MSG_CB msg_cb;
	NET_STATUS_CB status_cb;
}NET_LISTEN_INF;
NET_API_EXPORT HNNODE NETListen(NET_LISTEN_INF* inf);
NET_API_EXPORT NET_STATUS NETReListen(HNNODE h);
typedef struct NET_CONNECT_INF
{
	int xmlflag;
	const char* xml;
	NET_LOG_CB log_cb;
	NET_MSG_CB msg_cb;
	NET_STATUS_CB status_cb;
}NET_CONNECT_INF;
NET_API_EXPORT HNNODE NETConnect(NET_CONNECT_INF* inf);
NET_API_EXPORT NET_STATUS NETReConnect(HNNODE h);
NET_API_EXPORT NET_STATUS NETClose(HNNODE h);
NET_API_EXPORT const char* NETGetURI(HNNODE h);
typedef enum NET_NODE_FLAG {
	NET_NODE_FLAG_DEBUG = 0x01, //调试标志，如果设置会额外输出NODE调试信息
	NET_NODE_FLAG_USER1 = 0x02, //用户自定义标志1
	NET_NODE_ROLE_USER2 = 0x04, //用户自定义标志2
	NET_NODE_ROLE_USER3 = 0x08, //用户自定义标志3
	NET_NODE_ROLE_USER4 = 0x10, //用户自定义标志4
}NET_NODE_FLAG;
NET_API_EXPORT void NETSetFlag(HNNODE h, size_t flags);
NET_API_EXPORT size_t NETGetFlag(HNNODE h);
//NET_API_EXPORT void* NETGetData(HNNODE h);
//NET_API_EXPORT void NETSetData(HNNODE h, void* data);

NET_API_EXPORT NET_MSG* NETMsgNew(NET_MSG_TYPE type);
NET_API_EXPORT NET_STATUS NETMsgRef(NET_MSG* msg);
NET_API_EXPORT NET_STATUS NETMsgDelete(NET_MSG* msg);
typedef enum NET_MSG_CLEAR_FLAGS
{
	//NET_MSG_CLEAR_ALL = 0,
	NET_MSG_CLEAR_PARAM = 0X01,
	//NET_MSG_CLEAR_ATTR = 0X02,
	NET_MSG_CLEAR_DATA = 0X04,
}NET_MSG_CLEAR_FLAGS;
NET_API_EXPORT NET_STATUS NETMsgClear(NET_MSG* msg, size_t flags);
NET_API_EXPORT NET_STATUS NETMsgSetNode(NET_MSG* msg, HNNODE h);
NET_API_EXPORT HNNODE NETMsgGetNode(NET_MSG* msg);
NET_API_EXPORT NET_STATUS NETMsgSetID(NET_MSG* msg, size_t id);
NET_API_EXPORT size_t NETMsgGetID(NET_MSG* msg);
NET_API_EXPORT NET_STATUS NETMsgSetParam(NET_MSG* msg, const char* name, const char* value);
NET_API_EXPORT NET_STATUS NETMsgSetParamAsInt(NET_MSG* msg, const char* name, ssize_t value);
NET_API_EXPORT NET_STATUS NETMsgSetParamAsUInt(NET_MSG* msg, const char* name, size_t value);
NET_API_EXPORT NET_STATUS NETMsgSetParamAsDouble(NET_MSG* msg, const char* name, double value);
NET_API_EXPORT const char* NETMsgGetParam(NET_MSG* msg, const char* name);
NET_API_EXPORT ssize_t NETMsgGetParamAsInt(NET_MSG* msg, const char* name, ssize_t def);
NET_API_EXPORT size_t NETMsgGetParamAsUInt(NET_MSG* msg, const char* name, size_t def);
NET_API_EXPORT double NETMsgGetParamAsDouble(NET_MSG* msg, const char* name, double def);
//NET_API_EXPORT NET_STATUS NETMsgSetAttr(NET_MSG* msg, const char* data, const size_t size);
//NET_API_EXPORT const char* NETMsgGetAttr(NET_MSG* msg, size_t* size);
NET_API_EXPORT NET_STATUS NETMsgSetData(NET_MSG* msg, const char* data, const size_t size);
NET_API_EXPORT const char* NETMsgGetData(NET_MSG* msg, size_t* size);

NET_API_EXPORT NET_STATUS NETSendMsg(NET_MSG* msg, NET_MSG** rsp, size_t timeout);
//NET_API_EXPORT NET_STATUS NETWaitMsg(NET_MSG* msg, NET_MSG** rsp);

#pragma pack(pop)

#if defined(__cplusplus)
}

namespace net {

#if USE_NET_MSG
	class Msg
	{
	private:
		NET_MSG* msg_ = nullptr; 
		bool delete_flag_ = false;
	public:
		Msg() {}
		Msg(NET_MSG* msg, bool ref = false) :msg_(msg) {
			if (ref) {
				NETMsgRef(msg_);
				delete_flag_ = true;
			}
		}
		Msg(NET_MSG_TYPE type) :msg_(NETMsgNew(type)), delete_flag_(true) {

		}
		~Msg() {
			if (delete_flag_) {
				NETMsgDelete(msg_);
				msg_ = nullptr;
				delete_flag_ = false;
			}
		}
		Msg(const Msg& msg) {
			msg_ = msg.msg_;
			delete_flag_ = msg.delete_flag_;
			if (delete_flag_) {
				NETMsgRef(msg_);
			}
		}
		Msg& operator=(const Msg& msg) {
			msg_ = msg.msg_;
			delete_flag_ = msg.delete_flag_;
			if (delete_flag_) {
				NETMsgRef(msg_);
			}
			return *this;
		}
		Msg(Msg&& msg) {
			msg_ = msg.msg_;
			msg.msg_ = nullptr;
			delete_flag_ = msg.delete_flag_;
			msg.delete_flag_ = false;
		}
		Msg& operator=(Msg&& msg) {
			msg_ = msg.msg_;
			msg.msg_ = nullptr;
			delete_flag_ = msg.delete_flag_;
			msg.delete_flag_ = false;
			return *this;
		}

		inline operator NET_MSG*() const { return msg_; }
		inline NET_MSG_TYPE GetType() const { return msg_->type; }

		inline NET_STATUS Clear(size_t flags) { return NETMsgClear(msg_, flags); }

		inline HNNODE GetNode() const { return NETMsgGetNode(msg_); }
		inline NET_STATUS SetNode(HNNODE h) { return NETMsgSetNode(msg_, h); }

		inline size_t GetID() const { return NETMsgGetID(msg_); }
		inline NET_STATUS SetID(size_t id) { return NETMsgSetID(msg_, id); }

		inline const char* GetParam(const char* name) const { return NETMsgGetParam(msg_, name); }
		inline const char* GetParam(const char* name, const char* def) const { 
			const char* val = NETMsgGetParam(msg_, name);
			if (val) {
				return val;
			}
			return def;
		}
		inline ssize_t GetParamAsInt(const char* name, ssize_t def = 0) const { return NETMsgGetParamAsInt(msg_, name, def); }
		inline size_t GetParamAsUInt(const char* name, size_t def = 0) const { return NETMsgGetParamAsUInt(msg_, name, def); }
		inline double GetParamAsDouble(const char* name, double def = 0) const { return NETMsgGetParamAsDouble(msg_, name, def); }
		inline NET_STATUS SetParam(const char* name, const char* value) { return NETMsgSetParam(msg_, name, value); }
		inline NET_STATUS SetParamAsInt(const char* name, ssize_t value) { return NETMsgSetParamAsInt(msg_, name, value); }
		inline NET_STATUS SetParamAsUInt(const char* name, size_t value) { return NETMsgSetParamAsUInt(msg_, name, value); }
		inline NET_STATUS SetParamAsDouble(const char* name, double value) { return NETMsgSetParamAsDouble(msg_, name, value); }

		/*inline const char* GetAttr(size_t* size) const { return NETMsgGetAttr(msg_, size); }
		inline NET_STATUS SetAttr(const char* data, const size_t size) { return NETMsgSetAttr(msg_, data, size);  }*/

		inline const char* GetData(size_t* size) const { return NETMsgGetData(msg_, size); }
		inline NET_STATUS SetData(const char* data, const size_t size) { return NETMsgSetData(msg_, data, size); }
	};
#endif//USE_NET_MSG

	class Node
	{
	protected:
		HNNODE h_ = nullptr;
	public:
		Node(HNNODE h) :h_(h)
		{
		}
		~Node()
		{
			if (h_) {
				NETClose(h_);
				h_ = nullptr;
			}
		}

		operator HNNODE() const { return h_; }

		const char* GetURI() { return NETGetURI(h_); }

		void SetFlag(size_t flags) { NETSetFlag(h_, flags); }
		size_t GetFlag() { return NETGetFlag(h_); }

		/*void* GetData() { return NETGetData(h_); }
		void SetData(void* data) { NETSetData(h_, data); }*/

		/*NET_STATUS SendMsg(NET_MSG* msg, NET_MSG** rsp, size_t timeout) { 
			return NETSendMsg(msg, rsp, timeout); 
		}*/
	};

	class Client : public Node
	{
	public:
		Client(HNNODE h) : Node(h)
		{
		}
		Client(NET_CONNECT_INF& inf) : Node(NETConnect(&inf))
		{
		}

		HNNODE Connect(NET_CONNECT_INF& inf) { h_ = NETConnect(&inf); return h_; }
		NET_STATUS ReConnect() { return NETReConnect(h_); }
	};

	class Server : public Node
	{
	public:
		Server(HNNODE h) : Node(h)
		{
		}
		Server(NET_LISTEN_INF& inf) : Node(NETListen(&inf))
		{
		}

		HNNODE Listen(NET_LISTEN_INF& inf) { h_ = NETListen(&inf); return h_; }
		NET_STATUS ReListen() { return NETReListen(h_); }
	};
}

#endif//__cplusplus

#endif//_H_NET_H_