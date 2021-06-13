#include "config.h"

namespace sylar {

// Config::ConfigVarMap Config::s_datas;

ConfigVarBase::ptr Config::LookUpBase(const std::string &name){
    RWMutexType::ReadLock lock(GetMutex());
    auto iter = GetDatas().find(name);
    return iter == GetDatas().end() ? nullptr : iter->second;
}

static void ListAllMember(const std::string& prefix, const YAML::Node& node, 
                          std::list<std::pair<std::string, const YAML::Node>> &output) {
    if(prefix.find_first_not_of("abcdefghijklmnopqrstuvwxyz._0123456789") != std::string::npos){
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Config invalid name: " << prefix << " : " << node;
        return;
    }
    output.push_back(std::make_pair(prefix, node));
    if(node.IsMap()){
        for (auto iter = node.begin(); iter != node.end(); iter++){
            // std::cout << "prefix: " << prefix + "." + iter->first.Scalar() << std::endl;
            ListAllMember(prefix.empty() ? iter->first.Scalar() : prefix + "." + iter->first.Scalar(),
                          iter->second, output);
        }
    }
}

void Config::LoadFromYaml(const YAML::Node &root){
    std::list<std::pair<std::string, const YAML::Node>> all_node; 
    ListAllMember("", root, all_node);

    for (auto& i: all_node) {
        std::string key = i.first;
        if(key.empty()) {
            continue;
        }
        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        ConfigVarBase::ptr var = LookUpBase(key);
        if(var) {
            if(i.second.IsScalar()) {
                var->fromString(i.second.Scalar());
            }
            else {
                std::stringstream ss;
                ss << i.second;
                var->fromString(ss.str());
            }
        }
    }
}

void Config::Visit(std::function<void(ConfigVarBase::ptr)> cb) {
    RWMutexType::ReadLock lock(GetMutex());
    ConfigVarMap &m = GetDatas();
    for (auto iter = m.begin(); iter != m.end(); iter++) {
        cb(iter->second);
    }
}
}