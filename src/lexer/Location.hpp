#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <fmt/format.h>


namespace nython::lexer {

struct Location {

    uint32_t row         = 1;
    uint32_t column      = 1;
    std::string filename = (char*)"stdin";

    Location(): row{1}, column{1}, filename{"stdin"} {
    }

    Location(uint32_t r, uint32_t c, const std::string& filename): row(r), column(c), filename(filename) {
    }

    Location(const std::string& filename) : Location(uint32_t(1), uint32_t(1) , filename) {
    }

    Location(const Location& o): row(o.row), column(o.column), filename(o.filename) {
    }

    Location(Location&& o) : row(o.row), column(o.column), filename(o.filename) {
    }

    Location& operator=(const Location& o) {
        this->row      = o.row;
        this->column   = o.column;
        this->filename = o.filename;
        return *this;
    }

    Location& operator=(Location&& o) {
        if(this!=&o){
            this->row      = o.row;
            this->column   = o.column;
            this->filename = o.filename;
        }
        return *this;
    }

    inline void reset(int row = 1, int colunm = 1) {
        this->row    = row;
        this->column = column;
    }

    // Operator overloads
    // Return if the location equals another location
    bool operator==(const Location& other) {
        return row == other.row && column == other.column;
    }

    // Return if the location must be sorted before another location
    bool operator<(const Location& other) {
        if (row < other.row)
          return true;
        else if (column < other.column)
          return true;
        else
          return false;
    }

    friend std::ostream& operator<<(std::ostream& os, Location location) {
        location.write_to_stream(os);
        return os;
    }

    inline void write_to_stream(std::ostream& stream) const {
        stream << "In file: " <<  this->filename << " in row(" << this->row << ") and column(" << this->column << ")";
    }

    inline  operator std::string() {
        return toString();
    }

    inline  operator const char*() {
        return toString().c_str();
    }

    inline std::string toString() {
        std::stringstream os;
        os << "In file: " <<  this->filename << " in row(" << this->row << ") and column(" << this->column << ")";
        return os.str().c_str();
    }
};

}

namespace fmt
{
  using namespace nython;
  using namespace nython::lexer;

  // Class that defines a formatter for a location
  template <>
  struct formatter<Location> : formatter<std::string_view>
  {
    inline std::string stringify(Location location)
    {
      return fmt::format("line {}, col {}", location.row, location.column);
    }

    template <typename FormatContext>
    auto format(Location location, FormatContext& ctx)
    {
      return formatter<std::string_view>::format(stringify(location), ctx);
    }
  };
}
