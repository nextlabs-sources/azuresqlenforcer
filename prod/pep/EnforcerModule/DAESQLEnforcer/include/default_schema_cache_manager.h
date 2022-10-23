#ifndef DEFAULT_SCHEMA_CACHE_MANAGER_H_
#define DEFAULT_SCHEMA_CACHE_MANAGER_H_

#include <chrono>
#include <condition_variable>
#include <functional>
#include <string>
#include <mutex>

#include <boost/compute/detail/lru_cache.hpp>

class DefaultSchemaCacheManager {
 public:
// region Type alias
  using QueryDefaultSchemaFun = std::function<std::string(const std::string& dbname,
                                                          const std::string& user_name)>;
// endregion Type alias

// region Constructors and assignment operators
  DefaultSchemaCacheManager(const DefaultSchemaCacheManager&) = delete;
  DefaultSchemaCacheManager& operator=(const DefaultSchemaCacheManager&) = delete;
// endregion Constructors and assignment operators

// region Getters
  std::string GetDefaultSchema(const std::string& dbname,
                               const std::string& user_name) noexcept;
// endregion Getters

// region Setters
  void SetQueryDefaultSchemaFun(const QueryDefaultSchemaFun& query_default_schema_fun)  noexcept {
    std::lock_guard<std::mutex> lock_guard{mutex_};
    query_default_schema_fun_ = query_default_schema_fun;
  }

  void SetRefreshInterval(unsigned int refresh_interval) noexcept {
    std::lock_guard<std::mutex> lock_guard{mutex_};
    refresh_interval_ = std::chrono::seconds{refresh_interval};
  }
// endregion Setters

// region Other functions
  static DefaultSchemaCacheManager& Instance() {
    static DefaultSchemaCacheManager cache_manager;
    return cache_manager;
  }

  void StartRefreshCache(unsigned int refresh_interval) noexcept {
    std::lock_guard<std::mutex> lock_guard{mutex_};

    refresh_interval_ = std::chrono::seconds{refresh_interval_};
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
  static constexpr auto kKeyFormat = "%s:%s";
// endregion Static constants

// region Constructors and assignment operators
  DefaultSchemaCacheManager() = default;
// endregion Constructors and assignment operators

// region Other functions
  std::string QueryDefaultSchema(const std::string& dbname,
                                 const std::string& user_name) noexcept;

  void RefreshCache() noexcept;
// endregion Other functions

// region Data members
  static constexpr auto kDefaultSchema_ = "dbo";
  static constexpr auto kTimeout = 5;
  std::condition_variable data_cond_{};
  std::mutex mutex_{};
  QueryDefaultSchemaFun query_default_schema_fun_{};
  std::chrono::seconds refresh_interval_{60 * 60 * 24};
  bool should_refresh_cache_{true};
  boost::compute::detail::lru_cache<std::string, std::string> default_schema_cache_{1024};
// endregion Data members
};

#endif //DEFAULT_SCHEMA_CACHE_MANAGER_H_
