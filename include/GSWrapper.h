#ifndef __GSWRAPPER_H__
#define __GSWRAPPER_H__

#include <string>
#include <vector>
#include <array>
#include <sstream>

class GsWrapper
{
    public:
        GsWrapper(const std::string &gsPath = "gs") : GsPath(gsPath) {}

        bool CompressPdf(const std::string &inputPath, const std::string &outputPath);
        bool ExtractPages(const std::string &inputPath, const std::string &outputPath, int firstPage, int lastPage);
        bool MergePdfs(const std::vector<std::string> &inputPaths, const std::string &outputPath);
        bool ImageToPdf(const std::vector<std::string> &imagePaths, const std::string &outputPath);
        bool PdfToImages(const std::string &inputPath, const std::string &outputPattern, int dpi = 150);

    private:
        std::string GsPath;
        bool RunCommand(const std::vector<std::string> &args, std::string &output, std::string &error);
};

bool GsWrapper::CompressPdf(const std::string &inputPath, const std::string &outputPath)
{
    std::vector<std::string> args = {
        GsPath, "-sDEVICE=pdfwrite",
        "-dCompatibilityLevel=1.4",
        "-dPDFSETTINGS=/ebook",
        "-dNOPAUSE", "-dQUIET", "-dBATCH",
        "-sOutputFile=" + outputPath,
        inputPath};
    std::string out, err;
    return RunCommand(args, out, err);
}

bool GsWrapper::ExtractPages(const std::string &inputPath, const std::string &outputPath, int firstPage, int lastPage)
{
    std::vector<std::string> args = {
        GsPath, "-sDEVICE=pdfwrite",
        "-dNOPAUSE", "-dBATCH", "-dQUIET",
        "-dFirstPage=" + std::to_string(firstPage),
        "-dLastPage=" + std::to_string(lastPage),
        "-sOutputFile=" + outputPath,
        inputPath};
    std::string out, err;
    return RunCommand(args, out, err);
}

bool GsWrapper::MergePdfs(const std::vector<std::string> &inputPaths, const std::string &outputPath)
{
    std::vector<std::string> args = {
        GsPath, "-dBATCH", "-dNOPAUSE", "-q", "-sDEVICE=pdfwrite",
        "-sOutputFile=" + outputPath};
    args.insert(args.end(), inputPaths.begin(), inputPaths.end());
    std::string out, err;
    return RunCommand(args, out, err);
}

bool GsWrapper::ImageToPdf(const std::vector<std::string> &imagePaths, const std::string &outputPath)
{
    std::vector<std::string> args = {
        GsPath, "-dBATCH", "-dNOPAUSE", "-q", "-sDEVICE=pdfwrite",
        "-sOutputFile=" + outputPath};
    args.insert(args.end(), imagePaths.begin(), imagePaths.end());
    std::string out, err;
    return RunCommand(args, out, err);
}

bool GsWrapper::PdfToImages(const std::string &inputPath, const std::string &outputPattern, int dpi)
{
    std::vector<std::string> args = {
        GsPath,
        "-dBATCH", "-dNOPAUSE", "-q",
        "-sDEVICE=png16m",
        "-r" + std::to_string(dpi),
        "-sOutputFile=" + outputPattern,
        inputPath};
    std::string out, err;
    return RunCommand(args, out, err);
}

bool GsWrapper::RunCommand(const std::vector<std::string> &args, std::string &output, std::string &error)
{
    std::ostringstream cmd;
    for (const auto &arg : args)
    {
        cmd << "\"" << arg << "\" ";
    }
    std::string command = cmd.str() + "2>&1";

    std::array<char, 128> buffer;
    std::string result;
    FILE *pipe = popen(command.c_str(), "r");
    if (!pipe)
        return false;
    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    {
        result += buffer.data();
    }
    int status = pclose(pipe);
    output = result;
    error = (status == 0) ? "" : result;
    return status == 0;
}

#endif
