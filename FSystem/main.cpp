#include <iostream>
#include "Filesystem.h"

int main() {
    Filesystem FS;
    std::string cmd,path, name;

    std::cout<<"Unesite naredbu na isparavan način!\n Za pomoć: help c c";

    while (cmd!="exit") {
        std::cout << "\n##  ";
        std::cin >> cmd >> path >> name;
        if (cmd == "mkdir") {
            if (!FS.createFoDir(path, name,false))
                std::cout << "Error with directory creating.";
        }

        if(cmd=="create") {
            if (!FS.createFoDir(path, name,true))
                std::cout << "Error with file creating.";
        }
        if (cmd == "ls")
            FS.listingPath(path);

        if (cmd == "rename")
            FS.renameName(path, name);

        if (cmd == "echo") {
            std::string text;
            std::getline(std::cin, text);
            name += text;
            if (!FS.echo(path, name))
                std::cout << "Error with writing.";
        }

        if(cmd=="cat")
            FS.cat(path);

        if(cmd=="cp")
            FS.copy(path,name);

        if(cmd=="mv")
            FS.move(path,name);

        if(cmd=="rm")
            FS.remove(path,name);

        if(cmd=="put")
            FS.upload(path);

        if(cmd=="get")
            FS.download(path,name);

        if (cmd == "stat")
            FS.stat(path);
        if(cmd=="free")
            FS.free();

        if(cmd=="find")
            FS.find(path);

        if(cmd=="help")
            FS.help();
    }
        FS.deleteSystem();
        return 0;
}
