# VSSLロボットのソフトウェア要求仕様書

仕様書の最新版は下記URLにアップロードされています

https://ssl-roots.github.io/vssl_m5stack/

## ドキュメント生成方法

### asciidoctorのバイナリを使う場合

```sh
# asciidoctorのインストール
$ sudo apt install asciidoctor
```

```sh
# HTMLファイルを生成する
$ cd vssl_m5stack/specifications
$ asciidoctor requirements_specifications.adoc

# 生成されたHTMLファイルを開く
$ google-chrome requirements_specifications.html
```

### dockerイメージを使う場合

```sh
# HTMLファイルを生成する
$ cd vssl_m5stack/specifications
$ docker run -it -u $(id -u):$(id -g) -v $PWD:/documents/ asciidoctor/docker-asciidoctor asciidoctor requirements_specifications.adoc

# 生成されたHTMLファイルを開く
$ google-chrome requirements_specifications.html
```

## References

この仕様書は
https://github.com/ShotaAk/usdm_template
を使って生成しています
