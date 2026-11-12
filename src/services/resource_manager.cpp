//
// Created by Dom on 12.11.2026.
//

#include "resource_manager.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace resource_manager {
    std::string getRequest(http_request::request &request_struct) {
        std::string file_path = FILE_PATH_ROOT + request_struct.url;

        json js;

        if (file_path.ends_with(".json")) {
            std::ifstream ifs(file_path);
            if (ifs) {
                js = json::parse(ifs);
            }else {
                //Not found
            }
        }else {
            std::ifstream file(file_path, std::ios::binary | std::ios::ate);
            if (file) {
                std::streamsize file_size = file.tellg();
                file.seekg(0, std::ios::beg);

                std::string body(file_size, '\0');
                file.read(body.data(), file_size);
                return body;
            }else {
                //Not found
            }
        }

        auto parameters = request_struct.parameters;

        if (parameters.empty()) {
            return js.dump(4);
        }

        json result;
        for (const auto& user: js) {
            bool user_matches = true;
            for (const auto& param: parameters) {
                const auto& v = user[param.first];
                std::string s = v.is_string() ? v.get<std::string>() : v.dump();
                //Is json value equal to given query parameters
                if (s != param.second) {
                    user_matches = false;
                    break;
                }
            }

            if (user_matches) {
                result.push_back(user);
            }

        }
        return result.dump(4);
    }
}
