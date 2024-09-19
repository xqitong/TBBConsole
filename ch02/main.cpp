//  此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdafx.h"
#include "fig_2_03.h"
#include "fig_2_04.h"
#include "fig_2_05.h"
#include "fig_2_06.h"  
#include "fig_2_07.h"
#include "fig_2_08.h"
#include "fig_2_10.h"
#include "fig_2_11.h"
#include "fig_2_12.h"
#include "fig_2_14.h"
#include "fig_2_15.h"
#include "fig_2_16.h"
#include "fig_2_17.h"
#include "fig_2_18.h"
#include "fig_2_19.h"
#include "fig_2_20.h"
#include "fig_2_22.h"
#include "fig_2_23.h" 
#include "fig_2_24.h" 
#include "fig_2_28.h" 
#include "fig_2_30.h"

int main()
{
   std::cout << "可支持的线程数量: " << tbb::this_task_arena::max_concurrency() << std::endl;
   //ch02::fig_2_28::main();
   ch02::fig_2_30::main();
}
