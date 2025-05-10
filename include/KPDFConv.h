#ifndef __K_PDF_CONVERTER_H__
#define __K_PDF_CONVERTER_H__

#include <string>
#include <vector>
#include <array>
#include <sstream>

#include "GSWrapper.h"

//! @brief Ghostscript を使用して PDF を操作するためのラッパークラス
class KPDFConverter: public GSWrapper
{
	private:
		std::string InpPath = "";
		std::string OutPath = "";
	public:
		KPDFConverter(const std::string &input_path="", const std::string &output_path="");
		void SetPath (const std::string &input_path="", const std::string &output_path="");

	public:
		bool CompressPdf              (const std::string &input_path="", const std::string &output_path="");
		bool ExtractPages             (std::string page, const std::string &input_path="", const std::string &output_path="");
		bool MergePdfs                (const std::vector<std::string> &input_paths, const std::string &output_path="");
		bool ImageToPdf               (const std::vector<std::string> &image_paths, const std::string &output_path="");
		bool PdfToImages              (const std::string &input_path="", const std::string &output_path="", int dpi = 350);
		bool ConvertToGrayscalePdf    (const std::string &input_path="", const std::string &output_path="");
		bool ConvertToMonochromeImages(const std::string &input_path="", const std::string &output_path="", int dpi = 600);
};

//! constructor
KPDFConverter::KPDFConverter(const std::string &input_path, const std::string &output_path): GSWrapper()
{
	KPDFConverter::SetPath(input_path, output_path);
}

//! Pathを記録する
//! @param input_path 入力PDFファイルのパス
//! @param output_path 出力PDFファイルのパス
void KPDFConverter::SetPath(const std::string &input_path, const std::string &output_path)
{
	if ( !input_path.empty()) InpPath=input_path;
	if (!output_path.empty()) OutPath=output_path;
}

//! PDFを圧縮する
//! @param input_path 入力PDFファイルのパス
//! @param output_path 出力PDFファイルのパス
bool KPDFConverter::CompressPdf (const std::string &input_path, const std::string &output_path)
{
	std::string Buf;

	KPDFConverter::SetPath(input_path, output_path);

	if (output_path.empty())
    {
        size_t dot_pos = input_path.find_last_of(".");
        if (dot_pos != std::string::npos)
			Buf=input_path.substr(0, dot_pos)+"-compressed"+input_path.substr(dot_pos);
        else Buf=input_path+"-compressed.pdf";
		OutPath=Buf;
    }

	return GSWrapper::CompressPdf(InpPath, OutPath);
}

//! 指定したページ範囲をPDFから抽出する
//! @param page ページ範囲(e.g. 1-3,7-12,44-49)
//! @param input_path 入力PDFファイルのパス
//! @param output_path 結果のPDFファイルのパス
bool KPDFConverter::ExtractPages(std::string page, const std::string &input_path, const std::string &output_path)
{
	KPDFConverter::SetPath(input_path, output_path);

	std::vector<std::array<int, 2>> Result;
    std::stringstream ss(page);
    std::string token;

    while (std::getline(ss, token, ',')) 
	{
        size_t dash_pos = token.find('-');
        if (dash_pos != std::string::npos) 
		{
            int start = std::stoi(token.substr(0, dash_pos));
            int end = std::stoi(token.substr(dash_pos + 1));
            Result.push_back({start, end});
        }
    }

	for (const auto& range : Result) 
	{
		std::string Buf="-extraction-error";
        // std::cout<<range[0]<<"->"<<range[1]<<"\n";
		if (output_path.empty())
		{
			size_t dot_pos = input_path.find_last_of(".");
			if (dot_pos != std::string::npos) 
				Buf=input_path.substr(0, dot_pos)+"-"+std::to_string(range[0])+"-"+std::to_string(range[1])+input_path.substr(dot_pos);
			OutPath=Buf;
		}
		GSWrapper::ExtractPages(InpPath, OutPath, range[0], range[1]);
    }

	return true;
}

//! 複数のPDFを1つのPDFに結合する
//! @param input_paths 入力PDFファイルのパス一覧
//! @param output_path 出力PDFファイルのパス
//! @return bool
bool KPDFConverter::MergePdfs   (const std::vector<std::string> &input_paths, const std::string &output_path)
{
	OutPath="merged.pdf";
	if (output_path.empty())
	{
		size_t dot_pos = input_paths[0].find_last_of(".");
		if (dot_pos != std::string::npos) 
			OutPath=input_paths[0].substr(0, dot_pos)+"-merged"+input_paths[0].substr(dot_pos);
	}
	return GSWrapper::MergePdfs(input_paths, OutPath);
}

//! 画像ファイルのリストを1つのPDFに変換する
//! @param image_paths 入力画像ファイルのパス一覧
//! @param output_path 出力PDFファイルのパス
//! @return bool
bool KPDFConverter::ImageToPdf  (const std::vector<std::string> &image_paths, const std::string &output_path)
{
	OutPath="image-to-pdf.pdf";
	if(!output_path.empty()) OutPath=output_path;

	return GSWrapper::ImageToPdf(image_paths, OutPath);
}

//! PDFを複数のPNG画像に変換する
//! @param input_path 入力PDFファイルのパス
//! @param output_path 出力ファイル名のパターン（例：page-%03d.png）
//! @param dpi 解像度（dpi単位）
//! @return bool
bool KPDFConverter::PdfToImages (const std::string &input_path, const std::string &output_path, int dpi)
{
	KPDFConverter::SetPath(input_path, output_path);
	if (output_path.empty())
	{
		size_t dot_pos = input_path.find_last_of(".");
		if (dot_pos != std::string::npos) 
			OutPath=input_path.substr(0, dot_pos)+"-%03d"+input_path.substr(dot_pos);
	}

	return GSWrapper::PdfToImages(InpPath, OutPath, dpi);
}

//! PDFをグレースケールに変換する
//! @param input_path 入力PDFファイルのパス
//! @param output_path 出力PDFファイルのパス
//! @return bool
bool KPDFConverter::ConvertToGrayscalePdf(const std::string &input_path, const std::string &output_path)
{
	KPDFConverter::SetPath(input_path, output_path);
	if (output_path.empty())
	{
		size_t dot_pos = input_path.find_last_of(".");
		if (dot_pos != std::string::npos) 
			OutPath=input_path.substr(0, dot_pos)+"-grayscale"+input_path.substr(dot_pos);
	}
	return GSWrapper::ConvertToGrayscalePdf(InpPath, OutPath);
}

//! PDFをモノクロ（1ビット）PNG画像に変換する
//! @param input_path 入力PDFファイルのパス
//! @param output_path 出力ファイル名のパターン
//! @param dpi 解像度（dpi単位）
//! @return bool
bool KPDFConverter::ConvertToMonochromeImages(const std::string &input_path, const std::string &output_path, int dpi)
{
	KPDFConverter::SetPath(input_path, output_path);
	if (output_path.empty())
	{
		size_t dot_pos = input_path.find_last_of(".");
		if (dot_pos != std::string::npos) 
			OutPath=input_path.substr(0, dot_pos)+"-monochrome"+input_path.substr(dot_pos);
	}

	return GSWrapper::ConvertToMonochromeImages(InpPath, OutPath, dpi);
}

#endif