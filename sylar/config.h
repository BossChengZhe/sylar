#ifndef __SYLAR_CONFIG_H__
#define __SYLAR_CONFIG_H__

#include <memory>
#include <sstream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "sylar/log.h" 
#include <yaml-cpp/yaml.h>
#include <functional>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

namespace sylar {

class ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    /**
     * @brief 构造函数
     * @param[in] name 配置参数名称[0-9a-z_.]
     * @param[in] description 配置参数描述
     */
    ConfigVarBase(const std::string &name, const std::string description = "")
        :m_name(name),
        m_description(description) {
        std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
    }
    /**
     * @brief 析构函数
     */
    virtual ~ConfigVarBase(){}
    /**
     * @brief 返回配置参数名称
     */
    const std::string &getName() const { return m_name; }
    /**
     * @brief 返回配置参数的描述
     */
    const std::string &getDescription() const { return m_description; }
    /**
     * @brief 转成字符串
     */
    virtual std::string toString() = 0;
    /**
     * @brief 从字符串初始化值
     */
    virtual bool fromString(const std::string &val) = 0;
    virtual std::string getTypeName() const = 0;   

protected:
    /// 配置参数的名称
    std::string m_name;
    /// 配置参数的描述
    std::string m_description;
};

// F form type, T to_type
template<class F, class T>
class LexicalCast{
public:
    T operator()(const F& v) {
        return boost::lexical_cast<T>(v);
    }
};

template<class T>
class LexicalCast<std::string, std::vector<T>> {
public:
    std::vector<T> operator() (const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::vector<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); i++){
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::vector<T>, std::string> {
public:
    std::string operator() (const std::vector<T>& v){
        YAML::Node node;
        for(auto &i : v) {
            node.push_back(YAML::Node(  LexicalCast<T, std::string>()(i))); 
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<class T>
class LexicalCast<std::string, std::list<T>> {
public:
    std::list<T> operator() (const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::list<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); i++){
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::list<T>, std::string> {
public:
    std::string operator() (const std::list<T>& v){
        YAML::Node node;
        for(auto &i : v) {
            node.push_back(YAML::Node(  LexicalCast<T, std::string>()(i))); 
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<class T>
class LexicalCast<std::string, std::set<T>> {
public:
    std::set<T> operator() (const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::set<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); i++){
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::set<T>, std::string> {
public:
    std::string operator() (const std::set<T>& v){
        YAML::Node node;
        for(auto &i : v) {
            node.push_back(YAML::Node(  LexicalCast<T, std::string>()(i))); 
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<class T>
class LexicalCast<std::string, std::unordered_set<T>> {
public:
    std::unordered_set<T> operator() (const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::unordered_set<T> vec;
        std::stringstream ss;
        for (size_t i = 0; i < node.size(); i++){
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::unordered_set<T>, std::string> {
public:
    std::string operator() (const std::unordered_set<T>& v){
        YAML::Node node;
        for(auto &i : v) {
            node.push_back(YAML::Node(  LexicalCast<T, std::string>()(i))); 
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<class T>
class LexicalCast<std::string, std::map<std::string, T> > {
public:
    std::map<std::string, T> operator() (const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::map<std::string, T> vec;
        std::stringstream ss;
        for (auto iter = node.begin(); iter != node.end(); iter++) {
            ss.str("");
            ss << iter->second;
            vec.insert(std::make_pair(iter->first.Scalar(), LexicalCast<std::string, T>()(ss.str())));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::map<std::string, T>, std::string> {
public:
    std::string operator() (const std::map<std::string, T>& v){
        YAML::Node node;
        for(auto &i : v) {
            node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

template<class T>
class LexicalCast<std::string, std::unordered_map<std::string, T> > {
public:
    std::unordered_map<std::string, T> operator() (const std::string& v){
        YAML::Node node = YAML::Load(v);
        typename std::unordered_map<std::string, T> vec; 
        std::stringstream ss;
        for (auto iter = node.begin(); iter != node.end(); iter++) {
            ss.str("");
            ss << iter->second;
            vec.insert(std::make_pair(iter->first.Scalar(), LexicalCast<std::string, T>()(ss.str())));
        }
        return vec;
    }
};

template<class T>
class LexicalCast<std::unordered_map<std::string, T>, std::string> {
public:
    std::string operator() (const std::unordered_map<std::string, T>& v){
        YAML::Node node;
        for(auto &i : v) {
            node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

// FromStr T operator()(const std::string&)
// ToStr std::string operator()(const T&)
template<class T, class FromStr = LexicalCast<std::string, T> , 
                  class ToStr = LexicalCast<T, std::string>>
class ConfigVar : public ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    typedef std::function<void (const T &old_value, const T &new_value)> on_change_cb;
    /**
     * @brief 通过参数名,参数值,描述构造ConfigVar
     * @param[in] name 参数名称有效字符为[0-9a-z_.]
     * @param[in] default_value 参数的默认值
     * @param[in] description 参数的描述
     */
    ConfigVar(const std::string& name, const T& default_value, const std::string& description = "")
        :ConfigVarBase(name, description),
        m_val(default_value){
    }

    std::string toString() override {
        try {
            // return boost::lexical_cast<std::string>(m_val);
            return ToStr()(m_val);
        }
        catch (std::exception &e){
            SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ConfigVar::toString exception"
                                              << e.what() << " convert: " <<  typeid(m_val).name() << " to string";
        }
        return "";
    }

    bool fromString(const std::string &val) override {
        try {
            // m_val = boost::lexical_cast<T>(val);
            setValue(FromStr()(val));
        }
        catch(std::exception& e){
            SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ConfigVar::toString exception"
                                              << e.what() << " convert: string to" <<  typeid(m_val).name();
        }
        return false;
    }
    /**
     * @brief 获取当前参数的值
     */
    const T getValue() const { return m_val; }
    /**
     * @brief 设置当前参数的值
     * @details 如果参数的值有发生变化,则通知对应的注册回调函数
     */
    void setValue(const T &v) { 
        if(v == m_val)
            return;
        for(auto &i :m_cbs) { 
            // 通过函数回调的方式通知对应参数发生改变
            i.second(m_val, v);
        }
        m_val = v;
    }
    std::string getTypeName() const override { return typeid(T).name(); }
    void addListener(uint64_t key, on_change_cb cb) { m_cbs[key] = cb; }
    void deleteListener(uint64_t key) { m_cbs.erase(key); }
    on_change_cb getListener(uint64_t key) { return m_cbs[key]; }
    void clearListener() { m_cbs.clear(); }

private:
    T m_val;
    // 变更回调函数组，uint64_t key要求唯一，一般可以用hash
    std::map<uint64_t, on_change_cb> m_cbs;
};

class Config {
public:
    typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;
    /**
     * @brief 获取/创建对应参数名的配置参数
     * @param[in] name 配置参数名称
     * @param[in] default_value 参数默认值
     * @param[in] description 参数描述
     * @details 获取参数名为name的配置参数,如果存在直接返回
     *          如果不存在,创建参数配置并用default_value赋值
     * @return 返回对应的配置参数,如果参数名存在但是类型不匹配则返回nullptr
     * @exception 如果参数名包含非法字符[^0-9a-z_.] 抛出异常 std::invalid_argument
     */
    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string &name, 
                                const T& default_value, const std::string & description = ""){
        auto iter = s_datas.find(name);
        if(iter!=s_datas.end()) {
            auto tmp = std::dynamic_pointer_cast<ConfigVar<T>>(iter->second);
            if (tmp) {
                SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "Lookup name=" << name << " exists";
                return tmp;
            } 
            else {
                SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Lookup name=" << name << " exists but type not "
                                                  << typeid(T).name() << " real type=" << iter->second->getTypeName()
                                                  << " " << iter->second->toString(); 
                // throw std::invalid_argument(name);
                return nullptr;
            }
        }

        typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
        s_datas[name] = v;
        return v;
    }
    /**
     * @brief 查找配置参数
     * @param[in] name 配置参数名称
     * @return 返回配置参数名为name的配置参数
     */
    template<class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name) {
        auto it = s_datas.find(name);
        if(it == s_datas.end()){
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
    }

    static void LoadFromYaml(const YAML::Node &node);
    static ConfigVarBase::ptr LookUpBase(const std::string &name);

private:
    static ConfigVarMap s_datas;
};

}

#endif