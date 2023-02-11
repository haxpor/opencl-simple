//+------------------------------------------------------------------+
//|                                                      OpenCLX.mqh |
//|                                          Copyright 2022, haxpor. |
//|                                                 https://wasin.io |
//+------------------------------------------------------------------+
#property copyright "Copyright 2022, haxpor."
#property link      "https://wasin.io"

#import "openclsimple.dll"
void clsimple_listall();
int clsimple_compute(const int& arr_1[], const int& arr_2[], int& arr_3[], int num_elem);
#import