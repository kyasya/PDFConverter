#include <iostream>
#include "KFileSystem.h"
#include "KPDFConv.h"

void ShowHelp()
{
	std::cout<<"-- HELP --\n"
	<<"pdfconv-cui [mode] [input-path] (page-range@extractor) [output-path(option)]"<<std::endl;
}

int main(int argc, char **argv)
{
	if (argc<=2){ShowHelp(); return -1;}
	std::string Mode=argv[1];

	bool RunFlag=false;

	KPDFConverter gs;

	std::cout<<KFileSystem::PathExists("pd.pdf")<<std::endl;

	if (Mode=="--extract")
	{
		std::string Buf="";
		if(argc==5) Buf=argv[4];
		RunFlag = gs.ExtractPages(argv[3], argv[2], Buf);
	}

	// // 3. PDF結合（2つを連結）
	// std::vector<std::string> filesToMerge = {
	// 	"./data.pdf", "./data_pages_1_2.pdf"};
	// if (gs.MergePdfs(filesToMerge, "./data_merged.pdf"))
	// {
	// 	std::cout << "PDF結合成功\n";
	// }
	// else
	// {
	// 	std::cerr << "PDF結合失敗\n";
	// }

	// 4. 画像 → PDF
	// std::vector<std::string> images = {
	// 	"./image1.jpg", "./image2.jpg"};
	// if (gs.ImageToPdf(images, "./images_converted.pdf"))
	// {
	// 	std::cout << "画像からPDF変換成功\n";
	// }
	// else
	// {
	// 	std::cerr << "画像からPDF変換失敗\n";
	// }

	// 5. PDF → 画像（data.pdf を PNG 画像に）
	if (gs.PdfToImages("./pd.pdf", "./page-%03d.png", 150))
	{
		std::cout << "PDFから画像変換成功\n";
	}
	else
	{
		std::cerr << "PDFから画像変換失敗\n";
	}

	if(RunFlag){std::cout<<"Success"<<std::endl; return 0;}

	std::cout<<"Failed"<<std::endl;
	return -1;
}
