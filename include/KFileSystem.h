#pragma once

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
#define stat _stat
#define S_ISDIR(mode) (((mode) & _S_IFMT) == _S_IFDIR)
#else
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#endif

/**
 * @namespace KFileSystem
 * @brief クロスプラットフォーム対応のファイルシステム操作ユーティリティ群
 *
 * Linux / macOS / Windows に対応したシンプルなファイル・ディレクトリ操作関数を提供。
 */
namespace KFileSystem
{

    /**
     * @brief 指定されたパスが存在するかを確認。
     * @param path 確認するファイルまたはディレクトリのパス
     * @return 存在すれば true、存在しなければ false
     */
    bool PathExists(const std::string &path)
    {
        return access(path.c_str(), 0) == 0;
    }

    /**
     * @brief 指定されたパスがディレクトリかどうかを確認。
     * @param path 確認するパス
     * @return ディレクトリであれば true、そうでなければ false
     */
    bool IsDirectory(const std::string &path)
    {
        struct stat sb;
        return (stat(path.c_str(), &sb) == 0) && S_ISDIR(sb.st_mode);
    }

    /**
     * @brief 単一階層のディレクトリを作成。
     * @param path 作成するディレクトリのパス
     * @return bool:true/false
     */
    bool MakeDirectory(const std::string &path)
    {
#if defined(_WIN32)
        return _mkdir(path.c_str()) == 0;
#else
        return mkdir(path.c_str(), 0755) == 0;
#endif
    }

    /**
     * @brief 再帰的にディレクトリを作成（mkdir -p 相当）。
     * @param path 作成するディレクトリのパス
     * @return bool:true/false
     */
    bool MakeDirectories(const std::string &path)
    {
        if (path.empty())
            return false;
        std::string current;
        for (size_t i = 0; i < path.length(); ++i)
        {
            current += path[i];
            if (path[i] == '/' || (path[i] == '\\' && i != 2))
            {
                if (!PathExists(current))
                {
                    if (!MakeDirectory(current))
                        return false;
                }
            }
        }
        if (!PathExists(path))
            return MakeDirectory(path);
        return true;
    }

    /**
     * @brief カレントディレクトリを変更（cd 相当）。
     * @param path 移動先ディレクトリのパス
     * @return bool:true/false
     */
    bool ChangeDirectory(const std::string &path)
    {
#if defined(_WIN32)
        return _chdir(path.c_str()) == 0;
#else
        return chdir(path.c_str()) == 0;
#endif
    }

    /**
     * @brief 指定ディレクトリ内のエントリ（ファイル・サブディレクトリ）名一覧を取得。
     * @param path 対象ディレクトリのパス
     * @return std::vector<std::string> の名前一覧
     */
    std::vector<std::string> ListDirectory(const std::string &path)
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
                if (name != "." && name != "..")
                {
                    entries.push_back(name);
                }
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
                if (name != "." && name != "..")
                {
                    entries.push_back(name);
                }
            }
            closedir(dir);
        }
#endif
        return entries;
    }

    /**
     * @brief 現在の作業ディレクトリを取得（pwd 相当）。
     * @return 絶対パスの文字列
     */
    std::string GetCurrentPath()
    {
        char buffer[4096];
#if defined(_WIN32)
        if (_getcwd(buffer, sizeof(buffer)) != nullptr)
        {
            return std::string(buffer);
        }
#else
        if (getcwd(buffer, sizeof(buffer)) != nullptr)
        {
            return std::string(buffer);
        }
#endif
        return std::string(); // 失敗時
    }

} // namespace KFileSystem
