#ifndef PNANA_FEATURES_SSH_CLIENT_H
#define PNANA_FEATURES_SSH_CLIENT_H

#include <string>

// 前向声明
namespace pnana {
namespace ui {
    struct SSHConfig;
}
}

namespace pnana {
namespace features {

// SSH 结果结构
struct SSHResult {
    bool success;
    std::string content;
    std::string error;
};

// SSH 客户端接口（通过 CGO 调用 Go 代码）
class SSHClient {
public:
    SSHClient();
    ~SSHClient();
    
    // 连接到 SSH 服务器并读取文件
    SSHResult readFile(const ui::SSHConfig& config);
    
    // 连接到 SSH 服务器并写入文件
    SSHResult writeFile(const ui::SSHConfig& config, const std::string& content);
    
private:
    void* go_client_;  // Go 客户端句柄（如果需要）
};

} // namespace features
} // namespace pnana

#endif // PNANA_FEATURES_SSH_CLIENT_H

