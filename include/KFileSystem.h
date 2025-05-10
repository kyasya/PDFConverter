#ifndef __K_FILESYSTEM_H__
#define __K_FILESYSTEM_H__

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#if defined(_WIN32)
    #include <windows.h>
    #include <direct.h>
    #include <io.h>
    #include <sys/stat.h>
    #define access _access
#else
    #include <unistd.h>
    #include <dirent.h>
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

namespace KFileSystem
{
    bool PathExists     (const std::string &path);
    bool IsDirectory    (const std::string &path);
    bool MakeDirectory  (const std::string &path);
    bool MakeDirectories(const std::string &path);
    bool ChangeDirectory(const std::string &path);
    std::vector<std::string> ListDirectory(const std::string &path);
    std::string GetCurrentPath();
} // namespace KFileSystem

//! 指定されたパスが存在するかを確認する。
//! @param path ファイルまたはディレクトリのパス
//! @return 存在すれば true、存在しなければ false
bool KFileSystem::PathExists(const std::string &path)
{
    return access(path.c_str(), 0) == 0;
}

//! 指定されたパスがディレクトリかどうかを確認する。
//! @param path 判定するパス
//! @return ディレクトリであれば true
bool KFileSystem::IsDirectory(const std::string &path)
{
    struct stat sb;
    return (stat(path.c_str(), &sb) == 0) && S_ISDIR(sb.st_mode);
}

//! 単一階層のディレクトリを作成する。
//! @param path 作成するディレクトリのパス
//! @return 成功すれば true
bool KFileSystem::MakeDirectory(const std::string &path)
{
#if defined(_WIN32)
    return _mkdir(path.c_str()) == 0;
#else
    return mkdir(path.c_str(), 0755) == 0;
#endif
}

//! 再帰的にディレクトリを作成する（mkdir -p 相当）。
//! @param path 作成するディレクトリのパス
//! @return 成功すれば true
bool KFileSystem::MakeDirectories(const std::string &path)
{
    if (path.empty()) return false;

    std::string current;
    for (size_t i = 0; i < path.length(); ++i)
    {
        current += path[i];
        if (path[i] == '/' || (path[i] == '\\' && i != 2))
        {
            if (!PathExists(current)) {if (!MakeDirectory(current)) return false;}
        }
    }
    if (!PathExists(path)) return MakeDirectory(path);

    return true;
}

//! カレントディレクトリを変更する（cd 相当）。
//! @param path 移動先ディレクトリのパス
//! @return 成功すれば true
bool KFileSystem::ChangeDirectory(const std::string &path)
{
#if defined(_WIN32)
    return _chdir(path.c_str()) == 0;
#else
    return chdir(path.c_str()) == 0;
#endif
}

//! 指定ディレクトリ内のファイル・ディレクトリ名一覧を取得する。
//! @param path 対象ディレクトリのパス
//! @return 名前のリスト（"." と ".." を除外）
std::vector<std::string> KFileSystem::ListDirectory(const std::string &path)
{
    std::vector<std::string> entries;

#if defined(_WIN32)
    std::string searchPath = path + "\\*";
    WIN32_FIND_DATAA findData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findData);

    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            std::string name = findData.cFileName;
            if (name != "." && name != "..") entries.push_back(name);
        } while (FindNextFileA(hFind, &findData));
        FindClose(hFind);
    }
#else
    DIR *dir = opendir(path.c_str());
    if (dir)
    {
        struct dirent *entry;
        while ((entry = readdir(dir)) != nullptr)
        {
            std::string name = entry->d_name;
            if (name != "." && name != "..") entries.push_back(name);
        }
        closedir(dir);
    }
#endif
    return entries;
}

//! 現在の作業ディレクトリの絶対パスを取得する。
//! @return カレントディレクトリの絶対パス（失敗時は空文字列）
std::string KFileSystem::GetCurrentPath()
{
    char buffer[4096];
#if defined(_WIN32)
    if (_getcwd(buffer, sizeof(buffer)) != nullptr) return std::string(buffer);
#else
    if (getcwd(buffer, sizeof(buffer)) != nullptr) return std::string(buffer);
#endif
    return std::string();
}

#endif