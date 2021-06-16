

build-dir: 
	mkdir -p build

encode-sandbox: build-dir src/encode-sandbox.cpp
	g++ src/encode-sandbox.cpp -I sentencepiece/src/builtin_pb/ -lsentencepiece -o build/encode-sandbox

decode-sandbox: build-dir src/decode-sandbox.cpp
	g++ src/decode-sandbox.cpp -I sentencepiece/src/builtin_pb/ -lsentencepiece -o build/decode-sandbox

download: 
	mkdir -p data \
		&& cd data \
		&&  wget http://data.statmt.org/bergamot/models/deen/ende.student.tiny11.tar.gz \
		&& tar xf ende.student.tiny11.tar.gz

encode: build-dir src/encode.cpp
	g++ src/encode.cpp -I sentencepiece/src/builtin_pb/ -lsentencepiece -o build/encode
