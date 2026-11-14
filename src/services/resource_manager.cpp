//
// Created by Dom on 12.11.2026.
//

#include "resource_manager.h"
#include <nlohmann/json.hpp>

#include "../utils/logger.h"

#define INDENT 4
using json = nlohmann::json;

namespace resource_manager {
    UsersRepository::UsersRepository() {
        // Dont know if i need something
    }
    std::string UsersRepository::handleRequest(const http_request::request& request_struct) {
        const http_request::Method method = request_struct.method;
        file_path = request_struct.url;
        try {
            switch (method) {
                case http_request::Get:
                    return getRequest(request_struct);
                case http_request::Post:
                    return postRequest(request_struct);
                case http_request::Delete:
                    return deleteRequest(request_struct);
                // case http_request::Put:
                //     return putRequest(request_struct);
                // case http_request::Patch:
                //     return patchRequest(request_struct);
                default:
                    return "Unkown method";
            }
        }catch (const std::exception& ex) {
            logger::error(ex.what());
            return "Internal error";
        }

    }
    std::string UsersRepository::getRequest(const http_request::request& request_struct) {
        json js;

        if (file_path.ends_with(".json")) {
            js = getJsonFromFile();
        }else {
            std::ifstream file(file_path, std::ios::binary | std::ios::ate);
            if (file) {
                std::streamsize file_size = file.tellg();
                file.seekg(0, std::ios::beg);

                std::string body(file_size, '\0');
                file.read(body.data(), file_size);
                return body;
            }

            throw std::runtime_error("Requested file not found");
        }

        auto parameters = request_struct.parameters;

        if (parameters.empty()) {
            return js.dump(INDENT);
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
        return result.dump(INDENT);
    }

    std::string UsersRepository::postRequest(const http_request::request& request_struct) {
        std::string file_path = FILE_PATH_ROOT + request_struct.url;

        if (!json::accept(request_struct.body)) {
            logger::warn("User provided invalid json:\n" + request_struct.body);
            return "Provided JSON input was not valid";
        }

        json new_user = json::parse(request_struct.body);
        std::ifstream users_file_in(file_path);

        if (users_file_in.is_open()) {
        json old_users_json = json::parse(users_file_in);

        old_users_json.push_back(new_user);
        users_file_in.close();
        std::ofstream users_file_out(file_path);
        users_file_out << old_users_json.dump(INDENT);

        }else {
            logger::warn("Couldn't open file with path: " + file_path);
            return "Failed to open file";
        }

        return new_user.dump(INDENT);
    }

    std::string UsersRepository::deleteRequest(const http_request::request& request_struct) {
        std::string file_path = FILE_PATH_ROOT + request_struct.url;
        if (request_struct.parameters.size()) {

        }

        json users = getJsonFromFile();

        auto parameters = request_struct.parameters;

        if (parameters.empty()) {
            return "Request missing parameters";
        }

        json remaining = json::array();
        json users_to_delete = json::array();

        for (const auto& user : users) {
            bool user_matches = true;
            for (const auto& param : parameters) {
                auto it = user.find(param.first);
                if (it == user.end()) {
                    user_matches = false;
                    break;
                }
                std::string s = it->is_string() ? it->get<std::string>() : it->dump();
                if (s != param.second) {
                    user_matches = false;
                    break;
                }
            }

            if (user_matches) {
                users_to_delete.push_back(user);
            }
            else {
                remaining.push_back(user);
            }
        }

        users = remaining;
        std::ofstream users_file_out(file_path);
        users_file_out << users.dump(INDENT);

        return users_to_delete.dump(INDENT);
    }
    json UsersRepository::getJsonFromFile() {
        std::ifstream file(file_path);
        if (file.is_open()) {
            return json::parse(file);
        }
        throw std::runtime_error( "Couldn't open file with path: " + file_path);
    }
}
