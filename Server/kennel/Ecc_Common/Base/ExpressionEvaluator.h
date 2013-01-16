#ifndef EXPRESSION_EVALUATOR__INC__
#define EXPRESSION_EVALUATOR__INC__

#pragma once
#pragma warning (disable:4786)

#include <iostream>
#include <map>
#include <stack>
#include <string>

using namespace std;

class clsOID;

//#ifndef _SV_EXP_ITEMS
//#define _SV_EXP_ITEMS
    typedef map<string, unsigned long, less<string> >  SVItems;
    typedef SVItems::iterator SVItem;
    typedef SVItems::const_iterator SVConstItem;
//#endif

//#ifndef _SV_EXP_ROWS
//#define _SV_EXP_ROWS
    typedef map<string, SVItems, less<string> > SVRows;
    typedef SVRows::iterator SVRow;
    typedef SVRows::const_iterator SVConstRow;
//#endif

namespace ExpressionEvaluator
{
    enum
    {
        eval_ok = 0,
        eval_unbalanced,
        eval_invalidoperator,
        eval_invalidoperand,
        eval_evalerr,
        eval_idiv = -9
    };

//SVRows      g_svResultList;

    int calculateLong(std::string expr, long &r);
    int calculateDouble(std::string expr, double &r);
    int toRPN(string exp, string &rpn);
    void addList(clsOID &clsoid, int nIndex, unsigned long ulValue, unsigned long lTime);
    
    unsigned long GetOidValue(const string &chOID);
    template <typename T>int evaluateRPN(string rpn, T &result);
}

#endif