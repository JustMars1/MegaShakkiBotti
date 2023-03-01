#include <string>
#include <vector>
#include <unordered_map>

class Kieli
{
public:
    std::unordered_map<std::string, std::string> kaannokset;
    const std::string& get(const std::string& avain) const;    
};
