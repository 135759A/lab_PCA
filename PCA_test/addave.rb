#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
# ファイル名: .rb
# 役割：
# 引数：KL展開後のspecファイル、ave.data
# 返り値：平均値が足されたspecファイル
# 実行例：ruby addave.rb *.spec *_ave.data > *_addave.spec
# -----------------------------------

# 
# 引数:
# 返り値:
def addAverage(line, f_array, i)
	line.chomp! # 改行コードを破壊

	# 元 + 平均 = 加算ベクトル
	line.split("\t").each{|x|
		addave = x.to_f + f_array[i].to_f
		printf("%.5f ",addave)
	}
	printf("\n")
end


# -------------------------------
# main 
# ファイル読み込み
f1 = open("#{ARGV[0]}")
f2 = open("#{ARGV[1]}")

f2_array = Array.new(1000)

# aveファイルから平均値を取り出し, f2_arrayに代入
i = 0
while line2 = f2.gets
	line2.split(" ").each{|x| # 空白で区切る
		f2_array[i] = x
		#print "#{f2_array[i]} "
		i += 1
	}
end

# 平均値を足していく
row = 0
while line1 = f1.gets 
	if row != 0 then # 1行目は無視する
		addAverage(line1, f2_array, row) # goto 10
	else 
		printf("#{line1}")
	end
	row += 1
end

# ファイル読み込み終了
f1.close
f2.close
