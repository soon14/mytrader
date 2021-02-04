#pragma once

#ifndef _H_ZQDB_H_
#define _H_ZQDB_H_

#include <net.h>
#include <mdbase.h>

#if defined(LIB_ZQDB_API) && defined(WIN32)
#ifdef LIB_ZQDB_API_EXPORT
#define ZQDB_API_EXPORT __declspec(dllexport)
#else
#define ZQDB_API_EXPORT __declspec(dllimport)
#endif
#else
#define ZQDB_API_EXPORT 
#endif

#if defined(__cplusplus)
extern "C" {
#endif//

#pragma pack(push, 1)

//证券主要状态：
//连接状态，Cli和Srv连接状态
//行情状态，包括市场、产品、代码状态，其中市场包括连接状态和市场状态
//交易状态，即用户状态，也就是说用户的连接状态
//其中连接状态提供专门一张表用于表示各种状态，名称分别是服务（zqsrv）、市场（CFFEX）、用户（USER）
#define STR_ZQDB_TABLE_STATUS "STATUS"

//模块
//所有模块表 MODULE = 模块列表[NAME,...]
#define STR_ZQDB_TABLE_MODULE "MODULE"

//行情表结构
//所有市场表 EXCHANGE = 市场列表[CODE,...]
//具体市场表 CODE = 品种/板块列表[CODE,NAME,...]
//具体品种/板块表 CODE.EXCHANGE = 代码静态和快照数据列表[CODE,NAME,...]
//代码明细表 CODE.TICK = 明细数据列表[TICK,...]
//代码K线表(M1、DAY) CODE.M1 = K线数据列表[...]
//代码其他数据表 CODE.XXX = 其他数据列表[...]
#define STR_ZQDB_TABLE_EXCHANGE "EXCHANGE"

//交易表结构
//具体用户表 USER.MODULE = 用户列表[USERID,NAME,...]
//具体持仓/委托/成交等表 XXX.MODULE = [XXX,...]
#define STR_ZQDB_TABLE_USER "USER"
#define STR_ZQDB_TABLE_INVESTOR "INVESTOR"
#define STR_ZQDB_TABLE_ACCOUNT "ACCOUNT"
#define STR_ZQDB_TABLE_POSITION "POSITION"
#define STR_ZQDB_TABLE_ORDER "ORDER"
#define STR_ZQDB_TABLE_TRADE "TRADE"


//全局策略表,包含所有策略表信息
#define STR_ZQDB_TABLE_STRATEGY "STRATEGY"

//全局产品表,包含所有自定义产品表信息（类似ETF、自定义投资组合）
#define STR_ZQDB_TABLE_PRODUCT "PRODUCT"

//全局自选表,包含所有自选表信息
#define STR_ZQDB_TABLE_SELFSEL "SELFSEL"

//最长7个字符，第8个始终为0
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_TYPE = "TYPE"; //uint8_t 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_DATE = "DATE"; //uint32_t YYYYMMDD 日期
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_TIME = "TIME"; //uint32_t HHMMSSsss 时间
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_STATUS = "STATUS"; //char 状态
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_EXCHANGE = "EXCHAGE"; //string 市场代码
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_PRODUCT = "PRDUCT"; //string 品种代码
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CODE = "CODE"; //string 代码
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_NAME = "NAME"; //string 名称
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ID = "ID"; //string ID
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BROKER = "BROKER"; //string 经纪商ID
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_INVESTOR = "INVSTOR"; //string 投资者ID
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ACCOUNT = "ACCOUNT"; //string 投资者ID
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_USER = "USER"; //string 用户ID
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_TTM_COUNT = "TTMCNT"; //uint8_t 交易时段数
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_TTM_FROM1 = "TTMFM1"; //uint16_t 交易时段开始1
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_TTM_TO1 = "TTMTO1"; //uint16_t 交易时段结束1
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_TTM_FROM2 = "TTMFM2"; //uint16_t 交易时段开始2
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_TTM_TO2 = "TTMTO2"; //uint16_t 交易时段开始2
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_TTM_FROM3 = "TTMFM3"; //uint16_t 交易时段开始3
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_TTM_TO3 = "TTMTO3"; //uint16_t 交易时段开始3
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_TTM_FROM4 = "TTMFM4"; //uint16_t 交易时段开始4
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_TTM_TO4 = "TTMTO4"; //uint16_t 交易时段开始4
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_TTM_ZONE = "TTMZ"; //uint8_t 交易时区
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_TRADE_DAY = "TDAY"; //uint32_t YYYYMMDD 交易日
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ENTER_REASON = "REASON"; //char 进入原因
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_OBJECT = "OBJ"; //string 交易标的
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_MULTIPLE = "MULTI"; //string 交易合约乘数
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_PRICE_TICK = "PXTICK"; //double 最小价格变动
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_OPEN = "OPEN"; //double 开盘价
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_HIGH = "HIGH"; //double 最高价
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_LOW = "LOW"; //double 最低价
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_PRICE = "PRICE"; //double 价格
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CLOSE = "CLOSE"; //double 收盘价
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_YCLOSE = "YCLOSE"; //double 昨收价
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_VOLUME = "VOLUME"; //double 成交量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_LEAVE_VOLUME = "LVVOL"; //double 剩余量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_AMOUNT = "AMOUNT"; //double 成交额
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASK1 = "ASK1"; //double 卖一
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASK2 = "ASK2"; //double 卖二
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASK3 = "ASK3"; //double 卖三
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASK4 = "ASK4"; //double 卖四
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASK5 = "ASK5"; //double 卖五
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASK6 = "ASK6"; //double 卖六
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASK7 = "ASK7"; //double 卖七
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASK8 = "ASK8"; //double 卖八
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASK9 = "ASK9"; //double 卖九
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASK10 = "ASK10"; //double 卖十量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKV1 = "ASKV1"; //double 卖一量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKV2 = "ASKV2"; //double 卖二量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKV3 = "ASKV3"; //double 卖三量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKV4 = "ASKV4"; //double 卖四量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKV5 = "ASKV5"; //double 卖五量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKV6 = "ASKV6"; //double 卖六量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKV7 = "ASKV7"; //double 卖七量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKV8 = "ASKV8"; //double 卖八量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKV9 = "ASKV9"; //double 卖九量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKV10 = "ASKV10"; //double 卖十量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKX1 = "ASKX1"; //double 卖一差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKX2 = "ASKX2"; //double 卖二差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKX3 = "ASKX3"; //double 卖三差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKX4 = "ASKX4"; //double 卖四差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKX5 = "ASKX5"; //double 卖五差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKX6 = "ASKX6"; //double 卖六差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKX7 = "ASKX7"; //double 卖七差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKX8 = "ASKX8"; //double 卖八差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKX9 = "ASKX9"; //double 卖九差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ASKX10 = "ASKX10"; //double 卖十差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BID1 = "BID1"; //double 买一
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BID2 = "BID2"; //double 买二
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BID3 = "BID3"; //double 买三
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BID4 = "BID4"; //double 买四
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BID5 = "BID5"; //double 买五
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BID6 = "BID6"; //double 买六
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BID7 = "BID7"; //double 买七
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BID8 = "BID8"; //double 买八
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BID9 = "BID9"; //double 买九
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BID10 = "BID10"; //double 买十量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDV1 = "BIDV1"; //double 买一量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDV2 = "BIDV2"; //double 买二量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDV3 = "BIDV3"; //double 买三量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDV4 = "BIDV4"; //double 买四量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDV5 = "BIDV5"; //double 买五量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDV6 = "BIDV6"; //double 买六量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDV7 = "BIDV7"; //double 买七量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDV8 = "BIDV8"; //double 买八量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDV9 = "BIDV9"; //double 买九量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDV10 = "BIDV10"; //double 买十量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDX1 = "BIDX1"; //double 买一差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDX2 = "BIDX2"; //double 买二差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDX3 = "BIDX3"; //double 买三差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDX4 = "BIDX4"; //double 买四差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDX5 = "BIDX5"; //double 买五差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDX6 = "BIDX6"; //double 买六差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDX7 = "BIDX7"; //double 买七差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDX8 = "BIDX8"; //double 买八差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDX9 = "BIDX9"; //double 买九差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_BIDX10 = "BIDX10"; //double 买十差量
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_TICK_COUNT = "TICKCNT"; //uint32_t 成交笔数
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CURRENCY = "CNY"; //char 币种
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_DIRECTION = "DIR"; //char 订单方向
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_OFFSET = "OFFSET"; //char 开平标志
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ORDER_TYPE = "ORDTYPE"; //char 订单类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_ORDER_STATUS = "ORDSTAT"; //char 订单状态

static const MDB_FIELD_ID ZQDB_FIELD_TYPE = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_TYPE; //uint8_t 类型
static const MDB_FIELD_ID ZQDB_FIELD_DATE = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_DATE; //uint32_t YYYYMMDD 日期
static const MDB_FIELD_ID ZQDB_FIELD_TIME = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_TIME; //uint32_t HHMMSSsss 时间
static const MDB_FIELD_ID ZQDB_FIELD_STATUS = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_STATUS; //char 状态
static const MDB_FIELD_ID ZQDB_FIELD_EXCHANGE = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_EXCHANGE; //string 市场代码
static const MDB_FIELD_ID ZQDB_FIELD_PRODUCT = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_PRODUCT; //string 品种代码
static const MDB_FIELD_ID ZQDB_FIELD_CODE = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_CODE; //string 代码
static const MDB_FIELD_ID ZQDB_FIELD_NAME = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_NAME; //string 名称
static const MDB_FIELD_ID ZQDB_FIELD_ID = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_ID; //string ID
static const MDB_FIELD_ID ZQDB_FIELD_BROKER = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_BROKER; //string 经纪商ID
static const MDB_FIELD_ID ZQDB_FIELD_INVESTOR = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_INVESTOR; //string 投资者ID
static const MDB_FIELD_ID ZQDB_FIELD_ACCOUNT = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_ACCOUNT; //string 投资者ID
static const MDB_FIELD_ID ZQDB_FIELD_USER = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_USER; //string 用户ID
static const MDB_FIELD_ID ZQDB_FIELD_TTM_COUNT = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_TTM_COUNT; //uint8_t 交易时段数
static const MDB_FIELD_ID ZQDB_FIELD_TTM_FROM1 = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_TTM_FROM1; //uint16_t 交易时段开始1
static const MDB_FIELD_ID ZQDB_FIELD_TTM_TO1 = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_TTM_TO1; //uint16_t 交易时段结束1
static const MDB_FIELD_ID ZQDB_FIELD_TTM_FROM2 = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_TTM_FROM2; //uint16_t 交易时段开始2
static const MDB_FIELD_ID ZQDB_FIELD_TTM_TO2 = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_TTM_TO2; //uint16_t 交易时段开始2
static const MDB_FIELD_ID ZQDB_FIELD_TTM_FROM3 = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_TTM_FROM3; //uint16_t 交易时段开始3
static const MDB_FIELD_ID ZQDB_FIELD_TTM_TO3 = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_TTM_TO3; //uint16_t 交易时段开始3
static const MDB_FIELD_ID ZQDB_FIELD_TTM_FROM4 = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_TTM_FROM4; //uint16_t 交易时段开始4
static const MDB_FIELD_ID ZQDB_FIELD_TTM_TO4 = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_TTM_TO4; //uint16_t 交易时段开始4
static const MDB_FIELD_ID ZQDB_FIELD_TTM_ZONE = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_TTM_ZONE; //uint8_t 交易时区
static const MDB_FIELD_ID ZQDB_FIELD_TRADE_DAY = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_TRADE_DAY; //uint32_t YYYYMMDD 交易日
static const MDB_FIELD_ID ZQDB_FIELD_ENTER_REASON = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_ENTER_REASON; //char 进入原因
static const MDB_FIELD_ID ZQDB_FIELD_OBJECT = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_OBJECT; //string 交易标的
static const MDB_FIELD_ID ZQDB_FIELD_MULTIPLE = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_MULTIPLE; //string 交易合约乘数
static const MDB_FIELD_ID ZQDB_FIELD_PRICE_TICK = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_PRICE_TICK; //double 最小价格变动
static const MDB_FIELD_ID ZQDB_FIELD_OPEN = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_OPEN; //double 开盘价
static const MDB_FIELD_ID ZQDB_FIELD_HIGH = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_HIGH; //double 最高价
static const MDB_FIELD_ID ZQDB_FIELD_LOW = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_LOW; //double 最低价
static const MDB_FIELD_ID ZQDB_FIELD_PRICE = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_PRICE; //double 价格
static const MDB_FIELD_ID ZQDB_FIELD_CLOSE = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_CLOSE; //double 收盘价
static const MDB_FIELD_ID ZQDB_FIELD_YCLOSE = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_YCLOSE; //double 昨收价
static const MDB_FIELD_ID ZQDB_FIELD_VOLUME = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_VOLUME; //double 成交量
static const MDB_FIELD_ID ZQDB_FIELD_AMOUNT = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_AMOUNT; //double 成交额
static const MDB_FIELD_ID ZQDB_FIELD_TICK_COUNT = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_TICK_COUNT; //uint32_t 成交笔数 
static const MDB_FIELD_ID ZQDB_FIELD_CURRENCY = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_CURRENCY; //char 币种
static const MDB_FIELD_ID ZQDB_FIELD_DIRECTION = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_DIRECTION; //char 订单方向
static const MDB_FIELD_ID ZQDB_FIELD_OFFSET = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_OFFSET; //char 开平标志
static const MDB_FIELD_ID ZQDB_FIELD_ORDER_TYPE = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_ORDER_TYPE; //char 订单类型
static const MDB_FIELD_ID ZQDB_FIELD_ORDER_STATUS = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_ORDER_STATUS; //char 订单状态

#define INVALID_VALUE 1e16
#define INVALID_CTL_VALUE 1e15
#define INVALID_POS (size_t)-1
#define IsInvalidValue(v) ((v)<-INVALID_CTL_VALUE || (v)>INVALID_CTL_VALUE)
#define IsInvalidPos(p) ((p)==INVALID_POS)

#define INT64_TO_DOUBLE(x)		(double)((int64_t)(x))
#define UINT64_TO_DOUBLE(x)		(double)((usinged  int64_t)(x))

#define YYMMDD_TO_ULONG(y,m,d)		((y)*10000+m*100+d)
#define ULONG_TO_YYMMDD(x,y,m,d)	((y)=(x)/10000,(m)=(x)/100%100,(d)=(x)%100)

#define HHMMSS_TO_ULONG(h,m,s)		((h)*10000+m*100+s)
#define ULONG_TO_HHMMSS(x,h,m,s)	((h)=(x)/10000,(m)=(x)/100%100,(s)=(x)%100)

#define HHMMSSsss_TO_ULONG(h,m,s,sss)		(HHMMSS_TO_ULONG(h,m,s)*1000+(sss))
#define ULONG_TO_HHMMSSsss(x,h,m,s,sss)		((h)=(x)/(10000000),(m)=(x)/100000%100000,(s)=(x)/1000%1000,(sss)=(x)%1000)

#define MINUTE_TO_HHMM(x)		((x)/60*100 + (x)%60)
#define HHMM_TO_MINUTE(x)		((x)/100*60 + (x)%100)

#define SECOND_TO_HHMM(x)		MINUTE_TO_HHMM((x)/60)
#define HHMM_TO_SECOND(x)		(HHMM_TO_MINUTE(x)*60)

#define HHMMSS_TO_MINUTE(x)		HHMM_TO_MINUTE((x)/100)
#define HHMMSSsss_TO_MINUTE(x)	HHMMSS_TO_MINUTE((x)/1000)
#define MINUTE_TO_HHMMSS(x)		(MINUTE_TO_HHMM(x)*100)
#define MINUTE_TO_HHMMSSsss(x)	(MINUTE_TO_HHMMSS(x)*1000)

#define HHMMSS_TO_SECOND(x)		((x)/10000*60*60 + (x)%10000/100*60 + (x)%10000%100)
#define HHMMSSsss_TO_SECOND(x)	HHMMSS_TO_SECOND((x)/1000)
#define SECOND_TO_HHMMSS(x)		((x)/(60*60)*10000 + (x)%(60*60)/60*100 + (x)%60)
#define SECOND_TO_HHMMSSsss(x)	(SECOND_TO_HHMMSS(x)*1000)

enum UpdateFlag 
{
	Add 			= 0,
	Remove,
	Update,
};

enum FindFlag 
{
	FindOrCreate 	= 0,
	FindOnly,
	CreateOnly,
};

#define		MAX_EXCHANGE_LENGTH		7	//市场代码最大长度
#define		MAX_PRODUCT_LENGTH		7	//产品代码最大长度
#define		MAX_CODE_LENGTH			14	//商品代码最大长度
#define		MAX_NAME_LENGTH			31	//商品名称最大长度

// # Chinese
#define EXCHANGE_CFFEX "CFFEX"         //# China Financial Futures Exchange
#define EXCHANGE_SHFE "SHFE"           //# Shanghai Futures Exchange
#define EXCHANGE_CZCE "CZCE"           //# Zhengzhou Commodity Exchange
#define EXCHANGE_DCE "DCE"             //# Dalian Commodity Exchange
#define EXCHANGE_INE "INE"             //# Shanghai International Energy Exchange
#define EXCHANGE_SSE "SSE"             //# Shanghai Stock Exchange
#define EXCHANGE_SZSE "SZSE"           //# Shenzhen Stock Exchange
//#define EXCHANGE_SGE "SGE"             //# Shanghai Gold Exchange
//#define EXCHANGE_WXE "WXE"             //# Wuxi Steel Exchange

//     # Global
//     SMART = "SMART"         # Smart Router for US stocks
//     NYMEX = "NYMEX"         # New York Mercantile Exchange
//     GLOBEX = "GLOBEX"       # Globex of CME
//     IDEALPRO = "IDEALPRO"   # Forex ECN of Interactive Brokers
//     CME = "CME"             # Chicago Mercantile Exchange
//     ICE = "ICE"             # Intercontinental Exchange
//     SEHK = "SEHK"           # Stock Exchange of Hong Kong
//     HKFE = "HKFE"           # Hong Kong Futures Exchange
//     SGX = "SGX"             # Singapore Global Exchange
//     CBOT = "CBT"            # Chicago Board of Trade
//     DME = "DME"             # Dubai Mercantile Exchange
//     EUREX = "EUX"           # Eurex Exchange
//     APEX = "APEX"           # Asia Pacific Exchange
//     LME = "LME"             # London Metal Exchange
//     BMD = "BMD"             # Bursa Malaysia Derivatives
//     TOCOM = "TOCOM"         # Tokyo Commodity Exchange
//     EUNX = "EUNX"           # Euronext Exchange

//     # CryptoCurrency
//     BITMEX = "BITMEX"
//     OKEX = "OKEX"
//     HUOBI = "HUOBI"
//     BITFINEX = "BITFINEX"

//通用状态
#define STATUS_OFFLINE '0' //离线
#define STATUS_CONNECT '1' //连接中
#define STATUS_CONNECTED '2' //已连接
#define STATUS_LOGGED '3' //已登录

/////////////////////////////////////////////////////////////////////////
///交易状态类型
/////////////////////////////////////////////////////////////////////////
///开盘前
#define XMT_Status_BeforeTrading '0'
///非交易
#define XMT_Status_NoTrading '1'
///连续交易
#define XMT_Status_Continous '2'
///集合竞价报单
#define XMT_Status_AuctionOrdering '3'
///集合竞价价格平衡
#define XMT_Status_AuctionBalance '4'
///集合竞价撮合
#define XMT_Status_AuctionMatch '5'
///收盘
#define XMT_Status_Closed '6'

/////////////////////////////////////////////////////////////////////////
///进入交易状态原因类型
/////////////////////////////////////////////////////////////////////////
///自动切换
#define XMT_Enter_Reason_Automatic '1'
///手动切换
#define XMT_Enter_Reason_Manual '2'
///熔断
#define XMT_Enter_Reason_Fuse '3'

//数据深度
#define XMT_DATA_LEVEL 5


	enum
	{
		MARKET_DATE_CHANGED = 0X0001,
		/*MARKET_NAMETABLE_CHANGED = 0X0002,
		MARKET_NAMETABLEATTACH_CHANGED = 0X0004,
		MARKET_STATICDATA_CHANGED = 0X0008,
		MARKET_DKPHDATA_CHANGED = 0X0010,*/

		MARKET_TIME_CHANGED = 0X0020,
		MARKET_COMMODITY_PRICE_CHANGED = 0X0040,	//价变，量额不一定变
		MARKET_COMMODITY_VOLUME_CHANGED = 0X0080,	//成交量变，成交额肯定变
		MARKET_COMMODITY_AMOUNT_CHANGED = MARKET_COMMODITY_VOLUME_CHANGED,
		MARKET_OTHER_CHANGED = 0X0100,

		MARKET_COMMODITY_TICK_CHANGED = MARKET_COMMODITY_VOLUME_CHANGED,
		MARKET_COMMODITY_TREND_CHANGED = MARKET_COMMODITY_PRICE_CHANGED | MARKET_COMMODITY_VOLUME_CHANGED,
	};

//周期定义
enum PERIODTYPE
{
	CYC_TICK=0,
	CYC_5SEC, //10秒基数的秒周期
	CYC_ANYSEC,
	CYC_1MIN,
	CYC_5MIN,
	CYC_15MIN,
	CYC_30MIN,
	CYC_60MIN,
	CYC_ANYMIN,
	CYC_DAY,
	CYC_WEEK,
	CYC_MONTH,
	CYC_SEASON,
	CYC_YEAR,
	CYC_ANYDAY,
	CYC_MAX,
};

//除权方式定义
enum DWTYPE
{
	DW_NONE=0,	//不除权
	DW_BACKWARD,//向后	
	DW_FORWARD, //向前
	DW_DAY,		//指定日期
	DW_ALL,		//全程复权
	DW_MAX,
};

/** 
    * @brief 价格类型定义 
    */
enum PRICETYPE
{
	PRICE_OPEN = 0,		//开盘价格
	PRICE_HIGH,			//最高价格
	PRICE_LOW,			//最低价格
	PRICE_CLOSE,		//收盘价格
	//PRICE_MEDIAN,		//中间值（高+低）/2
	//PRICE_TYPICAL,		//典型价格（高+低+收盘价）/3
	//PRICE_WEIGHTED,		//平均价格（高+低+收盘价格+开盘价格）/4
	//PRICE_AMOUNT,		//成交额
	//PRICE_VOLUME,		//成交量
	//PRICE_TICKVOLUME,	//跳动量
	//PRICE_AVPRICE,		//平均价（成交额/成交量）
};

/** 
    * @brief 常用周期定义 
    */
#define SECOND_OF_PERIOD_TICK		    0
#define SECOND_OF_PERIOD_1SEC		    1
#define SECOND_OF_PERIOD_5SEC		    5
#define SECOND_OF_PERIOD_1MIN		    60
#define SECOND_OF_PERIOD_5MIN		    60*5
#define SECOND_OF_PERIOD_15MIN		    60*15
#define SECOND_OF_PERIOD_30MIN		    60*30
#define SECOND_OF_PERIOD_HOUR		    60*60
#define SECOND_OF_PERIOD_4HOUR		    60*60*4
#define SECOND_OF_PERIOD_DAY		    60*60*24
#define SECOND_OF_PERIOD_WEEK		    60*60*24*7
#define SECOND_OF_PERIOD_MONTH		    60*60*24*30
#define SECOND_OF_PERIOD_SEASON		    60*60*24*90
#define SECOND_OF_PERIOD_YEAR		    60*60*24*365

//User status.
#define USER_STATUS_OFFLINE '0' //离线
#define USER_STATUS_CONNECT '1' //连接中
#define USER_STATUS_CONNECTED '2' //已连接
#define USER_STATUS_LOGGED '3' //已登录

//Direction of order/trade/position.
#define DIRECTION_LONG '0'
#define DIRECTION_SHORT '1'
#define DIRECTION_NET '2'

//Offset of order/trade.
#define OFFSET_OPEN '0'
#define OFFSET_CLOSE '1'
#define OFFSET_CLOSETODAY '2'
#define OFFSET_CLOSEYESTERDAY '3'

//Order status.
#define ORDER_STATUS_SUBMITTING '0'
#define ORDER_STATUS_NOTTRADED '1'
#define ORDER_STATUS_PARTTRADED '2'
#define ORDER_STATUS_ALLTRADED '3'
#define ORDER_STATUS_CANCELLED '4'
#define ORDER_STATUS_REJECTED '5'

//Order type.
#define ORDER_LIMIT '0'
#define ORDER_MARKET '1'
#define ORDER_STOP '2'
#define ORDER_FAK '3' //立即成交剩余指令自动撤销指令(FAK指令)，指在限定价位下达指令，如果该指令下部分申报手数成交，该指令下剩余申报手数自动被系统撤销。
#define ORDER_FOK '4' //立即全部成交否则自动撤销指令(FOK指令)，指在限定价位下达指令，如果该指令下所有申报手数未能全部成交，该指令下所有申报手数自动被系统撤销。

//Option type.
#define OPTION_CALL '0'
#define OPTION_PUT '1'

//Currency.
#define CURRENCY_USD '0'
#define CURRENCY_HKD '1'
#define CURRENCY_CNY '2'

typedef struct tagStatusInfo
{
	char Name[MAX_NAME_LENGTH + 1]; //状态名称，分别是市场（CFFEX）、服务（zqsrv）、模块名（ctp）
	char Status; //STATUS_OFFLINE
}STATUSINFO, *PSTATUSINFO;

typedef struct tagModuleInfo
{
	char Name[MAX_NAME_LENGTH + 1]; //模块名称，模块名（ctp）
	//char Desc[1024];
}MODULEINFO, *PMODULEINFO;

typedef struct tagTradeTimeInfo
{
	uint8_t TradeTimeCount;
	uint16_t TradeTime[4][2];
}TRADETIMEINFO,*PTRADETIMEINFO;

typedef struct tagExchangeInfo
{
    char Exchange[MAX_EXCHANGE_LENGTH + 1];
	char Name[MAX_NAME_LENGTH + 1];
	union
	{
		TRADETIMEINFO TradeTimeInfo;
		struct
		{
			uint8_t TradeTimeCount; // 交易时段数量
			uint16_t TradeTime[4][2]; // 交易时段描述[最多只是4个交易时段，每个时段包括开始、结束时间，精确到分，HH * 60 + MM格式]
		};
	};
	uint8_t TradeTimeZone; // 该市场所处时区
	uint32_t TradeDay; // 当前交易日期[YYYYMMDD格式]
	uint32_t Date; // 市场日期[YYYYMMDD格式]
	uint32_t Time; // 市场时间[HHMMSS格式]
	//char Status[4]; // 市场状态，最多3级状态，Status[0]是业务状态，Status[1-3]是连接状态
	char Status; //市场状态
	char EnterReason; // 进入状态原因
}EXCHANGEINFO,*PEXCHANGEINFO;

typedef struct tagProductInfo
{
	char Exchange[MAX_EXCHANGE_LENGTH + 1];
	char Product[MAX_PRODUCT_LENGTH + 1];
	char Name[MAX_NAME_LENGTH + 1];
	union
	{
		TRADETIMEINFO TradeTimeInfo;
		struct
		{
			uint8_t TradeTimeCount; // 交易时段数量
			uint16_t TradeTime[4][2]; // 交易时段描述[最多只是4个交易时段，每个时段包括开始、结束时间，精确到分，HH * 60 + MM格式]
		};
	};
	char Status; // 市场交易状态
	char EnterReason; // 进入交易状态
} PRODUCTINFO,*PPRODUCTINFO;

typedef struct tagCodeInfo
{
		char Exchange[MAX_EXCHANGE_LENGTH + 1];
		char Product[MAX_PRODUCT_LENGTH + 1];
		char Code[MAX_CODE_LENGTH + 1];
		char Name[MAX_NAME_LENGTH + 1];
		uint32_t Date; // YYYYMMDD
		uint32_t Time; // 交易时间HHMMSS
		char Status; //市场交易状态
		char EnterReason; //进入交易状态
		double YClose; //昨收
		double Open;
		double High;
		double Low;
		double Close; // 交易价格，当然也是最新价，单位：元
		double Amount; // 总额
		double Volume; // 总成交量，单位：最小单位(股，张等)
}CODEINFO,*PCODEINFO;

 //明细结构
 typedef struct tagCodeTick
 {
	 uint32_t Time; // 交易时间HHMMSSsss
	 double Close; // 交易价格，当然也是最新价，单位：元，放大PriceRate倍
	 //double Amount; // 现额
	 double Volume; // 成交量(现量)，单位：最小单位(股，张等)
 } TICK,*PTICK;

 typedef struct tagCodeHistory
 {
	 uint32_t Date;
	 uint32_t Time;
	 double Open; // 开盘价，单位：元(针对大盘指数表示开盘指数)，放大PriceRate倍
	 double High; // 最高价，单位：元(针对大盘指数表示最高指数)，放大PriceRate倍
	 double Low; // 最低价，单位：元(针对大盘指数表示最低指数)，放大PriceRate倍
	 double Close; // 收盘价，单位：元(针对大盘指数表示收盘指数)，放大PriceRate倍
	 double Amount; // 成交金额，单位：元
	 double Volume; // 成交量，单位：最小单位(如股、张等)
 } KDATA,*PKDATA,BAR,*PBAR;

 ////

 //UserID 可以有多个InvestorID InvestorID跟投资相关
 //UserID 可以有多个AccountID AccountID跟资金相关

 ///投资者信息
 struct tagInvestorInfo
 {
	 char Broker[MAX_NAME_LENGTH + 1]; //经纪公司代码
	 char User[MAX_NAME_LENGTH + 1]; //用户代码
	 char Investor[MAX_NAME_LENGTH + 1]; //投资者代码
	 char InvestorName[MAX_NAME_LENGTH + 1]; //投资者名称
 };

 ///资金账户信息
 struct tagAccountInfo
 {
	 char Broker[MAX_NAME_LENGTH + 1]; //经纪公司代码
	 char User[MAX_NAME_LENGTH + 1]; //用户代码
	 char Account[MAX_NAME_LENGTH + 1]; //投资者帐号
 };

 ///用户信息
 struct tagUserInfo
 {
	 char Broker[MAX_NAME_LENGTH + 1]; //经纪公司代码
	 char User[MAX_NAME_LENGTH + 1]; //用户代码
	 char Status; //用户状态 USER_STATUS_OFFLINE
 };

 ///持仓信息
 struct tagPositionInfo
 {
	 char Broker[MAX_NAME_LENGTH + 1]; //经纪公司代码
	 char Investor[MAX_NAME_LENGTH + 1]; //投资者代码
	 char User[MAX_NAME_LENGTH + 1]; //用户代码
	 char Exchange[MAX_EXCHANGE_LENGTH + 1]; //市场
	 char Code[MAX_CODE_LENGTH + 1]; //代码
	 char Currency; //币种 CURRENCY_CNY
	 double Price; //价格
	 double Volume;  //量
	 char Direction; //持仓方向 DIRECTION_LONG
 };

 ///订单信息
 struct tagOrderInfo
 {
	 char Broker[MAX_NAME_LENGTH + 1]; //经纪公司代码
	 char Investor[MAX_NAME_LENGTH + 1]; //投资者代码
	 char User[MAX_NAME_LENGTH + 1]; //用户代码
	 char Order[MAX_NAME_LENGTH + 1]; //订单ID
	 char Exchange[MAX_EXCHANGE_LENGTH + 1]; //市场
	 char Code[MAX_CODE_LENGTH + 1]; //代码
	 char Currency; //币种 CURRENCY_CNY
	 double Price; //价格
	 double Volume; //量
	 double LeaveVolume; //剩余量
	 char Direction; //订单方向 DIRECTION_LONG
	 char Offset; //开平标志 OFFSET_OPEN
	 char Type; //订单类型 ORDER_LIMIT
	 char Status; //订单状态 ORDER_STATUS_SUBMITTING
 };

 ///成交信息
 struct tagTradeInfo
 {
	 char Broker[MAX_NAME_LENGTH + 1]; //经纪公司代码
	 char Investor[MAX_NAME_LENGTH + 1]; //投资者代码
	 char User[MAX_NAME_LENGTH + 1]; //用户代码
	 char Trade[MAX_NAME_LENGTH + 1]; //成交ID
	 char Exchange[MAX_EXCHANGE_LENGTH + 1]; //市场
	 char Code[MAX_CODE_LENGTH + 1]; //代码
	 char Currency; //币种 CURRENCY_CNY	
	 double Price; //价格
	 double Volume; //量
	 char Direction; //订单方向 DIRECTION_LONG
	 char Offset; //开平标志 OFFSET_OPEN
	 char Type; //成交类型 
 };

 ////

 typedef enum ZQDB_HANDLE_TYPE
 {
	 ZQDB_HANDLE_TYPE_UNKNOWN = 0,
	 ZQDB_HANDLE_TYPE_EXCHANGE,
	 ZQDB_HANDLE_TYPE_PRODUCT,
	 ZQDB_HANDLE_TYPE_CODE,
	 ZQDB_HANDLE_TYPE_MODULE,
	 ZQDB_HANDLE_TYPE_USER,
	 ZQDB_HANDLE_TYPE_INVESTOR,
	 ZQDB_HANDLE_TYPE_ACCOUNT,
	 ZQDB_HANDLE_TYPE_POSITION,
	 ZQDB_HANDLE_TYPE_ORDER,
	 ZQDB_HANDLE_TYPE_TRADE,
	 ZQDB_HANDLE_TYPE_CALCINPUT,
	 ZQDB_HANDLE_TYPE_CALCRESULT,
	 ZQDB_HANDLE_TYPE_CALCFUNC,
	 ZQDB_HANDLE_TYPE_CALCDATA,
	 ZQDB_HANDLE_TYPE_CALCULATOR,
	 ZQDB_HANDLE_TYPE_VIEW,
 }ZQDB_HANDLE_TYPE;

 typedef struct ZQDB_HANDLE
 {
	 ZQDB_HANDLE_TYPE type;
 }ZQDB_HANDLE;

 typedef struct ZQDB_HANDLE *HZQDB;

//自定义产品表信息
typedef struct tagSelfProductInfo
{
	char name[MAX_NAME_LENGTH + 1]; //名称
}SELFPRODUCTINFO,*PSELFPRODUCTINFO;

//自选表信息
typedef struct tagSelfselInfo
{
	char name[MAX_NAME_LENGTH + 1]; //名称
}SELFSELINFO,*PSELFSELINFO;

//API

//ZQDB_API_EXPORT bool ZQDBIsInvalidValue(double value);
//ZQDB_API_EXPORT bool ZQDBIsInvalidPos(size_t pos);

ZQDB_API_EXPORT bool ZQDBProductCombine(const char* exchange, const char* product, char* dst);
ZQDB_API_EXPORT bool ZQDBProductSplit(const char* src, char* exchange, char* product);

ZQDB_API_EXPORT bool ZQDBCodeCombine(const char* exchange, const char* code, char* dst);
ZQDB_API_EXPORT bool ZQDBCodeSplit(const char* src, char* exchange, char* code);

ZQDB_API_EXPORT const char* ZQDBCycle2Str(PERIODTYPE cycle);
ZQDB_API_EXPORT char* ZQDBStrCycle(char* str, PERIODTYPE cycle, size_t cycleex);

ZQDB_API_EXPORT int ZQDBCompareCycle(uint32_t olddate, uint32_t newdate, uint32_t oldtime, uint32_t newtime, PERIODTYPE cycle, size_t cycleex);

ZQDB_API_EXPORT bool ZQDBSameCycle(uint32_t olddate, uint32_t newdate, uint32_t oldtime, uint32_t newtime, PERIODTYPE cycle, size_t cycleex);
ZQDB_API_EXPORT bool ZQDBSameCycleIntraday(uint32_t oldtime, uint32_t newtime, PERIODTYPE cycle, size_t cycleex);

ZQDB_API_EXPORT size_t ZQDBGetAllStatusCount();
ZQDB_API_EXPORT size_t ZQDBGetAllStatus(STATUSINFO* status, size_t count);
ZQDB_API_EXPORT void ZQDBSetStatus(const char* name, char status);
ZQDB_API_EXPORT char ZQDBGetStatus(const char* name);

ZQDB_API_EXPORT MDB_STATUS ZQDBUpdateAllExchange();
ZQDB_API_EXPORT size_t ZQDBGetAllExchangeCount();
ZQDB_API_EXPORT size_t ZQDBGetAllExchange(HZQDB* exchange, size_t count);
ZQDB_API_EXPORT size_t ZQDBGetAllProductCount(HZQDB h);
ZQDB_API_EXPORT size_t ZQDBGetAllProduct(HZQDB h, HZQDB* product, size_t count);
ZQDB_API_EXPORT size_t ZQDBGetAllCodeCount(HZQDB h);
ZQDB_API_EXPORT size_t ZQDBGetAllCode(HZQDB h, HZQDB* code, size_t count);

//交易日切换清理标志
enum {
	CLEAR_MARKET_DATA = 0X0001,
	SAVE_MARKET_DATA = 0X0002,
	CLEAR_MARKET_HIS_DATA = 0X0010,
	DELETE_MARKET_HIS_DATA = 0X0020,
	CLEAR_TRADE_DATA = 0X0100,
	SAVE_TRADE_DATA = 0X0200,
	CLEAR_TRADE_HIS_DATA = 0X1000,
	DELETE_TRADE_HIS_DATA = 0X2000,
};
ZQDB_API_EXPORT MDB_STATUS ZQDBBeginNewTradingDay(const char* exchange, uint32_t flags);
//具体更新由对应模块自行实现，基本上是等待一下，先清理数据（Tick、M1、M5等历史数据），然后更新市场、产品、代码等快照数据
ZQDB_API_EXPORT void ZQDBDoNewTradingDayClear(const char* exchange, uint32_t flags);
ZQDB_API_EXPORT void ZQDBEndNewTradingDay(const char* exchange, uint32_t flags);

ZQDB_API_EXPORT HZQDB ZQDBUpdateOrInitExchange(EXCHANGEINFO* exchange);
ZQDB_API_EXPORT HZQDB ZQDBUpdateOrInitProduct(MDB_FIELD* field, size_t field_num, PRODUCTINFO* product, size_t max_count);
ZQDB_API_EXPORT HZQDB ZQDBUpdateOrInitCode(MDB_FIELD* field, size_t field_num, CODEINFO* code, size_t max_count);

ZQDB_API_EXPORT HZQDB ZQDBGetExchange(const char* exchange);
ZQDB_API_EXPORT HZQDB ZQDBGetProduct(const char* product);
ZQDB_API_EXPORT HZQDB ZQDBGetCode(const char* code);

ZQDB_API_EXPORT size_t ZQDBGetAllModuleCount();
ZQDB_API_EXPORT size_t ZQDBGetAllModule(HZQDB* module, size_t count);

ZQDB_API_EXPORT MDB_STATUS ZQDBBeginModuleNewTradingDay(HZQDB module, uint32_t flags);
//具体更新由对应模块自行实现，基本上是等待一下，先清理数据（委托、持仓、成交等数据），然后更新委托、持仓、成交等数据
ZQDB_API_EXPORT void ZQDBDoModuleNewTradingDayClear(HZQDB module, uint32_t flags);
ZQDB_API_EXPORT void ZQDBEndModuleNewTradingDay(HZQDB module, uint32_t flags);

ZQDB_API_EXPORT HZQDB ZQDBUpdateOrInitModule(MODULEINFO* module);

ZQDB_API_EXPORT HZQDB ZQDBUpdateOrInitUser(HZQDB module, MDB_FIELD* field, size_t field_num, tagUserInfo* user, size_t max_count);

ZQDB_API_EXPORT HZQDB ZQDBUpdateOrInitInvestor(HZQDB user, MDB_FIELD* field, size_t field_num, tagInvestorInfo* investor, size_t max_count);
ZQDB_API_EXPORT HZQDB ZQDBUpdateOrInitAccount(HZQDB user, MDB_FIELD* field, size_t field_num, tagAccountInfo* account, size_t max_count);
ZQDB_API_EXPORT HZQDB ZQDBUpdateOrInitPosition(HZQDB user, MDB_FIELD* field, size_t field_num, tagPositionInfo* position, size_t max_count);
ZQDB_API_EXPORT HZQDB ZQDBUpdateOrInitOrder(HZQDB user, MDB_FIELD* field, size_t field_num, tagOrderInfo* order, size_t max_count);
ZQDB_API_EXPORT HZQDB ZQDBUpdateOrInitTrade(HZQDB user, MDB_FIELD* field, size_t field_num, tagTradeInfo* trade, size_t max_count);

ZQDB_API_EXPORT size_t ZQDBGetAllUserCount();
ZQDB_API_EXPORT size_t ZQDBGetAllUser(HZQDB* user, size_t count);

ZQDB_API_EXPORT HZQDB ZQDBGetModule(const char* module);
ZQDB_API_EXPORT HZQDB ZQDBGetUser(const char* user);

ZQDB_API_EXPORT size_t ZQDBGetInvestorCount(HZQDB h);
ZQDB_API_EXPORT size_t ZQDBGetInvestor(HZQDB h, HZQDB* investor, size_t count);
ZQDB_API_EXPORT size_t ZQDBGetAccountCount(HZQDB h);
ZQDB_API_EXPORT size_t ZQDBGetAccount(HZQDB h, HZQDB* account, size_t count);
ZQDB_API_EXPORT size_t ZQDBGetPositionCount(HZQDB h);
ZQDB_API_EXPORT size_t ZQDBGetPosition(HZQDB h, HZQDB* position, size_t count);
ZQDB_API_EXPORT size_t ZQDBGetOrderCount(HZQDB h);
ZQDB_API_EXPORT size_t ZQDBGetOrder(HZQDB h, HZQDB* order, size_t count);
ZQDB_API_EXPORT size_t ZQDBGetTradeCount(HZQDB h);
ZQDB_API_EXPORT size_t ZQDBGetTrade(HZQDB h, HZQDB* trade, size_t count);

ZQDB_API_EXPORT bool ZQDBIsDisable(HZQDB h);

ZQDB_API_EXPORT HZQDB ZQDBGetParent(HZQDB h);
ZQDB_API_EXPORT void* ZQDBGetValue(HZQDB h);
ZQDB_API_EXPORT MDB_STATUS ZQDBNormalizeField(HZQDB h, ZQDB_HANDLE_TYPE child, MDB_FIELD* field, size_t field_num);
ZQDB_API_EXPORT MDB_STATUS ZQDBGetFieldValue(HZQDB h, MDB_FIELD* field, size_t field_num, char* data);
ZQDB_API_EXPORT MDB_STATUS ZQDBGetFieldAsInt(HZQDB h, MDB_FIELD* field, size_t field_num, ssize_t* value);
ZQDB_API_EXPORT MDB_STATUS ZQDBGetFieldAsDouble(HZQDB h, MDB_FIELD* field, size_t field_num, double* value);
ZQDB_API_EXPORT MDB_STATUS ZQDBGetFieldAsStr(HZQDB h, MDB_FIELD* field, size_t field_num, char** data, size_t* size, const char* format);

ZQDB_API_EXPORT void ZQDBUpdateMarketData(HZQDB h, MDB_FIELD* fields, size_t field_num, CODEINFO* data);

ZQDB_API_EXPORT void ZQDBAddTickData(HZQDB h, MDB_FIELD* field, size_t field_num, const TICK* data, size_t max_count);
ZQDB_API_EXPORT void ZQDBAddKData(HZQDB h, PERIODTYPE cycle, MDB_FIELD* field, size_t field_num, const KDATA* data, size_t max_count);
ZQDB_API_EXPORT void ZQDBUpdateKData(HZQDB h, PERIODTYPE cycle, MDB_FIELD* field, size_t field_num, const KDATA* data);

ZQDB_API_EXPORT size_t ZQDBGetDataAttrCount(HZQDB h, PERIODTYPE cycle, size_t cycleex);
ZQDB_API_EXPORT MDB_STATUS ZQDBGetDataAttr(HZQDB h, PERIODTYPE cycle, size_t cycleex, size_t attr_num, MDB_FIELD* attr);

ZQDB_API_EXPORT size_t ZQDBGetDataMaxCount(HZQDB h, PERIODTYPE cycle, size_t cycleex);
ZQDB_API_EXPORT size_t ZQDBGetDataCount(HZQDB h, PERIODTYPE cycle, size_t cycleex, size_t* elem_sz);
ZQDB_API_EXPORT MDB_STATUS ZQDBGetDataValue(HZQDB h, PERIODTYPE cycle, size_t cycleex, size_t pos, size_t* num, void* data);

ZQDB_API_EXPORT size_t ZQDBGetHisDataMaxCount(HZQDB h, PERIODTYPE cycle, size_t cycleex);
ZQDB_API_EXPORT size_t ZQDBGetHisDataCount(HZQDB h, PERIODTYPE cycle, size_t cycleex, size_t* elem_sz);
ZQDB_API_EXPORT MDB_STATUS ZQDBGetHisDataValue(HZQDB h, PERIODTYPE cycle, size_t cycleex, size_t pos, size_t* num, void* data);

enum {
	CLEAR_HIS_DATA = 0,
	DELETE_HIS_DATA = 0x01,
}; 
ZQDB_API_EXPORT void ZQDBClearHisData(HZQDB h, PERIODTYPE cycle, size_t flags);

//ZQDB_API_EXPORT void ZQDBGetAccount(HZQDB h, MDB_FIELD* field, size_t field_num, const char* data);
//ZQDB_API_EXPORT void ZQDBGetOrder(HZQDB h, MDB_FIELD* field, size_t field_num, const char* data, size_t max_count);
//ZQDB_API_EXPORT void ZQDBGetTrade(HZQDB h, MDB_FIELD* field, size_t field_num, const char* data, size_t max_count);
//ZQDB_API_EXPORT void ZQDBGetPosition(HZQDB h, MDB_FIELD* field, size_t field_num, const char* data, size_t max_count);
//

//ZQDB_API_EXPORT bool ZQDBIsSrv();
//static inline bool ZQDBIsApi() { return !ZQDBIsSrv(); }
ZQDB_API_EXPORT bool ZQDBIsIPC();
ZQDB_API_EXPORT bool ZQDBIsRPC();

ZQDB_API_EXPORT size_t ZQDBReqID(size_t type);
ZQDB_API_EXPORT size_t ZQDBReqType(size_t id);

ZQDB_API_EXPORT MDB_STATUS ZQDBSendMsg(NET_MSG* msg, NET_MSG** rsp, size_t timeout);
ZQDB_API_EXPORT MDB_STATUS ZQDBRequest(HZQDB h, NET_MSG* msg, NET_MSG** rsp, size_t timeout);
ZQDB_API_EXPORT MDB_STATUS ZQDBReqLogin(HZQDB h, NET_MSG* msg, NET_MSG** rsp, size_t timeout);
ZQDB_API_EXPORT MDB_STATUS ZQDBReqLogout(HZQDB h, NET_MSG* msg, NET_MSG** rsp, size_t timeout);
ZQDB_API_EXPORT MDB_STATUS ZQDBReqNewOrder(HZQDB h, NET_MSG* msg, NET_MSG** rsp, size_t timeout);
ZQDB_API_EXPORT MDB_STATUS ZQDBReqCancelOrder(HZQDB h, NET_MSG* msg, NET_MSG** rsp, size_t timeout);

typedef void(*ZQDB_NOTIFY_ENABLE_CB)(HZQDB h, void* data);
typedef void(*ZQDB_NOTIFY_DISABLE_CB)(HZQDB h, void* data);
typedef void(*ZQDB_NOTIFY_APPEND_CB)(HZQDB h, void* data);
typedef void(*ZQDB_NOTIFY_REMOVE_CB)(HZQDB h, void* data);
typedef void(*ZQDB_NOTIFY_UPDATE_CB)(HZQDB h, void* data);

struct ZQDB_NOTIFY_INF
{
	void* data;
	ZQDB_NOTIFY_ENABLE_CB enable_cb;
	ZQDB_NOTIFY_DISABLE_CB disable_cb;
	ZQDB_NOTIFY_APPEND_CB append_cb;
	ZQDB_NOTIFY_REMOVE_CB remove_cb;
	ZQDB_NOTIFY_UPDATE_CB update_cb;
};

ZQDB_API_EXPORT void ZQDBNotifyDispatch(const ZQDB_NOTIFY_INF& inf, HMDB hdb, HMTABLE htb, MDB_NOTIFY_DATA* notify);

typedef struct ZQDB_VIEW_INF
{
	int xmlflag;
	const char* xml;
}ZQDB_VIEW_INF;
ZQDB_API_EXPORT MDB_STATUS ZQDBCloseView(HZQDB h);
ZQDB_API_EXPORT HZQDB ZQDBOpenView(const char* name, ZQDB_VIEW_INF* inf);

#pragma pack(pop)

#if defined(__cplusplus)
}

#include <string>
#include <vector>

namespace zqdb {

	inline bool ProductCombine(const char* exchange, const char* product, char* dst) { return ZQDBProductCombine(exchange, product, dst); }
	inline bool ProductSplit(const char* src, char* exchange, char* product) { return ZQDBProductSplit(src, exchange, product); }

	inline bool CodeCombine(const char* exchange, const char* code, char* dst) { return ZQDBCodeCombine(exchange, code, dst); }
	inline bool CodeSplit(const char* src, char* exchange, char* code) { return ZQDBCodeSplit(src, exchange, code); }

	inline const char* Cycle2Str(PERIODTYPE cycle) { return ZQDBCycle2Str(cycle); }
	inline char* StrCycle(char* str, PERIODTYPE cycle, size_t cycleex = 0) { return ZQDBStrCycle(str, cycle, cycleex); }

	inline bool SameCycle(uint32_t olddate, uint32_t newdate, uint32_t oldtime, uint32_t newtime, PERIODTYPE cycle, size_t cycleex) { return ZQDBSameCycle(olddate, newdate, oldtime, newtime, cycle, cycleex); }
	inline bool SameCycleIntraday(uint32_t oldtime, uint32_t newtime, PERIODTYPE cycle, size_t cycleex) { return ZQDBSameCycleIntraday(oldtime, newtime, cycle, cycleex); }

	inline char GetStatus(const char* name) { return ZQDBGetStatus(name); }

	class AllStatus : public std::vector<STATUSINFO> {
	public:
		AllStatus() {
			std::vector<STATUSINFO>& vec = *this;
			auto vec_count = ZQDBGetAllStatusCount();
			if (vec_count > 0) {
				vec.resize(vec_count);
				vec.resize(ZQDBGetAllStatus(&vec[0], vec.size()));
			}
		}
	};
	class AllExchange : public std::vector<HZQDB> {
	public:
		AllExchange() {
			std::vector<HZQDB>& vec = *this;
			auto vec_count = ZQDBGetAllExchangeCount();
			if (vec_count > 0) {
				vec.resize(vec_count);
				vec.resize(ZQDBGetAllExchange(&vec[0], vec.size()));
			}
		}
	};
	class AllProduct : public std::vector<HZQDB> {
	public:
		AllProduct(HZQDB exchange = nullptr) {
			std::vector<HZQDB>& vec = *this;
			auto vec_count = ZQDBGetAllProductCount(exchange);
			if (vec_count > 0) {
				vec.resize(vec_count);
				vec.resize(ZQDBGetAllProduct(exchange, &vec[0], vec.size()));
			}
		}
	};
	class AllCode : public std::vector<HZQDB> {
	public:
		AllCode(HZQDB h = nullptr) {
			std::vector<HZQDB>& vec = *this;
			if (h) {
				switch (h->type)
				{
				case ZQDB_HANDLE_TYPE_EXCHANGE: {
					AllProduct products(h);
					for (auto product : products)
					{
						auto vec_count = ZQDBGetAllCodeCount(product);
						if (vec_count > 0) {
							auto old_count = vec.size();
							vec.resize(old_count + vec_count);
							vec.resize(old_count + ZQDBGetAllCode(product, &vec[old_count], vec_count));
						}
					}
				} break;
				case ZQDB_HANDLE_TYPE_PRODUCT: {
					auto vec_count = ZQDBGetAllCodeCount(h);
					if (vec_count > 0) {
						vec.resize(vec_count);
						vec.resize(ZQDBGetAllCode(h, &vec[0], vec.size()));
					}
				} break;
				case ZQDB_HANDLE_TYPE_CODE: {
					vec.emplace_back(h);
				} break;
				default:
					break;
				}
			}
			else {
				auto vec_count = ZQDBGetAllCodeCount(nullptr);
				if (vec_count > 0) {
					vec.resize(vec_count);
					vec.resize(ZQDBGetAllCode(nullptr, &vec[0], vec.size()));
				}
			}
		}
	};
	class AllModule : public std::vector<HZQDB> {
	public:
		AllModule() {
			std::vector<HZQDB>& vec = *this;
			auto vec_count = ZQDBGetAllModuleCount();
			if (vec_count > 0) {
				vec.resize(vec_count);
				vec.resize(ZQDBGetAllModule(&vec[0], vec.size()));
			}
		}
	};
	class AllUser : public std::vector<HZQDB> {
	public:
		AllUser(HZQDB module = nullptr) {
			std::vector<HZQDB>& vec = *this;
			auto vec_count = ZQDBGetAllUserCount();
			if (vec_count > 0) {
				vec.resize(vec_count);
				vec.resize(ZQDBGetAllUser(&vec[0], vec.size()));
			}
		}
	};
	class AllInvestor : public std::vector<HZQDB> {
	public:
		AllInvestor(HZQDB user = nullptr) {
			std::vector<HZQDB>& vec = *this;
			auto vec_count = ZQDBGetInvestorCount(user);
			if (vec_count > 0) {
				vec.resize(vec_count);
				vec.resize(ZQDBGetInvestor(user, &vec[0], vec.size()));
			}
		}
	};
	class AllAccount : public std::vector<HZQDB> {
	public:
		AllAccount(HZQDB user = nullptr) {
			std::vector<HZQDB>& vec = *this;
			auto vec_count = ZQDBGetAccountCount(user);
			if (vec_count > 0) {
				vec.resize(vec_count);
				vec.resize(ZQDBGetAccount(user, &vec[0], vec.size()));
			}
		}
	};
	class AllPosition : public std::vector<HZQDB> {
	public:
		AllPosition(HZQDB user = nullptr) {
			std::vector<HZQDB>& vec = *this;
			auto vec_count = ZQDBGetPositionCount(user);
			if (vec_count > 0) {
				vec.resize(vec_count);
				vec.resize(ZQDBGetPosition(user, &vec[0], vec.size()));
			}
		}
	};
	class AllOrder : public std::vector<HZQDB> {
	public:
		AllOrder(HZQDB user = nullptr) {
			std::vector<HZQDB>& vec = *this;
			auto vec_count = ZQDBGetOrderCount(user);
			if (vec_count > 0) {
				vec.resize(vec_count);
				vec.resize(ZQDBGetOrder(user, &vec[0], vec.size()));
			}
		}
	};
	class AllTrade : public std::vector<HZQDB> {
	public:
		AllTrade(HZQDB user = nullptr) {
			std::vector<HZQDB>& vec = *this;
			auto vec_count = ZQDBGetTradeCount(user);
			if (vec_count > 0) {
				vec.resize(vec_count);
				vec.resize(ZQDBGetTrade(user, &vec[0], vec.size()));
			}
		}
	};

	class Handle : public ZQDB_HANDLE
	{
	public:
		Handle(ZQDB_HANDLE_TYPE type) {
			this->type = type;
		}
	};

	class Object {
	protected:
		HZQDB h_;
	public:
		Object(HZQDB h = nullptr) :h_(h) {}
		~Object() { }

		inline operator HZQDB() const { return h_; }

		inline HZQDB GetParent() { return ZQDBGetParent(h_); }
		inline void* GetValue() { return ZQDBGetValue(h_); }
		//inline const char* GetCode() { return ZQDBGetCode(h_); }

		inline MDB_STATUS NormalizeField(MDB_FIELD* field, size_t field_num, ZQDB_HANDLE_TYPE child = ZQDB_HANDLE_TYPE_UNKNOWN) { return ZQDBNormalizeField(h_, child, field, field_num); }
		inline MDB_STATUS GetFieldValue(MDB_FIELD* field, size_t field_num, char* data) { return ZQDBGetFieldValue(h_, field, field_num, data); }
		inline MDB_STATUS GetFieldAsInt(MDB_FIELD* field, size_t field_num, ssize_t* value) { return ZQDBGetFieldAsInt(h_, field, field_num, value); }
		inline MDB_STATUS GetFieldAsDouble(MDB_FIELD* field, size_t field_num, double* value) { return ZQDBGetFieldAsDouble(h_, field, field_num, value); }
		inline MDB_STATUS GetFieldAsStr(MDB_FIELD* field, size_t field_num, char** data, size_t* size, const char* format = nullptr) { return ZQDBGetFieldAsStr(h_, field, field_num, data, size, format); }
		inline MDB_STATUS NormalizeField(MDB_FIELD& field, ZQDB_HANDLE_TYPE child = ZQDB_HANDLE_TYPE_UNKNOWN) {
			return NormalizeField(&field, 1, child);
		}
		inline MDB_STATUS GetFieldValue(MDB_FIELD& field, char* data) {
			return GetFieldValue(&field, 1, data);
		}
		inline ssize_t GetFieldAsInt(MDB_FIELD& field, ssize_t def = 0) {
			ssize_t val = 0;
			if (MDB_STATUS_OK == GetFieldAsInt(&field, 1, &val)) {
				return val;
			}
			return def;
		}
		inline double GetFieldAsDouble(MDB_FIELD& field, double def = INVALID_VALUE) {
			double val = 0;
			if (MDB_STATUS_OK == GetFieldAsDouble(&field, 1, &val)) {
				return val;
			}
			return def;
		}
		inline const char* GetFieldAsStr(MDB_FIELD& field, char* str, size_t len, const char* fmt = nullptr) {
			if (MDB_STATUS_OK == GetFieldAsStr(&field, 1, &str, &len, fmt)) {
				return str;
			}
			return str;
		}
	};

	template<class TInfo>
	class ObjectT : public Object {
	public:
		using Object::Object;

		inline operator TInfo*()// const
		{ 
			return (TInfo*)(this->GetValue()); 
		}

		inline TInfo *operator->()// const
		{	// return pointer to resource
			return (TInfo*)(this->GetValue());
		}
	};

	class Exchange : public Object {
		typedef Object Base;
	public:
		Exchange(HZQDB h = nullptr) :Base(h) {}
		Exchange(const char* exchange) :Base(ZQDBGetExchange(exchange)) {}
		~Exchange() { }

		EXCHANGEINFO* GetValue() { return (EXCHANGEINFO*)Base::GetValue(); }
	};

	class Product : public Object {
		typedef Object Base;
	public:
		Product(HZQDB h = nullptr) :Base(h) {}
		Product(const char* product) :Base(ZQDBGetProduct(product)) {}
		~Product() { }

		HZQDB GetExchange() { return ZQDBGetParent(h_); }
		tagProductInfo* GetValue() { return (tagProductInfo*)ZQDBGetValue(h_); }
	};

	class Code : public ObjectT<tagCodeInfo> {
		typedef ObjectT<tagCodeInfo> Base;
	public:
		Code(HZQDB h = nullptr) :Base(h) {}
		Code(const char* code) :Base(ZQDBGetCode(code)) {}
		~Code() { }

		HZQDB GetExchange() { return ZQDBGetParent(ZQDBGetParent(h_)); }
		HZQDB GetProduct() { return ZQDBGetParent(h_); }
		tagCodeInfo* GetValue() { return (tagCodeInfo*)ZQDBGetValue(h_); }
		
		void UpdateMarketData(MDB_FIELD* fields, size_t field_num, CODEINFO* data) { ZQDBUpdateMarketData(h_, fields, field_num, data); }

		void AddTickData(MDB_FIELD* field, size_t field_num, const TICK* data, size_t max_count) { ZQDBAddTickData(h_, field, field_num, data, max_count); }
		void AddKData(PERIODTYPE cycle, MDB_FIELD* field, size_t field_num, const KDATA* data, size_t max_count) { ZQDBAddKData(h_, cycle, field, field_num, data, max_count); }
		void UpdateKData(PERIODTYPE cycle, MDB_FIELD* field, size_t field_num, const KDATA* data) { ZQDBUpdateKData(h_, cycle, field, field_num, data); }

		size_t GetDataMaxCount(PERIODTYPE cycle, size_t cycleex) { return ZQDBGetDataMaxCount(h_, cycle, cycleex); }
		size_t GetDataCount(PERIODTYPE cycle, size_t cycleex, size_t* elem_sz = nullptr) { return ZQDBGetDataCount(h_, cycle, cycleex, elem_sz); }
		MDB_STATUS GetDataValue(PERIODTYPE cycle, size_t cycleex, size_t pos, size_t* num, void* data) { return ZQDBGetDataValue(h_, cycle, cycleex, pos, num, data); }
		
		size_t GetHisDataMaxCount(PERIODTYPE cycle, size_t cycleex) { return ZQDBGetHisDataMaxCount(h_, cycle, cycleex); }
		size_t GetHisDataCount(PERIODTYPE cycle, size_t cycleex, size_t* elem_sz = nullptr) { return ZQDBGetHisDataCount(h_, cycle, cycleex, elem_sz); }
		MDB_STATUS GetHisDataValue(PERIODTYPE cycle, size_t cycleex, size_t pos, size_t* num, void* data) { return ZQDBGetHisDataValue(h_, cycle, cycleex, pos, num, data); }
	};

	class Msg : public net::Msg
	{
		typedef net::Msg Base;
	public:	
		using Base::Base;

		inline NET_STATUS SetReqID(size_t req_type) { return SetID(ZQDBReqID(req_type)); }
		inline size_t GetReqID() { return GetID(); }
		inline size_t GetReqType() const { return ZQDBReqType(GetID()); }
	};

	template<class T>
	class INotifyT
	{
	public:
		void OnNotifyEnable(HZQDB h) { }
		void OnNotifyDisable(HZQDB h) { }
		void OnNotifyAppend(HZQDB h) { }
		void OnNotifyRemove(HZQDB h) { }
		void OnNotifyUpdate(HZQDB h) { }
		void OnNotify(HMDB hdb, HMTABLE htb, MDB_NOTIFY_DATA* notify)
		{
			const ZQDB_NOTIFY_INF inf = { static_cast<T*>(this)
				, [](HZQDB h, void* data) { ((T*)data)->OnNotifyEnable(h); }
				, [](HZQDB h, void* data) { ((T*)data)->OnNotifyDisable(h); }
				, [](HZQDB h, void* data) { ((T*)data)->OnNotifyAppend(h); }
				, [](HZQDB h, void* data) { ((T*)data)->OnNotifyRemove(h); }
				, [](HZQDB h, void* data) { ((T*)data)->OnNotifyUpdate(h); }
			};
			ZQDBNotifyDispatch(inf, hdb, htb, notify);
		}
	};

	class Base {
	private:
		/*static void CB_LOG(int level, const char* logstr)
		{
			T::Inst().OnLog(level, logstr);
		}

		static void CB_STATUS(HNNODE h, NET_NODE_STATUS status)
		{
			T::Inst().OnStatus(h, status);
		}

		static int CB_MSG(HNMSG hmsg)
		{
			return T::Inst().OnMsg(hmsg);
		}

		static void CB_NOTIFY(HMDB hdb, HMTABLE htb, MDB_NOTIFY_DATA* notify)
		{
			T::Inst().OnNotify(hdb, htb, notify);
		}*/
	public:
		Base() {
		}
		~Base() {

		}
	};
}

#endif//

#endif//_H_ZQDB_H_