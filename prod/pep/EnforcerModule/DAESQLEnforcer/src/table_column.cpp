#include "table_column.h"

// region Constructors and assignment operators
TableColumn::TableColumn(const std::string &column_name, const std::string &column_type)
    : column_name_(column_name),
      column_type_(MaskItem::GetDataType(EMDB_DB_SQLSERVER, column_type)) {}
// endregion Constructors and assignment operators
