#ifndef Utilities_StaticAnalyzers_EDMChecker_h
#define Utilities_StaticAnalyzers_EDMChecker_h
#include <clang/AST/DeclCXX.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/StmtVisitor.h>
#include <clang/AST/ParentMap.h>
#include <clang/Analysis/CFGStmtMap.h>
#include <clang/Analysis/CallGraph.h>
#include <llvm/Support/SaveAndRestore.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CallEvent.h>
#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
#include <llvm/ADT/SmallString.h>

#include "CmsException.h"
#include "CmsSupport.h"

namespace clangcms {

class EDMChecker : public clang::ento::Checker<clang::ento::check::ASTDecl<clang::CXXRecordDecl>,
						clang::ento::check::ASTDecl<clang::CXXMethodDecl>,
						clang::ento::check::ASTDecl<clang::FunctionTemplateDecl> > 
{
  mutable clang::OwningPtr< clang::ento::BugType> BT;


public:
  void checkASTDecl(const clang::CXXRecordDecl *CRD, clang::ento::AnalysisManager& mgr,
                    clang::ento::BugReporter &BR) const ;

  void checkASTDecl(const clang::CXXMethodDecl *CMD, clang::ento::AnalysisManager& mgr,
                    clang::ento::BugReporter &BR) const ;

  void checkASTDecl(const clang::FunctionTemplateDecl *TD, clang::ento::AnalysisManager& mgr,
                    clang::ento::BugReporter &BR) const ;


private:
  CmsException m_exception;
};

}
#endif
