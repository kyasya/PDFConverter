#include <iostream>
#include "KFileSystem.h"
#include "GSWrapper.h"

int main()
{
	GsWrapper gs;

	// // 1. 圧縮
	// if (gs.CompressPdf("./data.pdf", "./data_compressed.pdf"))
	// {
	// 	std::cout << "PDF圧縮成功\n";
	// }
	// else
	// {
	// 	std::cerr << "PDF圧縮失敗\n";
	// }

	// 2. ページ抽出（1〜2ページ）
	if (gs.ExtractPages("./pd.pdf", "./data_pages-1-2.pdf", 1, 2))
	{
		std::cout << "ページ抽出成功\n";
	}
	else
	{
		std::cerr << "ページ抽出失敗\n";
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

	return 0;
}
