#include <iostream>

#include <memory>
#include "vfspp/VFS.h"
#include "vfspp/NativeFileSystem.hpp"
#include "vfspp/ZipFileSystem.hpp"
#include "vfspp/MemoryFileSystem.hpp"

using namespace vfspp;

int main() {
    miniz_cpp::zip_file file;

    file.writestr("file1.txt", "this is file 1");
    file.writestr("file2.txt", "this is file 2");
    file.writestr("file3.txt", "this is file 3");
    file.writestr("file4.txt", "this is file 4");
    file.writestr("file5.txt", "this is file 5");

    file.save("Resources.zip");

    // Register native filesystem during development or zip for distribution build

    IFileSystemPtr memFS(new MemoryFileSystem());

    vfspp::IFileSystemPtr rootFS = nullptr;
#define DISTRIBUTION_BUILD
#if defined(DISTRIBUTION_BUILD)
    rootFS = std::make_unique<ZipFileSystem>("Resources.zip");
#else
    rootFS = std::make_unique<NativeFileSystem>("Resources");
#endif

    rootFS->Initialize();

    if (rootFS->IsInitialized()) {
        std::cout << "Root filesystem initialized successfully!" << std::endl;
    }

    memFS->Initialize();

    if (memFS->IsInitialized()) {
        std::cout << "memory filesystem initialized successfully!" << std::endl;
    }

    VirtualFileSystemPtr vfs(new VirtualFileSystem());
    vfs->AddFileSystem("/", memFS);
    vfs->AddFileSystem("/", std::move(rootFS));

    // Example: Open a save file
    if (auto saveFile = vfs->OpenFile(FileInfo("/savefile.txt"), IFile::FileMode::ReadWrite)) {
        if (saveFile->IsOpened()) {
            std::cout << "Save file opened successfully!" << std::endl;
        }
        else {
            std::cout << "Failed to open save file!" << std::endl;
        }
    }
}
