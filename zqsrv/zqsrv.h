#pragma once

#include <zqif.h>

#ifndef LOG4E
#define LOG4E(format,...) printf("%s " format "\n", __FUNCTION__, ##__VA_ARGS__)
#endif//LOG4E
#ifndef LOG4W
#define LOG4W LOG4E
#endif//LOG4W
#ifndef LOG4I
#define LOG4I LOG4E
#endif//LOG4I
#ifndef LOG4D
#define LOG4D LOG4E
#endif//LOG4D

#include <XUtil/XServer.hpp>

namespace zqdb
{
	class Server
		: public XUtil::XServer<Server>
	{
		typedef XUtil::XServer<Server> Base;

	private:
		boost::asio::deadline_timer timer_;
		enum Status
		{
			STOP,
			INIT,
			RUN
		};
		Status status_;

	public:

		static Server& Inst();

		Server();

		const char *name() { return "zqdbsrv"; }

		void init(int argc, char *argv[]);
		void term();

		inline bool is_run() { return status_ == Status::RUN; }

		void stop();

	protected:
		//
		void register_timer();
		void unregister_timer();

		void on_timer();
	};
} // namespace zqdb
