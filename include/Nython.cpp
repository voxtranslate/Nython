#include <Nython.hpp>

namespace nython {
  // Constructor
  Nython::Nython(){
  }

  // Run code from a source file
  int Nython::run(reader::SourceCode& source) {
    std::ofstream out("cache_dir/runtime.log");
    auto lexer = lexer::Lexer(source);
    // Tokenize the source string
    auto tokens = lexer.tokenize();
    if (lexer.hasErrors()) {
        lexer.statistics();
        return NYTHON_EXIT_DATAERR;
    }
    out << lexer.toString();
    out.close();
    return NYTHON_EXIT_OK;
  }

  int Nython::run_repl() {
    return NYTHON_EXIT_OK;
  }

  // Run code from a file
  int Nython::run_file(const std::string& file) {
    try {
      // Run the source from the specified file
      auto source = reader::SourceCode(file);
  	  return run(source);
    } catch (exception::SourceException& ex) {
      fmt::print(fmt::fg(fmt::color::crimson), "{}\n", ex.message());
      return NYTHON_EXIT_IOERR;
    } catch(kernel::RegexException& ex) {
        fmt::print(fmt::fg(fmt::color::crimson), "{}\n", ex.message());
        return NYTHON_EXIT_IOERR;
    } catch (exception::SourceException* ex) {
      fmt::print(fmt::fg(fmt::color::crimson), "{}\n", ex->message());
      delete ex;
      return NYTHON_EXIT_IOERR;
    } catch(kernel::RegexException* ex) {
        fmt::print(fmt::fg(fmt::color::crimson), "{}\n", ex->message());
        delete ex;
        return NYTHON_EXIT_IOERR;
    } catch(exception::Exception* ex) {
        fmt::print(fmt::fg(fmt::color::crimson), "{}\n", ex->message());
        delete ex;
        return NYTHON_EXIT_IOERR;
    } catch(exception::UnexpectedCharError& ex) {
        fmt::print(fmt::fg(fmt::color::crimson), "UnexpectedCharError: {}\n", ex.message());
        return NYTHON_EXIT_IOERR;
    } catch(std::bad_alloc& ex) {
        fmt::print(fmt::fg(fmt::color::crimson), "Allocation failure: can not allocate more memory: {}\n", ex.what());
        return NYTHON_EXIT_IOERR;
    } catch(std::overflow_error& ex) {
        fmt::print(fmt::fg(fmt::color::crimson), "OverFlow Error: can not continue any more. {}\n", ex.what());
        return NYTHON_EXIT_IOERR;
    } catch(std::system_error& ex) {
        fmt::print(fmt::fg(fmt::color::crimson), "A System error occurred with value: {}\n", ex.what());
        return NYTHON_EXIT_IOERR;
    } catch(std::runtime_error& ex) {
        fmt::print(fmt::fg(fmt::color::crimson), "During runtime this error occur : {}\n", ex.what());
        return NYTHON_EXIT_IOERR;
    } catch(std::exception& ex) {
        fmt::print(fmt::fg(fmt::color::crimson), "Undefined Exception occur with Value: {}\n", ex.what());
        return NYTHON_EXIT_IOERR;
    }
  }
}
