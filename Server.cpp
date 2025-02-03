#include <iostream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include "baseconsole.cpp"

#define PORT 7777

using std::cout;
using std::endl;

UserBase userB;
MessageBase messB;

std::string output, messages, input;

class Server {
    protected:
        int serverD, socketD;
        struct sockaddr_in address;
        int opt = 1;
        int addrlen = sizeof(address);
        char buffer[1024] = {0};

        std::string l, p, n, result, out, inp, al;
    public:
        Server() { }
        ~Server() { }

        void StartServer() {
            if((serverD = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
                perror("Socket failed");
                ExitProgram();
            }
            if(setsockopt(serverD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
                perror("Setsockopt");
                ExitProgram();
            }
            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;
            address.sin_port = htons(PORT);
            if(bind(serverD, (struct sockaddr*)&address, sizeof(address)) < 0) {
                perror("Bind failed");
                ExitProgram();
            }
            if(listen(serverD, 3) < 0) {
                perror("Listen");
                ExitProgram();
            }

            cout << "Server waiting any connection..." << endl;

            if((socketD = accept(serverD, (struct sockaddr*)&address, (socklen_t*)&address)) < 0) {
                perror("accept");
                ExitProgram();
            }
            cout << "User online." << endl;

            while(true) {
                MainMenu(socketD);

                std::string choice = OnMessage();
                
                if(choice == "1") RegMenu(socketD, choice);
                else if(choice == "2") {
                    if(LoginMenu(socketD, choice)) {
                        while(true) {
                            ChatMenu(socketD);

                            std::string choice = OnMessage();

                            if(choice == "1") {
                                SendMessageAll(socketD, choice, output);
                            } else if(choice == "2") {
                                EnterMessage(userB.ShowAllUser() + "\nselect login for input message\n");
                                input = OnMessage(); 
                                SendMessageOneUser(socketD, choice, output, input);
                            } else if(choice == "3") {
                                OpenChatAll(socketD, choice, output);
                            } else if(choice == "4") {
                                EnterMessage(userB.ShowAllUser() + "\nselect login for open chat");
                                input = OnMessage(); 
                                OpenOneChat(socketD, choice, output, input);
                            } else if(choice == "5") {
                                RebindLogin(socketD, choice, output);
                                break;
                            } else if(choice == "6") {
                                RebindPass(socketD, choice, output);
                                break;
                            } else if(choice == "7") {
                                RebindName(socketD, choice, output);
                                break;
                            } else if(choice == "0") break;
                            else EnterMessage("This option not yet");
                        }
                    }
                } else if(choice == "3") EnterMessage(userB.ShowAllUser());
                else if(choice == "4") {
                    EnterMessage("Enter name user for ban: ");
                    EnterMessage(userB.BlockUser(OnMessage()));

                } else if(choice == "5") {
                    EnterMessage("Enter name user for unban: ");
                    EnterMessage(userB.UnblockUser(OnMessage()));
                } else if(choice == "endl") ExitProgram();
                else EnterMessage("This option not yet");
            }
        }

        void EnterMessage(std::string response) {
            send(socketD, response.c_str(), response.length(), 0);
        }

        std::string OnMessage() {
            memset(buffer, 0, sizeof(buffer));
            int bytes = read(socketD, buffer, 1024);
            if(bytes <= 0) {
                std::cout << "User offline." << std::endl;
                ExitProgram();
            } 
            std::cout << buffer << std::endl;
            return buffer;
        }

        void MainMenu(int socketD) {
            EnterMessage("\nWellcome to the chat!\n\n"
                "Choice options: "
                "\n1 - Registration with user"
                "\n2 - Login in chat"
                "\n3 - View all user"
                "\n4 - Ban user"
                "\n5 - Unban user"
                "\nendl - Exit program\n");
        }
        void RegMenu(int socketD, std::string choice) {
            EnterMessage("\nEnter login for new user: ");
            l = OnMessage();

            EnterMessage("Enter password for new user: ");
            p = OnMessage();

            EnterMessage("Enter name for new user: ");
            n = OnMessage();

            EnterMessage(userB.Registration(l, p, n, true));
        }
        bool LoginMenu(int socketD, std::string choice) {
            if(userB.CheckUser()) {
                EnterMessage("\nNot user in program\n");
            }
            else {
                EnterMessage("Enter login user: ");
                l = OnMessage();

                EnterMessage("Enter password user: ");
                p = OnMessage();

                output = l;
                if(userB.Logining(l,p) == "Entered in program\n") return true;
            }
            return false;
        }
        void ChatMenu(int socketD) {
            EnterMessage("Use chioce: "
                "\n1 - Send message all"
                "\n2 - Send message one user"
                "\n3 - Open chat all"
                "\n4 - Open chat one user"
                "\n5 - Rebind login"
                "\n6 - Rebind password"
                "\n7 - Rebind name"
                "\n0 - Back to main menu");
        }
        void SendMessageAll(int socketD, std::string choice, std::string output) {
            EnterMessage("Enter all chat\n Enter message: ");
            std::string text = OnMessage();
            bool task = false;
            while(!task) {
                for(auto& user : userB.userVc)
                    for(User& usr : user)
                        messB.NewMailAllUser(output, text, usr.GetLogin(), true);
                task = true;
            }
            EnterMessage("Message upload\n");
        }
        void SendMessageOneUser(int socketD, std::string choice, std::string output, std::string input) {
            EnterMessage("Send message for " + input);
            std::string text = OnMessage();
            messB.NewMainOneUser(output, text, input, false);
            EnterMessage("Message upload\n");
        }
        void OpenChatAll(int socketD, std::string choice, std::string output) {
            EnterMessage("Enter all's chat\n" + messB.ShowAllChatMessages());
        }
        void OpenOneChat(int socketD, std::string choice, std::string output, std::string input) {
            EnterMessage("Enter chat with " + input + "\n" + messB.ShowOneChatMessages(output, input));
        }
        void RebindLogin(int socketD, std::string choice, std::string output) {
            EnterMessage("Enter new login: \n");
            std::string text = OnMessage();
            for(auto& us : userB.userVc)
                for(User& usr : us)
                    if(output == usr.GetLogin())
                        if(text != usr.GetLogin())
                            usr.SetLogin(text);
            EnterMessage("Update login");
        }
        void RebindPass(int socketD, std::string choice, std::string output) {
            EnterMessage("Enter new pass: \n");
            std::string text = OnMessage();
            for(auto& us : userB.userVc)
                for(User& usr : us)
                    if(output == usr.GetLogin())
                        usr.SetPass(text);
            EnterMessage("Update password");
        }
        void RebindName(int socketD, std::string choice, std::string output) {
            EnterMessage("Enter new name: \n");
            std::string text = OnMessage();
            for(auto& us : userB.userVc)
                for(User& usr : us)
                    if(output == usr.GetLogin())
                        if(text == usr.GetName())
                            usr.SetName(text);
            EnterMessage("Update name");
        } 
        void ExitProgram() {
            close(socketD);
            exit(1);
        }
};