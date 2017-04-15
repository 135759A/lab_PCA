#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
# ファイル名: rowAverage.rb
# 役割：元のspecファイルから平均値を求め、
# 　　　元のspecファイルから平均値を引く
# 引数：specファイル
# 返り値：差分specファイル
# 実行例：ruby rowAverage.rb *.spec

# 1行ごとに平均値と差分をとる関数
def diffAverage(line)
	line.chomp! # 改行コードを破壊
	sum = 0.0
	i = 0
	# 平均値を求める
	line.split(" ").each{|x| # 空白で区切る
		sum += x.to_f
		i += 1
	}
	ave = 0
	ave = sum/i
	printf("%.5f",ave)

	# 元 - 平均 = 差分ベクトル
	diff = 0
	line.split(" ").each{|x|
		diff = x.to_f - ave
		# printf("%.5f ",diff)
	}
	printf("\n")
end

# main
row = 0
while line = gets 
	if row != 0 then # 1行目は無視する
		diffAverage(line) # goto 10
	else
          printf("#{line}")
        end
	row += 1
end

