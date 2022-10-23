#ifndef TABLE_METADATA_CACHE_MANAGER_H_
#define TABLE_METADATA_CACHE_MANAGER_H_

#include <functional>
#include <string>
#include <thread>
#include <map>
#include <mutex>

#include <boost/compute/detail/lru_cache.hpp>

#include "table_metadata.h"
#include "table_column.h"

class TableMetadataCacheManager {
 public:
// region Type alias
  using QueryTableColumnsFun = std::function<std::vector<TableColumn>(const std::string& dbname,
                                                                      const std::string& schema,
                                                                      const std::string& table)>;
// endregion Type alias

// region Constructors and assignment operators
  TableMetadataCacheManager(const TableMetadataCacheManager&) = delete;
  TableMetadataCacheManager& operator=(const TableMetadataCacheManager&) = delete;
// endregion Constructors and assignment operators

// region Getters
  TableMetadata GetTableMetadata(const std::string& dbname,
                                 const std::string& schema,
                                 const std::string& table) noexcept;
// endregion Getters

// region Setters
  void SetQueryTableColumnsFun(const QueryTableColumnsFun& query_table_columns_fun) noexcept {
    std::lock_guard<std::mutex> lock_guard{mutex_};
    query_table_columns_fun_ = query_table_columns_fun;
  }

  void SetRefreshInterval(unsigned int refresh_interval) noexcept {
    std::lock_guard<std::mutex> lock_guard{mutex_};
    refresh_interval_ = std::chrono::seconds(refresh_interval);
  }
// endregion Setters

// region Other functions
  static TableMetadataCacheManager& Instance() {
    static TableMetadataCacheManager cache_manager;
    return cache_manager;
  }

  void StartRefreshCache(unsigned int refresh_interval) noexcept {
    std::lock_guard<std::mutex> lock_guard{mutex_};

    refresh_interval_ = std::chrono::seconds(refresh_interval);
    std::thread t([&] {
      while (should_refresh_cache_) {
        std::this_thread::sleep_for(refresh_interval_);
        RefreshCache();
      }
    });
    t.detach();
  }

  void StopRefreshCache() noexcept {
    std::lock_guard<std::mutex> lock_guard{mutex_};
    should_refresh_cache_ = false;
  }
// endregion Other functions

 private:
// region Static constants
  static constexpr auto kKeyFormat = "%s:%s:%s";
// endregion Static constants

// region Constructors and assignment operators
  TableMetadataCacheManager() = default;
// endregion Constructors and assignment operators

// region Other functions
  TableMetadata QueryTableMetadata(const std::string& dbname,
                                   const std::string& schema,
                                   const std::string& table) noexcept;

  void RefreshCache() noexcept;
// endregion Other functions

// region Data members
  std::mutex mutex_{};
  QueryTableColumnsFun query_table_columns_fun_{};
  std::chrono::seconds refresh_interval_{60 * 60 * 24};
  bool should_refresh_cache_{true};
  boost::compute::detail::lru_cache<std::string, TableMetadata> table_metadata_cache_{1024};
// endregion Data members
};

#endif //TABLE_METADATA_CACHE_MANAGER_H_
