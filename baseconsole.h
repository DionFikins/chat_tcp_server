#pragma once
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

std::string login, pass, name;
bool enable;

class User {
    protected:
        int _id;
        std::string _login, _pass, _name;
        bool _enable;
    public:
        User(int id, std::string login, std::string pass, std::string name, bool enable);
        ~User();

    const int GetId() const;
    const std::string GetLogin() const;
    const std::string GetPass() const;
    const std::string GetName() const;
    const bool GetEnable() const;
    
    const std::string GetUserData() const;

    void SetLogin(std::string value);
    void SetPass(std::string value);
    void SetName(std::string value);
    void SetEnable(bool value);
};

class UserBase {
    protected:
        User* us;
    public:
        UserBase();
        UserBase(const std::vector<std::vector<User>>& users);
        ~UserBase();
        vector<vector<User>> userVc;


        std::string Registration(std::string login, std::string pass, std::string name, bool enable);
        bool CheckUser();
        std::string Logining(std::string login, std::string pass);

        std::string SearchByID(int id);

        std::string SearchByLogin(std::string value);
        std::string SearchByName(std::string value);
        bool EnableUserSearch(std::string value);

        std::string BlockUser(std::string value);
        std::string UnblockUser(std::string value);
        std::string ShowAllUser();
        std::string ShowAllName();
};

class Message {
    protected:
        std::string _output, _input;
        bool _alls;
        std::string _messages;
    public:
        Message(std::string output, std::string messages, std::string input, bool alls);
        ~Message();

        const std::string GetOuput() const;
        const std::string GetInput() const;
        const std::string GetMessages() const;
        const bool GetAlls() const;
};

class MessageBase {
    protected:
        Message* msg;
    public:
        MessageBase();
        ~MessageBase();
        vector<vector<Message>> messageVc;
        bool NewMailAllUser(std::string output, std::string messages, std::string input, bool alls);
        bool NewMainOneUser(std::string output, std::string messages, std::string input, bool alls);

        std::string ShowAllChatMessages();
        std::string ShowOneChatMessages(std::string output, std::string input);
};