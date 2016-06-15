/*
* Geosciences Template Library
*
* Copyright (c) 2008
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose.
* It is provided "as is" without express or implied warranty.
*/
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include "vertex3d.h"



begin_gtl_namespace
begin_gdb_namespace

/** @defgroup math  Math-数学库
*  @{
*/

class CORE_API MathEx
{
public:


	/** @name 属性（成员变量）
	* @{
	*/

	/** 成员变量 */
	static const double  TOL;
	static const double	 TOL_F;
	static const double  ONE_PI;
	static const double  TWO_PI;
	static const double  HALF_PI;
	static const double  dDegToRad;
	static const double  dRadToDeg;
	static const double  dDoubleMax;
	static const double  dDoubleMin;

	/** @} */ // 属性结尾



	/** @name 构造与析构函数
	*  @{
	*/
protected:
	/** 构造函数
	*/
	MathEx(void);
	/** 析构函数
	*/
	~MathEx(void);
	/** @} */

	/** @name 计算操作
	*  @{
	*/
public:

	/** 判断符号
	* @param [in] iValue 输入数值
	* @return 正负
	*/
	static inline int isign(int iValue){ return (iValue > 0 ? +1 : (iValue < 0 ? -1 : 0)); }
	/** 判断符号
	* @param [in] dValue 输入数值
	* @return 正负
	* -1 表示为正
	* --1 表示为负
	* -0 表示为0
	*/
	static double sign(double dValue);
	/** 计算反余弦值
	* @param [in] fValue 输入数据
	* @return 角度
	*/
	static double acos(double fValue);

	/** 计算反正弦值
	* @param [in] fValue 输入数据
	* @return 角度
	*/
	static double asin(double fValue);

	/** 计算反正切值
	* @param [in] dValue 输入数据
	* @return 角度
	*/
	static inline double atan(double fValue) { return (::atan(fValue)); }

	/** 计算正弦函数
	* @param [in] dValue 数据
	* @return 正弦值
	*/
	static inline double atan2(double fY, double fX) { return (::atan2(fY, fX)); }

	/** 计算正弦函数
	* @param [in] dValue 数据
	* @return 正弦值
	*/
	static  double radianToDegree(double angle);

	/** 计算正弦函数
	* @param [in] dValue 数据
	* @return 正弦值
	*/
	static double degreeToRadian(double angle);

	/** 使用标准模版库（STL）的泛型式快速排序，来源于wikipedia
	* @param BidirectionalIterator:	列表的迭代器类型
	* @param Compare:两个对象的比较函数
	* @return
	*/
	template< typename BidirectionalIterator, typename Compare >
	static void quick_sortEx(BidirectionalIterator first, BidirectionalIterator last, Compare cmp) {
		if (first != last) {
			BidirectionalIterator left = first;
			BidirectionalIterator right = last;
			BidirectionalIterator pivot = left++;

			while (left != right) {
				if (cmp(*left, *pivot)) {
					++left;
				}
				else {
					while ((left != right) && cmp(*pivot, *right))
						right--;
					std::iter_swap(left, right);
				}
			}

			if (cmp(*pivot, *left))
				--left;
			std::iter_swap(first, left);

			quick_sortEx(first, left, cmp);
			quick_sortEx(right, last, cmp);
		}
	}
	/** 使用标准模版库（STL）的泛型式快速排序，来源于wikipedia
	* @param BidirectionalIterator:	列表的迭代器
	* @return
	*/
	template< typename BidirectionalIterator >
	static void quick_sort(BidirectionalIterator first, BidirectionalIterator last) {
		quick_sortEx(first, last,
			std::less_equal< typename std::iterator_traits< BidirectionalIterator >::value_type >()
			);
	}
	/** @} */
};
/** @} */



end_gdb_namespace
end_gtl_namespace


