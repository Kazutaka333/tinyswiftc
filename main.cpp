#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/IR/Module.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>
#include <memory>

int main(int argc, char *argv[]) {
  // Your code to use the lexer functionality goes here
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <input_file>\n";
    return 1;
  }
  // check if --debug flag is supplied

  std::string FileName;
  for (int I = 1; I < argc; ++I) {
    if (std::string(argv[I]) == "--debug") {
      debugEnabled = true;
    } else {
      FileName = argv[I];
    }
  }

  Lexer LexerInstance = Lexer(FileName);
  std::optional<Token> Token;

  Parser ParserInstance(LexerInstance);
  auto Context = std::make_unique<llvm::LLVMContext>();
  auto Module = std::make_unique<llvm::Module>(FileName, *Context);
  auto Builder = std::make_unique<llvm::IRBuilder<>>(*Context);

  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();
  auto TargetTriple = llvm::sys::getDefaultTargetTriple();

  Module->setTargetTriple(TargetTriple);

  std::string Error;
  auto Target = llvm::TargetRegistry::lookupTarget(TargetTriple, Error);

  // Print an error and exit if we couldn't find the requested target.
  // This generally occurs if we've forgotten to link in the TargetRegistry or
  // we have a bogus target triple.
  if (!Target) {
    llvm::errs() << Error;
    return 1;
  }

  auto CPU = "generic";
  auto Features = "";

  llvm::TargetOptions Opt;
  auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features,
                                                   Opt, llvm::Reloc::PIC_);

  Module->setDataLayout(TargetMachine->createDataLayout());

  ParserInstance.root->codegen(*Context, *Module, *Builder);

  // write LLVM IR
  auto IRFileName = "build/output.ll";
  std::error_code IREC;
  llvm::raw_fd_ostream IrOutput(IRFileName, IREC);
  Module->print(IrOutput, nullptr);

  auto OutputFileName = "build/output.o";
  std::error_code EC;
  llvm::raw_fd_ostream Dest(OutputFileName, EC,
                            llvm::sys::fs::OpenFlags::OF_None);

  llvm::legacy::PassManager Pass;
  auto FileType = llvm::CodeGenFileType::ObjectFile;

  if (TargetMachine->addPassesToEmitFile(Pass, Dest, nullptr, FileType)) {
    llvm::errs() << "TargetMachine can't emit a file of this type";
    return 1;
  }
  Pass.run(*Module);
  Dest.flush();

  return 0;
}
