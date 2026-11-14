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
    result UsersRepository::handleRequest(const http_request::request& request_struct) {
        const http_request::Method method = request_struct.method;
        file_path = FILE_PATH_ROOT + request_struct.url;
        try {
            switch (method) {
                case http_request::Get:
                    return getRequest(request_struct);
                case http_request::Post:
                    return postRequest(request_struct);
                case http_request::Delete:
                    return deleteRequest(request_struct);
                case http_request::Put:
                    return putRequest(request_struct);
                case http_request::Patch:
                    return patchRequest(request_struct);
                default:
                    return {.status_code = http_response::Not_Found, .body = "Unkown method"};
            }
        }catch (const std::exception& ex) {
            logger::error(ex.what());
            return {.status_code =http_response::Internal_Server_Error, .body = "Internal error"};
        }

    }
    result UsersRepository::getRequest(const http_request::request& request_struct) {
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
                return {.status_code = http_response::Ok, .body = body};
            }

            throw std::runtime_error("Requested file not found");
        }

        auto parameters = request_struct.parameters;

        if (parameters.empty()) {
            return {.status_code = http_response::Ok, .body = js.dump(INDENT)};
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
        return {.status_code = http_response::Ok, .body = result.dump(INDENT)};
    }

    result UsersRepository::postRequest(const http_request::request& request_struct) {

        if (!json::accept(request_struct.body)) {
            logger::warn("User provided invalid json:\n" + request_struct.body);
            return {.status_code = http_response::Forbidden, .body = "Provided JSON input was not valid"};
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
            throw std::runtime_error("Failed to open file");
        }

        return {.status_code = http_response::Created, .body = new_user.dump(INDENT)};
    }

    result UsersRepository::deleteRequest(const http_request::request& request_struct) {

        json users = getJsonFromFile();

        auto parameters = request_struct.parameters;

        if (parameters.empty()) {
            return {.status_code = http_response::Bad_Request, .body = "Request missing parameters"};
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

        return {.status_code = http_response::Ok, .body = users_to_delete.dump(INDENT)};
    }

    result UsersRepository::putRequest(const http_request::request &request_struct) {
        auto parameters = request_struct.parameters;
        if (!parameters.contains("id")) {
            return {.status_code = http_response::Bad_Request, .body = "Provided request didn't contain \"id\" parameter"};
        }

        try {
            int user_id = std::stoi(request_struct.parameters.at("id"));
            json users = getJsonFromFile();
            json body = json::parse(request_struct.body);
            bool found = false, created = false;

            for (auto& user : users) {
                if (user.value("id", -1) != user_id) continue;
                found = true;
                body["id"] = user_id;
                user = body;
                break;
            }

            if (!found) {
                body["id"] = user_id;
                users.push_back(body);
                created = true;
            }

            std::ofstream users_file_out(file_path);
            users_file_out << users.dump(INDENT);

            if (created) {
                return {.status_code = http_response::Created, .body = body.dump(INDENT)};
            }

            return {.status_code = http_response::Ok, .body = body.dump(INDENT)};
        }catch (const json::exception& ex) {
            logger::warn(ex.what());
            return {.status_code = http_response::Bad_Request, .body = "Provided body was not a valid JSON format"};
        }catch (const std::exception& ex) {
            logger::warn(ex.what());
            throw std::runtime_error("Couldn't read \"id\" parameter");
        }

    }

    result UsersRepository::patchRequest(const http_request::request &request_struct) {
        auto parameters = request_struct.parameters;
        if (!parameters.contains("id")) {
            return {.status_code = http_response::Bad_Request, .body = "Provided request didn't contain \"id\" parameter"};
        }

        try {
            int user_id = std::stoi(request_struct.parameters.at("id"));
            json users = getJsonFromFile();
            json body = json::parse(request_struct.body);
            json updated_user;

            for (auto& user : users) {
                if (user["id"] == user_id) {
                    // Looping through every body elements to update the existing users.json
                    for (auto& [key, value]: body.items()) {
                        //Protection for extra "
                        if (key == "id" || !user.contains(key)) {
                            continue;
                        }
                        user[key] = value;
                        updated_user.push_back(json::object_t::value_type(key, value));
                    }
                    break;
                }
            }

            if (updated_user.empty()) {
                return {.status_code = http_response::Bad_Request, .body = "Fields with given properties didn't exist in user"};
            }

            std::ofstream users_file_out(file_path);
            users_file_out << users.dump(INDENT);

            return {.status_code = http_response::Ok, .body = updated_user.dump(INDENT)};
        }catch (const json::exception& ex) {
            logger::warn(ex.what());
            return {.status_code = http_response::Bad_Request, .body = "Provided body was not a valid JSON format"};
        }catch (const std::exception& ex) {
            logger::warn(ex.what());
            throw std::runtime_error("Couldn't read \"id\" parameter");
        }
    }

    json UsersRepository::getJsonFromFile() const{
        std::ifstream file(file_path);
        if (file.is_open()) {
            return json::parse(file);
        }
        throw std::runtime_error( "Couldn't open file with path: " + file_path);
    }
}
