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

int main(int argc, char *argv[]) {
  sentencepiece::SentencePieceProcessor processor;
  // "data/ende.student.tiny11/vocab.deen.spm"
  const auto status = processor.Load(argv[1]);
  if (!status.ok()) {
    std::cerr << status.ToString() << std::endl;
    // error
  }

  sentencepiece::SentencePieceText spt;
  int counter = 0;

  std::string input;
  while (std::getline(std::cin, input)) {
    ++counter;
    std::cout << "// Example " << counter << std::endl;
    // Encode
    processor.Encode(input, &spt);
    std::vector<int> spieces;
    for (const auto &piece : spt.pieces()) {
      spieces.push_back(piece.id());
    }

    // Decode
    sentencepiece::SentencePieceText spt_decoded;
    processor.Decode(spieces, &spt_decoded);
    // Print string
    std::cout << "std::string source = \"" << spt.text() << "\";\n";

    // Print static string-view gen code
    std::cout << "std::vector<std::pair<size_t, size_t>> sourceMarkers = { \n";

    for (size_t i = 0; i < spt.pieces().size(); i++) {
      auto &piece = spt.pieces()[i];
      auto &decoded_piece = spt_decoded.pieces()[i];
      std::cout << "{" << piece.begin() << "," << piece.end() - piece.begin()
                << "}" << ((i == spt.pieces().size() - 1) ? "  " : ", ")
                << " //  " << piece.piece() << "|" << piece.surface() << "|"
                << decoded_piece.surface() << "\n";
    }
    std::cout << "};\n";

    bool first = true;
    std::cout << "std::vector<size_t> wordIdxs = { \n";
    for (size_t i = 0; i < spt.pieces().size(); i++) {
      auto &piece = spt.pieces()[i];
      std::cout << (first ? "  " : ", ");
      std::cout << piece.id();
      first = false;
    }
    std::cout << "}; \n";
  }
}
