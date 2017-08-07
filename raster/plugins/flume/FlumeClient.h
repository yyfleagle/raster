/*
 * Copyright (C) 2017, Yeolar
 */

#pragma once

#include <functional>
#include <memory>
#include <vector>
#include "raster/plugins/flume/gen-cpp/Scribe.h"
#include "raster/plugins/flume/gen-cpp/scribe_types.h"
#include "raster/protocol/thrift/SyncClient.h"
#include "raster/util/Conv.h"
#include "raster/util/Logging.h"
#include "raster/util/ProducerConsumerQueue.h"

namespace rdd {

class FlumeClient {
public:
  FlumeClient(const ClientOption& option,
         const std::string& category,
         const std::string& logDir = "/opt/logs/logs/flume_delay")
    : category_(category), logDir_(logDir), queue_(10240) {
    client_ = std::make_shared<TSyncClient<ScribeClient>>(option);
    if (!client_->connect()) {
      RDDLOG(ERROR) << "connect flume client failed";
    }
  }

  ~FlumeClient() {
    send();
  }

  bool add(const std::string& message) {
    if (queue_.isFull()) {
      return false;
    }
    LogEntry entry;
    entry.category = category_;
    entry.message = message + "\n";
    queue_.write(entry);
    return true;
  }

  void send() {
    std::vector<LogEntry> entries;
    while (!queue_.isEmpty()) {
      LogEntry entry;
      queue_.read(entry);
      entries.push_back(entry);
    }
    sendInBatch(entries);
  }

private:
  void sendInBatch(const std::vector<LogEntry>& entries);
  void writeToDisk(const std::vector<LogEntry>& entries);

  std::string category_;
  std::string logDir_;
  ProducerConsumerQueue<LogEntry> queue_;
  std::shared_ptr<TSyncClient<ScribeClient>> client_;
};

} // namespace rdd