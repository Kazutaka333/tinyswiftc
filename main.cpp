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

  std::string fileName;
  for (int i = 1; i < argc; ++i) {
    if (std::string(argv[i]) == "--debug") {
      debugEnabled = true;
    } else {
      fileName = argv[i];
    }
  }

  Lexer lexer = Lexer(fileName);
  std::optional<Token> token;

  Parser parser(lexer);
  auto Context = std::make_unique<llvm::LLVMContext>();
  auto Module = std::make_unique<llvm::Module>(fileName, *Context);
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

  llvm::TargetOptions opt;
  auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features,
                                                   opt, llvm::Reloc::PIC_);

  Module->setDataLayout(TargetMachine->createDataLayout());

  parser.root->codegen(*Context, *Module, *Builder);

  // write LLVM IR
  auto IRFileName = "build/output.ll";
  std::error_code IREC;
  llvm::raw_fd_ostream irOutput(IRFileName, IREC);
  Module->print(irOutput, nullptr);

  auto FileName = "build/output.o";
  std::error_code EC;
  llvm::raw_fd_ostream dest(FileName, EC, llvm::sys::fs::OpenFlags::OF_None);

  llvm::legacy::PassManager pass;
  auto FileType = llvm::CodeGenFileType::ObjectFile;

  if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    llvm::errs() << "TargetMachine can't emit a file of this type";
    return 1;
  }
  pass.run(*Module);
  dest.flush();

  return 0;
}
