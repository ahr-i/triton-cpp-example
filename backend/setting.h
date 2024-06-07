#ifndef __SETTING_H__
#define __SETTING_H__

#include <string>
#include <map>

class Setting {
public:
    Setting(const std::string& filename);
    std::string get(const std::string& key) const;

private:
    std::map<std::string, std::string> settings;
    void loadSettings(const std::string& filename);
};

#endif
