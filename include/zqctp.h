#pragma once

#ifndef _H_ZQCTP_H_
#define _H_ZQCTP_H_

#include <zq.h>
#include <ThostFtdcTraderApi.h>
#include <ThostFtdcMdApi.h>

#if defined(__cplusplus)
extern "C" {
#endif//

#pragma pack(push, 1)

	//

#pragma pack(pop)

inline const char STATUS_CTP2VT(const char status) {
	switch (status)
	{
	case THOST_FTDC_OAS_Submitted:
		return ORDER_STATUS_SUBMITTING;
		break;
	case THOST_FTDC_OAS_Accepted:
		return ORDER_STATUS_SUBMITTING;
		break;
	case THOST_FTDC_OAS_Rejected:
		return ORDER_STATUS_REJECTED;
		break;
	case THOST_FTDC_OST_NoTradeQueueing:
		return ORDER_STATUS_PARTTRADED;
		break;
	case THOST_FTDC_OST_PartTradedQueueing:
		return ORDER_STATUS_ALLTRADED;
		break;
	case THOST_FTDC_OST_Canceled:
		return ORDER_STATUS_CANCELLED;
		break;
	default:
		break;
	}
	return 0;
}

inline const char DIRECTION_VT2CTP(const char direction) {
	switch (direction)
	{
	case DIRECTION_LONG:
		return THOST_FTDC_D_Buy;
	case DIRECTION_SHORT:
		return THOST_FTDC_D_Sell;
	}
	return 0;
}
inline const char DIRECTION_CTP2VT(const char direction) {
	switch (direction)
	{
	case THOST_FTDC_D_Buy:
		return DIRECTION_LONG;
	case THOST_FTDC_D_Sell:
		return DIRECTION_SHORT;
	}
	return 0;
}
inline const char PDIRECTION_VT2CTP(const char direction) {
	switch (direction)
	{
	case DIRECTION_LONG:
		return THOST_FTDC_PD_Long;
	case DIRECTION_SHORT:
		return THOST_FTDC_PD_Short;
	case DIRECTION_NET:
		return THOST_FTDC_PD_Net;
	}
	return 0;
}
inline const char PDIRECTION_CTP2VT(const char direction) {
	switch (direction)
	{
	case THOST_FTDC_PD_Long:
		return DIRECTION_LONG;
	case THOST_FTDC_PD_Short:
		return DIRECTION_SHORT;
	case THOST_FTDC_PD_Net:
		return DIRECTION_NET;
	}
	return 0;
}

inline const char ORDERTYPE_VT2CTP(const char type) {
	switch (type)
	{
	case ORDER_LIMIT:
		return THOST_FTDC_OPT_LimitPrice;
	case ORDER_MARKET:
		return THOST_FTDC_OPT_AnyPrice;
	default:
		break;
	}
	return 0;
}
inline const char ORDERTYPE_CTP2VT(const char type) {
	switch (type)
	{
	case THOST_FTDC_OPT_LimitPrice:
		return ORDER_LIMIT;
	case THOST_FTDC_OPT_AnyPrice:
		return ORDER_MARKET;
	default:
		break;
	}
	return 0;
}

inline const char OFFSET_VT2CTP(const char offset) {
	switch (offset)
	{
	case OFFSET_OPEN:
		return THOST_FTDC_OF_Open;
	case OFFSET_CLOSE:
		return THOST_FTDC_OFEN_Close;
	case OFFSET_CLOSETODAY:
		return THOST_FTDC_OFEN_CloseToday;
	case OFFSET_CLOSEYESTERDAY:
		return THOST_FTDC_OFEN_CloseYesterday;
	default:
		break;
	}
	return 0;
}
inline const char OFFSET_CTP2VT(const char offset) {
	switch (offset)
	{
	case THOST_FTDC_OF_Open:
		return OFFSET_OPEN;
	case THOST_FTDC_OFEN_Close:
		return OFFSET_CLOSE;
	case THOST_FTDC_OFEN_CloseToday:
		return OFFSET_CLOSETODAY;
	case THOST_FTDC_OFEN_CloseYesterday:
		return OFFSET_CLOSEYESTERDAY;
	default:
		break;
	}
	return 0;
}

// EXCHANGE_CTP2VT = {
//     "CFFEX": Exchange.CFFEX,
//     "SHFE": Exchange.SHFE,
//     "CZCE": Exchange.CZCE,
//     "DCE": Exchange.DCE,
//     "INE": Exchange.INE
// }

//inline const char PRODUCT_CTP2VT(const char type) {
//	switch (type)
//	{
//	case THOST_FTDC_PC_Futures:
//		return PRODUCT_FUTURES;
//	case THOST_FTDC_PC_Options:
//		return PRODUCT_OPTION;
//	case THOST_FTDC_PC_Combination:
//		return PRODUCT_SPREAD;
//	case THOST_FTDC_PC_Spot:
//		return PRODUCT_SPOT;
//	case THOST_FTDC_PC_EFP:
//		return PRODUCT_SPOT;
//	case THOST_FTDC_PC_SpotOption:
//		return PRODUCT_OPTION;
//#ifdef CTP_SOOPT
//	case THOST_FTDC_PC_ETFOption:
//		return PRODUCT_OPTION;
//	case THOST_FTDC_PC_Stock:
//		return PRODUCT_STOCK;
//#endif//
//	default:
//		break;
//	}
//	return 0;
//}

inline const char OPTIONTYPE_CTP2VT(const char type) {
	switch (type)
	{
	case THOST_FTDC_CP_CallOptions:
		return OPTION_CALL;
	case THOST_FTDC_CP_PutOptions:
		return OPTION_PUT;
	default:
		break;
	}
	return 0;
}

#if defined(__cplusplus)
}

#endif//

#endif//_H_ZQCTP_H_