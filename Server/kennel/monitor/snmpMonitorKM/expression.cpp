#include "expression.h"


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
    const char *    pcszOp;
    int             nPrecedence;
};

const operator_t operators[] =
{
    {"<>=*/%+-^&|\\", -1}, // Operators[0] is reserved for storing all symbols that can be used in operators
    {operator_mul, 19}, {operator_div, 19}, {operator_idiv, 19}, 
    {operator_mod, 18},
    {operator_shl, 17}, {operator_shr, 17},
    {operator_sub, 16}, {operator_add, 16},
    {operator_xor, 15}, {operator_band, 15}, {operator_bor, 15}, {operator_nor, 15}, {operator_nand, 15},
    {operator_land, 14}, {operator_lor, 14}, 
    {operator_iseq, 13}, {operator_lt, 13}, {operator_gt, 13},{operator_gte, 13}, {operator_ne, 13}, {operator_lte, 13}
};


CSVExpression::CSVExpression()
{
}

CSVExpression::~CSVExpression()
{
}

CSVExpression::isOperator(const string &szOp)
{
    int i = 0, nOpLen = 0;
    string s = operators[0].pcszOp;
    // scan as long as it is a valid operator
    // an operator might  have not just one symbol to represent it

    while (i < static_cast<int>( szOp.length() ) )
    {
        if (s.find(szOp[i]) == s.npos)
            break;
        nOpLen ++;
        i ++;
    }

    // no valid symbol in operator!s
    if (!nOpLen)
        return -1;

    // identify what operator that is and return its index
    // 1 = highest , N = lowest
    // -1 = invalid operator
    s = szOp.substr(0, nOpLen);
    for (i = 1; i < sizeof(operators) / sizeof(operator_t); i ++)
    {
        if (s == operators[i].pcszOp)
            return i;
    }
    return -1;
}

void CSVExpression::AddFields(const string &pcszRow, const string &cszField, unsigned __int64 ulValue)
{
    CSVResultRow svrow = m_ResultList.find(pcszRow);
    if(svrow != m_ResultList.end())
    {
        CSVField svitem = svrow->second.find(cszField);
        if(svitem != svrow->second.end())
            svitem->second = ulValue;
        else
            svrow->second[cszField] = ulValue;
    }
    else
    {
        CSVFields svFields;
        svFields[cszField] = ulValue;
        m_ResultList[pcszRow] = svFields;
    }
}

int CSVExpression::calculateDouble(const string &szExpr, double &dResult)
{
    int nRet = eval_ok;
    string szRpn("");
    if((nRet = toRPN(szExpr, szRpn)) == eval_ok)
    {
        nRet = evaluateRPN(szRpn, dResult);
    }
    return nRet;
}

int CSVExpression::toRPN(const string &szExpr, string &rpn)
{
    std::stack<string> st;
    string token, topToken;
    char token1;
    int  tokenLen, topPrecedence, idx, precedence;
    string SEP(" "), EMPTY("");

    rpn = "";

    for (int i = 0; i < static_cast<int>(szExpr.length()); i++)
    {
      token1 = szExpr[i];

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
      idx = isOperator(szExpr.substr(i));
    
      // an operand
      if (idx < 0)
      {
        tokenLen = getToken(szExpr.substr(i));
        if (tokenLen == 0)
          return eval_invalidoperand;

        token = szExpr.substr(i, tokenLen);
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
        precedence = operators[idx].nPrecedence;
        topPrecedence = 0;

        // get current operator 
        tokenLen = static_cast<int>(strlen(operators[idx].pcszOp));
        token    = szExpr.substr(i, tokenLen);
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
              topPrecedence = operators[idx].nPrecedence;
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

int CSVExpression::getToken(const string &str)
{
    int i=0, tokenLen = 0;
    while ( (i < static_cast<int>(str.length())) && (isalnum(str[i]) || (str[i]=='.')))
    {
        tokenLen++;
        i++;
    }
    return tokenLen;
}

int CSVExpression::evaluateRPN(const string &rpn, double &result)
{
    std::stack<double> st;
    char token1;
    string token;
    double r, op1, op2;
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

            r = static_cast<double>(d);
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
            }
            else
                return eval_unbalanced;

            tokenLen = static_cast<int>(strlen(operators[idx].pcszOp));
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
                r = (__int64)op1 % (__int64)op2;
            else if (token == operator_add)
                r = op1 + op2;
            else if (token == operator_sub)
                r = op1 - op2;
            else if (token == operator_land)
                r = op1 && op2;
            else if (token == operator_band)
                r = (__int64)op1 & (__int64)op2;
            else if (token == operator_lor)
                r = op1 || op2;
            else if (token == operator_bor)
                r = (__int64)op1 | (__int64)op2;
            else if (token == operator_xor)
                r = (__int64)op1 ^ (__int64)op2;
            else if (token == operator_nor)
                r = ~((__int64)op1 | (__int64)op2);
            else if (token == operator_nand)
                r = ~((__int64)op1 & (__int64)op2);
            else if (token == operator_iseq)
                r = op1 == op2;
            else if (token == operator_ne)
                r = op1 != op2;
            else if (token == operator_shl)
                r = (__int64)op1 << (__int64)op2;
            else if (token == operator_shr)
                r = (__int64)op1 >> (__int64)op2;
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

bool CSVExpression::IsNumbric(const char* chValue)
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

unsigned __int64 CSVExpression::GetOidValue(const string &chOID)
{
    unsigned __int64 ulValue = 0;

    int nPos = static_cast<int>(chOID.find("."));
    string szOID(chOID), szRight("value");
    if(nPos > 0)
    {
        szOID = chOID.substr(0, nPos);
        szRight = chOID.substr(nPos + 1);
    }

    CSVResultRow svrow = m_ResultList.find(szOID);
    if(svrow != m_ResultList.end())
    {
        CSVField svitem = svrow->second.find(szRight);
        if(svitem != svrow->second.end())
            ulValue = svitem->second;
    }
    return ulValue;
}