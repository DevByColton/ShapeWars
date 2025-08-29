#ifndef USERINTERFACE_H
#define USERINTERFACE_H

class UserInterface {
public:
    UserInterface() = default;
    ~UserInterface() = default;

    static UserInterface &instance() {
        static auto *instance = new UserInterface;
        return *instance;
    }

    void draw();
};



#endif //USERINTERFACE_H
