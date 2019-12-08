#ifndef _violin
#define _violin

#include "violin_0.h"
#include "violin_1.h"
#include "violin_2.h"
#include "violin_3.h"
#include "violin_4.h"
#include "violin_5.h"
#include "violin_6.h"
#include "violin_7.h"
#include "violin_8.h"
#include "violin_9.h"
#include "violin_10.h"
#include "violin_11.h"
#include "violin_12.h"
#include "violin_13.h"

const unsigned int violin_TablePtrsCount = 14;

static HiResWTSet* violin_TablePtrs[violin_TablePtrsCount] = { &violin_0_TableSet, &violin_1_TableSet, &violin_2_TableSet, &violin_3_TableSet, &violin_4_TableSet, &violin_5_TableSet, &violin_6_TableSet, &violin_7_TableSet, &violin_8_TableSet, &violin_9_TableSet, &violin_10_TableSet, &violin_11_TableSet, &violin_12_TableSet, &violin_13_TableSet }; 

// --- Define Wavform Names: Here is where you can override the table names all at once;
//     Remember to keep the names short and simple, refrain from the underscore, and keep the character count below 32 for compatibiltiy with brick files.
static std::string violin_TableNames[violin_TablePtrsCount] = { 
	"violin hello",
	"violin world",
	"violin 2",
	"violin 3",
	"violin 4",
	"violin 5",
	"violin 6",
	"violin 7",
	"violin 8",
	"violin 9",
	"violin 10",
	"violin 11",
	"violin 12",
	"violin 13" }; 

// --- This is the bank descriptor; you can initialize the bank with it. 
static BankDescriptor violin_BankDescriptor(violin_TablePtrsCount, &violin_TablePtrs[0], &violin_TableNames[0]);

#endif // definer
