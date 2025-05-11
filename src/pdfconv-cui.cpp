#include <iostream>
#include "KFileSystem.h"
#include "KPDFConv.h"
#include "KCmdParser.h"


int main(int argc, char **argv)
{
	std::cout<<"argc="<<argc<<std::endl;
	KCmdParser Args(argv[0]);
	Args.AddOption("extract:ext", "mode:extaction pdf");
	Args.AddOption("range:r", "range of pages", "1-1");
	// Args.AddOption("input:i");
	Args.AddOption("output:o");

	if (argc<=2){Args.PrintHelp(); return -1;}
	// std::string Mode=argv[1];

	Args.Parse(argc, argv);

	KPDFConverter gs;
	bool RunFlag = false;

	// std::cout<<Args.Has("extract")<<std::endl;

	if (Args.IsOpt("extract"))
	{
		// std::string Buf="";
		if (Args.IsOpt("range")) 
		{
			auto Buf = Args.Get<std::string>("range");

			// std::cout<<"Buf:"<<Buf<<std::endl;
			RunFlag = gs.ExtractPages(Buf, argv[2]);
		}
	}

// 	// // 3. PDF結合（2つを連結）
// 	// std::vector<std::string> filesToMerge = {
// 	// 	"./data.pdf", "./data_pages_1_2.pdf"};
// 	// if (gs.MergePdfs(filesToMerge, "./data_merged.pdf"))
// 	// {
// 	// 	std::cout << "PDF結合成功\n";
// 	// }
// 	// else
// 	// {
// 	// 	std::cerr << "PDF結合失敗\n";
// 	// }

// 	// 4. 画像 → PDF
// 	// std::vector<std::string> images = {
// 	// 	"./image1.jpg", "./image2.jpg"};
// 	// if (gs.ImageToPdf(images, "./images_converted.pdf"))
// 	// {
// 	// 	std::cout << "画像からPDF変換成功\n";
// 	// }
// 	// else
// 	// {
// 	// 	std::cerr << "画像からPDF変換失敗\n";
// 	// }

// 	// 5. PDF → 画像（data.pdf を PNG 画像に）
// 	if (gs.PdfToImages("./pd.pdf", "./page-%03d.png", 150))
// 	{
// 		std::cout << "PDFから画像変換成功\n";
// 	}
// 	else
// 	{
// 		std::cerr << "PDFから画像変換失敗\n";
// 	}

// 	if(RunFlag){std::cout<<"Success"<<std::endl; return 0;}

// 	std::cout<<"Failed"<<std::endl;
	return -1;
}
