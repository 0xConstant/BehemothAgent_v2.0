#include "file_search.h"
#include "check_perms.h"
#include <queue>
#include <filesystem>
#include <algorithm>
#include <execution>
#include <windows.h>


std::vector<std::wstring> FileSearcher(const std::wstring& dir, const std::vector<std::wstring>& fileTypes, const std::unordered_set<std::wstring>& forbiddenDirs)
{
    std::vector<std::wstring> result;
    std::queue<std::filesystem::path> todo;
    todo.push(dir);

    while (!todo.empty())
    {
        std::filesystem::path currentDir = todo.front();
        todo.pop();

        std::vector<std::filesystem::path> subDirs;
        std::vector<std::filesystem::path> files;

        try
        {
            for (const auto& entry : std::filesystem::directory_iterator(currentDir))
            {
                if (entry.is_directory())
                {
                    subDirs.push_back(entry.path());
                }
                else if (entry.is_regular_file())
                {
                    files.push_back(entry.path());
                }
            }
        }
        catch (const std::exception&) {}

        std::vector<std::filesystem::path> validSubDirs;
        std::copy_if(subDirs.begin(), subDirs.end(), std::back_inserter(validSubDirs), [&](const std::filesystem::path& subDir)
            {
                return forbiddenDirs.find(subDir.filename().wstring()) == forbiddenDirs.end();
            });

        for (const auto& validSubDir : validSubDirs)
        {
            todo.push(validSubDir);
        }

        std::vector<std::wstring> validFiles;
        std::transform(files.begin(), files.end(), std::back_inserter(validFiles), [&](const std::filesystem::path& file)
            {
                if (std::find_if(fileTypes.begin(), fileTypes.end(), [&](const std::wstring& fileType)
                    {
                        return file.extension() == fileType;
                    }) != fileTypes.end() && CheckPerms(file.wstring(), GENERIC_WRITE))
                {
                    return file.wstring();
                }
                return std::wstring(L"");
            });

        validFiles.erase(std::remove(validFiles.begin(), validFiles.end(), std::wstring(L"")), validFiles.end());
        result.insert(result.end(), validFiles.begin(), validFiles.end());
    }

    return result;
}



