#include "ast.h"

#include "error.h"
#include "parser.h"
#include "token.h"
#include "visitor.h"


/*
 * Accept
 */

void EmptyStmt::Accept(Visitor* v) {
    v->VisitEmptyStmt(this);
}

void LabelStmt::Accept(Visitor* v) {
    v->VisitLabelStmt(this);
}

void IfStmt::Accept(Visitor* v) {
    v->VisitIfStmt(this);
}

void JumpStmt::Accept(Visitor* v) {
    v->VisitJumpStmt(this);
}

void ReturnStmt::Accept(Visitor* v) {
    v->VisitReturnStmt(this);
}

void CompoundStmt::Accept(Visitor* v) {
    v->VisitCompoundStmt(this);
}

void BinaryOp::Accept(Visitor* v) {
    v->VisitBinaryOp(this);
}

void UnaryOp::Accept(Visitor* v) {
    v->VisitUnaryOp(this);
}

void ConditionalOp::Accept(Visitor* v) {
    v->VisitConditionalOp(this);
}

void FuncCall::Accept(Visitor* v) { 
    v->VisitFuncCall(this);
}
/*
void Variable::Accept(Visitor* v) {
    v->VisitVariable(this);
}
*/
void Constant::Accept(Visitor* v) {
    v->VisitConstant(this);
}

void TempVar::Accept(Visitor* v) {
    v->VisitTempVar(this);
}

void FuncDef::Accept(Visitor* v) {
    v->VisitFuncDef(this);
}

void TranslationUnit::Accept(Visitor* v) {
    v->VisitTranslationUnit(this);
}


long long BinaryOp::EvalInteger(const Token* errTok)
{
    // TypeChecking should make sure of this constrains
    assert(_ty->IsInteger());

#define L   _lhs->EvalInteger(errTok)
#define R   _rhs->EvalInteger(errTok)

    //bool res = true;
    switch (_op) {
    case '+':
        return L + R;
    case '-':
        return L - R;
    case '*':
        return L * R;
    case '/':
    case '%':
        {
            int l = L, r = R;
            if (r == 0)
                ;//Error(this, "division by zero");
            return _op == '%'? (l % r): (l / r);
        }
    case '<':
        return L < R;
    case '>':
        return L > R;
    case '|':
        return L | R;
    case '&':
        return L & R;
    case '^':
        return L ^ R;
    case Token::LEFT_OP:
        return L << R;
    case Token::RIGHT_OP:
        return L >> R;
    case Token::AND_OP:
        return L && R;
    case Token::OR_OP:
        return L || R;
    case '=':  
    case ',':
        return R;
    default:
        assert(0);
    }
}

/*
 * Unary Operators
 */
bool UnaryOp::IsLVal(void) const {
    // only deref('*') op is lvalue;
    // so it's only deref will override this func
    return (Token::DEREF == _op);
}

long long UnaryOp::EvalInteger(const Token* errTok)
{
#define VAL _operand->EvalInteger(errTok)

    switch (_op) {
    case Token::PLUS:
        return VAL;
    case Token::MINUS:
        return -VAL;
    case '~':
        return ~VAL;
    case '!':
        return !VAL;
    case Token::CAST:
        return VAL;
    default:
        Error(errTok->Coord(), "expect constant integer");
    }

    return 0;   // Make compiler happy
}

/*
 * Conditional Operator
 */

long long ConditionalOp::EvalInteger(const Token* errTok)
{
    int cond = _cond->EvalInteger(errTok);
    if (cond) {
        return _exprTrue->EvalInteger(errTok);
    } else {
        return _exprFalse->EvalInteger(errTok);
    }
}

long long FuncCall::EvalInteger(const Token* errTok) {
    Error(errTok->Coord(),
            "function call is not allowed in constant expression");
    return 0;   // Make compiler happy
}

long long Identifier::EvalInteger(const Token* errTok) {
    Error(errTok->Coord(), "identifier in constant expression");
    return 0;   // Make compiler happy
}

long long Constant::EvalInteger(const Token* errTok) {
    if (_ty->IsFloat()) {
        Error(errTok->Coord(), "expect integer, but get floating");
    }
    return _ival;
}

long long TempVar::EvalInteger(const Token* errTok) {
    Error(errTok->Coord(),
            "function call is not allowed in constant expression");
    return 0;   // Make compiler happy
}


