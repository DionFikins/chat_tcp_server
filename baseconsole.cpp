#include "baseconsole.h"

UserBase usrB;

User::User(int id, std::string login, std::string pass, std::string name, bool enable) 
    : _id(id), _login(login), _pass(pass), _name(name), _enable(enable) { }
User::~User() {
    _id = 0;
    _login = "";
    _pass = "";
    _name = "";
    _enable = "";
}

const int User::GetId() const { return _id; }

const std::string User::GetLogin() const { return _login; }
const std::string User::GetPass() const { return _pass; }
const std::string User::GetName() const { return _name; }
const bool User::GetEnable() const { return _enable; }

const std::string User::GetUserData() const { 
     return "Login: " + _login + " Pass: " + _pass + " Name: " + _name + "\n";  
}

void User::SetLogin(std::string value) { _login = value; }
void User::SetPass(std::string value) { _pass = value; }
void User::SetName(std::string value) { _name = value; }
void User::SetEnable(bool value) { _enable = value; }

UserBase::UserBase() { };
UserBase::~UserBase() { }
UserBase::UserBase(const std::vector<std::vector<User>> &users) : userVc(users) { };

std::string UserBase::Registration(std::string login, std::string pass, std::string name, bool enable) {
    for(auto& ac : userVc)
        for(User& usr : ac)
            if(login == usr.GetLogin())
                return "Error add's user!\nThis user added in program earlier\nPress any keys for to the continue...\n";
    int id = userVc.size() + 1;
    userVc.push_back({User(id, login, pass, name, enable)});
    return "User add in program\nPress any keys for to the continue...\n";
}
bool UserBase::CheckUser() {
    if(userVc.empty()) return true;
    return false;
}
std::string UserBase::Logining(std::string login, std::string pass) {
    for(auto& us : userVc)
        for(User& usr : us)
            if(usr.GetLogin() == login && 
                usr.GetPass() == pass &&
                usr.GetEnable())
                    return "Entered in program\n";
    return "Login or pass is incorrect,\nor user is deactivated\n";
}

std::string UserBase::SearchByID(int id) {
    for(auto& i : userVc)
        for(User& usr : i)
            if(id == usr.GetId())
                return usr.GetName();
    return "User not found";
}

std::string UserBase::SearchByLogin(std::string value) {
    for(auto& us : userVc)
        for(User& usr : us)
            if(value == usr.GetLogin())
                return usr.GetName();
    return "User not found";
}
std::string UserBase::SearchByName(std::string value) {
    for(auto& us : userVc)
        for(User& usr : us)
            if(value == usr.GetName())
                return usr.GetLogin();
    return "User not found";
}
bool UserBase::EnableUserSearch(std::string value) {
    for(auto& us : userVc)
        for(User& usr : us)
            if(usr.GetLogin() == value)
                if(usr.GetEnable())
                    return true;
    return false;
}
std::string UserBase::BlockUser(std::string value) {
    if(CheckUser()) return "Zero users in program\n";
    else {
        for(auto& ac : userVc)
            for(User& usr : ac) 
                if(value == usr.GetLogin()) {
                    if(!usr.GetEnable()) return "User banned latest";
                    else {
                        usr.SetEnable(false);
                        return "User banned now";
                    } 
                }        
        return "User not found";
    }
        
}
std::string UserBase::UnblockUser(std::string value) {
    if(CheckUser()) return "Zero users in program\n";
    else {
        for(auto& ac : userVc)
            for(User& usr : ac)
                if(value == usr.GetLogin()) {
                    if(!usr.GetEnable()) {
                        usr.SetEnable(true);
                        return "User unban";
                    }
                    else return "User not banned";
                }
        return "User not found";
    }
}

std::string UserBase::ShowAllUser() {
    std::string text = "All users in program: \n";
    if(CheckUser()) return "Zero users in program\n";
    else {
        for(auto& ac : userVc)
            for(User& usr : ac)
                if(usr.GetEnable())
                    text += usr.GetUserData();
                else text += "Banned - " + usr.GetUserData();
    }
    return text;
}

std::string UserBase::ShowAllName() {
    std::string text = "All uesrs name in program: \n";
    if(CheckUser()) return "Zero users in program\n";
    else {
        for(auto& ac : userVc)
            for(User& usr : ac)
                if(usr.GetEnable())
                    text += usr.GetName() + "\n";
    }
    return text;
}

Message::Message(std::string output, std::string messages, std::string input, bool alls)
    : _output(output), _messages(messages), _input(input), _alls(alls) {}
Message::~Message() { }

const std::string Message::GetOuput() const { return _output; }
const std::string Message::GetInput() const { return _input; }
const std::string Message::GetMessages() const { return _messages; }
const bool Message::GetAlls() const { return _alls; }

MessageBase::MessageBase() { }
MessageBase::~MessageBase() { }

bool MessageBase::NewMailAllUser(std::string output, std::string messages, std::string input, bool alls)
{
    messageVc.push_back({Message(output, messages, input, alls)});                    
    return true;
}

bool MessageBase::NewMainOneUser(std::string output, std::string messages, std::string input, bool alls) {
    messageVc.push_back({Message(output, messages, input, alls)});
    return true;
}

std::string MessageBase::ShowAllChatMessages() {
    if(messageVc.empty()) return "Error load all's chat";
    else {
        std::string text;
        for(auto& _mail : messageVc)
            for(Message& mg : _mail)
                if(mg.GetAlls())
                    text += "Output: " + mg.GetOuput() + " Message: " + mg.GetMessages() + "\n";
        
        return text;
    }
}

std::string MessageBase::ShowOneChatMessages(std::string output, std::string input) {
    std::string text;
    for(auto& _mail : messageVc)
        for(Message& mg : _mail)
            if(!mg.GetAlls())
                text += "Output: " + mg.GetOuput() + " Message: " + mg.GetMessages() + " Input: " + mg.GetInput() + "\n";
    return text;
}