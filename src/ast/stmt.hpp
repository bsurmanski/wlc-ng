#ifndef _STMT_HPP
#define _STMT_HPP

class Stmt {
};

class CompoundStmt : public Stmt {
};

class LabelStmt : public Stmt {
};

class CaseStmt : public Stmt {
};

class IfStmt : public Stmt {
};

class LoopStmt : public Stmt {
};

class WhileStmt : public LoopStmt {
};

class DoWhileStmt : public LoopStmt {
};

class ForStmt : public LoopStmt {
};

class ForEachStmt : public LoopStmt {
};

class UseStmt : public Stmt {
};

class AssignStmt : public Stmt {
};

class GotoStmt : public Stmt {
};

class BreakStmt : public Stmt {
};

class ContinueStmt : public Stmt {
};

class ReturnStmt : public Stmt {
};

#endif
