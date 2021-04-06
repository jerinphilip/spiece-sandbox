#include <sentencepiece.pb.h>
#include <sentencepiece_processor.h>
#include <vector>

void print(std::string label, const std::string &reference, sentencepiece::SentencePieceText_SentencePiece &piece, bool source){
    std::cout << label << ": ";
    std::cout << "ByteRange[" << piece.begin() << ":" << piece.end() <<"] ";   // beginning of byte offset
    std::cout << "[intr:" << piece.piece() << "] ";   // internal representation.
    std::cout << "[extr:" << piece.surface() << "] " ; // external representation. spt.text().substr(begin, end - begin) == surface().
    if(source){
        std::cout << "[mapped:" << reference.substr(piece.begin(), piece.end() - piece.begin()) << "] " ; // external representation. spt.text().substr(begin, end - begin) == surface().
    }
    std::cout << "[id: " << piece.id() << "] " ;      // vocab id
    std::cout << std::endl;
}

int main(){
    sentencepiece::SentencePieceProcessor processor;
    const auto status = processor.Load("/home/jerin/code/uedin/browsermt/mts/models/en.32768.spm");
    if (!status.ok()) {
        std::cerr << status.ToString() << std::endl;
        // error
    }

    sentencepiece::SentencePieceText spt;
    int counter = 0;

    std::string input;
    while(std::getline(std::cin, input)){
        ++counter;
        std::cout << "# Example " << counter << std::endl;
        // Encode
        processor.Encode(input, &spt);
        std::vector<int> spieces;
        std::cout << "original: "<< spt.text() << std::endl;   // This is the same as the input.
        for (const auto &piece : spt.pieces()) {
            spieces.push_back(piece.id());
        }

        // Decode
        std::string holding;
        std::vector<std::string_view> byteRanges;
        auto load = [&holding, &processor, &byteRanges](std::vector<int> &spieces){
            sentencepiece::SentencePieceText spt_decoded;
            processor.Decode(spieces, &spt_decoded);
            holding = spt_decoded.text();
            std::string_view holding_view(holding);
            int idx{0};
            for(auto &piece: spt_decoded.pieces()){
                std::string_view range = holding_view.substr(piece.begin(), piece.end() - piece.begin());
                byteRanges.push_back(range);
                std::cout << "[" << 
                    piece.begin() << ":" << piece.end() << "] [" << idx << ": " << range << "]" << std::endl;
            }
        };

        load(spieces);
        std::cout << "Holding: " << holding << std::endl;
        int idx{0};
        for(auto &range: byteRanges){
            std::cout << "[" << idx << ": " << range  << "]" ;
            ++idx;
        }

    }
}
