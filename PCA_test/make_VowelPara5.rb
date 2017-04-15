#!/usr/bin/env ruby
# -*- coding: utf-8 -*-
# ファイル名: make_VowelPara5.rb
# 役割：母音空間による次元圧縮とそれに関連する操作をしたスペクトラムファイル生成
# 引数：主軸となる母音specファイル、単語音声specファイル、選択される主軸次数
# 返り値：母音空間のパラメータ(vowel_space.para)
# 実行例：ruby make_VowelPara.rb AnSysaiueo10-1 AnSys_himawari 3 
# -----------------------------------

# 平均値ファイル生成
system("ruby rowAverage.rb #{ARGV[0]}.spec > #{ARGV[0]}_ave.data")
system("ruby rowAverage.rb #{ARGV[1]}.spec > #{ARGV[1]}_ave.data")

# 平均値(パワー)を引いたspecファイル生成
system("ruby rowAverageDifference.rb #{ARGV[0]}.spec > #{ARGV[0]}_diff.spec")
system("ruby rowAverageDifference.rb #{ARGV[1]}.spec > #{ARGV[1]}_diff.spec")

# 固有ベクトル生成
system("cc MakeEigenF0.c eigenV.c -o MakeEigenF0.exe")
system("./MakeEigenF0.exe #{ARGV[0]}.F0 < #{ARGV[0]}_diff.spec")

# 特徴軸の選択と母音空間の次元圧縮
system("cc new_spec2pca_out_ndim.c -o new_spec2pca_out_ndim.exe")
system("./new_spec2pca_out_ndim.exe #{ARGV[2]} < #{ARGV[1]}_diff.spec")

# 次元圧縮されたベクトルからspecファイル生成
system("cc Vowel2Spec.c -o Vowel2Spec.exe")
system("./Vowel2Spec.exe #{ARGV[2]} vowel_space.para")

# reSpec.specの名前を変更
system("mv reSpec.spec new_#{ARGV[1]}.spec")

# 平均値(パワー)を足したspecファイル
system("ruby addave.rb new_#{ARGV[1]}.spec #{ARGV[1]}_ave.data > #{ARGV[1]}_addave.spec")
puts(" ")
puts("Generate #{ARGV[1]}_addave.spec")