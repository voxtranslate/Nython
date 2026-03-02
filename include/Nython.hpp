#ifndef __NYTHON__HPP
#define __NYTHON__HPP

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Winline"

/// Custom inclusions
#include <Type.hpp>
#include <Util.hpp>
#include <Class.hpp>
#include <Object.hpp>
#include <Regex.hpp>
#include <Logger.hpp>
#include <Parser.hpp>
#include <Interpreter.hpp>
#include <Definitions.hpp>
#include <ConsoleManager.hpp>
#include <VirtualMachine.hpp>


namespace nython
{
  // Class that defines the high-level code of the application
  class Nython : public std::enable_shared_from_this<Nython>
  {
    private:
      // The virtual machine for the application
      std::shared_ptr<vm::VirtualMachine> vm_;


    public:
      // Constructor
      Nython();

      // Run code from a source file
      int run(reader::SourceCode& source);

      // Run code from a read-eval-print loop
      int run_repl();

      // Run code from a file
      int run_file(const std::string& file);
  };
}

#endif // __NYTHON__HPP
