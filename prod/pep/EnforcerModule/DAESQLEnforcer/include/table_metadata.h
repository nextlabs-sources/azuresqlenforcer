#ifndef TABLE_METADATA_H_
#define TABLE_METADATA_H_

#include <string>
#include <vector>

#include "table_column.h"

class TableMetadata {
 public:
// region Constructors and assignment operators
  explicit TableMetadata() = default; // TODO: remove
  explicit TableMetadata(const std::string &database_name,
                         const std::string &schema_name,
                         const std::string &table_name)
      : database_name_(database_name),
        schema_name_(schema_name),
        table_name_(table_name),
        columns_(){}
// endregion Constructors and assignment operators

// region Getters
  std::vector<TableColumn> GetColumns() const noexcept {
    return columns_;
  }

  std::string GetDatabaseName() const noexcept {
    return database_name_;
  }

  std::string GetSchemaName() const noexcept {
    return schema_name_;
  }

  std::string GetTableName() const noexcept {
    return table_name_;
  }
// endregion Getters

// region Other functions
  void AddColumn(const TableColumn& table_column) noexcept {
    columns_.emplace_back(table_column);
  }

  void AddColumns(const std::vector<TableColumn>& table_columns) noexcept;

  void ClearColumns() noexcept {
    columns_.clear();
  }
// endregion Other functions

 private:
// region Data members
  std::vector<TableColumn> columns_;
  std::string database_name_;
  std::string schema_name_;
  std::string table_name_;
// endregion Data members
};


#endif //TABLE_METADATA_H_
