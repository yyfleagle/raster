/*
 * Copyright 2017 Yeolar
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "accelerator/event/EventLoop.h"
#include "raster/net/NetHub.h"
#include "raster/net/Service.h"

namespace rdd {

class Acceptor {
 public:
  Acceptor(std::shared_ptr<NetHub> hub);

  void addService(std::unique_ptr<Service> service);

  void configService(
      const std::string& name,
      int port,
      const TimeoutOption& timeout);

  void start();
  void stop();

 private:
  void listen(Service* service, int backlog = 64);

  std::shared_ptr<NetHub> hub_;
  std::unique_ptr<acc::EventLoop> loop_;
  std::map<std::string, std::unique_ptr<Service>> services_;
};

} // namespace rdd
