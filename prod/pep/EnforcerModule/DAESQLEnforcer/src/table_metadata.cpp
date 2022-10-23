#include "table_metadata.h"

// region Other functions
void TableMetadata::AddColumns(const std::vector<TableColumn> &table_columns) noexcept {
  for (const auto& column : table_columns) {
    AddColumn(column);
  }
}
// endregion Other functions
