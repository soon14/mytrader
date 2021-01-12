#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include "zqsrv.h"

extern "C" {

	void OnLog(int level, const char* logstr)
	{
		printf("%s\n", logstr);
	}

	void OnNetStatus(HNNODE h, NET_NODE_STATUS status)
	{

	}

	int OnNetMsg(HNMSG hmsg)
	{
		return 0;
	}

	void OnMDBNotify(HMDB hdb, HMTABLE htb, MDB_NOTIFY_DATA* notify)
	{
		if (hdb && htb) {
			printf("%s %s %d\n", MDBGetName(hdb), MDBTableGetName(hdb, htb), notify->notify);
		}
	}
}

namespace zqdb {

    Server& Server::Inst()
    {
        static Server s_srv;
        return s_srv;
    }

    Server::Server() : Base(), timer_(service()), status_(Status::STOP)
    {
        
    }

	//const char *name() { return "zqdbsrv"; }

	void Server::init(int argc, char *argv[])
	{
		status_ = Status::INIT;

		Base::init(argc, argv);

		//boost::filesystem::path log_path = data_path();
		//std::string log_file = log_path.append(name()).string();
		//XLogger_Init(log_file.c_str(), XLoggerLevel::Debug, 100 * 1024 * 1024);

		//size_t max_thread_count = std::thread::hardware_concurrency();

		ZQDB_INF zqdb_inf = { XUtil::XML_FLAG_PTREE, (const char*)&cfg_, &OnLog, &OnNetStatus, &OnNetMsg, &OnMDBNotify };
        ZQDBStart(&zqdb_inf);

		/*auto ctp_opt = cfg_.get_child_optional("ctp");
		if (ctp_opt) {
			boost::property_tree::ptree ctp_cfg = ctp_opt.get();
			ctp_ = std::make_shared<CTP>();
			ctp_->start((char*)&ctp_cfg, XUtil::XML_FLAG_PTREE);
		}*/
		
		register_timer();

		status_ = Status::RUN;
	}

	void Server::term()
	{
		Base::term();
	}

	//inline bool is_run() { return status_ == Status::RUN; }
	
	void Server::stop()
	{
		status_ = Status::STOP;

		unregister_timer();

		Base::stop();

		/*if(ctp_)
			ctp_->stop();

		ctp_.reset();*/

		ZQDBStop();
	}

    //
	void Server::register_timer() 
	{
		timer_.expires_from_now(boost::posix_time::milliseconds(200));
		timer_.async_wait(std::bind(&Server::on_timer, this));
	}

	void Server::unregister_timer()
	{
		boost::system::error_code ec;
		timer_.cancel(ec);
	}

	void Server::on_timer()
	{
		if (!is_run()) {
			return;
		}

		register_timer();
	}
}