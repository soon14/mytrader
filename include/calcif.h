#pragma once

#ifndef _H_CALC_IF_H_
#define _H_CALC_IF_H_

#include <calc.h>
#include <zqif.h>

#if defined(__cplusplus)
extern "C" {
#endif//

#pragma pack(push, 1)

CALC_API_EXPORT MDB_STATUS ZQCalcStart(ZQDB_INF* inf);
CALC_API_EXPORT MDB_STATUS ZQCalcStop();

#pragma pack(pop)

#if defined(__cplusplus)
}

#endif//

#endif//_H_CALC_IF_H_