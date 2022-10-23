#include "table_metadata_cache_manager.h"

#include <exception>

#include "commfun.h"
#include "DAELog.h"

// region Getters
TableMetadata TableMetadataCacheManager::GetTableMetadata(const std::string &dbname,
                                                          const std::string &schema,
                                                          const std::string &table) noexcept {
  std::lock_guard<std::mutex> lock_guard{mutex_};

  const auto key = CommonFun::StringFormat(kKeyFormat, dbname.c_str(), schema.c_str(), table.c_str());
  if (auto table_metadata = table_metadata_cache_.get(key)) {
    return table_metadata.value();
  } else {
    return QueryTableMetadata(dbname, schema, table);
  }
}
// endregion Getters

// region Other functions
TableMetadata TableMetadataCacheManager::QueryTableMetadata(const std::string &dbname,
                                                   const std::string &schema,
                                                   const std::string &table) noexcept {
  TableMetadata table_metadata{dbname, schema, table};

  if (query_table_columns_fun_) {
    try {
      const auto table_columns = query_table_columns_fun_(dbname, schema, table);
      table_metadata.AddColumns(table_columns);

      const auto key = CommonFun::StringFormat(kKeyFormat, dbname.c_str(), schema.c_str(), table.c_str());
      table_metadata_cache_.insert(key, table_metadata);
    } catch (const std::exception& e) {
      theLog->WriteLog(log_error,
                       R"(Failed to query table metadata for "%s"."%s"."%s": %s)",
                       dbname.c_str(),
                       schema.c_str(),
                       table.c_str(),
                       e.what());
    }
  } else {
    theLog->WriteLog(log_error,
                     R"(Failed to query table metadata for "%s"."%s"."%s": The query table columns function is not set.)",
                     dbname.c_str(),
                     schema.c_str(),
                     table.c_str());
  }

  return table_metadata;
}

void TableMetadataCacheManager::RefreshCache() noexcept {
  std::lock_guard<std::mutex> lock_guard{mutex_};
  table_metadata_cache_.clear();
  theLog->WriteLog(log_debug, "Refresh the table metadata cache.");
}
// endregion Other functions
