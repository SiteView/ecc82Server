#include <stdlib.h>
#include "expressionevaluator.h"
#include "SNMP_lib.h"


/*
  History
  ----------
  Sunday, November 2, 2003
             * Initial version

  Monday, November 3, 2003
             * Fixed precedence rule of multiplication

  Tuesday, November 4, 2003
             * Fixed a bug in isOperator()
             * Added support for negative and positive numbers as: -1 or +1
               (initially they were supported as: 0-1 or 0+1)
             * Added exception handling and foolproofs against malformed expression
             * Added >=, <=, != operators
*/

/* ----------------------------------------------------------------------------- 
 * Copyright (c) 2003 Lallous <lallousx86@yahoo.com>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ----------------------------------------------------------------------------- 
 */

/*

  ExpressionEvaluator is a set of functions allowing you to evaluate a given expression.
  Currently two versions are exported from the ExpressionEvaluator namespace:

  int ExpressionEvaluator::calculateLong(std::string expr, long &result);
  int ExpressionEvaluator::calculateDouble(std::string expr, double &result);

  Both functions might return an error code defined in the header file / enum having
  set of values as eval_XXXX.
  To check for success, do this:
  if (ExpressionEvaluator::calculateXXX(expr, xxx) == ExpressionEvaluator::eval_ok)
  {
    // .......
  }

*/
#include "ExpressionEvaluator.h"

extern void DumpLog(const char * pszFileName, const char *pBuffer, const int nLen);

namespace ExpressionEvaluator
{
  const char *operator_mul   = "*";
  const char *operator_div   = "/";
  const char *operator_idiv  = "\\";
  const char *operator_mod   = "%";
  const char *operator_add   = "+";
  const char *operator_sub   = "-";
  const char *operator_xor   = "^";
  const char *operator_band  = "&";
  const char *operator_bor   = "|";
  const char *operator_land  = "&&";
  const char *operator_lor   = "||";
  const char *operator_nor   = "!|";
  const char *operator_nand  = "!&";
  const char *operator_iseq  = "==";
  const char *operator_ne    = "!=";
  const char *operator_shl   = "<<";
  const char *operator_shr   = ">>";
  const char *operator_lt    = "<";
  const char *operator_gt    = ">";
  const char *operator_gte   = ">=";
  const char *operator_lte   = "<=";

  struct operator_t
  {
    const char *op;
    int  precedence;
  };

  SVRows      g_svResultList;
  //ResultList *pResultList ;

  const operator_t operators[] =
  {
    {"<>=*/%+-^&|\\", -1}, // Operators[0] is reserved for storing all symbols that can be used in operators
    {operator_mul, 19}, 
    {operator_div, 19}, {operator_idiv, 19}, 
    {operator_mod, 18},
    {operator_shl, 17}, {operator_shr, 17},
    {operator_sub, 16}, {operator_add, 16},
    {operator_xor, 15}, {operator_band, 15}, {operator_bor, 15}, {operator_nor, 15}, 
    {operator_land, 14}, {operator_lor, 14}, {operator_nand, 15},
    {operator_iseq, 13}, {operator_lt, 13}, {operator_gt, 13},
    {operator_gte, 13}, {operator_ne, 13}, {operator_lte, 13}
  };
  
  bool IsNumbric(const char* chValue)
  {
	  int nIndex = 0;
	  if(chValue[0] == '-' || chValue[0] == '+')
		  nIndex ++;
	  int nPoint = 0;
	  for(int i = nIndex; i< static_cast<int>(strlen(chValue)); i++)
	  {
		  if(chValue[i] < '0' || chValue[i] > '9')
		  {
			  if(chValue[i] == '.' && nPoint == 0 &&
				  i < static_cast<int>(strlen(chValue)) - 1)
			  {
				  nPoint ++;
			  }
			  else
			  {
				  return false;
			  }
		  }
	  }
	  return true;
  }

    unsigned long GetOidValue(const string &chOID)
    {
        unsigned long ulValue = 0;

        int nPos = static_cast<int>(chOID.find("."));
        string szOID(chOID), szRight("value");
        if(nPos > 0)
        {
            szOID = chOID.substr(0, nPos);
            szRight = chOID.substr(nPos + 1);
        }

        SVRow svrow = g_svResultList.find(szOID);
        if(svrow != g_svResultList.end())
        {
            SVItem svitem = svrow->second.find(szRight);
            if(svitem != svrow->second.end())
                ulValue = svitem->second;
            //else
            //    cout << "find value name is " << szRight.c_str() <<"not found" << endl;

            //if(szOID == "oid7" || szOID == "oid8" )
            //    for(SVItem item = svrow->second.begin(); item != svrow->second.end(); item ++)
            //        cout << item->first.c_str() << "\t=\t" << item->second << endl;
        }
        //else
        //{
        //    cout << "not found" << endl;
        //}



        //char chTmp[256] = {0};
        //sprintf(chTmp , "%s", chOID);
        //strlwr(chTmp);
        //char *pDest = strstr(chTmp,"oid");
        //if(pDest != NULL)
        //{
        //    int nIndex = 0 , nTemp = 0;
        //    nIndex = atoi(pDest+3) -1;
        //    ResultList *pTemp = pResultList;
        //    while(pTemp != NULL)
        //    {
        //        if(nIndex == nTemp)
        //        {
        //            pDest = NULL;
        //            pDest = strstr(chTmp, ".");
        //            if(pDest != NULL)
        //            {
        //                if(strcmp(pDest+1 , "max") == 0)
        //                {
        //                    ulValue = pTemp->Max;
        //                }
        //                else if(strcmp(pDest+1 , "min") == 0)
        //                {
        //                    ulValue = pTemp->Min;
        //                }
        //                else if(strcmp(pDest+1 , "avg") == 0)
        //                {
        //                    ulValue = pTemp->Avg;
        //                }
        //                else if(strcmp(pDest+1 , "total") == 0)
        //                {
        //                    ulValue = pTemp->Total;
        //                }
        //                else if(strcmp(pDest+1 , "time") == 0)
        //                {
        //                    ulValue = pTemp->time;
        //                }
        //                else if (strcmp(pDest+1,"indexcount")==0)
        //                {
        //                    ulValue = pTemp->IndexCount;
        //                }
        //            }
        //            else
        //            {
        //                ulValue = pTemp->Value;
        //            }
        //        break;
        //        }
        //        else
        //        {
        //            pTemp = pTemp->pNext;
        //            nTemp ++;
        //        }
        //    }
        //}
        //free( chTmp);
        return ulValue;
    }
  // Returns < 0 if 'op' is not an operator
  // Otherwise it returns the index of the operator in the 'operators' array
  // The index can be used to get the operator's precedence value.
  int isOperator(string op)
  {
    int i = 0, oplen = 0;
    string s = operators[0].op;
    // scan as long as it is a valid operator
    // an operator might  have not just one symbol to represent it

    while (i < static_cast<int>(op.length()))
    {
      if (s.find(op[i]) == s.npos)
        break;
      oplen++;
      i++;
    }

    // no valid symbol in operator!s
    if (!oplen)
      return -1;

    // identify what operator that is and return its index
    // 1 = highest , N = lowest
    // -1 = invalid operator
    s = op.substr(0, oplen);
    for (i=1; i<sizeof(operators)/sizeof(operator_t); i++)
    {
      if (s == operators[i].op)
        return i;
    }
    return -1;
  }

  // returns the operands length.
  // scans as long as the current value is an alphanumeric or a decimal seperator
  int getToken(string str)
  {
    int i=0, tokenLen = 0;
    while ( (i < static_cast<int>(str.length())) && (isalnum(str[i]) || (str[i]=='.')))
    {
      tokenLen++;
      i++;
    }
    return tokenLen;
  }

  int toRPN(string exp, string &rpn)
  {
    std::stack<string> st;
    string token, topToken;
    char token1;
    int  tokenLen, topPrecedence, idx, precedence;
    string SEP(" "), EMPTY("");

    rpn = "";

    for (int i = 0; i < static_cast<int>(exp.length()); i++)
    {
      token1 = exp[i];

      // skip white space
      if (isspace(token1))
        continue;

      // push left parenthesis
      else if (token1 == '(')
      {
        st.push(EMPTY+token1);
        continue;
      }

      // flush all stack till matching the left-parenthesis
      else if (token1 == ')')
      {
        for (;;)
        {
          // could not match left-parenthesis
          if (st.empty())
            return eval_unbalanced;

          topToken = st.top();
          st.pop();
          if (topToken == "(")
            break;
          rpn.append(SEP + topToken);
        }
        continue;
      }

      // is this an operator?
      idx = isOperator(exp.substr(i));
    
      // an operand
      if (idx < 0)
      {
        tokenLen = getToken(exp.substr(i));
        if (tokenLen == 0)
          return eval_invalidoperand;

        token = exp.substr(i, tokenLen);
        rpn.append(SEP + token);
        i += tokenLen - 1;
        continue;
      }

      // is an operator
      else
      {
        // expression is empty or last operand an operator
        if (rpn.empty() || (isOperator(token) > 0))
        {
          rpn.append(SEP + "0");
        }
        // get precedence
        precedence = operators[idx].precedence;
        topPrecedence = 0;

        // get current operator 
        tokenLen = static_cast<int>(strlen(operators[idx].op));
        token    = exp.substr(i, tokenLen);
        i += tokenLen - 1;
        for (;;)
        {
          // get top's precedence
          if (!st.empty())
          {
            topToken = st.top();
            idx = isOperator(topToken.c_str());
            if (idx < 0)
              topPrecedence = 1; // give a low priority if operator not ok!
            else
              topPrecedence = operators[idx].precedence;
          }

          if (st.empty() || st.top() == "(" ||
              precedence > topPrecedence
             )
          {
            st.push(token);
            break;
          }
          // operator has lower precedence then pop it
          else
          {
            st.pop();
            rpn.append(SEP + topToken);
          }
        }
        continue;
      }
    }

    for (;;)
    {
      if (st.empty())
        break;
      topToken = st.top();
      st.pop();
      if (topToken != "(")
        rpn.append(SEP + topToken);
      else
      {
        return eval_unbalanced;
      }
    }
    return eval_ok;
  }

  template <typename T>
  int evaluateRPN(string rpn, T &result)
  {
    std::stack<T> st;
    char token1;
    string token;
    T r, op1, op2;
    int idx, tokenLen;

    for (int i = 0;i < static_cast<int>(rpn.length()); i ++)
    {
      token1 = rpn[i];
      if (isspace(token1))
        continue;

      idx = isOperator(rpn.substr(i));

      // not an operator
      if (idx < 0)
      {
        tokenLen = getToken(rpn.substr(i));
        token    = rpn.substr(i, tokenLen);
        char *errstr = NULL; 
        double d = 0;
		if(IsNumbric(token.c_str()))
			d = strtod(token.c_str(), &errstr);
		else
		{
			d = GetOidValue(token);
		}

        if (errstr != NULL && *errstr)
          return eval_invalidoperand;

        r = static_cast<T>(d);
        st.push(r);
        i += tokenLen - 1;
        continue;
      }
      // an operator
      else
      {
        // get operand1 and op2
        op2 = st.top(); st.pop();
        if (!st.empty())
        {
          op1 = st.top(); st.pop();
        } else
          return eval_unbalanced;

        tokenLen = static_cast<int>(strlen(operators[idx].op));
        token    = rpn.substr(i, tokenLen);
        i += tokenLen - 1;

        if (token == operator_mul)
          r = op1 * op2;
        else if (token == operator_idiv)
			if(op2 == 0)
				return eval_idiv;
			else
				r =  (op1 / op2);
        else if (token == operator_div)
			if(op2 == 0)
				return eval_idiv;
			else
				r =  (op1 / op2);
        else if (token == operator_mod)
          r = (long)op1 % (long)op2;
        else if (token == operator_add)
          r = op1 + op2;
        else if (token == operator_sub)
          r = op1 - op2;
        else if (token == operator_land)
          r = op1 && op2;
        else if (token == operator_band)
          r = (long)op1 & (long)op2;
        else if (token == operator_lor)
          r = op1 || op2;
        else if (token == operator_bor)
          r = (long)op1 | (long)op2;
        else if (token == operator_xor)
          r = (long)op1 ^ (long)op2;
        else if (token == operator_nor)
          r = ~((long)op1 | (long)op2);
        else if (token == operator_nand)
          r = ~((long)op1 & (long)op2);
        else if (token == operator_iseq)
          r = op1 == op2;
        else if (token == operator_ne)
          r = op1 != op2;
        else if (token == operator_shl)
          r = (long)op1 << (long)op2;
        else if (token == operator_shr)
          r = (long)op1 >> (long)op2;
        else if (token == operator_lt)
          r = op1 < op2;
        else if (token == operator_lte)
          r = op1 <= op2;
        else if (token == operator_gt)
          r = op1 > op2;
        else if (token == operator_gte)
          r = op1 >= op2;
        // push result
        st.push(r);
      }
    }
    result = st.top();
    st.pop();
    if (!st.empty())
      return eval_evalerr;
    return eval_ok;
  }

  template <typename T> int calculate(string expr, T &r)
  {
    string rpn;
    int err = eval_evalerr; // unexpected error

    try
    {
      if ( (err = toRPN(expr, rpn)) != eval_ok)
        return err;
      err = evaluateRPN(rpn, r);
    }
    catch(...)
    {
    }
    return err;
  }
  
  int calculateLong(string expr, long &r)
  {
    return calculate(expr, r);
  }

  int calculateDouble(string expr, double &r)
  {
    return calculate(expr, r);
  }

  void addList(clsOID &clsoid, int nIndex, unsigned long ulValue, unsigned long lTime)
  {
    char chOIDName[8] = {0};
    sprintf(chOIDName, "oid%d", nIndex);
    string szOIDName = chOIDName;
    SVItems items;
    items["avg"] = clsoid.Average();
    items["max"] = clsoid.Max();
    items["min"] = clsoid.Min();
    items["total"] = static_cast<unsigned long>(clsoid.Total());
    items["indexcount"] = clsoid.IndexCount();
    items["value"] = ulValue;
    items["time"] = lTime;
    g_svResultList[szOIDName] = items;
  }

  //void SetResultList(ResultList *pList)
  //{
  //    pResultList = pList;
  //}
}