#ifndef _oboe
#define _oboe

#include "oboe_0.h"
#include "oboe_1.h"
#include "oboe_2.h"
#include "oboe_3.h"
#include "oboe_4.h"
#include "oboe_5.h"
#include "oboe_6.h"
#include "oboe_7.h"
#include "oboe_8.h"
#include "oboe_9.h"
#include "oboe_10.h"
#include "oboe_11.h"
#include "oboe_12.h"

const unsigned int oboe_TablePtrsCount = 13;

static HiResWTSet* oboe_TablePtrs[oboe_TablePtrsCount] = { &oboe_0_TableSet, &oboe_1_TableSet, &oboe_2_TableSet, &oboe_3_TableSet, &oboe_4_TableSet, &oboe_5_TableSet, &oboe_6_TableSet, &oboe_7_TableSet, &oboe_8_TableSet, &oboe_9_TableSet, &oboe_10_TableSet, &oboe_11_TableSet, &oboe_12_TableSet }; 

// --- Define Wavform Names: Here is where you can override the table names all at once;
//     Remember to keep the names short and simple, refrain from the underscore, and keep the character count below 32 for compatibiltiy with brick files.
static std::string oboe_TableNames[oboe_TablePtrsCount] = { 
	"oboe 0",
	"oboe 1",
	"oboe 2",
	"oboe 3",
	"oboe 4",
	"oboe 5",
	"oboe 6",
	"oboe 7",
	"oboe 8",
	"oboe 9",
	"oboe 10",
	"oboe 11",
	"oboe 12" }; 

// --- This is the bank descriptor; you can initialize the bank with it. 
static BankDescriptor oboe_BankDescriptor(oboe_TablePtrsCount, &oboe_TablePtrs[0], &oboe_TableNames[0]);

#endif // definer
