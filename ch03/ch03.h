#pragma once
#include <vector>
#include <filesystem>
#include <windows.h> 
namespace ch03 {
    class Helper {
    public:
        static std::string GetExePath() {
            char path[MAX_PATH];
            if (GetModuleFileNameA(NULL, path, MAX_PATH) != 0) {
                std::cout << "Executable path: " << path << std::endl;
            }
            else {
                std::cerr << "Failed to get the executable path.\n";
            }
            return path;
        }
        static std::string GetFolder(const std::string& file_path) {
            auto index = file_path.find_last_of('\\');
            // 将std::string转换为fs::path  
            std::filesystem::path path(file_path);

            // 获取父路径，即文件夹路径  
            std::filesystem::path folder_path = path.parent_path();

            // 将fs::path转换回std::string  
            return folder_path.string();
        }
    };
    class PNGImage {
    public:
        uint64_t frameNumber = -1;
        unsigned int width = 0, height = 0;
        std::shared_ptr<std::vector<unsigned char>> buffer;
        static const int numChannels = 4;
        static const int redOffset = 0;
        static const int greenOffset = 1;
        static const int blueOffset = 2;

        PNGImage() {}
        PNGImage(uint64_t frame_number, const std::string& file_name);
        PNGImage(const PNGImage& p);
        virtual ~PNGImage() {}
        void write() const;
    };
}