#ifndef _e_organ
#define _e_organ

#include "Organ_0.h"
#include "Organ_1.h"
#include "Organ_2.h"
#include "Organ_3.h"
#include "Organ_4.h"
#include "Organ_5.h"
#include "Organ_6.h"
#include "Organ_7.h"
#include "Organ_8.h"
#include "Organ_9.h"
#include "Organ_10.h"
#include "Organ_11.h"
#include "Organ_12.h"
#include "Organ_13.h"
#include "Organ_14.h"
#include "Organ_15.h"
#include "Organ_16.h"
#include "Organ_17.h"
#include "Organ_18.h"
#include "Organ_19.h"
#include "Organ_20.h"
#include "Organ_21.h"
#include "Organ_22.h"
#include "Organ_23.h"
#include "Organ_24.h"
#include "Organ_25.h"
#include "Organ_26.h"
#include "Organ_27.h"
#include "Organ_28.h"
#include "Organ_29.h"
#include "Organ_30.h"
#include "Organ_31.h"

// the table count
const unsigned int e_organ_TablePtrs_tableCount = 32;

// the master table, full of pointers to the other tablesets
static HiResWTSet* e_organ_TablePtrs[e_organ_TablePtrs_tableCount] = { &Organ_0_TableSet, &Organ_1_TableSet, &Organ_2_TableSet, &Organ_3_TableSet, &Organ_4_TableSet, &Organ_5_TableSet, &Organ_6_TableSet, &Organ_7_TableSet, &Organ_8_TableSet, &Organ_9_TableSet, &Organ_10_TableSet, &Organ_11_TableSet, &Organ_12_TableSet, &Organ_13_TableSet, &Organ_14_TableSet, &Organ_15_TableSet, &Organ_16_TableSet, &Organ_17_TableSet, &Organ_18_TableSet, &Organ_19_TableSet, &Organ_20_TableSet, &Organ_21_TableSet, &Organ_22_TableSet, &Organ_23_TableSet, &Organ_24_TableSet, &Organ_25_TableSet, &Organ_26_TableSet, &Organ_27_TableSet, &Organ_28_TableSet, &Organ_29_TableSet, &Organ_30_TableSet, &Organ_31_TableSet }; 

#endif // definer
