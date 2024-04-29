#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <vector>

template<typename Ftor>
void each_file(const std::filesystem::path &path, Ftor &&ftor)
{
    for (const auto entry : std::filesystem::directory_iterator(path))
    {
        const auto path = entry.path();

        if (path.filename().string().starts_with("."))
            continue;

        if (entry.is_directory())
            each_file(path, ftor);
        else if (entry.is_regular_file())
            ftor(path);
    }
}

int main()
{
    std::set<std::string> extensions = {".hpp", ".cpp", ".h", ".c", ".rb", ".chai", ".graphml", ".py", ".md"};

    using Files = std::vector<std::string>;
    std::map<std::string, Files> ext__files;
    std::map<std::string, std::string> file__content;
    each_file("/home/geertf/am", [&](const std::filesystem::path &path) {
        const std::string ext = path.extension().string();
        if (extensions.count(ext))
            ext__files[ext].push_back(path);
    });

    for (const auto &[ext, files] : ext__files)
    {
        std::cout << ext << "\t" << files.size() << std::endl;

        for (const auto &file : files)
        {
            const auto size = std::filesystem::file_size(file);
            auto &content = file__content[file];
            content.resize(size);
            std::ifstream fi(file, std::ios::binary);
            fi.read(content.data(), content.size());
        }
    }

    const auto total_size = std::accumulate(file__content.begin(), file__content.end(), 0u, [](auto sum, const auto &it) { return sum + it.second.size(); });
    std::cout << "total_size: " << total_size << std::endl;

    std::string all_content;
    all_content.resize(total_size);
    auto ptr = all_content.data();

    for (const auto &[ext, files] : ext__files)
    {
        for (const auto &file : files)
        {
            const auto &content = file__content[file];
            content.copy(ptr, content.size());
            ptr += content.size();
        }
    }

    std::ofstream fo("all_content.txt", std::ios::binary);
    fo.write(all_content.data(), all_content.size());

    return 0;
}
