# 各種ファイルの役割と依存関係、生成物
* ファイル名
  + 役割
  + 必要環境 
  + 読み込みファイル
  + 生成物

* make_VowelPara5.rb
  + 実験に用いるプログラムを一括実行するためのスクリプト
  + ruby
  + 母音分析Specファイル、母音分析F0ファイル、単語分析Specファイル(*.spec, *.F0, *.spec)
  + なし

* rowAverage.rb
  + パワー(音圧)を引くため、Specファイルの各フレームにある、130個のベクトルの平均値を求める。
  + ruby
  + 分析Specファイル(*.spec)
  + 分析Specファイルのフレーム平均値(*_ave.data)

* rowAverageDifference.rb
  + パワー(音圧)を引くため、フレーム平均値を求める。その後、各フレームのベクトルをフレーム平均値で引いていく。
  + ruby 
  + 分析Specファイル(*.spec)
  + フレーム平均値を引いた分析Specファイル(*_diff.spec)

* MakeEigenF0.c
  + F0値が50を超える値(有声音)に対し主成分分析を行う。
  + C
  + eigenV.c、母音分析F0ファイル(*.F0)、平均ベクトルとフレーム平均値を引いたSpecファイル(*_Fdiff.spec)
  + 128次元軸降順Specファイル(と言ってるけど、多分固有ベクトル)(npca.data)、平均ベクトル(avespec.data)

* eigenV.c
  + 実対称(エルミート)行列の固有値と固有ベクトル(eigen)を求める関数。
  + C
  + MakeEigenF0.cに呼び出される。

* new_spec2pca_out_ndim.c
  + 主成分分析を行い、主成分(=母音空間パラメータ)を求める。
  + C
  + 平均ベクトル(avespec.data)、固有ベクトル(npca.data)、
  + 次元圧縮した母音空間パラメータ(vowel_space.para)

* Vowel2Spec.c
  + npca.dataとavespec.data、vowel_space.paraから合成Specファイルを生成する。
  + C
  + 平均ベクトル(avespec.data)、固有ベクトル(npca.data)、次元圧縮した母音空間パラメータ(vowel_space.para)
  + 合成Specファイル(reSpec.spec)

* addave.rb
  + 分析Specファイルのフレーム平均値を、平均ベクトルを足した合成Specファイルに足す。
  + ruby
  + 平均ベクトルを足した合成Specファイル(*_Fadd.spec)、分析Specファイルのフレーム平均値(*_ave.data)
  + パワー(音圧)を戻した合成Specファイル(*_addave.spec)




