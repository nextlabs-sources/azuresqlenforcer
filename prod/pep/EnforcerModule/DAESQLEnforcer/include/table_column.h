#ifndef INCLUDE_TABLE_COLUMN_H_
#define INCLUDE_TABLE_COLUMN_H_

#include <string>

#include "EMMaskDef.h"

class TableColumn {
 public:
// region Constructors and assignment operators
  explicit TableColumn(const std::string& column_name, const std::string& column_type);
// endregion Constructors and assignment operators

// region Getters
  std::string GetColumnName() const noexcept {
    return column_name_;
  }

  EMDataType GetColumnType() const noexcept {
    return column_type_;
  }
// endregion Getters

 private:
// region Data members
  std::string column_name_;
  EMDataType column_type_;
// endregion Data members
};

#endif //INCLUDE_TABLE_COLUMN_H_
