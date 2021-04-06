#include <sentencepiece.pb.h>
#include <sentencepiece_processor.h>
#include <vector>

void print(std::string label, const std::string &reference,
           sentencepiece::SentencePieceText_SentencePiece &piece, bool source) {
  std::cout << label << ": ";
  std::cout << "ByteRange[" << piece.begin() << ":" << piece.end()
            << "] ";                              // beginning of byte offset
  std::cout << "[intr:" << piece.piece() << "] "; // internal representation.
  std::cout << "[extr:" << piece.surface()
            << "] "; // external representation. spt.text().substr(begin, end -
                     // begin) == surface().
  if (source) {
    std::cout << "[mapped:"
              << reference.substr(piece.begin(), piece.end() - piece.begin())
              << "] "; // external representation. spt.text().substr(begin, end
                       // - begin) == surface().
  }
  std::cout << "[id: " << piece.id() << "] "; // vocab id
  std::cout << std::endl;
}

int main() {
  sentencepiece::SentencePieceProcessor processor;
  const auto status = processor.Load(
      "/home/jerin/code/uedin/browsermt/mts/models/pl.32768.spm");
  if (!status.ok()) {
    std::cerr << status.ToString() << std::endl;
    // error
  }

  sentencepiece::SentencePieceText spt;
  int counter = 0;

  std::string input;
  while (std::getline(std::cin, input)) {
    ++counter;
    std::cout << "# Example " << counter << std::endl;
    // Encode
    processor.Encode(input, &spt);
    std::vector<int> spieces;
    std::cout << "original: " << spt.text()
              << std::endl; // This is the same as the input.
    for (const auto &piece : spt.pieces()) {
      spieces.push_back(piece.id());
    }

    // Decode
    sentencepiece::SentencePieceText spt_decoded;
    processor.Decode(spieces, &spt_decoded);
    std::cout << "decoded : " << spt_decoded.text()
              << std::endl; // This is the same as the decoded string.

    auto original_pieces = spt.pieces();
    auto decoded_pieces = spt_decoded.pieces();

    int size = std::min(original_pieces.size(), decoded_pieces.size());
    for (int i = 0; i < size; i++) {
      auto original = original_pieces[i];
      auto decoded = decoded_pieces[i];
      print("original", input, original, true);
      print("decoded ", input, decoded, false);
    }
    std::cout << std::endl;
  }
}
