#ifndef __NODE_MANAGER_H__
#define __NODE_MANAGER_H__

#include <cpprest/http_client.h>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

class NodeManager {
public:
    NodeManager(const std::string& health_check_url);
    std::vector<std::string> getNodeList();

private:
    void fetchNodeList();

    std::vector<std::string> node_list_;
    std::string health_check_url_;
    std::mutex mutex_;
};

#endif
