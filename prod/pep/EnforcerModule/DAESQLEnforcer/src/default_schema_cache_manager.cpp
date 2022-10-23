#include "default_schema_cache_manager.h"

#include <exception>
#include <thread>

#include "commfun.h"
#include "DAELog.h"

// region Getters
std::string DefaultSchemaCacheManager::GetDefaultSchema(const std::string &dbname,
                                                        const std::string &user_name) noexcept {
  try {
    if (user_name.empty() || dbname.empty()) {
      theLog->WriteLog(log_warning,
                         R"(Try to Get default schema, but username[%s] or dbname[%s] is empty. Use default Schema "%s")",
                         user_name.c_str(),
                         dbname.c_str(),
                         kDefaultSchema_);
      return kDefaultSchema_;
    }

    const auto key = CommonFun::StringFormat(kKeyFormat, dbname.c_str(), user_name.c_str());

    {
      std::lock_guard<std::mutex> lock{mutex_};

      if (const auto default_schema = default_schema_cache_.get(key)) {
        theLog->WriteLog(log_trace,
                         R"(Get default schema "%s" from cache by key "%s".)",
                         default_schema.value().c_str(),
                         key.c_str());
        return default_schema.value();
      }
    }

    std::thread query_default_schema_thread{[this, dbname, user_name, key] {
      try {
        auto queryed_schema = QueryDefaultSchema(dbname, user_name);

        if (queryed_schema.empty()) {
          queryed_schema = kDefaultSchema_;

          theLog->WriteLog(log_warning,
                           R"(Queried default schema for user "%s" in "%s" database is empty. Use the default schema "%s")",
                           user_name.c_str(),
                           dbname.c_str(),
                           kDefaultSchema_);
        }

        std::lock_guard<std::mutex> lock{mutex_};
        default_schema_cache_.insert(key, queryed_schema);
        theLog->WriteLog(log_trace,
                         R"(Insert default schema cache: key: "%s" value: "%s")",
                         key.c_str(),
                         queryed_schema.c_str());
        data_cond_.notify_one();
      } catch (const std::exception& e) {
        theLog->WriteLog(log_error,
                         R"(Error happened when get default schema for user "%s" in "%s" database: %s)",
                         user_name.c_str(),
                         dbname.c_str(),
                         e.what());
      }
    }};

    query_default_schema_thread.detach();

    {
      std::unique_lock<std::mutex> lock{mutex_};

      if (data_cond_.wait_for(lock,
                              std::chrono::seconds(kTimeout),
                              [&] { return default_schema_cache_.get(key).is_initialized(); })) {
          const auto schema = default_schema_cache_.get(key);

          theLog->WriteLog(log_debug,
                           R"(Queried default schema "%s" for user "%s" in "%s" database.)",
                           schema.value().c_str(),
                           user_name.c_str(),
                           dbname.c_str());
          return schema.value();
      } else {
        theLog->WriteLog(log_debug,
                         R"(Timeout to query default schema for user "%s" in "%s" database. Use the default schema "%s")",
                         user_name.c_str(),
                         dbname.c_str(),
                         kDefaultSchema_);
        return kDefaultSchema_;
      }
    }
  } catch (const std::exception& e) {
    theLog->WriteLog(log_error,
                     R"(Failed to get default schema for user "%s" in "%s" database: %s)",
                     user_name.c_str(),
                     dbname.c_str(),
                     e.what());
    return kDefaultSchema_;
  }
}
// endregion Getters

// region Other functions
std::string DefaultSchemaCacheManager::QueryDefaultSchema(const std::string &dbname,
                                                          const std::string &user_name) noexcept {
  std::string default_schema{};

  if (query_default_schema_fun_) {
    try {
      default_schema = query_default_schema_fun_(dbname, user_name);
    } catch (const std::exception& e) {
      theLog->WriteLog(log_error,
                       R"(Failed to query default schema for user "%s" in "%s" database: %s)",
                       user_name.c_str(),
                       dbname.c_str(),
                       e.what());
    }
  } else {
    theLog->WriteLog(log_error,
                     R"(Failed to query default schema for user "%s" in "%s" database: The query table columns function is not set.)",
                     user_name.c_str(),
                     dbname.c_str());
  }

  return default_schema;
}

void DefaultSchemaCacheManager::RefreshCache() noexcept {
  std::lock_guard<std::mutex> lock_guard{mutex_};
  default_schema_cache_.clear();
  theLog->WriteLog(log_debug, "Refresh the default schema cache.");
}
// endregion Other functions
