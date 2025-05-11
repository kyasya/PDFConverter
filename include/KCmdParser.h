#ifndef __K_CMD_PARSER_H__
#define __K_CMD_PARSER_H__

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <cstdlib>

//! @brief CUI向けコマンドライン引数パーサ
class KCmdParser
{
	private:
		struct CmdOpt
		{
			std::string Option;
			std::string ShortOpt;
			std::string DefaultVal;
			bool HasVal = false;
			std::string Value;
			std::string Description;
			bool Given = false;
		};

		std::vector<CmdOpt> OptionList;
		std::unordered_map<std::string, int> OptIndexMap;
		std::vector<std::string> PositionalArgs;
		std::string ProgramName;

		struct SpaceLength
		{
			size_t ShortOpt= 4;
			size_t LongOpt = 10;
		};
		SpaceLength SpText;

	public:
		KCmdParser(std::string name="program"){ProgramName=name;}
		void AddOption(const std::string& option, const std::string& description = "", const std::string& default_value = "");
		void Parse(int argc, char* argv[]);
		bool Has		(const std::string& opt);
		bool IsOpt		(const std::string& opt);
		int  GetOptIndex(const std::string& opt);

		template <typename T=std::string> T Get(const std::string& opt, int index=1);

		const std::vector<std::string>& GetPositionalArgs();
		void PrintHelp() const;
};

//! @brief オプションを追加する（"long[:short]" 形式に対応）
//! @param option "long[:short]" 形式のオプション名
//! @param description オプションの説明
//! @param default_value デフォルト値（文字列）
void KCmdParser::AddOption(const std::string& option, const std::string& description, const std::string& default_value)
{
    CmdOpt Opt;
    size_t ColonPos = option.find(':');

    Opt.Option   = (ColonPos == std::string::npos) ? option : option.substr(0, ColonPos);
    Opt.ShortOpt = (ColonPos == std::string::npos) ? 	 "" : option.substr(ColonPos +1);

    Opt.DefaultVal = default_value;
	if (!Opt.DefaultVal.empty()) Opt.HasVal = true;
    Opt.Value = default_value;
    Opt.Description = description;

	// オプション情報を格納&Mapにインデックスを登録
    int Index = OptionList.size();
    OptionList.push_back(Opt);

    OptIndexMap["--"+Opt.Option] = Index;
	SpText.LongOpt=std::max(SpText.LongOpt, Opt.Option.size()+3);
    if (!Opt.ShortOpt.empty()) 
	{
		OptIndexMap["-"+Opt.Option] = Index;
		SpText.ShortOpt=std::max(SpText.ShortOpt, Opt.Option.size()+3);
	}

	// std::cout<<"Add option:"<<Opt.Option<<"/"<<Opt.ShortOpt<<std::endl;
}

//! @brief コマンドライン引数を解析する
//! @param argc 引数の個数
//! @param argv 引数の配列
void KCmdParser::Parse(int argc, char* argv[])
{
    ProgramName = (argc > 0) ? argv[0] : "program";

    for (int i = 1; i < argc; ++i)
    {
		// std::cout<<"arg["<<i<<"]="<<argv[i]<<"\n";
        std::string Arg = argv[i];

        if (Arg == "--help" || Arg == "-h")
        {
            PrintHelp();
            std::exit(0);
        }
        else if (Arg.rfind("--", 0) == 0 || (Arg.rfind("-", 0) == 0 && Arg.length() == 2))
        {
            size_t EqPos = Arg.find('=');
            std::string Key = (EqPos != std::string::npos) ? Arg.substr( 0, EqPos) : Arg;
            std::string Val = (EqPos != std::string::npos) ? Arg.substr(EqPos + 1) : "true";

            auto It = OptIndexMap.find(Key);
            if (It == OptIndexMap.end())
            {
                std::cerr << "option value is invalid: " << Key << "\n";
                PrintHelp();
                std::exit(1);
            }

            CmdOpt& Opt = OptionList[It->second];
            if (Opt.HasVal && EqPos == std::string::npos && i + 1 < argc)
                Opt.Value = argv[++i];
            else Opt.Value = Val;
            Opt.Given = true;
			// std::cout<<"Opt.Value:"<<Opt.Value<<" Opt.Given:"<<Opt.Given<<std::endl;
        }
        else PositionalArgs.push_back(Arg);
    }
}

//! @brief オプションがコマンドラインで指定されたかを判定
//! @param opt 長いオプション名
//! @return 指定されていれば true
bool KCmdParser::Has(const std::string& opt)
{
    auto It = OptIndexMap.find("--" + opt);
    if (It != OptIndexMap.end())
        return OptionList[It->second].Given;
    return false;
}

//! @brief オプションがコマンドラインで指定されたかを判定
//! @param opt 長いオプション名
//! @return 指定されていれば true
bool KCmdParser::IsOpt(const std::string& opt)
{
    if (OptIndexMap.count("--"+opt) || OptIndexMap.count("-"+opt)) return true;
    return false;
}

//! @brief オプションが指定されている位置を取得
//! @param opt 長いオプション名
//! @return 指定されていれば true
int KCmdParser::GetOptIndex(const std::string& opt)
{
	int Index = -1;
    if (OptIndexMap.count("--"+opt)) Index = OptIndexMap["--"+opt];
	if ( OptIndexMap.count("-"+opt)) Index = OptIndexMap[ "-"+opt];
    return Index;
}

//! @brief オプションの値を型 T に変換して取得
//! @tparam T 取得したい型
//! @param opt 長いオプション名
//! @return 変換された値（失敗時はデフォルト構築）
template <typename T>
T KCmdParser::Get(const std::string& opt, int index)
{
	std::string strVal;
	if((OptIndexMap[opt]+index)<0) return T();

	strVal =  OptionList[OptIndexMap[opt]+index].Value;

	// std::cout<<"strVal:"<<strVal<<std::endl;

	if (std::is_same<T, std::string>::value) return strVal;
	else
	{
		T CastVal;
		std::istringstream Iss(strVal);
		
		Iss >> CastVal;
		return CastVal;
	}
    
    return T();
}

//! @brief 位置引数を取得
//! @return 位置引数の文字列リスト（const 参照）
const std::vector<std::string>& KCmdParser::GetPositionalArgs()
{
    return PositionalArgs;
}

//! @brief ヘルプメッセージを標準出力に表示
void KCmdParser::PrintHelp() const
{
    std::cout << "usage: " << ProgramName << " [options] ...\n";
    std::cout << "options:\n";

    for (const auto& Opt : OptionList)
    {
        std::string ShortOpt = Opt.ShortOpt.empty() ? "" : ("-" + Opt.ShortOpt + ",");
        std::string ValInfo = Opt.HasVal ? " (default=" + Opt.DefaultVal + ")" : "";

        std::cout << "  "  << std::setw(SpText.ShortOpt) << ShortOpt
                  << " --" << std::setw(SpText.LongOpt) << std::left << Opt.Option
                  << Opt.Description << ValInfo << "\n";
    }
	std::cout << "  "  << std::setw(SpText.ShortOpt) << "-h,"
                << " --" << std::setw(SpText.LongOpt) << std::left << "help"
                <<"print help\n";
}

#endif
