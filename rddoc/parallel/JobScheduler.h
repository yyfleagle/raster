/*
 * Copyright (C) 2017, Yeolar
 */

#pragma once

#include <map>
#include <vector>
#include "rddoc/coroutine/FiberManager.h"
#include "rddoc/net/Actor.h"
#include "rddoc/parallel/DAG.h"
#include "rddoc/parallel/Graph.h"
#include "rddoc/parallel/JobExecutor.h"
#include "rddoc/util/ReflectObject.h"

namespace rdd {

class ActorDAG : public DAG {
public:
  void execute(const ExecutorPtr& executor) {
    Singleton<Actor>::get()->execute(executor);
  }
};

typedef JobGraph<std::string> JobGraph;

class JobGraphManager {
public:
  JobGraphManager() {}

  JobGraph& getGraph(const std::string& key) {
    return graphs_[key];
  }

private:
  std::map<std::string, JobGraph> graphs_;
};

class JobScheduler {
public:
  JobScheduler() {}

  JobScheduler(const JobGraph& graph,
               const JobExecutor::ContextPtr& ctx = nullptr) {
    init(graph, ctx);
  }
  JobScheduler(const std::string& key,
               const JobExecutor::ContextPtr& ctx = nullptr) {
    init(Singleton<JobGraphManager>::get()->getGraph(key), ctx);
  }

  void add(const ExecutorPtr& executor,
           const std::string& name,
           const std::vector<std::string>& next) {
    map_[name] = dag_.add(executor);
    graph_.add(name, next);
  }

  void add(JobExecutor* executor,
           const JobExecutor::ContextPtr& ctx,
           const std::string& name,
           const std::vector<std::string>& next) {
    executor->setName(name);
    executor->setContext(ctx);
    add(ExecutorPtr(executor), name, next);
  }

  void add(const JobExecutor::ContextPtr& ctx,
           const std::string& name,
           const std::vector<std::string>& next) {
    auto clsname = name.substr(0, name.find(':'));
    add(makeReflectObject<JobExecutor>(clsname), ctx, name, next);
  }

  void run() {
    if (!dag_.empty()) {
      setDependency();
      ExecutorPtr executor = getCurrentExecutor();
      executor->addCallback(
          std::bind(&ActorDAG::schedule, &dag_, dag_.go(executor)));
      FiberManager::yield();
    }
  }

private:
  void init(const JobGraph& graph, const JobExecutor::ContextPtr& ctx) {
    for (auto& p : graph) {
      add(ctx, p.name, p.next);
    }
  }

  void setDependency() {
    for (auto& p : graph_) {
      for (auto& q : p.next) {
        dag_.dependency(map_[p.name], map_[q]);
      }
    }
    setup_ = true;
  }

  ActorDAG dag_;
  JobGraph graph_;
  std::map<std::string, DAG::Key> map_;
  bool setup_{false};
};

}
