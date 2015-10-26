#include "stmt.hpp"

#include <stddef.h>

/*
 * Stmt
 */

CompoundStmt *Stmt::asCompoundStmt() {
    return NULL;
}

LabelStmt *Stmt::asLabelStmt() {
    return NULL;
}

CaseStmt *Stmt::asCaseStmt() {
    return NULL;
}

IfStmt *Stmt::asIfStmt() {
    return NULL;
}

LoopStmt *Stmt::asLoopStmt() {
    return NULL;
}

WhileStmt *Stmt::asWhileStmt() {
    return NULL;
}

DoWhileStmt *Stmt::asDoWhileStmt() {
    return NULL;
}

ForStmt *Stmt::asForStmt() {
    return NULL;
}

ForEachStmt *Stmt::asForEachStmt() {
    return NULL;
}

UseStmt *Stmt::asUseStmt() {
    return NULL;
}

AssignStmt *Stmt::asAssignStmt() {
    return NULL;
}

GotoStmt *Stmt::asGotoStmt() {
    return NULL;
}

BreakStmt *Stmt::asBreakStmt() {
    return NULL;
}

ContinueStmt *Stmt::asContinueStmt() {
    return NULL;
}

ReturnStmt *Stmt::asReturnStmt() {
    return NULL;
}


bool Stmt::isCompoundStmt() {
    return (bool) asCompoundStmt();
}

bool Stmt::isLabelStmt() {
    return (bool) asLabelStmt();
}

bool Stmt::isCaseStmt() {
    return (bool) asCaseStmt();
}

bool Stmt::isIfStmt() {
    return (bool) asIfStmt();
}

bool Stmt::isLoopStmt() {
    return (bool) asLoopStmt();
}

bool Stmt::isWhileStmt() {
    return (bool) asWhileStmt();
}

bool Stmt::isDoWhileStmt() {
    return (bool) asDoWhileStmt();
}

bool Stmt::isForStmt() {
    return (bool) asForStmt();
}

bool Stmt::isForEachStmt() {
    return (bool) asForEachStmt();
}

bool Stmt::isUseStmt() {
    return (bool) asUseStmt();
}

bool Stmt::isAssignStmt() {
    return (bool) asAssignStmt();
}

bool Stmt::isGotoStmt() {
    return (bool) asGotoStmt();
}

bool Stmt::isBreakStmt() {
    return (bool) asBreakStmt();
}

bool Stmt::isContinueStmt() {
    return (bool) asContinueStmt();
}

bool Stmt::isReturnStmt() {
    return (bool) asReturnStmt();
}

/*
 * CompoundStmt
 */

CompoundStmt *CompoundStmt::asCompoundStmt() {
    return this;
}

CompoundStmt::CompoundStmt(DynArray<Stmt*> _stmts) : stmts(_stmts) {
}

/*
 * LabelStmt
 */

LabelStmt *LabelStmt::asLabelStmt() {
    return this;
}

/*
 * CastStmt
 */

CaseStmt *CaseStmt::asCaseStmt() {
    return this;
}

/*
 * IfStmt
 */

IfStmt *IfStmt::asIfStmt() {
    return this;
}

/*
 * LoopStmt
 */

LoopStmt *LoopStmt::asLoopStmt() {
    return this;
}

/*
 * WhileStmt
 */

WhileStmt *WhileStmt::asWhileStmt() {
    return this;
}

/*
 * DoWhileStmt
 */

DoWhileStmt *DoWhileStmt::asDoWhileStmt() {
    return this;
}

/*
 * ForStmt
 */

ForStmt *ForStmt::asForStmt() {
    return this;
}

/*
 * ForEachStmt
 */

ForEachStmt *ForEachStmt::asForEachStmt() {
    return this;
}

/*
 * UseStmt
 */

UseStmt *UseStmt::asUseStmt() {
    return this;
}


/*
 * AssignStmt
 */

AssignStmt::AssignStmt(Expr *_lhs, Expr *_rhs) : lhs(_lhs), rhs(_rhs) {
}

AssignStmt *AssignStmt::asAssignStmt() {
    return this;
}

/*
 * GotoStmt
 */

GotoStmt *GotoStmt::asGotoStmt() {
    return this;
}

/*
 * BreakStmt
 */

BreakStmt *BreakStmt::asBreakStmt() {
    return this;
}

/*
 * ContinueStmt
 */

ContinueStmt *ContinueStmt::asContinueStmt() {
    return this;
}

/*
 * ReturnStmt
 */

ReturnStmt *ReturnStmt::asReturnStmt() {
    return this;
}
