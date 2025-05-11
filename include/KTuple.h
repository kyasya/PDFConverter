#ifndef __K_TUPLE_H__
#define __K_TUPLE_H__

#pragma once

#include <tuple>
#include <stdexcept>
#include <type_traits>

template<typename... Ts> class KTuple
{
	private:
		std::tuple<Ts...> Data;
	public:
		KTuple(const Ts&... args): Data(args...){}

		std::tuple<Ts...>& GetTuple(){return Data;}
		const std::tuple<Ts...>& GetTuple() const{return Data;}

		// 読み取りアクセス
		auto& operator[](std::size_t index)
		{
			return AccessHelper<DataSize - 1>::Get(Data, index);
		}

		const auto& operator[](std::size_t index) const
		{
			return AccessHelper<DataSize - 1>::Get(Data, index);
		}

	private:
		static constexpr std::size_t DataSize = sizeof...(Ts);

		template<std::size_t I> struct AccessHelper
		{
			static auto& Get(std::tuple<Ts...>& t, std::size_t index)
			{
				if (I == index) return std::get<I>(t);
				else return AccessHelper<I-1>::Get(t, index);
			}

			static const auto& Get(const std::tuple<Ts...>& t, std::size_t index)
			{
				if (I == index) return std::get<I>(t);
				else return AccessHelper<I-1>::Get(t, index);
			}
		};

	private:
		template<>
		struct AccessHelper<static_cast<std::size_t>(-1)>{};
		// 到達不可だが明示することで最後のエラー処理にできる

		template<>
		struct AccessHelper<0>
		{
			static auto& Get(std::tuple<Ts...>& t, std::size_t index)
			{
				if (index == 0) return std::get<0>(t);
				else throw std::out_of_range("KTuple index out of range");
			}

			static const auto& Get(const std::tuple<Ts...>& t, std::size_t index)
			{
				if (index == 0) return std::get<0>(t);
				else throw std::out_of_range("KTuple index out of range");
			}
		};
};

#endif