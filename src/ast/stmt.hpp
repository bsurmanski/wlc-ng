#ifndef _STMT_HPP
#define _STMT_HPP

#include "common/dynarray.hpp"
#include "common/string.hpp"
#include "common/stringFormatter.hpp"

class Expr;

class CompoundStmt;
class LabelStmt;
class CaseStmt;
class IfStmt;
class LoopStmt;
class WhileStmt;
class DoWhileStmt;
class ForStmt;
class ForEachStmt;
class UseStmt;
class AssignStmt;
class GotoStmt;
class BreakStmt;
class ContinueStmt;
class ReturnStmt;

class Stmt {
    public:
    virtual ~Stmt();

    virtual CompoundStmt    *asCompoundStmt();
    virtual LabelStmt       *asLabelStmt();
    virtual CaseStmt        *asCaseStmt();
    virtual IfStmt          *asIfStmt();
    virtual LoopStmt        *asLoopStmt();
    virtual WhileStmt       *asWhileStmt();
    virtual DoWhileStmt     *asDoWhileStmt();
    virtual ForStmt         *asForStmt();
    virtual ForEachStmt     *asForEachStmt();
    virtual UseStmt         *asUseStmt();
    virtual AssignStmt      *asAssignStmt();
    virtual GotoStmt        *asGotoStmt();
    virtual BreakStmt       *asBreakStmt();
    virtual ContinueStmt    *asContinueStmt();
    virtual ReturnStmt      *asReturnStmt();

    virtual bool isCompoundStmt();
    virtual bool isLabelStmt();
    virtual bool isCaseStmt();
    virtual bool isIfStmt();
    virtual bool isLoopStmt();
    virtual bool isWhileStmt();
    virtual bool isDoWhileStmt();
    virtual bool isForStmt();
    virtual bool isForEachStmt();
    virtual bool isUseStmt();
    virtual bool isAssignStmt();
    virtual bool isGotoStmt();
    virtual bool isBreakStmt();
    virtual bool isContinueStmt();
    virtual bool isReturnStmt();

    virtual String serialized();
    virtual void serialize(StringFormatter &sfmt);
};

class CompoundStmt : public Stmt {
    DynArray<Stmt*> stmts;

    public:
    CompoundStmt(DynArray<Stmt*> stmts);

    virtual CompoundStmt *asCompoundStmt();
    virtual void serialize(StringFormatter &sfmt);
};

class LabelStmt : public Stmt {
    String id;

    public:
    LabelStmt(String _id);
    virtual LabelStmt *asLabelStmt();
    virtual void serialize(StringFormatter &sfmt);
};

class CaseStmt : public Stmt {
    public:
    virtual CaseStmt *asCaseStmt();
};

class IfStmt : public Stmt {
    public:
    virtual IfStmt *asIfStmt();
};

class LoopStmt : public Stmt {
    public:
    virtual LoopStmt *asLoopStmt();
};

class WhileStmt : public LoopStmt {
    public:
    virtual WhileStmt *asWhileStmt();
};

class DoWhileStmt : public LoopStmt {
    public:
    virtual DoWhileStmt *asDoWhileStmt();
};

class ForStmt : public LoopStmt {
    public:
    virtual ForStmt *asForStmt();
};

class ForEachStmt : public LoopStmt {
    public:
    virtual ForEachStmt *asForEachStmt();
};

class UseStmt : public Stmt {
    public:
    virtual UseStmt *asUseStmt();
};

class AssignStmt : public Stmt {
    Expr *lhs;
    Expr *rhs;

    public:
    AssignStmt(Expr *_lhs, Expr *_rhs);
    virtual AssignStmt *asAssignStmt();
};

class AuxAssignStmt : public Stmt {
    Expr *lhs;
    Expr *rhs;
    public:
    AuxAssignStmt(Expr *_lhs, Expr *_rhs);
};

class AddAssignStmt : public AuxAssignStmt {
};

class SubAssignStmt : public AuxAssignStmt {
};

class MulAssignStmt : public AuxAssignStmt {
};

class DivAssignStmt : public AuxAssignStmt {
};

class ModAssignStmt : public AuxAssignStmt {
};

class LShiftAssignStmt : public AuxAssignStmt {
};

class RShiftAssignStmt : public AuxAssignStmt {
};

class BitAndAssignStmt : public AuxAssignStmt {
};

class BitOrAssignStmt : public AuxAssignStmt {
};

class BitXorAssignStmt : public AuxAssignStmt {
};

class GotoStmt : public Stmt {
    String id;

    public:
    GotoStmt(String _id);
    virtual GotoStmt *asGotoStmt();
    virtual void serialize(StringFormatter &sfmt);
};

class BreakStmt : public Stmt {
    public:
    virtual BreakStmt *asBreakStmt();
    virtual void serialize(StringFormatter &sfmt);
};

class ContinueStmt : public Stmt {
    public:
    virtual ContinueStmt *asContinueStmt();
    virtual void serialize(StringFormatter &sfmt);
};

class ReturnStmt : public Stmt {
    Expr *value;
    public:
    ReturnStmt(Expr *_value);
    virtual ~ReturnStmt();
    virtual ReturnStmt *asReturnStmt();
    virtual void serialize(StringFormatter &sfmt);
};

#endif
