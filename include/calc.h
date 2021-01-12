#pragma once

#ifndef _H_ZQDB_CALC_H_
#define _H_ZQDB_CALC_H_

#include <zq.h>

#if defined(LIB_CALC_API) && defined(WIN32)
#ifdef LIB_CALC_API_EXPORT
#define CALC_API_EXPORT __declspec(dllexport)
#else
#define CALC_API_EXPORT __declspec(dllimport)
#endif
#else
#define CALC_API_EXPORT 
#endif

#if defined(__cplusplus)
extern "C" {
#endif//

#pragma pack(push, 1)

//#define MAX_INDICATOR_INPUT 32
//#define MAX_INDICATOR_REFLINE 16
//#define MAX_INDICATOR_INDEX	16
//
////这里的参数会影响计算结果
//typedef struct tagInputValue
//{
//	char name[MAX_NAME_LENGTH + 1];
//	union
//	{
//		double dvalue;
//		char value[8];
//	};
//	uint64_t number:1;
//	uint64_t digits:4;
//	uint64_t visible:1;
//}INPUTVALUE,*PINPUTVALUE;
//
////REFLINE
//typedef struct tagRefline
//{
//	char name[MAX_NAME_LENGTH + 1];
//	uint32_t type:4;				//ENUM_REFLINE_TYPE
//	uint32_t width:4;			//线宽
//	uint32_t style:4;			//INDICATOR_LINE_STYLE
//	uint32_t digits:4;			//指标精度,如果指定了精度就不使用商品的digits
//	uint32_t color;			//指标颜色
//	double dvalue;			//参考值
//}REFLINE,*PREFLINE;
//
//typedef struct tagIndexInfo
//{
//	char name[MAX_NAME_LENGTH + 1];
//	uint32_t type:4;				//INDICATOR_INDEX_TYPE
//	uint32_t shift:8;			//指标偏移
//	uint32_t begin:8;			//指标开始
//	uint32_t draw:4;				//INDICATOR_DRAW_TYPE
//	uint32_t line:4;				//INDICATOR_LINE_TYPE
//	uint32_t next:4;				//画线关联指标线
//	uint32_t :0;
//	uint32_t digits:4;			//指标精度,如果指定了精度就不使用商品的digits
//	uint32_t width:4;			//线宽
//	uint32_t color;			//指标颜色
//	//double min_value;		//指标最大值
//	//double max_value;		//指标最小值
//	//double base_value;	//基准值
//	//int level_count:8;	//水平线数目
//	//int level_style:24;	//水平线风格
//	//COLORREF level_color;	//水平线颜色
//}INDEXINFO,*PINDEXINFO;
//
//typedef struct tagIndicatorInfo
//{
//		char name[MAX_NAME_LENGTH + 1]; //名称
//		uint32_t type:3;
//		uint32_t style:9; //风格
//		uint32_t shift:7; //指标偏移
//		uint32_t intcount:5; //输入参数数目
//		uint32_t refcount:4; //参考线数目
//		uint32_t idxcount:4; //index个数
//		double refline[MAX_INDICATOR_REFLINE]; //参考线
//		INPUTVALUE input[MAX_INDICATOR_INPUT]; //输入参数
//		INDEXINFO index[MAX_INDICATOR_INDEX]; //index信息
//}INDICATORINFO,*PINDICATORINFO;

static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_SHIFT = "SHIFT"; //uint8_t 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_REFLINE_COUNT = "RFLCNT"; //uint8_t 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_REFLINE1 = "RFL1"; //double 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_REFLINE2 = "RFL2"; //double 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_REFLINE3 = "RFL3"; //double 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_REFLINE4 = "RFL4"; //double 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_REFLINE5 = "RFL5"; //double 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_REFLINE6 = "RFL6"; //double 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_REFLINE7 = "RFL7"; //double 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_RESULT_TYPE = "RLTTYPE"; //uint8_t 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_RESULT_SHIFT = "RLTSHFT"; //uint8_t 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_RESULT_BEGIN = "RLTBEGN"; //uint8_t 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_RESULT_DRAW_TYPE = "RLTDRAW"; //uint8_t 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_RESULT_LINE_TYPE = "RLTLINE"; //uint8_t 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_RESULT_NEXT = "RLTNEXT"; //uint8_t 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_RESULT_DIGITS = "RLTDGIT"; //uint8_t 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_RESULT_WIDTH = "RLTWDTH"; //uint8_t 类型
static const MDB_FIELD_NAME_ID STR_ZQDB_FIELD_CALC_RESULT_COLOR = "RLTCLR"; //uint8_t 类型

//static const MDB_FIELD_ID ZQDB_FIELD_CALC_TYPE = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_CALC_TYPE; //uint32_t 类型
//static const MDB_FIELD_ID ZQDB_FIELD_CALC_RESULT_TYPE = *(MDB_FIELD_ID*)STR_ZQDB_FIELD_CALC_RESULT_TYPE; //uint8_t 类型

/**
* @brief 计算类型
*/
enum CALC_TYPE
{
	CALC_MAJOR,	//主
	CALC_MINOR,	//副
};
#define CALC_TYPE_MAX 2

/**
* @brief 线类型
*/
enum CALC_RESULT_TYPE
{
	CALC_RESULT_LINE,		//线
	CALC_RESULT_CALC,		//辅助计算
};
#define CALC_RESULT_TYPE_MAX 2

//---------------------------------------------------------------------------
// 图表显示定义
//---------------------------------------------------------------------------

#ifndef CLR_NONE
#define CLR_NONE                0xFFFFFFFFL
#endif//CLR_NONE

/**
* @brief 显示坐标定义
*/
enum CALC_COORD_TYPE
{
	CALC_COORD_NORMAL = 0,		//普通等分坐标
	CALC_COORD_LOG = 0x01,		//对数坐标
	CALC_COORD_MASK1 = 0X0F,
	CALC_COORD_DIFF = 0x10,		//等差坐标(三等分坐标)
	CALC_COORD_SCALE = 0x20,	//等比坐标
	CALC_COORD_PERCENT = 0x40,	//百分比坐标
	CALC_COORD_GOLDEN = 0x80,	//黄金分割
	CALC_COORD_MASK2 = 0X0F,
};

/**
* @brief 指标线绘制方式
*/
enum CALC_DRAW_TYPE
{
	CALC_DRAW_LINE,				//画线
	CALC_DRAW_SECTION,			//线条
	CALC_DRAW_STICK,				//基于基线画柱状线
	CALC_DRAW_HISTOGRAM,			//基于基线画柱状图
	CALC_DRAW_HISTOGRAM2,		//两个指标命令的柱状图
	CALC_DRAW_HISTOGRAM3,		//三个指标命令的柱状图
	CALC_DRAW_HISTOGRAM4,		//四个指标命令的柱状图
	CALC_DRAW_CANDLES,			//以蜡烛台序列展示
	CALC_DRAW_ARROW,				//画箭头
	CALC_DRAW_FILLING,			//两层间的颜色
	CALC_DRAW_ZIGZAG,			//之字设计允许垂直界面
	CALC_DRAW_BARS,				//以字符序列展示

	CALC_DRAW_COLOR_LINE,		//多色线
	CALC_DRAW_COLOR_SECTION,		//多色分段
	CALC_DRAW_COLOR_HISTOGRAM,	//基于基线的多色直方图
	CALC_DRAW_COLOR_HISTOGRAM2,	//两个指标缓冲区的多色直方图
	CALC_DRAW_COLOR_CANDLES,		//多色蜡烛台
	CALC_DRAW_COLOR_ARROW,		//描绘多色箭头
	CALC_DRAW_COLOR_ZIGZAG,		//多色之字形
	CALC_DRAW_COLOR_BARS,		//多色字节
};
#define CALC_DRAW_TYPE_MAX 20

/**
* @brief 指标线线型
*/
enum CALC_LINE_TYPE
{
	CALC_LINE_SOLID,		//实线
	CALC_LINE_DASH,			//折线
	CALC_LINE_DOT,			//虚线
	CALC_LINE_DASHDOT,		//折点线
	CALC_LINE_DASHDOTDOT,	//双折点线
};
#define CALC_LINE_TYPE_MAX 5

/**
* @brief K线显示方式
*/
enum CALC_KLINE_TYPE
{
	CALC_KLINE_K = 0,	//K线
	CALC_KLINE_BAR,		//美国线
	CALC_KLINE_TREND,	//走势线
};
#define CALC_KLINE_TYPE_MAX 3

/**
* @brief 柱状线显示方式
*/
enum CALC_STICK_TYPE
{
	CALC_STICK_LINE = 0,	//柱状线
	CALC_STICK_BAR,			//柱状图
};
#define CALC_STICK_TYPE_MAX 2

/**
* @brief 柱状图显示方式
*/
enum CALC_HISTOGRAM_TYPE
{
	CALC_HISTOGRAM_LINE = 0,//柱状线
	CALC_HISTOGRAM_BAR,		//美国线
	CALC_HISTOGRAM_K,		//K线
};
#define CALC_HISTOGRAM_TYPE_MAX 3

//---------------------------------------------------------------------------
// 画线指标定义
//---------------------------------------------------------------------------

enum CALC_DRAWLINE_TYPE
{
	CALC_DRAWLINE_NONE = 0,	//
	CALC_DRAWLINE_DDLINE,			// 两点直线
	CALC_DRAWLINE_DOTDOT,			// 两点线段
	CALC_DRAWLINE_PLINE,				// 平行线
	CALC_DRAWLINE_PERLINE,			// 百分比线
	CALC_DRAWLINE_RADIALLINE,		// 两点射线
	CALC_DRAWLINE_BARLINE,			// 栅形线
	//UGUNSLINE,			// 上甘氏线
	//DGUNSLINE,			// 下甘氏线
	CALC_DRAWLINE_GUNSLINE,			// 甘氏线
	CALC_DRAWLINE_GOLDLINE,			// 黄金分割线
	CALC_DRAWLINE_BLACKHOLE,			// 神秘黑洞
	CALC_DRAWLINE_ELLIPSEARC,		// 圆弧
	CALC_DRAWLINE_MULTIARC,			// 多圆弧
	CALC_DRAWLINE_GOLDARC,			// 弧度黄金线
	CALC_DRAWLINE_CIRCLE,			// 圆
	CALC_DRAWLINE_MULTIELLIPSE,		// 多圆
	CALC_DRAWLINE_CYCLELINE,			// 周期线
	CALC_DRAWLINE_FABCYCLELINE,		// 费波纳契周期线
	CALC_DRAWLINE_SUZULINE,			// 速阻线
	CALC_DRAWLINE_SINLINE,			// 正弦线
	CALC_DRAWLINE_RECTANGLE,			// 矩形
	CALC_DRAWLINE_TEXT,				// 文字
	CALC_DRAWLINE_UPARROW,			// 上箭头
	CALC_DRAWLINE_DOWNARROW,			// 下箭头
	CALC_DRAWLINE_STOCKBOX,			// 箱体线
	CALC_DRAWLINE_REGRESSLINE,		// 线性回归
	CALC_DRAWLINE_REGRESSZONE,		// 线性回归带
	CALC_DRAWLINE_REGRESSCHANNEL,	// 回归通道
};
#define CALC_DRAWLINE_TYPE_MAX 27

typedef struct tagCalcPoint
{
	double dvalue;			//y值
	unsigned long date;		//x日期
	unsigned long time;		//x时间
							//char lable[32];
}TCALCPOINT, *PCALCPOINT;

typedef struct tagCalcData
{
	HZQDB h;
	PERIODTYPE cycle;
	size_t cycleex;
}CALCDATA, *PCALCDATA;

typedef struct tagCalcResult
{
	int data_count;
	int data_counted;
	int result_count;
	double** result_data;
}CALCRESULT, *PCALCRESULT;

////CODE表保存CODE的明细、K线等信息，即CODE下有哪些数据，明细和K线可以当指标处理
//typedef struct tagCodeValue
//{
//		uint8_t cycle; // 周期
//		union
//		{
//			tagIndicatorInfo indicator; //指标
//			char reserved[2048]; //
//		};
//}CODEVALUE,*PCODEVALUE;

////策略表信息
//typedef struct tagStrategyInfo
//{
//	char name[MAX_NAME_LENGTH + 1]; //名称
//}STRATEGYINFO,*PSTRATEGYINFO;

/**
* @brief 价格类型定义
*/
/*enum PRICE_TYPE
{
	PRICE_CLOSE,		//收盘价格
	PRICE_OPEN,			//开盘价格
	PRICE_HIGH,			//最高价格
	PRICE_LOW,			//最低价格
	PRICE_MEDIAN,		//中间值（高+低）/2
	PRICE_TYPICAL,		//典型价格（高+低+收盘价）/3
	PRICE_WEIGHTED,		//平均价格（高+低+收盘价格+开盘价格）/4
	PRICE_AMOUNT,		//成交额
	PRICE_VOLUME,		//成交量
	PRICE_TICKVOLUME,	//跳动量
	PRICE_AVPRICE,		//平均价（成交额/成交量）
};*/

/**
* @brief MA计算方法
*/
enum CALC_MA_METHOD
{
	CALC_MODE_SMA,	//简单移动平均线 (SMA)：Simple Moving Average
	CALC_MODE_EMA,	//指数移动平均线 (EMA)：Exponential MA
	CALC_MODE_SMMA,	//通畅移动平均线 (SMMA)：Smoothed MA
	CALC_MODE_LWMA,	//线性权数移动平均线 (LWMA)：Linear Weighted MA
	CALC_MODE_DMA,	//动态移动平均线 Dynamic Moving Average
	//CALC_MDOE_TMA, //递归移动平均
	//CALC_MODE_WMA, //加权移动平均
};

/**
* @brief 最大值最小值定义
* CalculatorEx指标的MMX数量和结果/Line数量一致
* K有开高低收4个MMX,分别是开高低收，对应Line是0,1,2,3
* AMOUNT有一个MMX
* VOLUME有一个MMX
* K/AMOUNT/VOLUME是特殊的CalculatorEx
* 综上所述，也就是每一条Line都有对应的MIN,MAX
*/
enum CALC_MMX_TYPE
{
	CALC_MMX_MIN = 0,
	CALC_MMX_MAX,
};
#define CALC_MMX_TYPE_MAX 2

//API

typedef MDB_STATUS(*ZQDB_CALC_FN)(HZQDB h, CALCDATA* data, CALCRESULT* result);

CALC_API_EXPORT MDB_STATUS ZQDBCloseCalcInput(HZQDB h);
CALC_API_EXPORT HZQDB ZQDBOpenCalcInput(MDB_FIELD* field, size_t field_num, char* data);
CALC_API_EXPORT HZQDB ZQDBCloneCalcInput(HZQDB h);

CALC_API_EXPORT MDB_STATUS ZQDBCloseCalcResult(HZQDB h);
CALC_API_EXPORT HZQDB ZQDBOpenCalcResult(MDB_FIELD* field, size_t field_num, char* data);
CALC_API_EXPORT HZQDB ZQDBCloneCalcResult(HZQDB h, size_t index);

CALC_API_EXPORT HZQDB ZQDBRegCalcFunc(const char* name, CALC_TYPE type, ZQDB_CALC_FN calc, HZQDB input, HZQDB* results, size_t result_num);
CALC_API_EXPORT HZQDB ZQDBGetCalcFunc(const char* name);

CALC_API_EXPORT const char* ZQDBGetCalcName(HZQDB h);
CALC_API_EXPORT CALC_TYPE ZQDBGetCalcType(HZQDB h);

//CALC_TYPE_MAX返回所有
CALC_API_EXPORT size_t ZQDBGetAllCalcFuncCount(size_t type);
CALC_API_EXPORT size_t ZQDBGetAllCalcFunc(size_t type, HZQDB* h, size_t count);

CALC_API_EXPORT MDB_STATUS ZQDBCloseCalcData(HZQDB data);
CALC_API_EXPORT HZQDB ZQDBOpenCalcData(HZQDB code, PERIODTYPE cycle, size_t cycleex);

CALC_API_EXPORT MDB_STATUS ZQDBCloseCalculator(HZQDB calc);
CALC_API_EXPORT HZQDB ZQDBOpenCalculator(const char* name, HZQDB data, HZQDB input);

CALC_API_EXPORT size_t ZQDBGetCalcInputAttrCount(HZQDB h);
//CALC_API_EXPORT MDB_STATUS ZQDBGetCalcInputAttr(HZQDB h, size_t pos, MDB_FIELD* field);
CALC_API_EXPORT MDB_STATUS ZQDBSetCalcInputAttrAsInt(HZQDB h, MDB_FIELD& field, ssize_t val);
CALC_API_EXPORT MDB_STATUS ZQDBSetCalcInputAttrAsDouble(HZQDB h, MDB_FIELD& field, double val);
CALC_API_EXPORT MDB_STATUS ZQDBSetCalcInputAttrAsStr(HZQDB h, MDB_FIELD& field, const char* val, size_t valen);
CALC_API_EXPORT ssize_t ZQDBGetCalcInputAttrAsInt(HZQDB h, MDB_FIELD& field, ssize_t def);
CALC_API_EXPORT double ZQDBGetCalcInputAttrAsDouble(HZQDB h, MDB_FIELD& field, double def);
CALC_API_EXPORT char* ZQDBGetCalcInputAttrAsStr(HZQDB h, MDB_FIELD& field, char* val, size_t valen);

CALC_API_EXPORT size_t ZQDBGetCalcResultCount(HZQDB h);
CALC_API_EXPORT size_t ZQDBGetCalcResultAttrCount(HZQDB h, size_t index);
//CALC_API_EXPORT MDB_STATUS ZQDBGetCalcResultAttr(HZQDB h, size_t index, size_t pos, MDB_FIELD* field);
CALC_API_EXPORT MDB_STATUS ZQDBSetCalcResultAttrAsInt(HZQDB h, size_t index, MDB_FIELD& field, ssize_t val);
CALC_API_EXPORT MDB_STATUS ZQDBSetCalcResultAttrAsDouble(HZQDB h, size_t index, MDB_FIELD& field, double val);
CALC_API_EXPORT MDB_STATUS ZQDBSetCalcResultAttrAsStr(HZQDB h, size_t index, MDB_FIELD& field, const char* val, size_t valen);
CALC_API_EXPORT ssize_t ZQDBGetCalcResultAttrAsInt(HZQDB h, size_t index, MDB_FIELD& field, ssize_t def);
CALC_API_EXPORT double ZQDBGetCalcResultAttrAsDouble(HZQDB h, size_t index, MDB_FIELD& field, double def);
CALC_API_EXPORT char* ZQDBGetCalcResultAttrAsStr(HZQDB h, size_t index, MDB_FIELD& field, char* val, size_t valen);

CALC_API_EXPORT void ZQDBUpdateCalc();

CALC_API_EXPORT void* ZQDBGetCalcDataValue(HZQDB h);
CALC_API_EXPORT const char* ZQDBGetCalcDataCode(HZQDB h);
CALC_API_EXPORT HZQDB ZQDBGetCalcDataBase(HZQDB h);
CALC_API_EXPORT PERIODTYPE ZQDBGetCalcDataCycle(HZQDB h);
CALC_API_EXPORT size_t ZQDBGetCalcDataCycleEx(HZQDB h);
CALC_API_EXPORT ssize_t ZQDBGetCalcDataFieldAsInt(HZQDB h, MDB_FIELD& field, ssize_t def);
CALC_API_EXPORT double ZQDBGetCalcDataFieldAsDouble(HZQDB h, MDB_FIELD& field, double def);
CALC_API_EXPORT char* ZQDBGetCalcDataFieldAsStr(HZQDB h, MDB_FIELD& field, char* val, size_t vallen, const char* format);

CALC_API_EXPORT size_t ZQDBGetCalcDataMaxCount(HZQDB data);
CALC_API_EXPORT size_t ZQDBGetCalcDataCount(HZQDB data);
CALC_API_EXPORT void* ZQDBGetCalcData(HZQDB data, MDB_FIELD* field);
CALC_API_EXPORT void* ZQDBGetCalcDataEx(HZQDB data, MDB_FIELD* field, size_t pos);
CALC_API_EXPORT ssize_t ZQDBGetCalcDataAsInt(HZQDB data, MDB_FIELD* field, size_t pos, ssize_t def);
CALC_API_EXPORT double ZQDBGetCalcDataAsDouble(HZQDB data, MDB_FIELD* field, size_t pos, double def);
CALC_API_EXPORT char* ZQDBGetCalcDataAsStr(HZQDB data, MDB_FIELD* field, size_t pos, char* val, size_t vallen, const char* format);

CALC_API_EXPORT MDB_STATUS ZQDBGetCalcDataPosByTime(HZQDB data, uint32_t date, uint32_t time, size_t *pos);
CALC_API_EXPORT MDB_STATUS ZQDBGetCalcDataTimeByPos(HZQDB data, size_t pos, uint32_t *date, uint32_t *time);

CALC_API_EXPORT size_t ZQDBGetCalcResultValueMaxCount(HZQDB calc);
CALC_API_EXPORT size_t ZQDBGetCalcResultValueCount(HZQDB calc);
CALC_API_EXPORT void* ZQDBGetCalcResultValue(HZQDB calc, size_t index, MDB_FIELD* field);
CALC_API_EXPORT void* ZQDBGetCalcResultValueEx(HZQDB calc, size_t index, size_t pos, MDB_FIELD* field);
CALC_API_EXPORT ssize_t ZQDBGetCalcResultValueAsInt(HZQDB h, size_t index, size_t pos, ssize_t def);
CALC_API_EXPORT double ZQDBGetCalcResultValueAsDouble(HZQDB h, size_t index, size_t pos, double def);
CALC_API_EXPORT char* ZQDBGetCalcResultValueAsStr(HZQDB h, size_t index, size_t pos, char* val, size_t vallen, const char* format);

#pragma pack(pop)

#if defined(__cplusplus)
}

namespace zqdb { namespace Calc {

class InputAttr
{
protected:
	HZQDB h_ = nullptr;
public:
	InputAttr(MDB_FIELD* field, size_t field_num, char* data = nullptr):h_(ZQDBOpenCalcInput(field, field_num, data))
	{

	}
	InputAttr(HZQDB h) :h_(ZQDBCloneCalcInput(h))
	{

	}
	InputAttr(const char* name) : InputAttr(ZQDBGetCalcFunc(name))
	{

	}
	/*InputAttr(InputAttr&& o) {
		h_ = o.h_;
		o.h_ = nullptr;
	}*/
	~InputAttr()
	{
		if(h_) {
			ZQDBCloseCalcInput(h_);
			h_ = nullptr;
		}
	}

	inline operator HZQDB() const { return h_; }
	inline HZQDB handle() { return h_; }

	size_t GetAttrCount(HZQDB h) { return ZQDBGetCalcInputAttrCount(h_); }
	//MDB_STATUS GetAttr(size_t pos, MDB_FIELD* field) { return ZQDBGetCalcInputAttr(h_, pos, field); }
	MDB_STATUS SetAttrAsInt(MDB_FIELD& field, ssize_t val) { return ZQDBSetCalcInputAttrAsInt(h_, field, val); }
	MDB_STATUS SetAttrAsDouble(MDB_FIELD& field, double val) { return ZQDBSetCalcInputAttrAsDouble(h_, field, val); }
	MDB_STATUS SetAttrAsStr(MDB_FIELD& field, const char* val, size_t valen = -1) { return ZQDBSetCalcInputAttrAsStr(h_, field, val, valen); }
	ssize_t GetAttrAsInt(MDB_FIELD& field, ssize_t def = 0) { return ZQDBGetCalcInputAttrAsInt(h_, field, def); }
	double GetAttrAsDouble(MDB_FIELD& field, double def = 0.) { return ZQDBGetCalcInputAttrAsDouble(h_, field, def); }
	char* GetAttrAsStr(MDB_FIELD& field, char* val, size_t valen) { return ZQDBGetCalcInputAttrAsStr(h_, field, val, valen); }
};

class ResultAttr
{
protected:
	HZQDB h_ = nullptr;
public:
	ResultAttr(MDB_FIELD* field, size_t field_num, char* data = nullptr):h_(ZQDBOpenCalcResult(field, field_num, data))
	{

	}
	ResultAttr(HZQDB h, size_t index = 0) :h_(ZQDBCloneCalcResult(h, index))
	{

	}
	~ResultAttr()
	{
		if(h_) {
			ZQDBCloseCalcResult(h_);
			h_ = nullptr;
		}
	}

	inline operator HZQDB() const { return h_; }
	inline HZQDB handle() { return h_; }

	size_t GetAttrCount(HZQDB h) { return ZQDBGetCalcResultAttrCount(h_, 0); }
	//MDB_STATUS GetAttr(size_t pos, MDB_FIELD* field) { return ZQDBGetCalcResultAttr(h_, 0, pos, field); }
	MDB_STATUS SetAttrAsInt(size_t index, MDB_FIELD& field, ssize_t val) { return ZQDBSetCalcResultAttrAsInt(h_, index, field, val); }
	MDB_STATUS SetAttrAsDouble(size_t index, MDB_FIELD& field, double val) { return ZQDBSetCalcResultAttrAsDouble(h_, index, field, val); }
	MDB_STATUS SetAttrAsStr(size_t index, MDB_FIELD& field, const char* val, size_t valen) { return ZQDBSetCalcResultAttrAsStr(h_, index, field, val, valen); }
	ssize_t GetAttrAsInt(size_t index, MDB_FIELD& field, ssize_t def = 0) { return ZQDBGetCalcResultAttrAsInt(h_, index, field, def); }
	double GetAttrAsDouble(size_t index, MDB_FIELD& field, double def = 0.) { return ZQDBGetCalcResultAttrAsDouble(h_, index, field, def); }
	char* GetAttrAsStr(size_t index, MDB_FIELD& field, char* val, size_t valen) { return ZQDBGetCalcResultAttrAsStr(h_, index, field, val, valen); }
};

class AllFunc : public std::vector<HZQDB> {
public:
	AllFunc(size_t type = CALC_TYPE_MAX) {
		std::vector<HZQDB>& funcs_ = *this;
		auto func_count = ZQDBGetAllCalcFuncCount(type);
		if (func_count > 0) {
			funcs_.resize(func_count);
			funcs_.resize(ZQDBGetAllCalcFunc(type, &funcs_[0], funcs_.size()));
		}
	}
};

class Func
{
protected:
	HZQDB h_ = nullptr;
public:
	Func() {}
	Func(const char* name, CALC_TYPE type, ZQDB_CALC_FN calc, HZQDB input, HZQDB* results, size_t result_num) :h_(ZQDBRegCalcFunc(name, type, calc, input, results, result_num))
	{

	}
	Func(const char* name) :h_(ZQDBGetCalcFunc(name))
	{

	}
	~Func()
	{
		
	}
	
	inline bool IsOpen() { return h_ != nullptr; }
	inline operator HZQDB() const { return h_; }
	inline HZQDB handle() { return h_; }

	const char* GetCalcName() { return ZQDBGetCalcName(h_); }
	CALC_TYPE GetCalcType() { return ZQDBGetCalcType(h_); }

	size_t GetInputAttrCount() { return ZQDBGetCalcInputAttrCount(h_); }
	//MDB_STATUS GetInputAttr(size_t pos, MDB_FIELD* field) { return ZQDBGetCalcInputAttr(h_, pos, field); }
	ssize_t GetInputAttrAsInt(MDB_FIELD& field, ssize_t def = 0) { return ZQDBGetCalcInputAttrAsInt(h_, field, def); }
	double GetInputAttrAsDouble(MDB_FIELD& field, double def = 0.) { return ZQDBGetCalcInputAttrAsDouble(h_, field, def); }
	char* GetInputAttrAsStr(MDB_FIELD& field, char* val, size_t valen) { return ZQDBGetCalcInputAttrAsStr(h_, field, val, valen); }

	size_t GetResultCount() { return ZQDBGetCalcResultCount(h_); }
	size_t GetResultAttrCount(size_t index) { return ZQDBGetCalcResultAttrCount(h_, index); }
	//MDB_STATUS GetResultAttr(size_t index, size_t pos, MDB_FIELD* field) { return ZQDBGetCalcResultAttr(h_, index, pos, field); }
	ssize_t GetResultAttrAsInt(size_t index, MDB_FIELD& field, ssize_t def = 0) { return ZQDBGetCalcResultAttrAsInt(h_, index, field, def); }
	double GetResultAttrAsDouble(size_t index, MDB_FIELD& field, double def = 0.) { return ZQDBGetCalcResultAttrAsDouble(h_, index, field, def); }
	char* GetResultAttrAsStr(size_t index, MDB_FIELD& field, char* val, size_t valen) { return ZQDBGetCalcResultAttrAsStr(h_, index, field, val, valen); }
};

class Data
{
protected:
	HZQDB h_ = nullptr;
	bool auto_close_ = true;
public:
	Data() {}
	Data(const char* code, PERIODTYPE cycle, size_t cycleex = 0) :h_(ZQDBOpenCalcData(ZQDBGetCode(code), cycle, cycleex)) {}
	Data(HZQDB code, PERIODTYPE cycle, size_t cycleex = 0):h_(ZQDBOpenCalcData(code, cycle, cycleex)) {}
	Data(HZQDB h, bool auto_close = false):h_(h), auto_close_(auto_close) {}
	~Data()
	{
		Close();
	}

	inline void Close() { 
		if (auto_close_) {
			if (h_) {
				ZQDBCloseCalcData(h_);
				h_ = nullptr;
			}
		}
	}
	inline HZQDB Open(HZQDB code, PERIODTYPE cycle, size_t cycleex = 0) { h_ = ZQDBOpenCalcData(code, cycle, cycleex);  auto_close_ = true; return h_; }
	inline HZQDB Open(HZQDB h, bool auto_close = false) { h_ = h; auto_close_ = auto_close; return h_; }
	inline bool IsOpen() { return h_ != nullptr; }
	inline operator HZQDB() const { return h_; }
	inline HZQDB handle() { return h_; }

	void* GetValue() { return ZQDBGetCalcDataValue(h_); }
	const char* GetCode() { return ZQDBGetCalcDataCode(h_); }
	HZQDB GetBase() { return ZQDBGetCalcDataBase(h_); }
	PERIODTYPE GetCycle() { return ZQDBGetCalcDataCycle(h_); }
	size_t GetCycleEx() { return ZQDBGetCalcDataCycleEx(h_); }

	ssize_t GetFieldAsInt(MDB_FIELD& field, ssize_t def = 0) { return ZQDBGetCalcDataFieldAsInt(h_, field, def); }
	double GetFieldAsDouble(MDB_FIELD& field, double def = 0.) { return ZQDBGetCalcDataFieldAsDouble(h_, field, def); }
	char* GetFieldAsStr(MDB_FIELD& field, char* val, size_t vallen, const char* format = nullptr) { return ZQDBGetCalcDataFieldAsStr(h_, field, val, vallen, format); }
	
	size_t GetDataMaxCount() { return ZQDBGetCalcDataMaxCount(h_); }
	size_t GetDataCount() { return ZQDBGetCalcDataCount(h_); }
	void* GetData(MDB_FIELD& field) { return ZQDBGetCalcData(h_, &field); }
	void* GetData(MDB_FIELD& field, size_t pos) { return ZQDBGetCalcDataEx(h_, &field, pos); }
	ssize_t GetDataAsInt(MDB_FIELD& field, size_t pos, ssize_t def) { return ZQDBGetCalcDataAsInt(h_, &field, pos, def); }
	double GetDataAsDouble(MDB_FIELD& field, size_t pos, double def) { return ZQDBGetCalcDataAsDouble(h_, &field, pos, def); }
	char* GetDataAsStr(MDB_FIELD& field, size_t pos, char* val, size_t vallen, const char* format = nullptr) { return ZQDBGetCalcDataAsStr(h_, &field, pos, val, vallen, format); }

	MDB_STATUS GetDataPosByTime(uint32_t date, uint32_t time, size_t *pos) { return ZQDBGetCalcDataPosByTime(h_, date, time, pos); }
	MDB_STATUS GetDataTimeByPos(size_t pos, uint32_t *date, uint32_t *time) { return ZQDBGetCalcDataTimeByPos(h_, pos, date, time); }
};

class Calculator
{
protected:
	HZQDB h_ = nullptr;
public:
	Calculator() {}
	Calculator(const char* name, HZQDB data, HZQDB input):h_(ZQDBOpenCalculator(name, data, input))
	{

	}
	~Calculator()
	{
		Close();
	}

	inline void Close() { 
		if(h_) {
			ZQDBCloseCalculator(h_);
			h_ = nullptr;
		}
	}
	inline HZQDB Open(const char* name, HZQDB data, HZQDB input) { h_ = ZQDBOpenCalculator(name, data, input); return h_; }
	inline bool IsOpen() { return h_ != nullptr; }
	inline operator HZQDB() const { return h_; }
	inline HZQDB handle() { return h_; }

	const char* GetCalcName() { return ZQDBGetCalcName(h_); }
	CALC_TYPE GetCalcType() { return ZQDBGetCalcType(h_); }

	size_t GetInputAttrCount() { return ZQDBGetCalcInputAttrCount(h_); }
	//MDB_STATUS GetInputAttr(size_t pos, MDB_FIELD* field) { return ZQDBGetCalcInputAttr(h_, pos, field); }
	MDB_STATUS SetInputAttrAsInt(MDB_FIELD& field, ssize_t val) { return ZQDBSetCalcInputAttrAsInt(h_, field, val); }
	MDB_STATUS SetInputAttrAsDouble(MDB_FIELD& field, double val) { return ZQDBSetCalcInputAttrAsDouble(h_, field, val); }
	MDB_STATUS SetInputAttrAsStr(MDB_FIELD& field, const char* val, size_t valen) { return ZQDBSetCalcInputAttrAsStr(h_, field, val, valen); }
	ssize_t GetInputAttrAsInt(MDB_FIELD& field, ssize_t def = 0) { return ZQDBGetCalcInputAttrAsInt(h_, field, def); }
	double GetInputAttrAsDouble(MDB_FIELD& field, double def = 0.) { return ZQDBGetCalcInputAttrAsDouble(h_, field, def); }
	char* GetInputAttrAsStr(MDB_FIELD& field, char* val, size_t valen) { return ZQDBGetCalcInputAttrAsStr(h_, field, val, valen); }

	size_t GetResultCount() { return ZQDBGetCalcResultCount(h_); }
	size_t GetResultAttrCount(size_t index) { return ZQDBGetCalcResultAttrCount(h_, index); }
	//MDB_STATUS GetResultAttr(size_t index, size_t pos, MDB_FIELD* field) { return ZQDBGetCalcResultAttr(h_, index, pos, field); }
	MDB_STATUS SetResultAttrAsInt(size_t index, MDB_FIELD& field, ssize_t val) { return ZQDBSetCalcResultAttrAsInt(h_, index, field, val); }
	MDB_STATUS SetResultAttrAsDouble(size_t index, MDB_FIELD& field, double val) { return ZQDBSetCalcResultAttrAsDouble(h_, index, field, val); }
	MDB_STATUS SetResultAttrAsStr(size_t index, MDB_FIELD& field, const char* val, size_t valen) { return ZQDBSetCalcResultAttrAsStr(h_, index, field, val, valen); }
	ssize_t GetResultAttrAsInt(size_t index, MDB_FIELD& field, ssize_t def = 0) { return ZQDBGetCalcResultAttrAsInt(h_, index, field, def); }
	double GetResultAttrAsDouble(size_t index, MDB_FIELD& field, double def = 0.) { return ZQDBGetCalcResultAttrAsDouble(h_, index, field, def); }
	char* GetResultAttrAsStr(size_t index, MDB_FIELD& field, char* val, size_t valen) { return ZQDBGetCalcResultAttrAsStr(h_, index, field, val, valen); }

	void* GetValue() { return ZQDBGetCalcDataValue(h_); }
	const char* GetCode() { return ZQDBGetCalcDataCode(h_); }
	PERIODTYPE GetCycle() { return ZQDBGetCalcDataCycle(h_); }
	size_t GetCycleEx() { return ZQDBGetCalcDataCycleEx(h_); }

	ssize_t GetFieldAsInt(MDB_FIELD& field, ssize_t def = 0) { return ZQDBGetCalcDataFieldAsInt(h_, field, def); }
	double GetFieldAsDouble(MDB_FIELD& field, double def = 0.) { return ZQDBGetCalcDataFieldAsDouble(h_, field, def); }
	char* GetFieldAsStr(MDB_FIELD& field, char* val, size_t vallen, const char* format = nullptr) { return ZQDBGetCalcDataFieldAsStr(h_, field, val, vallen, format); }
	
	size_t GetDataMaxCount() { return ZQDBGetCalcDataMaxCount(h_); }
	size_t GetDataCount() { return ZQDBGetCalcDataCount(h_); }
	void* GetData(MDB_FIELD& field) { return ZQDBGetCalcData(h_, &field); }
	void* GetData(MDB_FIELD& field, size_t pos) { return ZQDBGetCalcDataEx(h_, &field, pos); }
	ssize_t GetDataAsInt(MDB_FIELD& field, size_t pos, ssize_t def) { return ZQDBGetCalcDataAsInt(h_, &field, pos, def); }
	double GetDataAsDouble(MDB_FIELD& field, size_t pos, double def) { return ZQDBGetCalcDataAsDouble(h_, &field, pos, def); }
	char* GetDataAsStr(MDB_FIELD& field, size_t pos, char* val, size_t vallen, const char* format = nullptr) { return ZQDBGetCalcDataAsStr(h_, &field, pos, val, vallen, format); }

	MDB_STATUS GetDataPosByTime(HZQDB data, uint32_t date, uint32_t time, size_t *pos) { return ZQDBGetCalcDataPosByTime(h_, date, time, pos); }
	MDB_STATUS GetDataTimeByPos(HZQDB data, size_t pos, uint32_t *date, uint32_t *time) { return ZQDBGetCalcDataTimeByPos(h_, pos, date, time); }
	
	size_t GetResultValueMaxCount() { return ZQDBGetCalcResultValueMaxCount(h_); }
	size_t GetResultValueCount() { return ZQDBGetCalcResultValueCount(h_); }
	void* GetResultValue(size_t index, MDB_FIELD* field = nullptr) { return ZQDBGetCalcResultValue(h_, index, field); }
	void* GetResultValue(size_t index, size_t pos, MDB_FIELD* field = nullptr) { return ZQDBGetCalcResultValueEx(h_, index, pos, field); }
	ssize_t GetResultValueAsInt(size_t index, size_t pos, ssize_t def = 0) { return ZQDBGetCalcResultValueAsInt(h_, index, pos, def); }
	double GetResultValueAsDouble(size_t index, size_t pos, double def = 0.) { return ZQDBGetCalcResultValueAsDouble(h_, index, pos, def); }
	char* GetResultValueAsStr(size_t index, size_t pos, char* val, size_t vallen, const char* format = nullptr) { return ZQDBGetCalcResultValueAsStr(h_, index, pos, val, vallen, format); }
};

class Manager
{
protected:
public:

	inline HZQDB RegCalcFunc(const char* name, CALC_TYPE type, ZQDB_CALC_FN calc, HZQDB input, HZQDB* results, size_t result_num) { return ZQDBRegCalcFunc(name, type, calc, input, results, result_num); }
	inline HZQDB GetCalcFunc(const char* name) { return ZQDBGetCalcFunc(name); }

	inline void Update() { ZQDBUpdateCalc(); }
};

} }

#endif//

#endif//_H_ZQDB_CALC_H_