#include <iostream>
#include "KFileSystem.h"

int main()
{
    std::string dir = "example/output";

    if (!KFileSystem::PathExists(dir))
    {
        if (KFileSystem::MakeDirectories(dir))
        {
            std::cout << "Created directory: " << dir << std::endl;
        }
        else
        {
            std::cerr << "Failed to create directory.\n";
        }
    }

    std::cout << "Current directory: " << KFileSystem::GetCurrentPath() << "\n";

    auto list = KFileSystem::ListDirectory(".");
    std::cout << "Contents:\n";
    for (const auto &name : list)
    {
        std::cout << " - " << name << "\n";
    }

    return 0;
}
