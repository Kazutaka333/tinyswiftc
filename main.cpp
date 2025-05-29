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

int main() {
  // Your code to use the lexer functionality goes here
  std::string fileName = "example.swift";
  Lexer lexer = Lexer(fileName);
  std::optional<Token> token;

  Parser parser(lexer);
  auto context = std::make_unique<llvm::LLVMContext>();
  auto module = std::make_unique<llvm::Module>(fileName, *context);
  auto builder = std::make_unique<llvm::IRBuilder<>>(*context);

  llvm::InitializeAllTargetInfos();
  llvm::InitializeAllTargets();
  llvm::InitializeAllTargetMCs();
  llvm::InitializeAllAsmParsers();
  llvm::InitializeAllAsmPrinters();
  auto TargetTriple = llvm::sys::getDefaultTargetTriple();
  // std::cout << TargetTriple << std::endl;
  module->setTargetTriple(TargetTriple);

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

  module->setDataLayout(TargetMachine->createDataLayout());

  // Create function type: int main()
  llvm::FunctionType *funcType =
      llvm::FunctionType::get(llvm::Type::getInt32Ty(*context), false);

  // Create function
  llvm::Function *mainFunc = llvm::Function::Create(
      funcType, llvm::Function::ExternalLinkage, "main", *module);

  // Create entry basic block
  llvm::BasicBlock *entry = llvm::BasicBlock::Create(*context, "", mainFunc);

  builder->SetInsertPoint(entry);
  builder->CreateRet(builder->getInt32(3)); // Return 0

  // write LLVM IR
  auto IRFileName = "output.ll";
  std::error_code IREC;
  llvm::raw_fd_ostream irOutput(IRFileName, IREC);
  module->print(irOutput, nullptr);

  auto FileName = "output.o";
  std::error_code EC;
  llvm::raw_fd_ostream dest(FileName, EC, llvm::sys::fs::OpenFlags::OF_None);

  llvm::legacy::PassManager pass;
  auto FileType = llvm::CodeGenFileType::ObjectFile;

  if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
    llvm::errs() << "TargetMachine can't emit a file of this type";
    return 1;
  }
  pass.run(*module);
  dest.flush();

  return 0;
}
