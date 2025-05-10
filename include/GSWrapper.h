#ifndef __GS_WRAPPER_H__
#define __GS_WRAPPER_H__

#include <string>
#include <vector>
#include <array>
#include <sstream>

//! @brief Ghostscript を使用して PDF を操作するためのラッパークラス
class GSWrapper
{
	public:
		GSWrapper(){GSWrapper::Init();}

	private:
		std::string GSCmd="";
		std::string Output, Error;
		bool RunFrag=false;
	public:
			void Init();
			std::string GetOutputText() const {return  Output;}
			std::string GetErrorText () const {return   Error;}
			bool        IsRunSuccess () const {return RunFrag;}

	private:
		bool RunCommand(const std::vector<std::string> &args);

	public:
		bool CompressPdf              (const std::string &input_path, const std::string &output_path);
		bool ExtractPages             (const std::string &input_path, const std::string &output_path, int first_page=1, int last_page=1);
		bool MergePdfs                (const std::vector<std::string> &input_paths, const std::string &output_path);
		bool ImageToPdf               (const std::vector<std::string> &image_paths, const std::string &output_path);
		bool PdfToImages              (const std::string &input_path, const std::string &output_path, int dpi = 350);
		bool ConvertToGrayscalePdf    (const std::string &input_path, const std::string &output_path);
		bool ConvertToMonochromeImages(const std::string &input_path, const std::string &output_path, int dpi = 600);
};

//! 初期化
//! @details constructorのみで呼ばれる
void GSWrapper::Init()
{
	GSCmd = "gs";
#ifdef _WIN32
	GSCmd = "gswin64c";
#endif
}


//! Ghostscriptコマンドを構築して実行し、出力を取得する.
//! @details エラー情報はGet関数によって個別に呼び出す
//! @param args コマンドライン引数のリスト
//! @return bool
bool GSWrapper::RunCommand(const std::vector<std::string> &args)
{
	std::ostringstream Cmd;
	for (const auto &Arg : args) Cmd <<" "<< Arg;

	std::string Command = Cmd.str() + " 2>&1";
	// std::cout<<Command<<std::endl;

	std::array<char, 128> Buffer;
	std::string Result;

	FILE *Pipe = popen(Command.c_str(), "r");
	if (!Pipe) return false;
	
	while (fgets(Buffer.data(), Buffer.size(), Pipe) != nullptr) Result += Buffer.data();

	int Status = pclose(Pipe);

	Output = Result;
	Error = (Status == 0) ? "" : Result;

	RunFrag = bool(Status);
	return RunFrag;
}

//! PDFを圧縮する
//! @param input_path 入力PDFファイルのパス
//! @param output_path 出力PDFファイルのパス
//! @return bool
bool GSWrapper::CompressPdf(const std::string &input_path, const std::string &output_path)
{
	std::vector<std::string> Args=
	{
		GSCmd, "-sDEVICE=pdfwrite",
		"-dCompatibilityLevel=1.4",
		"-dPDFSETTINGS=/ebook",
		"-dNOPAUSE", "-dQUIET", "-dBATCH",
		"-sOutputFile=" + output_path,
		input_path
	};

	return GSWrapper::RunCommand(Args);
}

//! 指定したページ範囲をPDFから抽出する
//! @param input_path 入力PDFファイルのパス
//! @param output_path 出力PDFファイルのパス
//! @param first_page 抽出開始ページ番号
//! @param last_page 抽出終了ページ番号
//! @return bool
bool GSWrapper::ExtractPages(const std::string &input_path, const std::string &output_path, int first_page, int last_page)
{
	std::vector<std::string> Args=
	{
		GSCmd, "-sDEVICE=pdfwrite",
		"-dNOPAUSE", "-dBATCH", "-dQUIET",
		"-dFirstPage="+std::to_string(first_page),
		"-dLastPage=" +std::to_string(last_page),
		"-sOutputFile=" + output_path,
		input_path
	};

	return GSWrapper::RunCommand(Args);
}

//! 複数のPDFを1つのPDFに結合する
//! @param input_paths 入力PDFファイルのパス一覧
//! @param output_path 出力PDFファイルのパス
//! @return bool
bool GSWrapper::MergePdfs(const std::vector<std::string> &input_paths, const std::string &output_path)
{
	std::vector<std::string> Args=
	{
		GSCmd, "-dBATCH", "-dNOPAUSE", "-q", "-sDEVICE=pdfwrite",
		"-sOutputFile="+output_path,
	};
	Args.insert(Args.end(), input_paths.begin(), input_paths.end());
	
	return GSWrapper::RunCommand(Args);
}

//! 画像ファイルのリストを1つのPDFに変換する
//! @param image_paths 入力画像ファイルのパス一覧
//! @param output_path 出力PDFファイルのパス
//! @return bool
bool GSWrapper::ImageToPdf(const std::vector<std::string> &image_paths, const std::string &output_path)
{
	std::vector<std::string> Args=
	{
		GSCmd, "-dBATCH", "-dNOPAUSE", "-q", "-sDEVICE=pdfwrite",
		"-sOutputFile="+output_path,
	};
	Args.insert(Args.end(), image_paths.begin(), image_paths.end());
	
	return GSWrapper::RunCommand(Args);
}

//! PDFを複数のPNG画像に変換する
//! @param input_path 入力PDFファイルのパス
//! @param output_path 出力ファイル名のパターン（例：page-%03d.png）
//! @param dpi 解像度（dpi単位）
//! @return bool
bool GSWrapper::PdfToImages(const std::string &input_path, const std::string &output_path, int dpi)
{
	std::vector<std::string> Args=
	{
		GSCmd,
		"-dBATCH", "-dNOPAUSE", "-q",
		"-sDEVICE=png16m",
		"-r" + std::to_string(dpi),
		"-sOutputFile="+output_path,
		input_path
	};

	return GSWrapper::RunCommand(Args);
}

//! PDFをグレースケールに変換する
//! @param input_path 入力PDFファイルのパス
//! @param output_path 出力PDFファイルのパス
//! @return bool
bool GSWrapper::ConvertToGrayscalePdf(const std::string &input_path, const std::string &output_path)
{
	std::vector<std::string> Args=
	{
		GSCmd,
		"-sDEVICE=pdfwrite",
		"-dCompatibilityLevel=1.4",
		"-dColorConversionStrategy=Gray",
		"-dProcessColorModel=/DeviceGray",
		"-dNOPAUSE", "-dBATCH", "-dQUIET",
		"-sOutputFile="+output_path,
		input_path
	};

	
	return GSWrapper::RunCommand(Args);
}

//! PDFをモノクロ（1ビット）PNG画像に変換する
//! @param input_path 入力PDFファイルのパス
//! @param output_path 出力ファイル名のパターン
//! @param dpi 解像度（dpi単位）
//! @return bool
bool GSWrapper::ConvertToMonochromeImages(const std::string &input_path, const std::string &output_path, int dpi)
{
	std::vector<std::string> Args=
	{
		GSCmd,
		"-sDEVICE=pngmono",
		"-r"+std::to_string(dpi),
		"-dNOPAUSE", "-dBATCH", "-dQUIET",
		"-sOutputFile="+output_path,
		input_path
	};

	return GSWrapper::RunCommand(Args);
}

#endif
