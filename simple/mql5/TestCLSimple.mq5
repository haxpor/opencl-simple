//+------------------------------------------------------------------+
//|                                                  TestOpenCLX.mq5 |
//|                                          Copyright 2022, haxpor. |
//|                                                 https://wasin.io |
//+------------------------------------------------------------------+
#property copyright "Copyright 2022, haxpor."
#property link      "https://wasin.io"
#property version   "1.00"

#include "OpenCLSimple.mqh"

//+------------------------------------------------------------------+
//| Script program start function                                    |
//+------------------------------------------------------------------+
void OnStart()
{
   clsimple_listall();
   
   int arr_1[];
   int arr_2[];
   int arr_3[];
   
   ArrayResize(arr_1, 10000000);
   ArrayFill(arr_1, 0, ArraySize(arr_1), 1);
   
   ArrayResize(arr_2, 10000000);
   ArrayFill(arr_2, 0, ArraySize(arr_2), 1);
   
   ArrayResize(arr_3, 10000000);
   
   uint start_time = GetTickCount();
   
   int ret_code = clsimple_compute(arr_1, arr_2, arr_3, ArraySize(arr_1));
   if (ret_code != 0) {
      Print("Error occurs, code=", ret_code);
      return;
   }
   Print("Elapsed time: " + (string)(GetTickCount() - start_time) + " ms");
   
   bool is_valid = true;
   for (int i=0; i<ArraySize(arr_3); ++i) {
      if (arr_3[i] != 2) {
         Print("Something is wrong at index=" + (string)i);
         is_valid = false;
      }
   }
   
   if (is_valid) {
      Print("Passed test");
   }
}
