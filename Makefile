

build-dir: 
	mkdir -p build

encode: build-dir encode-sandbox.cpp
	g++ src/encode-sandbox.cpp -I sentencepiece/src/builtin_pb/ -lsentencepiece -o build/encode

decode: build-dir decode-sandbox.cpp
	g++ src/decode-sandbox.cpp -I sentencepiece/src/builtin_pb/ -lsentencepiece -o build/decode

download: 
	mkdir -p data \
		&& cd data \
		&&  wget http://data.statmt.org/bergamot/models/deen/ende.student.tiny11.tar.gz \
		&& tar xf ende.student.tiny11.tar.gz

